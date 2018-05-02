#ifndef ONEMAP_H
#define ONEMAP_H

#include "mapfloor.h"
#include "mapbackground.h"
#include <QImage>
//一个地图[有若干个元素组成]
class OneMap
{
public:
    OneMap();
    ~OneMap();

    //注册一个新的元素 获取一个ID
    int getNextId();

    void addPath(MapPath *path);

    void addFloor(MapFloor *floor);

    //删除一个元素
    void removePath(MapPath *path);
    void removeFloor(MapFloor *floor);

    void removePathById(int id);
    void removeFloorById(int id);

    //复制地图（深copy）
    OneMap* clone();

    QList<MapFloor *> getFloors(){return floors;}
    QList<MapPath *> getPaths(){return paths;}

    MapFloor *getFloorById(int id);
    MapPath *getPathById(int id);

    void setMaxId(int maxid){max_id = maxid; }
private:
    QList<MapFloor *> floors;///楼层
    QList<MapPath *> paths;///楼层之间的道路
    int max_id;
};

#endif // ONEMAP_H
