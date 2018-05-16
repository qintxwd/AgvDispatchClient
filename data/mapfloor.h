#ifndef MAPFLOOR_H
#define MAPFLOOR_H

#include <QList>
#include "mappath.h"
#include "mapbackground.h"
#include "mappoint.h"

class OneMap;

class MapFloor : public MapSpirit
{
    Q_OBJECT
public:
    MapFloor(int _id, std::string _name,QObject *parent = nullptr);
    ~MapFloor();

    void addPoint(MapPoint *p){points.append(p);}
    void addPath(MapPath *p){paths.append(p);}
    void setBkg(MapBackground *b){if(bkg!=nullptr)delete bkg;bkg = b;}

    void removePoint(MapPoint *p){points.removeAll(p);}
    void removePath(MapPath *p){paths.removeAll(p);}
    void removeBkg(){if(bkg!=nullptr)delete bkg;bkg = nullptr;}

    //复制地图（深copy）
    MapFloor* clone();

    MapPoint *getPointById(int id);

    MapPath *getPathById(int id);

    QList<MapPoint *> getPoints(){return points;}
    QList<MapPath *> getPaths(){return paths;}
    MapBackground *getBkg(){return bkg;}

private:
    QList<MapPoint *> points;///若干站点
    QList<MapPath *> paths;///若干路径
    MapBackground *bkg;///一个背景图片
};

#endif // MAPFLOOR_H
