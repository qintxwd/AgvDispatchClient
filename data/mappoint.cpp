#include "mappoint.h"

MapPoint::MapPoint(int _id, QString _name, Map_Point_Type _type, int _x, int _y, int _realX, int _realY, int _labelXoffset, int _labelYoffset, bool _mapChange, QObject *parent):
    MapSpirit(_id,_name,Map_Sprite_Type_Point,parent),
    x(_x),
    y(_y),
    labelXoffset(_labelXoffset),
    labelYoffset(_labelYoffset),
    mapChange(_mapChange),
    realX(_realX),
    realY(_realY),
    point_type(_type)
{
}

MapPoint::MapPoint(const MapPoint &p):
    MapSpirit(p),
    x(p.x),
    y(p.y),
    realX(p.realX),
    realY(p.realY),
    labelXoffset(p.labelXoffset),
    labelYoffset(p.labelYoffset),
    mapChange(p.mapChange),
    point_type(p.point_type)
{
}
