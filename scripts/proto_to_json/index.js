const fs = require('fs'); 
const xml2js = require('xml2js');
const readline = require('readline');
const MAPGROUPPOS_FILE = 'mapgrouppos.xml';
const MAPGROUPPROTO_FILE = 'mapgroupproto.xml';

// Function to parse XML file into JavaScript object
async function parseXML(filePath) {
    const data = fs.readFileSync(filePath, 'utf8');
    const parser = new xml2js.Parser({ explicitArray: false });
    return await parser.parseStringPromise(data); 
}

// Function to prompt the user for input from the command line
async function askUser(question) {
    const rl = readline.createInterface({ input: process.stdin, output: process.stdout });
    return new Promise(resolve => rl.question(question, answer => { rl.close(); resolve(answer); }));
}

// Normalize pitch to be within the -90 to 90 range
function normalizePitch(pitch) {
    return pitch > 90 ? -90 : pitch < -90 ? -90 : pitch; // Needs improvement, values above 90 should be negative counting backwards from -90
}

// Normalize roll to be within -180 to 180 range
function normalizeRoll(roll) {
    roll = (roll + 360) % 360;
    return roll > 180 ? roll - 360 : roll < -180 ? roll + 360 : roll;
}

// Normalize yaw to be within -180 to 180 range
function normalizeYaw(yaw) {
    yaw = (yaw + 360) % 360;
    return yaw > 180 ? yaw - 360 : yaw;
}

// Convert degrees to radians
function toRadians(degrees) {
    return degrees * (Math.PI / 180);
}

// Rotate relative position around yaw
function rotatePosition(absYaw, relX, relZ) {
    const yawRad = toRadians(absYaw); // Convert absolute yaw to radians
    const cosYaw = Math.cos(yawRad);
    const sinYaw = Math.sin(yawRad);

    // Rotate relative X and Z around the absolute Yaw
    const rotatedX = (cosYaw * relX - sinYaw * relZ);
    const rotatedZ = (sinYaw * relX + cosYaw * relZ);

    return [parseFloat(rotatedX.toFixed(6)), parseFloat(rotatedZ.toFixed(6))];
}

// Main function to convert the XML files to JSON format
async function convertFiles() {
    try {
        // Parsing the XML files into JavaScript objects
        const mapGroupPos = await parseXML(MAPGROUPPOS_FILE);
        const mapGroupProto = await parseXML(MAPGROUPPROTO_FILE);

        // Validate XML structure
        if (!mapGroupPos.map?.group || !mapGroupProto.prototype?.group) {
            console.error("Invalid XML format.");
            return;
        }

        // Get available groups from mapgroupproto.xml
        const protoGroups = Array.isArray(mapGroupProto.prototype.group) ? mapGroupProto.prototype.group : [mapGroupProto.prototype.group];
        console.log("Available groups:");
        protoGroups.forEach((g, i) => console.log(`[${i + 1}] ${g.$.name}`));

        // Ask user to select a group
        const userChoice = await askUser("Enter group number: ");
        const index = parseInt(userChoice) - 1;
        if (isNaN(index) || index < 0 || index >= protoGroups.length) return console.error("Invalid selection.");

        const groupName = protoGroups[index].$.name;

        // Get corresponding entries from mapgrouppos.xml
        const groups = Array.isArray(mapGroupPos.map.group) ? mapGroupPos.map.group : [mapGroupPos.map.group];
        const selectedGroupPosEntries = groups.filter(g => g.$.name === groupName);
        if (!selectedGroupPosEntries.length) return console.warn(`No positions found for ${groupName}.`);

        const output = { Objects: [] };

        // Process each group position entry
        for (const groupEntry of selectedGroupPosEntries) {
            const [absX, absY, absZ] = groupEntry.$.pos.split(" ").map(parseFloat);
            const [absR, absP, absYpr] = groupEntry.$.rpy.split(" ").map(parseFloat);

            console.log(`Processing ${groupName} at (${absX}, ${absY}, ${absZ})`);

            // Find proxy objects associated with the selected group
            const proxies = protoGroups.filter(pg => pg.$.name === groupName)
                .flatMap(pg => pg.dispatch?.proxy ? (Array.isArray(pg.dispatch.proxy) ? pg.dispatch.proxy : [pg.dispatch.proxy]) : []);

            for (const proxy of proxies) {
                const objectName = proxy.$.type;
                const [relX, relY, relZ] = proxy.$.pos ? proxy.$.pos.split(" ").map(parseFloat) : [0, 0, 0];
                const [relR, relP, relYpr] = proxy.$.rpy ? proxy.$.rpy.split(" ").map(parseFloat) : [0, 0, 0];

                // Ask user if they need to apply an offset
                const needOffset = (await askUser(`Apply offset for ${objectName}? (y/n): `)).toLowerCase();
                let offsetX = 0, offsetY = 0, offsetZ = 0;

                if (needOffset === "y") {
                    offsetX = parseFloat(await askUser("Enter X offset: ")) || 0;
                    offsetY = parseFloat(await askUser("Enter Y offset: ")) || 0;
                    offsetZ = parseFloat(await askUser("Enter Z offset: ")) || 0;
                }

                // Apply yaw rotation to the relative X and Z positions
                const [rotX, rotZ] = rotatePosition(absYpr, relX + offsetX, relZ + offsetZ);

                // Compute final positions and rotations
                const finalPosX = absX + rotX;
                const finalPosZ = absZ + rotZ;
                const finalPosY = absY + relY + offsetY;
                let finalRoll = absR + relR;
                let finalPitch = absP + relP;
                let finalYaw = absYpr + relYpr;

                // Normalize rotations
                finalPitch = normalizePitch(finalPitch);
                finalRoll = normalizeRoll(finalRoll);
                finalYaw = normalizeYaw(finalYaw);

                console.log(`Final Object: ${objectName} -> Pos: (${finalPosX}, ${finalPosY}, ${finalPosZ}) RPY: (${finalRoll}, ${finalPitch}, ${finalYaw})`);

                // Add object to output JSON
                output.Objects.push({
                    name: objectName,
                    pos: [finalPosX, finalPosY, finalPosZ],
                    ypr: [finalYaw, finalPitch, finalRoll],
                    scale: 1.0,
                    enableCEPersistency: 0
                });
            }
        }

        // Save output JSON file
        const outputFilePath = `${groupName}.json`;
        fs.writeFileSync(outputFilePath, JSON.stringify(output, null, 4));
        console.log(`Saved ${outputFilePath}`);
    } catch (error) {
        console.error("Error processing files:", error);
    }
}

// Start the conversion process
convertFiles();
