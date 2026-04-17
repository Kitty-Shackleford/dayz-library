import json
import re
import os
from glob import glob

# Ask once about scaling
apply_scale_input = input("Apply scale factor to trigger sizes? (y/n): ").strip().lower()
apply_scale = apply_scale_input in ["y", "yes"]

json_files = glob("*.json")

if not json_files:
    print("No JSON files found in this directory.")
    exit()

for file in json_files:
    print(f"Processing {file}...")

    with open(file, "r") as f:
        data = json.load(f)

    # Detect object list
    if isinstance(data, dict) and "Objects" in data:
        objects = data["Objects"]
    elif isinstance(data, list):
        objects = data
    else:
        print(f"Skipping {file} (unknown format)")
        continue

    pra_boxes = []

    for obj in objects:
        if not isinstance(obj, dict):
            continue

        name = obj.get("name", "").lower()

        if "grid_trigger" not in name:
            continue

        # Extract size from name
        size_match = re.search(r'(\d+)x(\d+)(?:x(\d+))?', name)

        if size_match:
            sx = float(size_match.group(1))
            sy = float(size_match.group(2))
            sz = float(size_match.group(3)) if size_match.group(3) else sy
        else:
            sx, sy, sz = 1, 1, 1

        size = [sx, sy, sz]

        if apply_scale:
            scale = obj.get("scale", 1.0)
            size = [s * scale for s in size]

        pos = obj.get("pos", [0, 0, 0])
        ypr = obj.get("ypr", [0, 0, 0])

        pra_boxes.append([size, ypr, pos])

    output_name = file.replace(".json", "_teleports.json")

    with open(output_name, "w") as f:
        json.dump({"PRABoxes": pra_boxes}, f, indent=4)

    print(f"✔ Created {output_name} ({len(pra_boxes)} triggers)")

print("Done.")