#include "mapgroup.h"

MapGroup::MapGroup(int _id, QString _name, QObject *parent) :
    MapSpirit(_id,_name,Map_Sprite_Type_Group,parent)
{

}


MapGroup::MapGroup(const MapGroup& b):
    MapSpirit(b),
    spirits(b.spirits),
    agvs(b.agvs)
{
}

void MapGroup::init(QList<int> _spirits,QList<int> _agvs)
{
    spirits = _spirits;
    agvs = _agvs;
}
