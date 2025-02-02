# cleans up duplicates for all jsons in local dir
import json
import os

def clean_json(file_path, new_y_position):
    with open(file_path, 'r') as f:
        data = json.load(f)

    unique_positions = set()
    cleaned_objects = []

    for obj in data.get("Objects", []):
        pos = obj["pos"]
        pos[1] = new_y_position  

        pos_tuple = tuple(pos)

        if pos_tuple not in unique_positions:
            unique_positions.add(pos_tuple)
            cleaned_objects.append(obj)

    # Log before writing changes
    print(f"Processing file: {file_path}")
    print(f"Original object count: {len(data['Objects'])}, Cleaned count: {len(cleaned_objects)}")

    # Update the original data with the cleaned objects
    data["Objects"] = cleaned_objects

    # Write back to the same file
    with open(file_path, 'w') as f:
        json.dump(data, f, indent=4)

# Process all JSON files in the current directory
new_y_position = 26.6  # Desired middle value for 'pos'

for file_name in os.listdir():
    if file_name.endswith('.json'):
        clean_json(file_name, new_y_position)

print("Processing complete!")
