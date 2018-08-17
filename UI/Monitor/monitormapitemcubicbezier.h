#ifndef MONITORMAPITEMCUBICBEZIER_H
#define MONITORMAPITEMCUBICBEZIER_H

#include <QGraphicsObject>
#include "../../data/onemap.h"
class MonitorMapItemStation;

class MonitorMapItemCubicBezier : public QGraphicsObject
{
    Q_OBJECT
public:
    MonitorMapItemCubicBezier(MonitorMapItemStation *_startStation,MonitorMapItemStation *_endStation,MapPath *_path,QGraphicsItem *parent = nullptr);

    enum { Type = UserType + 25};
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
protected:
//    void mousePressEvent(QGraphicsSceneMouseEvent *event);
//    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    QPointF P1;
    bool isDragingP1;
    QPointF P2;
    bool isDragingP2;
    MonitorMapItemStation *startStation;
    MonitorMapItemStation *endStation;
    MapPath *path;
//    QPointF triangle_start[3];//箭头三角
    QPointF triangle_end[3];//箭头三角


    QColor color;
};

#endif // MONITORMAPITEMCUBICBEZIER_H
