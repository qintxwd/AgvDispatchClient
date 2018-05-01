#ifndef MAPITEMLINE_H
#define MAPITEMLINE_H

#include <QGraphicsLineItem>
#include "../../data/onemap.h"

class MapItemStation;

class MapItemLine : public QGraphicsLineItem
{
    Q_OBJECT
public:
    explicit MapItemLine(MapItemStation *_startStation, MapItemStation *_endStation,MapPath *_path,QObject *parent = nullptr);
    enum { Type = UserType + 23 };
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    //起止站点位置变了以后，调用
    void updatePosition();
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);

    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
public:
    MapItemStation *startStation;
    MapItemStation *endStation;
    MapPath *path;
};

#endif // MAPITEMLINE_H
