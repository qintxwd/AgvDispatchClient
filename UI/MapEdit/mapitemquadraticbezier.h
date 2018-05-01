#ifndef MAPITEMQUADRATICBEZIER_H
#define MAPITEMQUADRATICBEZIER_H

#include <QGraphicsObject>
#include "../../data/onemap.h"
class MapItemStation;

class MapItemQuadraticBezier : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit MapItemQuadraticBezier(MapItemStation *_startStation,MapItemStation *_endStation,MapPath *_path,QObject *parent = nullptr);

    enum { Type = UserType + 24 };
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QPainterPath shape() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    QPointF P1;
    bool isDragingP1;
    MapItemStation *startStation;
    MapItemStation *endStation;
    MapPath *path;
};

#endif // MAPITEMQUADRATICBEZIER_H
