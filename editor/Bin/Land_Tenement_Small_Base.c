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
SpawnObject("Land_Tenement_Small", "4658.930176 350.032013 10335.799805", "-27.000099 0.000000 0.000000", 1);
SpawnObject("StaticObj_Furniture_shelf_1side", "4666.049805 342.472992 10345.200195", "-27.000000 0.000000 0.000000", 0.999996);
SpawnObject("StaticObj_Furniture_shelf_1side", "4667.017578 342.500000 10343.315430", "-26.197598 0.000000 0.000000", 0.999962);
SpawnObject("StaticObj_Furniture_case_d", "4667.465820 342.589996 10341.953125", "-117.000000 0.000000 0.000000", 0.999998);
SpawnObject("StaticObj_Furniture_lobby_table", "4667.759766 342.500000 10340.200195", "-27.000000 0.000000 0.000000", 1);
SpawnObject("StaticObj_Furniture_Anvil", "4668.290039 343.100006 10340.500000", "63.000000 0.000000 0.000000", 1);
SpawnObject("StaticObj_Furniture_shelf_DZ", "4666.614258 342.367340 10339.634766", "-116.276024 -0.000000 -0.000000", 0.999999);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4662.776855 343.209015 10346.870117", "62.618496 0.000000 0.000000", 1.315);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4662.776855 346.557526 10346.870117", "62.618496 0.000000 0.000000", 1.315);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4662.776855 349.980988 10346.870117", "62.618496 0.000000 0.000000", 1.315);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4662.776855 353.264282 10346.870117", "62.618496 0.000000 0.000000", 1.315);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4662.776855 356.631927 10346.870117", "62.618496 0.000000 0.000000", 1.315);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4659.450195 342.989990 10343.599609", "63.000000 0.000000 0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4652.010254 343.180206 10339.616211", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4647.220215 342.959015 10338.900391", "63.000000 0.000000 0.000000", 1.315);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4647.220215 346.590881 10338.900391", "63.000000 0.000000 0.000000", 1.315);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4647.220215 349.847595 10338.900391", "63.000000 0.000000 0.000000", 1.315);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4647.220215 353.481873 10338.900391", "63.000000 0.000000 0.000000", 1.315);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4647.220215 356.899323 10338.900391", "63.000000 0.000000 0.000000", 1.315);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4659.450195 346.399994 10343.599609", "63.000000 0.000000 0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4652.009766 346.399994 10339.599609", "63.000000 0.000000 0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4659.450195 349.781342 10343.599609", "63.000000 0.000000 0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4659.450195 353.149994 10343.599609", "63.000000 0.000000 0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4659.450195 356.600006 10343.599609", "63.000000 0.000000 0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4652.009766 349.895630 10339.599609", "63.000000 0.000000 0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4652.009766 353.250549 10339.599609", "63.000000 0.000000 0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4652.009766 356.658600 10339.599609", "63.000000 0.000000 0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4653.890137 343.000000 10325.700195", "-117.000000 0.000000 0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4653.890137 346.238525 10325.700195", "-117.000000 0.000000 0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4653.890137 349.906097 10325.700195", "-117.000000 0.000000 0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4653.890137 353.258118 10325.700195", "-117.000000 0.000000 0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4653.890137 356.759460 10325.700195", "-117.000000 0.000000 0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4657.443848 342.980927 10327.447266", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4657.443848 346.445374 10327.447266", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4657.443848 350.090973 10327.447266", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4657.443848 353.226624 10327.447266", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4657.443848 356.795197 10327.447266", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4665.908203 342.931213 10331.626953", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4669.588867 342.937592 10333.594727", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4665.908203 346.334351 10331.626953", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4665.908203 349.928131 10331.626953", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4665.908203 353.277222 10331.626953", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4665.908203 356.779175 10331.626953", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4669.588867 346.281708 10333.594727", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4669.588867 349.808746 10333.594727", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4669.588867 353.294647 10333.594727", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4669.588867 356.800232 10333.594727", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4662.548340 343.036133 10328.128906", "63.000000 0.000000 0.000000", 1.315);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4662.548340 346.461243 10328.128906", "63.000000 0.000000 0.000000", 1.315);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4662.548340 349.897186 10328.128906", "63.000000 0.000000 0.000000", 1.315);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4662.548340 353.365845 10328.128906", "63.000000 0.000000 0.000000", 1.315);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4662.548340 356.908325 10328.128906", "63.000000 0.000000 0.000000", 1.315);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4655.703613 346.381042 10341.511719", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4655.703613 349.783661 10341.511719", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4655.703613 353.155548 10341.511719", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4655.703613 356.470001 10341.511719", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Furniture_lobby_table", "4657.640137 342.500000 10335.599609", "152.447006 0.000000 0.000000", 1);
SpawnObject("StaticObj_Furniture_lobby_table", "4656.397949 342.500000 10338.039063", "152.447006 0.000000 0.000000", 1);
SpawnObject("StaticObj_Furniture_shelf_1side", "4661.140137 345.850006 10335.200195", "-28.546797 0.000000 0.000000", 0.999993);
SpawnObject("StaticObj_Furniture_shelf_1side", "4662.137207 345.850006 10333.361328", "-28.546795 0.000000 0.000000", 0.999993);
SpawnObject("StaticObj_Furniture_shelf_1side", "4663.089355 345.850006 10331.491211", "-28.546793 0.000000 0.000000", 0.999993);
SpawnObject("StaticObj_Furniture_tools_racking", "4658.697754 345.924072 10331.806641", "64.079773 -0.000000 -0.000000", 0.999993);
SpawnObject("StaticObj_Furniture_tools_racking", "4658.195801 345.924072 10332.826172", "64.079773 -0.000000 -0.000000", 0.999993);
SpawnObject("StaticObj_Furniture_bag_DZ", "4668.270996 343.857452 10335.544922", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Furniture_canister_DZ", "4669.390625 342.855835 10337.409180", "-110.883835 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Furniture_shelf_1side", "4666.140137 342.500000 10332.500000", "58.355690 0.000000 0.000000", 0.999993);
SpawnObject("StaticObj_Furniture_shelf_1side", "4664.434082 342.500000 10331.426758", "58.355690 0.000000 0.000000", 0.999993);
SpawnObject("StaticObj_Statue_Enoch1", "4650.072266 350.424988 10333.476563", "-26.291327 0.000000 0.000000", 0.300001);
SpawnObject("StaticObj_Furniture_Couch_Beige84x213x75_DZ", "4646.972656 349.267181 10336.915039", "-27.234470 -0.000000 -0.000000", 0.999997);
SpawnObject("StaticObj_Furniture_shelfs_small", "4651.029785 349.299988 10329.299805", "64.372894 0.000000 0.000000", 0.999993);
SpawnObject("StaticObj_Furniture_shelfs_small", "4652.027832 349.299988 10329.836914", "64.372894 0.000000 0.000000", 0.999993);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4654.813965 343.000000 10326.138672", "-117.000000 0.000000 0.000000", 1);
SpawnObject("StaticObj_Furniture_shelfs_small", "4652.951660 349.299988 10330.275391", "64.372894 0.000000 0.000000", 0.999993);
SpawnObject("CoyoteBag_Green", "4659.640137 353.726990 10340.716797", "154.000000 0.000000 0.000000", 0.99999);
SpawnObject("AKM", "4658.240234 353.899994 10341.299805", "64.008682 0.000000 0.000000", 0.999998);
SpawnObject("StaticObj_lobby_table", "4660.853516 352.700012 10343.284180", "154.129852 0.000000 0.000000", 1);
SpawnObject("StaticObj_lobby_table", "4659.420898 352.700012 10342.570313", "154.129852 0.000000 0.000000", 1);
SpawnObject("TTSKOPants", "4658.951172 353.160004 10342.214844", "-125.101685 0.000000 0.000000", 0.999999);
SpawnObject("TTsKOJacket_Camo", "4659.421875 353.200012 10342.527344", "-136.278091 0.000000 0.000000", 0.999999);
SpawnObject("TacticalGloves_Green", "4659.979980 353.170013 10342.700195", "0.000000 0.000000 0.000000", 1);
SpawnObject("TTSKOBoots", "4659.870117 353.179993 10343.000000", "0.000000 0.000000 0.000000", 0.999999);
SpawnObject("MilitaryBelt", "4658.830566 353.200012 10342.595703", "152.774979 0.000000 0.000000", 1);
SpawnObject("BoonieHat_Dubok", "4660.149902 353.174988 10343.200195", "-179.999969 -88.365295 -179.999969", 0.999984);
SpawnObject("AKM", "4658.269043 353.500000 10341.312500", "64.008682 0.000000 0.000000", 0.999998);
SpawnObject("PlateCarrierVest_Camo", "4660.746094 353.200012 10343.166992", "-125.063995 0.000000 0.000000", 1);
SpawnObject("Bear_Pink", "4660.433105 353.279999 10342.913086", "0.000000 0.000000 0.000000", 0.999999);
SpawnObject("Flag_Snake", "4661.263672 353.200012 10343.397461", "64.999992 0.000000 0.000000", 0.999998);
SpawnObject("HuntingBag_Hannah", "4668.100098 353.584991 10340.000000", "151.000000 0.000000 0.000000", 0.999988);
SpawnObject("HuntingBag_Hannah", "4667.397949 353.584991 10339.641602", "151.000000 0.000000 0.000000", 0.999988);
SpawnObject("HuntingBag_Hannah", "4666.625488 353.584991 10339.287109", "151.000000 0.000000 0.000000", 0.999988);
SpawnObject("HuntingBag_Hannah", "4668.095215 354.285004 10340.020508", "151.000000 0.000000 0.000000", 0.999988);
SpawnObject("HuntingBag_Hannah", "4667.393066 354.285004 10339.662109", "151.000000 0.000000 0.000000", 0.999988);
SpawnObject("HuntingBag_Hannah", "4666.620605 354.285004 10339.307617", "151.000000 0.000000 0.000000", 0.999987);
SpawnObject("Barrel_Green", "4661.819336 352.693604 10344.423828", "149.278427 -0.000000 -0.000000", 1);
SpawnObject("Barrel_Green", "4662.674805 352.693604 10344.895508", "149.278427 -0.000000 -0.000000", 1);
SpawnObject("Barrel_Green", "4663.507813 352.693604 10345.321289", "149.278427 -0.000000 -0.000000", 1);
SpawnObject("Barrel_Green", "4664.407227 352.693604 10345.828125", "149.278427 -0.000000 -0.000000", 1);
SpawnObject("HuntingBag_Hannah", "4665.914551 353.584991 10338.949219", "151.000000 0.000000 0.000000", 0.999988);
SpawnObject("HuntingBag_Hannah", "4665.909668 354.285004 10338.969727", "151.000000 0.000000 0.000000", 0.999987);
SpawnObject("HuntingBag_Hannah", "4665.241699 353.584991 10338.635742", "151.000000 0.000000 0.000000", 0.999988);
SpawnObject("HuntingBag_Hannah", "4665.236816 354.285004 10338.656250", "151.000000 0.000000 0.000000", 0.999987);
SpawnObject("StaticObj_Roadblock_Bags_Curve", "4664.104980 361.022583 10345.247070", "98.999962 0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Curve", "4668.894043 361.022583 10335.104492", "-143.969101 0.000000 -0.000000", 0.999957);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4668.609863 360.250000 10339.099609", "-27.000000 0.000000 0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4666.930664 360.270721 10342.542969", "-27.000000 0.000000 0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4667.844727 360.270691 10340.777344", "-27.000000 0.000000 0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Curve", "4653.009277 360.747803 10326.843750", "-82.923225 0.000000 -0.000000", 0.99998);
SpawnObject("StaticObj_Roadblock_Bags_Curve", "4647.884277 360.747803 10336.830078", "34.105324 0.000000 -0.000000", 0.999933);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4648.309082 359.975220 10332.854492", "151.070847 0.000000 0.000000", 0.999967);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4650.089844 359.995941 10329.440430", "151.070847 0.000000 0.000000", 0.999967);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4649.119629 359.995911 10331.267578", "151.070847 0.000000 0.000000", 0.999967);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4656.834961 360.200012 10327.289063", "63.717690 0.000000 0.000000", 0.999989);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4659.716797 360.250000 10328.742188", "63.717499 0.000000 0.000000", 0.999969);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4663.752930 360.300018 10330.861328", "63.717583 0.000000 0.000000", 0.999967);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4666.459473 360.372314 10332.314453", "63.717472 -0.000000 -0.000000", 0.999968);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4659.806641 360.050262 10343.668945", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4657.030762 360.050293 10342.119141", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4654.193359 360.050293 10340.625977", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4651.326172 360.050262 10339.142578", "62.999973 -0.000000 -0.000000", 1);
SpawnObject("StaticObj_Roadblock_Bags_Long", "4661.795898 360.300018 10329.895508", "63.717583 0.000000 0.000000", 0.999967);
SpawnObject("Land_Misc_DeerStand1", "4663.794434 363.100006 10344.046875", "-154.785965 0.000000 0.000000", 1);
SpawnObject("Land_Misc_DeerStand1", "4648.635254 363.100006 10336.156250", "99.193993 0.000000 0.000000", 0.999994);
SpawnObject("Land_Misc_DeerStand1", "4653.240234 363.299988 10328.200195", "8.999820 0.000000 0.000000", 1);
SpawnObject("Land_Misc_DeerStand1", "4667.792480 363.158630 10335.881836", "-69.936386 -0.000000 -0.000000", 0.999997);
SpawnObject("Land_Misc_Well_Pump_Yellow", "4668.062988 342.831360 10338.023438", "150.526535 -0.000000 -0.000000", 1);
SpawnObject("Land_FuelStation_Feed", "4670.180664 343.299988 10335.879883", "-116.193054 0.000000 0.000000", 0.999995);
SpawnObject("StaticObj_Platform1_Ramp", "4649.604980 339.149994 10353.077148", "-117.000000 0.000000 0.000000", 1);
SpawnObject("NonStrategic_Base", "4666.171875 339.000000 10344.954102", "17.999987 -0.000000 -0.000000", 1);


// Uncomment if you want to export loot from newly added buildings
// Position, Radius (increase if you have a larger map than Chernarus)
// GetCEApi().ExportProxyData(Vector(7500, GetGame().SurfaceY(7500, 7500), 7500), 20000);
