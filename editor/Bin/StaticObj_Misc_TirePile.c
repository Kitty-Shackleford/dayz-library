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
SpawnObject("StaticObj_Misc_TirePile", "4342.722168 342.648102 10443.251953", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("HatchbackWheel", "4342.536621 338.735199 10444.085938", "-117.000000 0.000000 -45.000000", 0.999988);
SpawnObject("CivSedanWheel", "4341.958984 338.679230 10443.352539", "0.000000 0.000000 -135.000000", 0.999995);
SpawnObject("Sedan_02_Wheel", "4342.463379 338.761749 10442.458984", "-65.316895 0.000000 45.000000", 0.999994);
SpawnObject("Truck_01_Wheel", "4343.640137 338.850128 10442.799805", "-152.999985 0.000000 24.000000", 0.999996);
SpawnObject("Offroad_02_Wheel", "4343.759766 338.817352 10443.799805", "152.999985 0.000000 21.499998", 0.999998);


// Uncomment if you want to export loot from newly added buildings
// Position, Radius (increase if you have a larger map than Chernarus)
// GetCEApi().ExportProxyData(Vector(7500, GetGame().SurfaceY(7500, 7500), 7500), 20000);
