#include "mapbackground.h"

MapBackground::MapBackground(int _id, QString _name, int _x, int _y, int _width, int _height)
    :x(_x),
      y(_y),
      width(_width),
      height(_height)
{
    setId(_id);
    setName(_name);
    setSpiritType(Map_Sprite_Type_Background);
}
