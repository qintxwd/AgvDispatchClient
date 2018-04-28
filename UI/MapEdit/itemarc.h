#ifndef ITEMARC_H
#define ITEMARC_H

#include <QGraphicsObject>
class ItemStation;

class ItemArc : public QGraphicsObject
{
    Q_OBJECT
public:
    ItemArc(ItemStation *_startStation,ItemStation *_endStation,int _id,QColor _color=QColor("green"),int _width=1, QGraphicsItem *parent = Q_NULLPTR);

    enum { Type = UserType + 11 };

    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    //改变线路颜色
    void setColor(QColor _color);

    //改变线路宽度
    void setWidth(int _width);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    QPainterPath shape() const;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    int width;
    bool isDragingP2;
    bool isDragingP3;
    bool outOfLimitLine(const QPoint &grid_pos);
public:
    //两个站点
    ItemStation *startStation;
    ItemStation *endStation;

    QPointF P2;
    QPointF P3;
    QColor color;
    int id;
};

#endif // ITEMARC_H
