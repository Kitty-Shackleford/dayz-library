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
SpawnObject("MountainBag_Red", "10286.804688 19.712908 2490.661865", "0.000000 0.000000 -4.999999", 0.999998);
SpawnObject("AKM", "10286.580078 19.968107 2490.569336", "-90.000000 0.000000 90.000000", 0.999995);
SpawnObject("TaloonBag_Violet", "10284.608398 19.694706 2490.797119", "0.000000 0.000000 0.000000", 1);
SpawnObject("Glock19", "10284.500000 19.799999 2490.659912", "-179.999969 0.000000 0.000000", 1);
SpawnObject("AliceBag_Camo", "10282.425781 19.763504 2490.783936", "0.000000 0.000000 0.000000", 1);
SpawnObject("M67Grenade", "10282.313477 20.138714 2490.656494", "0.000000 0.000000 0.000000", 0.999999);
SpawnObject("M67Grenade", "10282.366211 20.139576 2490.648438", "0.000000 0.000000 0.000000", 0.999999);
SpawnObject("M67Grenade", "10282.507813 20.141121 2490.641113", "0.000000 0.000000 0.000000", 0.999999);
SpawnObject("M67Grenade", "10282.436523 20.140259 2490.645264", "0.000000 0.000000 0.000000", 0.999999);
SpawnObject("M67Grenade", "10282.560547 20.141861 2490.638916", "0.000000 0.000000 0.000000", 0.999999);
SpawnObject("M4A1", "10282.660156 20.167639 2490.891113", "71.000000 0.000000 73.000008", 0.99999);
SpawnObject("Ammo_GrenadeM4", "10281.541016 19.702190 2491.331787", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("CombatKnife", "10282.352539 19.987700 2490.560059", "0.000000 -7.999846 90.000000", 0.999981);
SpawnObject("Canteen", "10282.205078 20.226299 2490.848877", "-88.686089 0.000000 19.999996", 0.999995);


// Uncomment if you want to export loot from newly added buildings
// Position, Radius (increase if you have a larger map than Chernarus)
// GetCEApi().ExportProxyData(Vector(7500, GetGame().SurfaceY(7500, 7500), 7500), 20000);
