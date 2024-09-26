static Object SpawnObject(string type, vector position, vector orientation, float scale = 1.0)
{
    Object obj = GetGame().CreateObjectEx(type, position, ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS);
    if (!obj) {
        Error("Failed to create object " + type);
        return null;
    }

    obj.SetPosition(position);
    obj.SetOrientation(orientation);
    obj.SetOrientation(obj.GetOrientation());
    obj.SetScale(scale);
    obj.Update();
	obj.SetAffectPathgraph(true, false);
	if (obj.CanAffectPathgraph()) {
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, obj);
    } 

    return obj;
}

// Paste anything below this line into the bottom of your 'void main()' function


// Created Objects
SpawnObject("ChristmasTree_Green", "4310.262695 343.234558 10490.705078", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("Wreck_SantasSleigh", "4315.625488 339.812866 10486.194336", "47.351986 -0.000000 -0.000000", 0.999997);
SpawnObject("XmasGiftRed1", "4309.562988 338.529541 10494.139648", "52.885433 -0.000000 -0.000000", 0.999997);
SpawnObject("XmasGiftRed2", "4313.593262 338.527893 10492.568359", "-178.608856 0.000000 0.000000", 0.999999);
SpawnObject("XmasGiftGreen2", "4313.958984 338.535370 10491.145508", "-89.999954 0.000000 0.000000", 1);
SpawnObject("XmasGiftGreen1", "4308.228027 338.539246 10493.499023", "52.885433 -0.000000 -0.000000", 0.999997);
SpawnObject("XmasGiftBlue1", "4310.857910 338.521545 10494.762695", "52.885433 -0.000000 -0.000000", 0.999997);
SpawnObject("XmasGiftBlue2", "4313.137695 338.518707 10494.824219", "44.999992 -0.000000 -0.000000", 1);
SpawnObject("XmasGiftRed1", "4310.504883 338.520111 10495.657227", "49.684250 -0.000000 -0.000000", 0.999985);
SpawnObject("XmasGiftRed2", "4313.586914 338.524658 10493.359375", "178.833282 0.000000 0.000000", 0.999987);
SpawnObject("XmasGiftGreen2", "4313.056641 338.537750 10491.114258", "-93.200531 0.000000 0.000000", 0.999986);
SpawnObject("XmasGiftGreen1", "4308.565430 338.531982 10494.851563", "49.684303 -0.000000 -0.000000", 0.999983);
SpawnObject("XmasGiftBlue1", "4309.503418 338.525665 10495.263672", "49.684303 -0.000000 -0.000000", 0.999983);
SpawnObject("XmasGiftBlue2", "4312.320801 338.521149 10494.375000", "-131.875366 -0.000000 -0.000000", 0.999993);
SpawnObject("StaticObj_Misc_SupplyBox1_DE", "4317.499023 338.814056 10481.310547", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Misc_SupplyBox2_DE", "4312.030762 339.110962 10478.672852", "27.000000 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Misc_SupplyBox3_DE", "4308.341309 339.072296 10484.734375", "27.000000 -0.000000 -0.000000", 1);


// Uncomment if you want to export loot from newly added buildings
// Position, Radius (increase if you have a larger map than Chernarus)
// GetCEApi().ExportProxyData(Vector(7500, GetGame().SurfaceY(7500, 7500), 7500), 20000);
