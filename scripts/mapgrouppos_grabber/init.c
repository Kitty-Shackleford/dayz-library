// DayZ Editor just needs an Empty init.c to function
// add whatever you'd like here for your own custom scripts :)
// %userprofile%\Documents\DayZ\Editor\Missions
// %localappdata%\DayZ
void WriteSpecificObjectPositionsToXML() {
  // Open file for writing
  FileHandle file = OpenFile("$profile:object_positions.xml", FileMode.WRITE);

  if (file != 0) {
    // Write the XML header
    FPrint(file, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\n");
    FPrint(file, "<map>\n");

    // Define the target object name
    string targetObject = "misc_chickencoop.p3d"; // must be p3d asset and must be all lowercase!

    // Create an array to store objects
    array < Object > objects = {};
    array < CargoBase > proxy_cargos = {}; // Needed as a parameter but not used

    // Get all objects within a large radius
    GetGame().GetObjectsAtPosition("7500 0 7500", 15000, objects, proxy_cargos);

    // Iterate over objects and filter by type
    foreach(Object obj: objects) {
      string objectPath = obj.GetShapeName(); // Get full P3D path
      TStringArray pathParts = {};
      objectPath.Split("\\", pathParts); // Split path into parts

      if (pathParts.Count() > 0 && pathParts[pathParts.Count() - 1] == targetObject) // Compare last part of path
      {
        vector position = obj.GetPosition();
        vector rotation = obj.GetOrientation(); // RPY values (Roll, Pitch, Yaw)

        // Convert orientation to an angle
        float azimuth = 0; // Initialize azimuth variable

        if (rotation[0] <= -90)
          azimuth = -270 - rotation[0];
        else
          azimuth = 90 - rotation[0];

        // Format the entry for XML output
        string entry = "<group name=\"" + targetObject + "\" pos=\"" + position[0] + " " + position[1] + " " + position[2] + "\" rpy=\"" + rotation[2] + " " + rotation[1] + " " + rotation[0] + "\" a=\"" + azimuth + "\" />\n";
          "rpy=\"" + rotation[2] + " " + rotation[1] + " " + rotation[0] + "\" a=\"" + azimuth + "\" />\n";

        // Write to file
        FPrint(file, entry);
      }
    }

    // Close the XML map tag and the file
    FPrint(file, "</map>\n");
    CloseFile(file);
    Print("Object positions written to object_positions.xml.");
  } else {
    Print("Failed to open file for writing.");
  }
}

void main() {
  // Call the function to find and write specific object positions to XML
  WriteSpecificObjectPositionsToXML();
}