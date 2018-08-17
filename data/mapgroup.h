#ifndef MAPGROUP_H
#define MAPGROUP_H

#include <list>
#include "mapspirit.h"

//一个区块，只允许部分车辆通行
class MapGroup : public MapSpirit
{
public:
    explicit MapGroup(int _id, std::string _name);
    virtual ~MapGroup();
    virtual MapSpirit *clone();
    MapGroup(const MapGroup& b) = delete;

    void clear(){spirits.clear();}

    void addSpirit(int spirit){spirits.push_back(spirit);}
    void removeSpirit(int spirit){spirits.remove(spirit);}
    std::list<int> getSpirits(){return spirits;}
private:
    std::list<int> spirits;//区块
};

#endif // MAPGROUP_H
