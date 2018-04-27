#include "mapfloor.h"

MapFloor::MapFloor(int _id, QString _name)
{
    setId(_id);
    setName(_name);
    setSpiritType(Map_Sprite_Type_Floor);
}
void MapFloor::addPoint(int p)
{
    points.push_back(p);
}
void MapFloor::addPath(int p)
{
    paths.push_back(p);
}
