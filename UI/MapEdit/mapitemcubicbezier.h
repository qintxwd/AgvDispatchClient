#ifndef MAPITEMCUBICBEZIER_H
#define MAPITEMCUBICBEZIER_H

#include <QGraphicsObject>
#include "../../data/onemap.h"
class MapItemStation;

class MapItemCubicBezier : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit MapItemCubicBezier(MapItemStation *_startStation,MapItemStation *_endStation,MapPath *_path,QGraphicsItem *parent = nullptr);

    enum { Type = UserType + 25};
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
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    QPointF P1;
    bool isDragingP1;
    QPointF P2;
    bool isDragingP2;
    MapItemStation *startStation;
    MapItemStation *endStation;
    MapPath *path;
    QPointF triangle_start[3];//箭头三角
    QPointF triangle_end[3];//箭头三角
};

#endif // MAPITEMCUBICBEZIER_H
