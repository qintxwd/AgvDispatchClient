#ifndef MONITORMAPITEMSTATIONNAME_H
#define MONITORMAPITEMSTATIONNAME_H

#include <QGraphicsTextItem>
#include "../../data/onemap.h"
class MonitorMapItemStation;

class MonitorMapItemStationName  : public QGraphicsTextItem
{
    Q_OBJECT
public:
    MonitorMapItemStationName(MonitorMapItemStation *_station,MapPoint *_point,QGraphicsItem *parent = nullptr);

    enum { Type = UserType + 22 };
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    MapPoint *getPoint(){return point;}
    MonitorMapItemStation *getStation(){return station;}

    void my_update();
signals:
    void sig_propertyChanged(MapSpirit *spirit);
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);

    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
private:
    //当前的站点
    MapPoint *point;
    MonitorMapItemStation *station;
};

#endif // MONITORMAPITEMSTATIONNAME_H
