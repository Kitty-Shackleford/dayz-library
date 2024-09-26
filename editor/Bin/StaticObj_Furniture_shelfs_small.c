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
SpawnObject("StaticObj_Furniture_shelfs_small", "4643.356445 339.009857 10352.041016", "0.000000 0.000000 0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.282715 339.984497 10352.356445", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.443848 339.984497 10352.362305", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.287109 339.984497 10351.896484", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.448242 339.984497 10351.902344", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.284668 339.984467 10352.048828", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.445801 339.984467 10352.054688", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.448242 339.984497 10351.902344", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.284668 339.984467 10352.048828", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.445801 339.984467 10352.054688", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.285156 339.984528 10352.204102", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.446289 339.984528 10352.209961", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.282715 339.984497 10352.356445", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.443848 339.984497 10352.362305", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.446289 339.984528 10352.209961", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.280762 339.624329 10352.390625", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.441895 339.624329 10352.396484", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.285156 339.624329 10351.930664", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.446289 339.624329 10351.936523", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.282715 339.624298 10352.083008", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.443848 339.624298 10352.088867", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.446289 339.624329 10351.936523", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.282715 339.624298 10352.083008", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.443848 339.624298 10352.088867", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.283203 339.624359 10352.238281", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.444336 339.624359 10352.244141", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.280762 339.624329 10352.390625", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.441895 339.624329 10352.396484", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.444336 339.624359 10352.244141", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.295898 339.326355 10352.365234", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.457031 339.326355 10352.371094", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.300293 339.326355 10351.905273", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.461426 339.326355 10351.911133", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.297852 339.326324 10352.057617", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.458984 339.326324 10352.063477", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.461426 339.326355 10351.911133", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.297852 339.326324 10352.057617", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.458984 339.326324 10352.063477", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.298340 339.326385 10352.212891", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.459473 339.326385 10352.218750", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.295898 339.326355 10352.365234", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.457031 339.326355 10352.371094", "0.000000 0.000000 -0.000000", 1);
SpawnObject("GiftBox_Small_1", "4643.459473 339.326385 10352.218750", "0.000000 0.000000 -0.000000", 1);


// Uncomment if you want to export loot from newly added buildings
// Position, Radius (increase if you have a larger map than Chernarus)
// GetCEApi().ExportProxyData(Vector(7500, GetGame().SurfaceY(7500, 7500), 7500), 20000);
