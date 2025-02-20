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
    return pitch > 90 ? -90 : pitch < -90 ? -90 : pitch; // needs improvment values above 90 should be negitive counting backwards from -90?
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
    const yawRad = toRadians(absYaw);
    const cosYaw = Math.cos(yawRad);
    const sinYaw = Math.sin(yawRad);

    return [
        parseFloat((cosYaw * relX - sinYaw * relZ).toFixed(6)), // Rotated X
        parseFloat((sinYaw * relX + cosYaw * relZ).toFixed(6))  // Rotated Z
    ];
}

// Main function to convert the XML files to JSON format
async function convertFiles() {
    try {
        // Parsing the XML files into JavaScript objects
        const mapGroupPos = await parseXML(MAPGROUPPOS_FILE);
        const mapGroupProto = await parseXML(MAPGROUPPROTO_FILE);

        // Checking if the required XML structure exists
        if (!mapGroupPos.map?.group || !mapGroupProto.prototype?.group) {
            console.error("Invalid XML format.");
            return;
        }

        // Ensuring we work with an array of groups (to handle both single and multiple groups)
        const protoGroups = Array.isArray(mapGroupProto.prototype.group) ? mapGroupProto.prototype.group : [mapGroupProto.prototype.group];
        console.log("Available groups:");
        protoGroups.forEach((g, i) => console.log(`[${i + 1}] ${g.$.name}`));

        // Asking the user to select a group by number
        const userChoice = await askUser("Enter group number: ");
        const index = parseInt(userChoice) - 1; // Converting the input into an index
        if (isNaN(index) || index < 0 || index >= protoGroups.length) return console.error("Invalid selection.");

        // Getting the group name from the selected index
        const groupName = protoGroups[index].$.name;

        // Ensuring we work with an array of map positions (in case there is only one)
        const groups = Array.isArray(mapGroupPos.map.group) ? mapGroupPos.map.group : [mapGroupPos.map.group];
        
        // Filtering the map positions that belong to the selected group
        const selectedGroupPosEntries = groups.filter(g => g.$.name === groupName);
        if (!selectedGroupPosEntries.length) return console.warn(`No positions found for ${groupName}.`);

        const output = { Objects: [] };

        // Looping through each map position entry for the selected group
        selectedGroupPosEntries.forEach((groupEntry, idx) => {
            const [absX, absY, absZ] = groupEntry.$.pos.split(" ").map(parseFloat); 
            const [absR, absP, absYpr] = groupEntry.$.rpy.split(" ").map(parseFloat);
            const angleA = parseFloat(groupEntry.$.a) || 0;

            console.log(`Processing ${groupName} at (${absX}, ${absY}, ${absZ})`);

            // Finding the proxy objects associated with the group in the prototype
            const proxies = protoGroups.filter(pg => pg.$.name === groupName)
                .flatMap(pg => pg.dispatch?.proxy ? (Array.isArray(pg.dispatch.proxy) ? pg.dispatch.proxy : [pg.dispatch.proxy]) : []);

            // Looping through each proxy object found for the group
            proxies.forEach(proxy => {
                const objectName = proxy.$.type;
                const [relX, relY, relZ] = proxy.$.pos ? proxy.$.pos.split(" ").map(parseFloat) : [0, 0, 0]; // Extracting relative position values (defaults to 0 if not available)
                const [relR, relP, relYpr] = proxy.$.rpy ? proxy.$.rpy.split(" ").map(parseFloat) : [0, 0, 0]; // Extracting relative rotation values (defaults to 0 if not available)

                // Apply yaw rotation to the relative X and Z positions
                const [rotX, rotZ] = rotatePosition(absYpr, relX, relZ);

                // Calculating the final position
                const finalPosX = absX + rotX;
                const finalPosZ = absZ + rotZ;
                const finalPosY = absY + relY;
                let finalRoll = absR + relR;
                let finalPitch = absP + relP;
                let finalYaw = absYpr + relYpr;

                // Debugging: Log the raw values for pitch and roll
                console.log(`Raw Pitch Calculation: absP(${absP}) + relP(${relP}) = finalPitch(${finalPitch})`);
                console.log(`Raw Roll Calculation: absR(${absR}) + relR(${relR}) = finalRoll(${finalRoll})`);

                // Normalize pitch, roll, and yaw
                finalPitch = normalizePitch(finalPitch);
                finalRoll = normalizeRoll(finalRoll);
                finalYaw = normalizeYaw(finalYaw);

                // Logging the final position and rotation for the object
                console.log(`Object: ${objectName} -> Pos: (${finalPosX}, ${finalPosY}, ${finalPosZ}) RPY: (${finalRoll}, ${finalPitch}, ${finalYaw})`);

                // Adding the object to the output structure
                output.Objects.push({ 
                    name: objectName, 
                    pos: [finalPosX, finalPosY, finalPosZ], 
                    ypr: [finalYaw, finalPitch, finalRoll], 
                    scale: 1.0, //default value
                    enableCEPersistency: 0 //set to false
                });
            });
        });

        // Saving the output to a JSON file
        const outputFilePath = `${groupName}.json`;
        fs.writeFileSync(outputFilePath, JSON.stringify(output, null, 4));
        console.log(`Saved ${outputFilePath}`);
    } catch (error) {
        console.error("Error processing files:", error);
    }
}

// Calling the convertFiles function to start the process
convertFiles();