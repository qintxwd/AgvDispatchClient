#ifndef MAPITEMLINE_H
#define MAPITEMLINE_H

#include <QGraphicsObject>
#include "../../data/onemap.h"
class MapItemStation;

class MapItemLine : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit MapItemLine(MapItemStation *_startStation,MapItemStation *_endStation,MapPath *_path,QGraphicsItem *parent = nullptr);

    enum { Type = UserType + 23};
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }
    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QPainterPath shape() const;

    MapItemStation *getStartStation(){return startStation;}

    MapItemStation *getEndStation(){return endStation;}

    MapPath *getPath(){return path;}
    void my_update();
signals:
    void sig_propertyChanged(MapSpirit *spirit);
private:
    MapItemStation *startStation;
    MapItemStation *endStation;
    MapPath *path;
//    QPointF triangle_start[3];//箭头三角
    QPointF triangle_end[3];//箭头三角
};

#endif // MAPITEMLINE_H
