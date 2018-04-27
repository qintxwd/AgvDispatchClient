#ifndef ONEMAP_H
#define ONEMAP_H

#include "mapfloor.h"

//一个地图[有若干个元素组成]
class OneMap
{
public:
    OneMap();
    ~OneMap();
    //注册一个新的元素
    int getNextId();

    void addSpirit(MapSpirit *spirit);

    void setBackgroudImage(QString fileName,QByteArray data){
        background = data;
        imgFileName = fileName;
    }

    //删除一个元素
    void removeSpirit(int id);
    void removeSpirit(MapSpirit *spirit);

    //复制地图（深copy）
    OneMap clone();

    QList<MapSpirit *> getSpirits(){return spirits;}

    MapSpirit *getSpiritById(int id);

    QList<MapSpirit *> getSpiritByType(MapSpirit::Map_Spirit_Type type);
private:
    QList<MapSpirit *> spirits;//地图上的所有元素
    int max_id;
    QByteArray background;
    QString imgFileName;
};

#endif // ONEMAP_H
