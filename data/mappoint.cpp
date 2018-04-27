#include "mappoint.h"

MapPoint::MapPoint(int _id,QString _name,int _x, int _y):
    x(_x),
    y(_y),
    labelXoffset(0),
    labelYoffset(-20),
    mapChange(false),
    realX(0),
    realY(0)
{
    setId(_id);
    setName(_name);
    setSpiritType(Map_Sprite_Type_Point);
}
