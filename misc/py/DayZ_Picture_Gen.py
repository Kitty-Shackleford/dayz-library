import json
import qrcode
from PIL import Image

# Function to create QR code pixel data
def get_qr_code_pixel_positions(data, spacing=0.05, height=0.1):
    qr = qrcode.QRCode(version=3, box_size=10, border=0)
    qr.add_data(data)
    qr.make(fit=True)
    
    qr_matrix = qr.get_matrix()
    armband_positions = []

    for row in range(len(qr_matrix)):
        for col in range(len(qr_matrix[row])):
            if qr_matrix[row][col]:
                position = (0, row * height, col * spacing)
                armband_positions.append(position)
    
    return armband_positions

# Function to create pixel data from an image
def get_image_pixel_positions(image_path, scaling_factor=1.0, spacing=0.1, height=0.1):
    img = Image.open(image_path)
    img = img.convert('RGB')
    
    # Scale the image
    new_size = (int(img.width * scaling_factor), int(img.height * scaling_factor))
    img = img.resize(new_size)

    width, height_img = img.size
    armband_positions = []

    for y in range(height_img):
        for x in range(width):
            r, g, b = img.getpixel((x, y))

            # Check for colors and match them to armband colors
            if r > 200 and g > 200 and b > 200:  # White
                armband_color = "Armband_White"
            elif r > 200 and g < 100 and b < 100:  # Red
                armband_color = "Armband_Red"
            elif r < 100 and g > 200 and b < 100:  # Green
                armband_color = "Armband_Green"
            elif r < 100 and g < 100 and b > 200:  # Blue
                armband_color = "Armband_Blue"
            elif r > 200 and g > 100 and b < 100:  # Orange
                armband_color = "Armband_Orange"
            elif r > 200 and g < 100 and b > 200:  # Pink
                armband_color = "Armband_Pink"
            elif r > 100 and g > 100 and b < 100:  # Yellow
                armband_color = "Armband_Yellow"
            else:
                continue  # Ignore other colors
            
            position = (0, y * height, x * spacing)
            armband_positions.append((armband_color, position))

    return armband_positions

# Function to create JSON data
def create_json_data(positions, coords, ypr):
    json_data = {
        "Objects": []
    }

    for armband_color, pos in positions:
        json_data["Objects"].append({
            "name": armband_color,
            "pos": [coords[0] + pos[0], coords[1] + pos[1], coords[2] + pos[2]],
            "ypr": [ypr[0], ypr[1], ypr[2]],
            "scale": 1.0,
            "enableCEPersistency": 0
        })

    return json_data

# Main function to get user input and save JSON
def main():
    choice = input("Do you want to create a QR code or use an image? (Enter 'QR' or 'Image'): ").strip().lower()
    
    coords_input = input("Enter the coordinates (x, y, z) separated by commas: ")
    coords = tuple(map(float, coords_input.split(',')))

    ypr_input = input("Enter the YPR values (yaw, pitch, roll) separated by commas (default is 0,0,0): ")
    ypr = tuple(map(float, ypr_input.split(','))) if ypr_input else (0.0, 0.0, 0.0)

    if choice == 'qr':
        url = input("Enter the URL for the QR code: ")
        spacing = float(input("Enter the spacing between armbands (default is 0.05): ") or 0.05)
        height = float(input("Enter the height between armbands (default is 0.1): ") or 0.1)

        positions = get_qr_code_pixel_positions(url, spacing, height)

        # All QR code armbands will default to black and white
        positions = [( "Armband_Black" if i % 2 == 0 else "Armband_White", pos) for i, pos in enumerate(positions)]

    elif choice == 'image':
        image_path = input("Enter the path to the image: ")
        scaling_factor = float(input("Enter the scaling factor for the image (default is 1.0): ") or 1.0)
        spacing = float(input("Enter the spacing between armbands (default is 0.1): ") or 0.1)
        height = float(input("Enter the height between armbands (default is 0.1): ") or 0.1)

        positions = get_image_pixel_positions(image_path, scaling_factor, spacing, height)

    else:
        print("Invalid choice. Please enter 'QR' or 'Image'.")
        return

    json_data = create_json_data(positions, coords, ypr)

    # Save JSON to a file
    with open('armband_data.json', 'w') as json_file:
        json.dump(json_data, json_file, indent=4)

    print("JSON data saved to armband_data.json")

# Run the script
if __name__ == "__main__":
    main()
