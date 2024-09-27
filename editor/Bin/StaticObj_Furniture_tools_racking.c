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
SpawnObject("StaticObj_Furniture_tools_racking", "4643.100098 338.989990 10353.099609", "0.000000 0.000000 0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.494629 339.086090 10353.059570", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.492676 339.086060 10353.216797", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.333496 339.086121 10353.054688", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.331543 339.086090 10353.211914", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.179688 339.086121 10353.054688", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.177734 339.086090 10353.211914", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.029297 339.086121 10353.052734", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.027344 339.086090 10353.209961", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4642.875488 339.086060 10353.050781", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4642.873535 339.086029 10353.208008", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4642.720703 339.086182 10353.050781", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4642.718750 339.086151 10353.208008", "0.000000 -0.000000 -0.000000", 0.999999);
SpawnObject("GiftBox_Small_1", "4643.490234 340.573334 10353.037109", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.488281 340.573303 10353.194336", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.329102 340.573364 10353.032227", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.327148 340.573334 10353.189453", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.175293 340.573364 10353.032227", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.173340 340.573334 10353.189453", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.024902 340.573364 10353.030273", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.022949 340.573334 10353.187500", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4642.871094 340.573303 10353.028320", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4642.869141 340.573273 10353.185547", "0.000000 -0.000000 -0.000000", 0.999999);
SpawnObject("GiftBox_Small_1", "4642.716309 340.573425 10353.028320", "0.000000 -0.000000 -0.000000", 0.999999);
SpawnObject("GiftBox_Small_1", "4642.714355 340.573395 10353.185547", "0.000000 -0.000000 -0.000000", 0.999999);
SpawnObject("GiftBox_Small_1", "4643.480957 340.092316 10353.079102", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.479004 340.092285 10353.236328", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.319824 340.092346 10353.074219", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.317871 340.092316 10353.231445", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.166016 340.092346 10353.074219", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.164063 340.092316 10353.231445", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.015625 340.092346 10353.072266", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.013672 340.092316 10353.229492", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4642.861816 340.092285 10353.070313", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4642.859863 340.092255 10353.227539", "0.000000 -0.000000 -0.000000", 0.999999);
SpawnObject("GiftBox_Small_1", "4642.707031 340.092407 10353.070313", "0.000000 -0.000000 -0.000000", 0.999999);
SpawnObject("GiftBox_Small_1", "4642.705078 340.092377 10353.227539", "0.000000 -0.000000 -0.000000", 0.999999);
SpawnObject("GiftBox_Small_1", "4643.483398 339.572479 10353.068359", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.481445 339.572449 10353.225586", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.322266 339.572510 10353.063477", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.320313 339.572479 10353.220703", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.168457 339.572510 10353.063477", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.166504 339.572479 10353.220703", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.018066 339.572510 10353.061523", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.016113 339.572479 10353.218750", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4642.864258 339.572449 10353.059570", "0.000000 -0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4642.862305 339.572418 10353.216797", "0.000000 -0.000000 -0.000000", 0.999999);
SpawnObject("GiftBox_Small_1", "4642.709473 339.572571 10353.059570", "0.000000 -0.000000 -0.000000", 0.999999);
SpawnObject("GiftBox_Small_1", "4642.707520 339.572540 10353.216797", "0.000000 -0.000000 -0.000000", 0.999999);


// Uncomment if you want to export loot from newly added buildings
// Position, Radius (increase if you have a larger map than Chernarus)
// GetCEApi().ExportProxyData(Vector(7500, GetGame().SurfaceY(7500, 7500), 7500), 20000);
