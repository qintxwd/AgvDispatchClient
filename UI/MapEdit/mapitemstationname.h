#ifndef MAPITEMSTATIONNAME_H
#define MAPITEMSTATIONNAME_H

#include <QGraphicsTextItem>
#include "../../data/onemap.h"
class MapItemStation;
class Scene;

class MapItemStationName : public QGraphicsTextItem
{
public:
    MapItemStationName(MapItemStation *_station,MapPoint *_point,QObject *parent = nullptr);

    enum { Type = UserType + 22 };
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    void update();
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    //当前的站点
    MapPoint *point;
    MapItemStation *station;
};

#endif // MAPITEMSTATIONNAME_H
