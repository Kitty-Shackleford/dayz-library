import xml.etree.ElementTree as ET

def adjust_lootmax(xml_data, overwrite_lootmax=False):
    # Parse the XML data
    root = ET.fromstring(xml_data)

    # Iterate over each group
    for group in root.findall("group"):
        total_group_lootmax = 0  # To hold the sum of lootmax for all containers in the group

        # Iterate over each container in the group
        for container in group.findall("container"):
            # Count the points in this container
            points = container.findall("point")
            point_count = len(points)

            # If lootmax is not already set or we want to overwrite it
            if "lootmax" not in container.attrib or overwrite_lootmax:
                container.set("lootmax", str(point_count))
            
            # Add this container's lootmax to the group's total lootmax
            total_group_lootmax += int(container.attrib["lootmax"])

        # Set lootmax for the group (sum of all container lootmax values)
        group.set("lootmax", str(total_group_lootmax))

    # Return the modified XML as a string
    return ET.tostring(root, encoding="unicode")


def read_xml_file(file_path):
    """Reads the XML content from a file."""
    with open(file_path, 'r') as file:
        return file.read()


def write_xml_file(file_path, xml_data):
    """Writes the modified XML content back to the file."""
    with open(file_path, 'w') as file:
        file.write(xml_data)


# Specify the name of your XML file located in the root directory
file_path = "mapgroupproto.xml"  # Replace with your actual XML file name

# Set the overwrite option
overwrite_lootmax = True  # Change to False if you don't want to overwrite existing lootmax values

# Read XML data from the file
xml_data = read_xml_file(file_path)

# Adjust the lootmax values in the XML
adjusted_xml = adjust_lootmax(xml_data, overwrite_lootmax)

# Write the adjusted XML back to the file (or to a new file)
write_xml_file("modified_" + file_path, adjusted_xml)

print("XML file has been modified and saved.")