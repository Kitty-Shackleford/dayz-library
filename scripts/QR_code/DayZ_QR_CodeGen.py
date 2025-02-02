import json
import qrcode
import math

# Function to create QR code and get pixel data
def get_qr_code_pixel_positions(data, spacing=0.015, height=0.025):  # Adjusted values
    qr = qrcode.QRCode(version=3, box_size=8, border=0)  # Smaller box size
    qr.add_data(data)
    qr.make(fit=True)
    
    qr_matrix = qr.get_matrix()  # Get the QR code as a matrix of booleans
    armband_positions = []

    for row in range(len(qr_matrix)):
        for col in range(len(qr_matrix[row])):
            # Calculate position based on row, col, and spacing
            position = (0, row * height, col * spacing)
            if qr_matrix[row][col]:  # If the pixel is black
                armband_positions.append(("Armband_Black", position))
            else:
                armband_positions.append(("Armband_White", position))
    
    return armband_positions

# Function to rotate a position around the Y-axis
def rotate_y(position, yaw):
    x, y, z = position
    yaw_rad = math.radians(yaw)
    new_x = x * math.cos(yaw_rad) - z * math.sin(yaw_rad)
    new_z = x * math.sin(yaw_rad) + z * math.cos(yaw_rad)
    return new_x, y, new_z

# Function to create JSON data
def create_json_data(url, spacing, coords, global_ypr, height):
    # Get the QR code pixel positions
    armband_positions = get_qr_code_pixel_positions(url, spacing, height)

    # Create JSON data structure
    json_data = {
        "Objects": []
    }

    # Rotate and add armbands to JSON
    for name, pos in armband_positions:
        rotated_pos = rotate_y(pos, global_ypr[0])  # Rotate around Y-axis using yaw
        json_data["Objects"].append({
            "name": name,
            "pos": [coords[0] + rotated_pos[0], coords[1] + rotated_pos[1], coords[2] + rotated_pos[2]],  
            "ypr": [global_ypr[0], 80.0, global_ypr[2]],  # Keep pitch constant
            "scale": 1.0,
            "enableCEPersistency": 0
        })

    return json_data

# Main function to get user input and save JSON
def main():
    url = input("Enter the URL for the QR code: ")  
    spacing = float(input("Enter the spacing between armbands (default is 0.015): ") or 0.015)

    coords_input = input("Enter the coordinates (x, y, z) separated by commas: ")
    coords = tuple(map(float, coords_input.split(',')))

    global_ypr_input = input("Enter the global YPR values (yaw, pitch, roll) separated by commas (default is 0.0, 80.0, 0.0): ")
    if global_ypr_input:
        global_ypr = tuple(map(float, global_ypr_input.split(',')))
    else:
        global_ypr = (0.0, 80.0, 0.0)

    height = float(input("Enter the height between armbands (default is 0.025): ") or 0.025)

    json_data = create_json_data(url, spacing, coords, global_ypr, height)

    # Save JSON to a file
    with open('armband_data.json', 'w') as json_file:
        json.dump(json_data, json_file, indent=4)

    print("JSON data saved to armband_data.json")

# Run the script
if __name__ == "__main__":
    main()
