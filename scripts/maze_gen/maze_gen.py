import json
import random

# Configurable object dimensions (modifiable)
OBJECT_DIMENSIONS = {
    "single_container": {"length": 6, "width": 2.5, "height": 2.5},
    "double_container": {"length": 6, "width": 2.5, "height": 5},
    "castle_wall": {"length": 18, "width": 2.5, "height": 10},
    "castle_gate": {"length": 18, "width": 5, "height": 12.5}
}

# Available container variants
SINGLE_CONTAINERS = ["Land_Container_1Aoh", "Land_Container_1Bo", "Land_Container_1Mo", "Land_Container_1Moh", "StaticObj_Container_1C", "StaticObj_Container_1D"]
DOUBLE_CONTAINERS = ["StaticObj_Container_2A", "StaticObj_Container_2B", "StaticObj_Container_2C", "StaticObj_Container_2D", "StaticObj_Container_2E"]

# Prompt user for maze size (default 10x10)
maze_rows = input("Enter number of maze rows (default 10): ") or "10"
maze_cols = input("Enter number of maze columns (default 10): ") or "10"
MAZE_SIZE = (int(maze_rows), int(maze_cols))

# Prompt user for maze starting position (default 7500, 0, 7500)
start_x = input("Enter starting X coordinate (default 7500): ") or "7500"
start_y = input("Enter starting Y coordinate (default 0): ") or "0"
start_z = input("Enter starting Z coordinate (default 7500): ") or "7500"
START_POS = (int(start_x), int(start_y), int(start_z))

# Prompt user for container type
container_choice = input("Choose inner maze type: (1) Single Containers, (2) Double Containers, (3) Mix: ")
if container_choice == "1":
    inner_object = "single_container"
elif container_choice == "2":
    inner_object = "double_container"
else:
    inner_object = "mixed"

# Set the radius around the gates where no containers will be placed (set to 0 for now)
NO_OBJECT_RADIUS = 5  # Adjust this value to control how much space around gates remains empty

def random_yaw():
    return random.uniform(-180, 180)

def apply_yaw_to_dimensions(dimensions, yaw):
    """
    Adjust the dimensions of the container based on yaw (rotation).
    Yaw can be 0, 90, 180, or 270 degrees.
    """
    if yaw == 90 or yaw == 270:
        # Swap length and width when rotated 90 or 270 degrees
        return {"length": dimensions["width"], "width": dimensions["length"], "height": dimensions["height"]}
    else:
        # No change for yaw = 0 or 180
        return dimensions

def generate_maze_grid(rows, cols):
    # Initialize grid: 0 = empty space, 1 = wall
    grid = [[1 for _ in range(cols)] for _ in range(rows)]
    
    # Start point is the entrance gate, which will be used for carving the maze
    start_row = 0  # Entrance gate row
    start_col = cols // 2  # Entrance gate col (center of the wall)
    grid[start_row][start_col] = 0  # Carve out space for the entrance
    
    # Recursive backtracking function to generate maze
    def carve_path(r, c):
        directions = [(0, 1), (1, 0), (0, -1), (-1, 0)]
        random.shuffle(directions)
        
        for dr, dc in directions:
            nr, nc = r + dr * 2, c + dc * 2
            if 0 < nr < rows and 0 < nc < cols and grid[nr][nc] == 1:
                grid[nr][nc] = 0
                grid[r + dr][c + dc] = 0  # Carve the wall between current and next
                carve_path(nr, nc)  # Recurse
    
    carve_path(start_row, start_col)
    return grid

maze_grid = generate_maze_grid(MAZE_SIZE[0], MAZE_SIZE[1])

# Generate maze layout
maze = []
entrance_x = START_POS[0] + (MAZE_SIZE[1] // 2) * (OBJECT_DIMENSIONS["castle_gate"]["length"])  # Adjusted for gate size
exit_x = entrance_x

# First, place entrance and exit gates in the middle of the outer walls
entrance_placed = False
exit_placed = False

# Coordinates for gates
entrance_pos = None
exit_pos = None

for row in range(MAZE_SIZE[0]):
    for col in range(MAZE_SIZE[1]):
        x = START_POS[0] + col * (OBJECT_DIMENSIONS["single_container"]["length"])  # Adjusted spacing for gates and walls
        z = START_POS[2] + row * (OBJECT_DIMENSIONS["single_container"]["length"])

        if row == 0 and col == MAZE_SIZE[1] // 2 and not entrance_placed:  # Entrance gate (middle of north wall)
            yaw = 0
            entrance_pos = (x, START_POS[1], z)
            maze.append({
                "name": "Land_Castle_Gate",
                "pos": entrance_pos,
                "ypr": [yaw, 0, 0],
                "scale": 1.0,
                "enableCEPersistency": 0
            })
            entrance_placed = True
        
        elif row == MAZE_SIZE[0] - 1 and col == MAZE_SIZE[1] // 2 and not exit_placed:  # Exit gate (middle of south wall)
            yaw = 180
            exit_pos = (x, START_POS[1], z)
            maze.append({
                "name": "Land_Castle_Gate",
                "pos": exit_pos,
                "ypr": [yaw, 0, 0],
                "scale": 1.0,
                "enableCEPersistency": 0
            })
            exit_placed = True

# After placing gates, place walls and containers
for row in range(MAZE_SIZE[0]):
    for col in range(MAZE_SIZE[1]):
        x = START_POS[0] + col * (OBJECT_DIMENSIONS["single_container"]["length"])  # Adjusted spacing for containers and walls
        z = START_POS[2] + row * (OBJECT_DIMENSIONS["single_container"]["length"])

        # Skip positions within the radius of the entrance and exit gates
        if (entrance_pos and 
            (abs(x - entrance_pos[0]) <= NO_OBJECT_RADIUS and abs(z - entrance_pos[2]) <= NO_OBJECT_RADIUS)) or \
           (exit_pos and 
            (abs(x - exit_pos[0]) <= NO_OBJECT_RADIUS and abs(z - exit_pos[2]) <= NO_OBJECT_RADIUS)):
            continue  # Skip containers near gates

        if row == 0 or row == MAZE_SIZE[0] - 1 or col == 0 or col == MAZE_SIZE[1] - 1:
            # Outer walls (aligned, west and east walls rotated 90 degrees)
            yaw = 90 if col == 0 or col == MAZE_SIZE[1] - 1 else 0
            # Place castle walls (ensure no overlap with gates)
            if not ((row == 0 and col == MAZE_SIZE[1] // 2) or (row == MAZE_SIZE[0] - 1 and col == MAZE_SIZE[1] // 2)):
                maze.append({
                    "name": "Land_Castle_Wall1_20",
                    "pos": [x, START_POS[1], z],
                    "ypr": [yaw, 0, 0],
                    "scale": 1.0,
                    "enableCEPersistency": 0
                })

        elif maze_grid[row][col] == 0:  # If it's an open space
            # Inner maze (pathways)
            if inner_object == "mixed":
                obj_type = random.choice(["single_container", "double_container"])
            else:
                obj_type = inner_object
            
            obj_name = random.choice(SINGLE_CONTAINERS) if obj_type == "single_container" else random.choice(DOUBLE_CONTAINERS)
            
            # Apply yaw adjustment to container dimensions
            yaw = random_yaw()
            container_dimensions = OBJECT_DIMENSIONS[obj_type]
            adjusted_dimensions = apply_yaw_to_dimensions(container_dimensions, yaw)
            
            # Adjust grid placement based on adjusted dimensions
            grid_x = x + adjusted_dimensions["length"] / 2
            grid_z = z + adjusted_dimensions["width"] / 2

            # Place inner containers (skipping the area around gates)
            if abs(x - entrance_x) > NO_OBJECT_RADIUS and abs(x - exit_x) > NO_OBJECT_RADIUS:
                maze.append({
                    "name": obj_name,
                    "pos": [grid_x, START_POS[1], grid_z],
                    "ypr": [yaw, 0, 0],
                    "scale": 1.0,
                    "enableCEPersistency": 0
                })

# Save to JSON
output = {"Objects": maze}
with open("maze.json", "w") as f:
    json.dump(output, f, indent=4)

print("Maze generated and saved to maze.json!")
