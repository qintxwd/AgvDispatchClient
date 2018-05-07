#ifndef ONEMAP_H
#define ONEMAP_H

#include "mapfloor.h"
#include "mapbackground.h"
#include "mapblock.h"


//一个地图[有若干个元素组成]
class OneMap : public QObject
{
    Q_OBJECT
public:
    OneMap(QObject *parent = nullptr);
    OneMap(const OneMap &b);

    ~OneMap();

    //注册一个新的元素 获取一个ID
    int getNextId();

    void addPath(MapPath *path);

    void addFloor(MapFloor *floor);

    void addBlock(MapBlock *block);

    //删除一个元素
    void removePath(MapPath *path);
    void removeFloor(MapFloor *floor);
    void removeBlock(MapBlock *block);

    void removeRootPathById(int id);
    void removeFloorById(int id);

    //复制地图（深copy）
    OneMap* clone();

    QList<MapFloor *> getFloors(){return floors;}
    QList<MapPath *> getRootPaths(){return rootpaths;}
    QList<MapBlock *> getBlocks(){return blocks;}

    MapFloor *getFloorById(int id);
    MapPath *getRootPathById(int id);
    MapBlock *getBlockById(int id);

    MapSpirit *getSpiritById(int id);

    void setMaxId(int maxid){max_id = maxid; }
private:
    QList<MapFloor *> floors;///楼层
    QList<MapPath *> rootpaths;///楼层之间的道路
    QList<MapBlock *> blocks;////block
    int max_id;
};

#endif // ONEMAP_H
