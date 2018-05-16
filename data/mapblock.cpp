#include "mapblock.h"

MapBlock::MapBlock(int _id, std::string _name, QObject *parent) :
    MapSpirit(_id,_name,Map_Sprite_Type_Block,parent)
{

}

MapBlock::MapBlock(const MapBlock& b):
    MapSpirit(b),
    spirits(b.spirits)
{
}
