#include "mappath.h"

MapPath::MapPath(int _id, QString _name, int _start, int _end, Map_Path_Type _type, int _length, int _p1x, int _p1y, int _p2x, int _p2y):
    start(_start),
    end(_end),
    length(_length),
    path_type(_type),
    p1x(_p1x),
    p1y(_p1y),
    p2x(p2x),
    p2y(p2y)
{
    setId(_id);
    setName(_name);
    setSpiritType(Map_Sprite_Type_Path);
}
