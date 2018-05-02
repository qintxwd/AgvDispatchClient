#ifndef MAPITEMSTATIONNAME_H
#define MAPITEMSTATIONNAME_H

#include <QGraphicsTextItem>
#include "../../data/onemap.h"
class MapItemStation;

class MapItemStationName : public QGraphicsTextItem
{
public:
    MapItemStationName(MapItemStation *_station,MapPoint *_point,QGraphicsItem *parent = nullptr);

    enum { Type = UserType + 22 };
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    MapPoint *getPoint(){return point;}
    MapItemStation *getStation(){return station;}

    void my_update();
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    //当前的站点
    MapPoint *point;
    MapItemStation *station;
};

#endif // MAPITEMSTATIONNAME_H
