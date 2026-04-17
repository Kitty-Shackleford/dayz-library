import json
import re
import os
from glob import glob

# Ask about applying scale
apply_scale_input = input("Apply scale factor to trigger sizes? (y/n): ").strip().lower()
apply_scale = apply_scale_input in ["y", "yes"]

# Number of breadcrumbs in transitional triggers
num_breadcrumbs = 5  # you can adjust as needed

json_files = glob("*.json")
if not json_files:
    print("No JSON files found in this directory.")
    exit()

for file in json_files:
    print(f"Processing {file}...")
    
    with open(file, "r") as f:
        data = json.load(f)

    # Determine object list
    if isinstance(data, dict) and "Objects" in data:
        objects = data["Objects"]
    elif isinstance(data, list):
        objects = data
    else:
        print(f"Skipping {file} (unknown format)")
        continue

    underground_triggers = []

    for obj in objects:
        if not isinstance(obj, dict):
            continue

        name = obj.get("name", "").lower()
        if "grid_trigger" not in name:
            continue

        # Extract size
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

        pos = obj.get("pos", [0,0,0])
        ypr = obj.get("ypr", [0,0,0])

        # Outer trigger (above)
        outer_pos = [pos[0], pos[1]+0.5, pos[2]]
        outer_trigger = {
            "Position": outer_pos,
            "Orientation": ypr,
            "Size": size,
            "EyeAccommodation": 1,
            "Breadcrumbs": [],
            "InterpolationSpeed": 1
        }

        # Inner trigger (below)
        inner_pos = [pos[0], pos[1]-0.5, pos[2]]
        inner_trigger = {
            "Position": inner_pos,
            "Orientation": ypr,
            "Size": size,
            "EyeAccommodation": 0,
            "Breadcrumbs": [],
            "InterpolationSpeed": 1
        }

        # Transitional trigger (with breadcrumbs)
        transitional_pos = [(outer_pos[0]+inner_pos[0])/2,
                            (outer_pos[1]+inner_pos[1])/2,
                            (outer_pos[2]+inner_pos[2])/2]
        breadcrumbs = []
        for i in range(num_breadcrumbs):
            t = i / (num_breadcrumbs-1)  # 0..1
            breadcrumb_pos = [
                outer_pos[0]*(1-t) + inner_pos[0]*t,
                outer_pos[1]*(1-t) + inner_pos[1]*t,
                outer_pos[2]*(1-t) + inner_pos[2]*t
            ]
            eye_acc = 1*(1-t)  # linearly decrease from 1→0
            breadcrumbs.append({
                "Position": breadcrumb_pos,
                "EyeAccommodation": round(eye_acc, 3),
                "UseRaycast": 1,
                "Radius": -1
            })

        transitional_trigger = {
            "Position": transitional_pos,
            "Orientation": ypr,
            "Size": size,
            "EyeAccommodation": 0,
            "Breadcrumbs": breadcrumbs
        }

        # Add triggers
        underground_triggers.extend([outer_trigger, transitional_trigger, inner_trigger])

    if underground_triggers:
        output_name = file.replace(".json", "_undergroundtriggers.json")
        with open(output_name, "w") as f:
            json.dump({"Triggers": underground_triggers}, f, indent=4)
        print(f"✔ Created {output_name} ({len(underground_triggers)} triggers)")

print("Done.")