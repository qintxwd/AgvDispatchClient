#include "onemap.h"

OneMap::OneMap():
    max_id(0)
{

}

OneMap::~OneMap()
{
    qDeleteAll(spirits);
}

int OneMap::getNextId()
{
    return ++max_id;
}

//注册一个新的元素
void OneMap::addSpirit(MapSpirit *spirit)
{
    spirits.append(spirit);
}

//删除一个元素
void OneMap::removeSpirit(int id)
{
    foreach (auto spirit, spirits) {
        if(spirit->getId() == id){
            spirits.removeAll(spirit);
            break;
        }
    }
}

//删除一个元素
void OneMap::removeSpirit(MapSpirit *spirit)
{
    spirits.removeAll(spirit);
}

MapSpirit *OneMap::getSpiritById(int id)
{
    foreach (auto spirit, spirits) {
        if(spirit->getId() == id){
            return spirit;
        }
    }
    return nullptr;
}

QList<MapSpirit *> OneMap::getSpiritByType(MapSpirit::Map_Spirit_Type type)
{
    QList<MapSpirit *> l;
    foreach (auto spirit, spirits) {
        if(spirit->getSpiritType() == type){
            l.push_back(spirit);
        }
    }
    return l;
}

//复制整个地图
OneMap OneMap::clone()
{
    OneMap onemap;
    foreach (auto spirit, spirits) {
        MapSpirit *p = nullptr;
        if(spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Point){
            MapPoint *pl = new MapPoint(*( static_cast<MapPoint *>(spirit) ));
            p = pl;
        }else if(spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Path){
            MapPath *pl = new MapPath(*( static_cast<MapPath *>(spirit) ));
            p = pl;
        }else if(spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Floor){
            MapFloor *pl = new MapFloor(*( static_cast<MapFloor *>(spirit) ));
            p = pl;
        }
        if(p == nullptr){
            continue;
        }
        onemap.addSpirit(p);
    }
    onemap.max_id = max_id;
    onemap.background = background;
    onemap.imgFileName = imgFileName;

    return onemap;
}

