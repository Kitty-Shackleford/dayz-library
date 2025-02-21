const STRUCTURE_NAME = "StaticObj_Lamp_City1"; // Object to place loot around
const IS_TYPE = "Proxy"; // Can Be "Loot", "Proxy", Or "Zombie"

// If IsType = Loot
const RANGE = "0.15";
const HEIGHT = "1.50";

// If IsType = Zombie
const ZTYPE = "Hospital"; // Can Be Any Of The Zombie Types - "Infected".
const DMIN = -1;
const DMAX = 4;
const RADIUS = 2;

let holder;

/**
 * Rotates a point (x, z) around a center point (cx, cz) by angle `a` (degrees)
 * @param {number} x - Object X coordinate
 * @param {number} z - Object Z coordinate
 * @param {number} cx - Center X coordinate
 * @param {number} cz - Center Z coordinate
 * @param {number} angle - Rotation angle in degrees
 * @returns {[number, number]} - Rotated coordinates [xr, zr]
 */
function rotate(x, z, cx, cz, angle) {
    const radians = (angle * Math.PI) / 180;
    const cosA = Math.cos(radians);
    const sinA = Math.sin(radians);

    // Offset position to be relative to center (cx, cz)
    const relX = x - cx;
    const relZ = z - cz;

    // Rotate around center
    const xr = relX * cosA - relZ * sinA + cx;
    const zr = relX * sinA + relZ * cosA + cz;

    return [xr, zr];
}

/**
 * Spawns an object and outputs XML-style data.
 * @param {string} objectName - Object type to spawn
 * @param {string} pos - Position (X Y Z)
 * @param {string} yaw - Rotation (Yaw Pitch Roll)
 */
function SpawnObject(objectName, pos, yaw) {
    const coords = pos.split(" ").map(parseFloat);
    const rotation = yaw.split(" ").map(parseFloat);

    // If it's the base structure, store its position & yaw
    if (objectName === STRUCTURE_NAME) {
        holder = { position: coords, yaw: rotation[0] };
        console.log(`Holder set for ${STRUCTURE_NAME} at ${holder.position.join(" ")} with yaw ${holder.yaw}`);
        return;
    }

    if (!holder) {
        console.error(`Error: Holder (Base Object Position) is not set for ${objectName}`);
        return;
    }

    const [x, y, z] = coords; // Object's absolute world position
    const [cx, cy, cz] = holder.position; // Structure's absolute world position
    const baseYaw = holder.yaw; // Structure's rotation angle

    // Convert to relative position
    const relX = x - cx;
    const relZ = z - cz;
    const relY = y - cy;

    // Apply rotation relative to the base structure's yaw
    const [rotatedX, rotatedZ] = rotate(relX, relZ, 0, 0, -baseYaw); // Negative baseYaw to correct rotation direction

    // Ensure correct rotation values
    const finalYaw = (rotation[0] + baseYaw) % 360; // Adjust yaw with base structure's yaw

    // Output correct format based on type
    if (IS_TYPE === "Loot") {
        console.log(`<point pos="${rotatedX.toFixed(6)} ${relY.toFixed(6)} ${rotatedZ.toFixed(6)}" range="${RANGE}" height="${HEIGHT}" />`);
    }
    if (IS_TYPE === "Proxy") {
        console.log(`<proxy type="${objectName}" pos="${rotatedX.toFixed(6)} ${relY.toFixed(6)} ${rotatedZ.toFixed(6)}" rpy="${rotation[1]} ${finalYaw} ${rotation[2]}" />`);
    }
    if (IS_TYPE === "Zombie") {
        console.log(`<zone name="Infected${ZTYPE}" smin="0" smax="0" dmin="${DMIN}" dmax="${DMAX}" x="${x}" z="${z}" r="${RADIUS}" />`);
    }
}

// INPUT
SpawnObject("StaticObj_Lamp_City1", "1947.511475 215.792130 7172.625488", "64.969093 0.000000 -0.000000", 0.999918);
SpawnObject("SpotlightLight", "1948.302246 220.183334 7174.050781", "-180.000000 -90.000000 -180.000000", 1);
