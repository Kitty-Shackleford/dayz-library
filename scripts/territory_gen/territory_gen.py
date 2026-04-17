import xml.etree.ElementTree as ET
import math
import os

# Animal -> zone type
animal_zone_map = {
    'Wolf': 'HuntingGround',
    'Bear': 'HuntingGround',
    'Pig': 'Grazing',
    'WildBoar': 'Grazing',
    'Domestic': 'Grazing',
    'Fox': 'Zone',
    'Hare': 'Zone',
    'Hen': 'Zone',
    'RedDeer': 'Grazing',
    'RoeDeer': 'Grazing',
    'Sheep': 'Grazing',
    'Goat': 'Grazing'
}

# Zone names per type (rotated)
zone_names_map = {
    'HuntingGround': ['HuntingGround', 'Rest', 'Water'],
    'Grazing': ['Grazing', 'Rest', 'Water'],
    'Zone': ['Zone1', 'Zone2', 'Zone3'],  # only for non-special animals
    'Rest': ['Rest']
}

special_animals = ['Hen', 'Hare', 'Fox']

# Radius per animal type
radius_map = {
    'Wolf': 250,
    'Bear': 250,
    'Pig': 100,
    'WildBoar': 100,
    'Domestic': 100,
    'RedDeer': 100,
    'RoeDeer': 100,
    'Sheep': 100,
    'Goat': 100,
    'Hen': 50,
    'Hare': 50,
    'Fox': 50,
    'Unknown': 50
}

def get_animal_from_group_name(group_name):
    for animal in animal_zone_map:
        if animal.lower() in group_name.lower():
            return animal
    return 'Unknown'

def generate_offset_positions(x, z, num_zones, radius):
    positions = []
    angle_step = 360 / num_zones
    for i in range(num_zones):
        angle_rad = math.radians(i * angle_step)
        offset_x = x + radius * math.cos(angle_rad)
        offset_z = z + radius * math.sin(angle_rad)
        positions.append((offset_x, offset_z))
    return positions

def find_xml_in_root():
    for file in os.listdir('.'):
        if file.endswith('.xml'):
            return file
    return None

def mapgrouppos_to_territory_final(zones_per_group=4):
    input_file = find_xml_in_root()
    if not input_file:
        print("No .xml file found in the current directory.")
        return

    print(f"Found XML file: {input_file}")
    animal_guess = None
    tree = ET.parse(input_file)
    root = tree.getroot()

    # Try to infer animal from first group
    first_group = root.find('group')
    if first_group is not None:
        group_name = first_group.attrib.get('name', '')
        animal_guess = get_animal_from_group_name(group_name)

    if animal_guess in [None, 'Unknown']:
        # ask user to choose animal type
        print("Select the animal type for this territory file from the list:")
        for idx, animal in enumerate(animal_zone_map.keys(), start=1):
            print(f"{idx}. {animal}")
        while True:
            choice = input("Enter number of animal type: ")
            if choice.isdigit() and 1 <= int(choice) <= len(animal_zone_map):
                animal_guess = list(animal_zone_map.keys())[int(choice)-1]
                break
            print("Invalid choice, try again.")
    print(f"Selected animal type: {animal_guess}")

    output_file = "territories.xml"
    territory_type = ET.Element('territory-type')

    for group in root.findall('group'):
        pos_str = group.attrib.get('pos', '0 0 0')
        x, y, z = [float(coord) for coord in pos_str.split()]

        zone_type = animal_zone_map.get(animal_guess, 'Rest')
        radius = radius_map.get(animal_guess, 50)

        # Determine zone names
        if animal_guess in special_animals:
            zone_names = [f'zone_{animal_guess.lower()}']
        else:
            zone_names = zone_names_map.get(zone_type, ['Rest'])

        # Cycle zone names if there are multiple zones
        zone_name_cycle = zone_names * ((zones_per_group // len(zone_names)) + 1)

        territory = ET.SubElement(territory_type, 'territory', color="4291611852")
        positions = generate_offset_positions(x, z, zones_per_group, radius)

        for i, (px, pz) in enumerate(positions):
            zone_name = zone_name_cycle[i]
            ET.SubElement(territory, 'zone',
                          name=zone_name,
                          smin="0", smax="0", dmin="0", dmax="0",
                          x=str(round(px, 3)), z=str(round(pz, 3)),
                          r=str(radius))

    tree_out = ET.ElementTree(territory_type)
    tree_out.write(output_file, encoding='UTF-8', xml_declaration=True)
    print(f"Territory file created: {output_file} for animal type: {animal_guess}")


# Run script
if __name__ == "__main__":
    mapgrouppos_to_territory_final(zones_per_group=5)