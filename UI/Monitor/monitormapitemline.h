#ifndef MONITORMAPITEMLINE_H
#define MONITORMAPITEMLINE_H

#include <QGraphicsObject>
#include "../../data/onemap.h"

class MonitorMapItemStation;

class MonitorMapItemLine : public QGraphicsObject
{
    Q_OBJECT
public:
    MonitorMapItemLine(MonitorMapItemStation *_startStation,MonitorMapItemStation *_endStation,MapPath *_path,QGraphicsItem *parent = nullptr);

    enum { Type = UserType + 23};
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    void setColor(QColor _color){color = _color;update();}

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QPainterPath shape() const;

    MonitorMapItemStation *getStartStation(){return startStation;}

    MonitorMapItemStation *getEndStation(){return endStation;}

    MapPath *getPath(){return path;}
    void my_update();
signals:
    void sig_propertyChanged(MapSpirit *spirit);
private:
    MonitorMapItemStation *startStation;
    MonitorMapItemStation *endStation;
    MapPath *path;
//    QPointF triangle_start[3];//箭头三角
    QPointF triangle_end[3];//箭头三角

    QColor color;
};

#endif // MONITORMAPITEMLINE_H
