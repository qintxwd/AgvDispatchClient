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
    rootpaths = b.rootpaths;
}

OneMap::~OneMap()
{
    qDeleteAll(floors);
    qDeleteAll(rootpaths);
    qDeleteAll(blocks);
    qDeleteAll(groups);
}

void OneMap::clear()
{
    qDeleteAll(floors);
    qDeleteAll(rootpaths);
    qDeleteAll(blocks);
    qDeleteAll(groups);
    max_id = 0;
}

void OneMap::addPath(MapPath *path)
{
    rootpaths.append(path);
}

void OneMap::addFloor(MapFloor *floor)
{
    floors.append(floor);
}

void OneMap::addBlock(MapBlock *block)
{
    blocks.append(block);
}

void  OneMap::addGroup(MapGroup *group)
{
    groups.append(group);
}

void OneMap::removeBlock(MapBlock *block)
{
    blocks.removeAll(block);
}

void OneMap::removeGroup(MapGroup *group)
{
    groups.removeAll(group);
}

void OneMap::removePath(MapPath *path)
{
    rootpaths.removeAll(path);
}
void OneMap::removeFloor(MapFloor *floor)
{
    floors.removeAll(floor);
}

void OneMap::removeRootPathById(int id)
{
    foreach (auto p, rootpaths) {
        if(p->getId() == id){
            rootpaths.removeAll(p);
            break;
        }
    }
}
void OneMap::removeFloorById(int id)
{
    foreach (auto f, floors) {
        if(f->getId() == id){
            floors.removeAll(f);
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
    foreach (auto p, rootpaths) {
        onemap->addPath(new MapPath(*p));
    }
    foreach (auto f, floors) {
        onemap->addFloor(f->clone());
    }
    foreach (auto b, blocks) {
        onemap->addBlock(new MapBlock(*b));
    }
    foreach (auto b, groups) {
        onemap->addGroup(new MapGroup(*b));
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

MapPath *OneMap::getRootPathById(int id)
{
    foreach (auto p, rootpaths) {
        if(p->getId() == id)return p;
    }

    return nullptr;
}

MapBlock *OneMap::getBlockById(int id)
{
    foreach (auto b, blocks) {
        if(b->getId() == id)return b;
    }
    return nullptr;
}

MapGroup *OneMap::getGroupById(int id)
{
    foreach (auto b, groups) {
        if(b->getId() == id)return b;
    }
    return nullptr;
}

MapSpirit *OneMap::getSpiritById(int id)
{
    MapFloor *f = getFloorById(id);
    if(f!=nullptr)return f;
    MapPath *p = getRootPathById(id);
    if(p)return p;
    MapBlock *b = getBlockById(id);
    if(b)return b;
    MapGroup *g = getGroupById(id);
    if(g)return g;

    foreach (auto floor, floors) {
        foreach (auto pa, floor->getPaths()) {
            if(pa->getId() == id)return pa;
        }
        foreach (auto po, floor->getPoints()) {
            if(po->getId() == id)return po;
        }
    }

    return nullptr;
}

