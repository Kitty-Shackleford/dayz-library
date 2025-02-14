import xml.etree.ElementTree as ET

def adjust_lootmax(xml_data):
		# Parse the XML data
		root = ET.fromstring(xml_data)

		# Iterate over each group
		for group in root.findall("group"):
				# Initialize a counter for total points in the group
				total_group_points = 0

				# Iterate over each container in the group
				for container in group.findall("container"):
						# Count the points in this container
						points = container.findall("point")
						point_count = len(points)

						# Update lootmax for container if not set
						if "lootmax" not in container.attrib:
								container.set("lootmax", str(point_count))

						# Add the points to the group's total points
						total_group_points += point_count

				# Set lootmax for the group
				group.set("lootmax", str(total_group_points))

		# Return the modified XML as a string
		return ET.tostring(root, encoding="unicode")

# Example usage
xml_data = """
<xml>	
		<group name="Land_HouseBlock_1F_Corner" lootmax="7"> <!-- count total point pos per group and set lootmax="total" -->
				<usage name="Town"/>
				<container name="lootFloor">
						<category name="tools"/>
						<category name="containers"/>
						<category name="clothes"/>
						<tag name="floor"/>
						<point pos="1.912137 -5.772652 -0.567444" range="0.558594" height="1.396484"/>
						<point pos="0.563250 -5.772652 -2.778766" range="0.624023" height="1.560057"/>
						<point pos="3.739752 -5.772652 -6.676141" range="0.684326" height="1.710815"/>
						<point pos="-3.886793 -1.787790 0.864424" range="0.723389" height="1.808473"/>
						<point pos="7.132498 -5.772652 -6.596429" range="0.765137" height="1.912842"/>
						<point pos="-2.581946 -5.772652 0.959700" range="0.815918" height="0.685328"/>
						<point pos="-1.782007 -1.787790 -6.501154" range="0.856445" height="2.000000"/>
						<point pos="0.093342 -5.772652 4.745423" range="1.016846" height="2.000000"/>
						<point pos="-4.316222 -1.787790 -2.673576" range="1.054199" height="2.000000"/>
						<point pos="-6.319985 -5.772652 6.947256" range="1.126953" height="2.000000"/>
						<point pos="-6.295583 -1.787790 6.865781" range="1.177246" height="2.000000"/>
						<point pos="-0.126004 -1.787790 4.185899" range="1.184814" height="2.000000"/>
						<point pos="-1.651521 -1.787790 -2.937994" range="1.199951" height="2.000000"/>
						<point pos="-3.475205 -5.772652 5.441312" range="1.199951" height="2.000000"/>
				</container>
				<container name="lootshelves">  <!-- if no lootmax value count total point pos for container and apply lootmax="total" -->
						<category name="tools"/>
						<category name="containers"/>
						<category name="clothes"/>
						<category name="food"/>
						<category name="books"/>
						<tag name="shelves"/>
						<point pos="2.103464 -4.998650 0.934805" range="0.100000" height="0.250000"/>
						<point pos="7.460555 -5.300398 -2.295069" range="0.134375" height="0.335938"/>
						<point pos="4.800971 -5.300398 -5.598933" range="0.134375" height="0.335938"/>
						<point pos="1.511772 -5.163290 -2.182967" range="0.155273" height="0.388183"/>
						<point pos="0.650279 -1.189096 -1.148223" range="0.203125" height="0.507813"/>
						<point pos="7.446488 -5.017157 -0.847401" range="0.203125" height="0.507813"/>
						<point pos="1.472553 -5.017157 -6.402965" range="0.203125" height="0.507813"/>
						<point pos="0.699314 -1.189096 -3.505494" range="0.203125" height="0.507813"/>
						<point pos="-3.105465 -1.787790 4.827123" range="0.209473" height="0.523682"/>
						<point pos="-4.132122 -1.189096 -4.244180" range="0.237500" height="0.593750"/>
						<point pos="-0.658572 -5.393858 7.358013" range="0.237500" height="0.593750"/>
						<point pos="-1.295940 -5.392750 6.446482" range="0.262695" height="0.656738"/>
						<point pos="-0.180783 -1.311777 -2.394867" range="0.271875" height="0.679688"/>
						<point pos="7.383480 -5.017157 -1.607492" range="0.271875" height="0.679688"/>
						<point pos="-0.133172 -1.311777 -1.560343" range="0.271875" height="0.679688"/>
						<point pos="-3.121933 -5.393085 7.349802" range="0.276855" height="0.692137"/>
						<point pos="-2.257827 -5.291140 7.522150" range="0.306250" height="0.765617"/>
						<point pos="-2.251506 -4.754930 -3.249357" range="0.375000" height="0.937500"/>
						<point pos="-6.616762 -5.017157 -1.572679" range="0.375000" height="0.937500"/>
						<point pos="4.927056 -5.017157 -1.046060" range="0.409375" height="1.023438"/>
						<point pos="-6.765487 -5.017157 -4.055752" range="0.409375" height="1.023438"/>
						<point pos="-2.118699 -5.017157 -6.378676" range="0.409375" height="1.023438"/>
						<point pos="-0.063638 -1.787790 6.881004" range="1.191650" height="2.000000"/>
				</container>
				<container name="lootweapons" lootmax="8">
						<category name="weapons"/>
						<category name="explosives"/>
						<point pos="-3.460548 -0.916616 -4.622668" range="0.203125" height="0.507813"/>
						<point pos="6.076481 -5.017157 -5.333157" range="0.271875" height="0.679688"/>
						<point pos="-1.535233 -5.291140 7.391975" range="0.306250" height="0.765617"/>
						<point pos="0.526483 -0.702738 -2.830511" range="0.312012" height="0.780029"/>
						<point pos="-0.600708 -1.787790 -0.590440" range="0.590332" height="1.475830"/>
						<point pos="0.544206 -5.772652 7.447115" range="0.627441" height="1.568602"/>
						<point pos="6.188104 -5.772652 0.153638" range="0.875732" height="2.000000"/>
						<point pos="0.359243 -1.787790 -6.408564" range="0.949463" height="2.000000"/>
				</container>
		</group>
</xml>
"""

# Adjust the lootmax values in the XML
adjusted_xml = adjust_lootmax(xml_data)
print(adjusted_xml)
