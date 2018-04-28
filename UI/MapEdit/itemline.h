#ifndef ITEMLINE_H
#define ITEMLINE_H

#include <QGraphicsLineItem>
#include "Bean/agvline.h"

class ItemStation;

class ItemLine : public QGraphicsLineItem
{
public:
    ItemLine(ItemStation *_startStation, ItemStation *_endStation, int _id, QColor _color=QColor("blue"), QGraphicsItem *parent = Q_NULLPTR);
    enum { Type = UserType + 12 };
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    //起止站点位置变了以后，调用
    void updatePosition();

    //修改了线路颜色以后，调用
    void updateColor(QColor _color);
protected:
    //鼠标进入
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);

    //鼠标离开
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

public:
    //两个站点
    ItemStation *startStation;
    ItemStation *endStation;

    QColor color;
    int id;
};

#endif // ITEMLINE_H
