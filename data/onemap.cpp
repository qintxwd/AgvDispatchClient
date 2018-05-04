#include "onemap.h"

OneMap::OneMap(QObject *parent):
    QObject(parent),
    max_id(0)
{

}

OneMap::OneMap(const OneMap &b)
{
    max_id = b.max_id;
    floors = b.floors;
    paths = b.paths;
}

OneMap::~OneMap()
{
    qDeleteAll(floors);
    qDeleteAll(paths);
}

void OneMap::addPath(MapPath *path)
{
    paths.append(path);
}

void OneMap::addFloor(MapFloor *floor)
{
    floors.append(floor);
}

void OneMap::removePath(MapPath *path)
{
    paths.removeAll(path);
}
void OneMap::removeFloor(MapFloor *floor)
{
    floors.removeAll(floor);
}

void OneMap::removePathById(int id)
{
    foreach (auto p, paths) {
        if(p->getId() == id){
            paths.removeAll(p);
            delete p;
            break;
        }
    }
}
void OneMap::removeFloorById(int id)
{
    foreach (auto f, floors) {
        if(f->getId() == id){
            floors.removeAll(f);
            delete f;
            break;
        }
    }
}

int OneMap::getNextId()
{
    return ++max_id;
}

//复制整个地图
OneMap *OneMap::clone()
{
    OneMap *onemap = new OneMap;
    onemap->setMaxId(max_id);
    foreach (auto p, paths) {
        onemap->addPath(new MapPath(*p));
    }
    foreach (auto f, floors) {
        onemap->addFloor(f->clone());
    }
    return onemap;
}

MapFloor *OneMap::getFloorById(int id)
{
    foreach (auto p, floors) {
        if(p->getId() == id)return p;
    }

    return nullptr;
}

MapPath *OneMap::getPathById(int id)
{
    foreach (auto p, paths) {
        if(p->getId() == id)return p;
    }

    return nullptr;
}

