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
SpawnObject("Land_Tenement_Small", "4686.379883 348.032013 10338.200195", "0.000000 0.000000 0.000000", 0.999998);
SpawnObject("AssaultBag_Ttsko", "4696.878418 352.500000 10336.928711", "90.000000 0.000000 0.000000", 0.999984);
SpawnObject("TTSKOPants", "4696.978516 352.700012 10336.382813", "0.000000 0.000000 -84.999985", 0.999998);
SpawnObject("TTsKOJacket_Camo", "4696.990723 352.700012 10335.690430", "-3.499998 -24.998087 -90.000000", 0.999988);
SpawnObject("TTSKOBoots", "4696.810059 350.709991 10337.000000", "69.499992 0.000000 0.000000", 0.999988);
SpawnObject("BoonieHat_Dubok", "4696.984863 352.095001 10336.396484", "90.000000 0.000000 0.000000", 0.999986);
SpawnObject("TacticalGloves_Green", "4696.790039 350.700012 10336.599609", "0.000000 0.000000 0.000000", 0.999999);
SpawnObject("BalaclavaMask_Blackskull", "4696.977051 352.000000 10335.585938", "-79.999985 -7.997601 -79.999985", 0.99995);
SpawnObject("AKM", "4696.962402 352.583008 10334.900391", "-90.000000 0.000000 0.000000", 0.999985);
SpawnObject("CZ75", "4696.994629 352.114014 10335.037109", "-90.000000 0.000000 0.000000", 0.999985);
SpawnObject("Barrel_Green", "4696.049316 350.726990 10333.148438", "0.000000 0.000000 0.000000", 1);
SpawnObject("Barrel_Green", "4695.983887 350.726990 10332.451172", "0.000000 0.000000 0.000000", 1);
SpawnObject("Barrel_Green", "4695.360840 350.726990 10333.223633", "0.000000 0.000000 0.000000", 1);
SpawnObject("Barrel_Green", "4695.295410 350.726990 10332.526367", "0.000000 0.000000 0.000000", 1);
SpawnObject("AssaultBag_Ttsko", "4696.850098 352.500000 10334.034180", "90.000000 0.000000 0.000000", 0.999984);
SpawnObject("TTSKOPants", "4696.950195 352.700012 10333.488281", "0.000000 0.000000 -84.999985", 0.999998);
SpawnObject("TTsKOJacket_Camo", "4696.962402 352.700012 10332.795898", "-3.499997 -24.997759 -90.000000", 0.999988);
SpawnObject("TTSKOBoots", "4696.781738 350.709991 10334.105469", "69.499992 0.000000 0.000000", 0.999988);
SpawnObject("BoonieHat_Dubok", "4696.956543 352.095001 10333.501953", "90.000000 0.000000 0.000000", 0.999986);
SpawnObject("TacticalGloves_Green", "4696.761719 350.700012 10333.705078", "0.000000 0.000000 0.000000", 0.999999);
SpawnObject("BalaclavaMask_Blackskull", "4696.948730 352.000000 10332.691406", "-79.999985 -7.997198 -79.999985", 0.99995);
SpawnObject("AKM", "4696.934082 352.583008 10332.005859", "-90.000000 0.000000 0.000000", 0.999985);
SpawnObject("CZ75", "4696.966309 352.114014 10332.142578", "-90.000000 0.000000 0.000000", 0.999985);


// Uncomment if you want to export loot from newly added buildings
// Position, Radius (increase if you have a larger map than Chernarus)
// GetCEApi().ExportProxyData(Vector(7500, GetGame().SurfaceY(7500, 7500), 7500), 20000);
