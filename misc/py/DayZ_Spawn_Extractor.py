import xml.etree.ElementTree as ET
import os
from collections import defaultdict

def extract_event_data(input_file):
    """Extracts 'event' and 'pos' data from the input XML and groups events with the same name."""
    tree = ET.parse(input_file)
    root = tree.getroot()

    event_data = defaultdict(list)

    # Process each event and group positions by event name
    for event in root.findall(".//event"):
        event_name = event.get('name')
        positions = [(pos.get('x'), pos.get('z')) for pos in event.findall(".//pos")]

        # Only add to the event_data if there are positions
        if positions:
            event_data[event_name].extend(positions)

    # Convert the dictionary into a list of grouped events
    grouped_event_data = []
    for event_name, positions in event_data.items():
        grouped_event_data.append({
            'name': event_name,
            'lifetime': '240',  # Hardcoded as per the example
            'counter': '-1',
            'positions': positions
        })

    return grouped_event_data

def extract_territory_data(input_file):
    """Extracts 'zone' data from the input XML and groups zones with the same name."""
    tree = ET.parse(input_file)
    root = tree.getroot()

    territory_data = defaultdict(list)

    # Process each zone and group positions by zone name
    for territory in root.findall(".//territory"):
        for zone in territory.findall(".//zone"):
            zone_name = zone.get('name')
            position = (zone.get('x'), zone.get('z'))

            # Only add to territory_data if there's at least one position
            if position:
                territory_data[zone_name].append(position)

    # Convert the dictionary into a list of grouped zones
    grouped_territory_data = []
    for zone_name, positions in territory_data.items():
        grouped_territory_data.append({
            'name': zone_name,
            'lifetime': '240',  # Hardcoded as per the example
            'counter': '-1',
            'positions': positions
        })

    return grouped_territory_data

def write_output(output_file, data):
    """Writes the extracted and formatted data to the output file."""
    with open(output_file, 'w') as file:
        for group in data:
            file.write(f'<group name="{group["name"]}" lifetime="{group["lifetime"]}" counter="{group["counter"]}">\n')
            for pos in group['positions']:
                file.write(f'    <pos x="{pos[0]}" z="{pos[1]}" />\n')
            file.write('</group>\n')

def process_all_files_in_dir(input_dir):
    """Process all XML files in the given directory and output formatted XMLs."""
    # Get all XML files in the directory
    for filename in os.listdir(input_dir):
        if filename.endswith('.xml'):
            input_file = os.path.join(input_dir, filename)
            output_file = os.path.join(input_dir, f'formatted_{filename}')
            
            # Extract data from the current XML file
            event_data = extract_event_data(input_file)
            territory_data = extract_territory_data(input_file)
            
            # Combine both types of data
            combined_data = event_data + territory_data
            
            # Only write output if there's any valid data
            if combined_data:
                write_output(output_file, combined_data)
                print(f"Formatted data for {filename} has been saved to {output_file}")
            else:
                print(f"No valid data found in {filename}, skipping.")

def main():
    input_dir = '.'  # The current directory, or provide a path if needed
    process_all_files_in_dir(input_dir)

if __name__ == "__main__":
    main()
