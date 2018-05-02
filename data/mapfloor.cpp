#include "mapfloor.h"

MapFloor::MapFloor(int _id, QString _name, OneMap *parentMap):
    MapSpirit(_id,_name,Map_Sprite_Type_Floor),
    bkg(nullptr),
    onemap(parentMap)
{
}

MapFloor::~MapFloor()
{
    if(bkg)delete bkg;
    qDeleteAll(points);
    qDeleteAll(paths);
}

//复制地图（深copy）
MapFloor* MapFloor::clone()
{
    MapFloor *f = new MapFloor(getId(),getName(),onemap);
    foreach (auto p, points) {
        MapPoint *newp = new MapPoint(*p);
        f->addPoint(newp);
    }

    foreach (auto p, paths) {
        MapPath *newp = new MapPath(*p);
        f->addPath(newp);
    }

    if(bkg){
        MapBackground *b = new MapBackground(*bkg);
        f->setBkg(b);
    }
    return f;
}

MapPoint *MapFloor::getPointById(int id)
{
    foreach (auto p, points) {
        if(p->getId() == id)return p;
    }
    return nullptr;
}

MapPath *MapFloor::getPathById(int id)
{
    foreach (auto p, paths) {
        if(p->getId() == id)return p;
    }
    return nullptr;
}
