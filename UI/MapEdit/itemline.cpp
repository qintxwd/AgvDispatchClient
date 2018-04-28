#include "itemline.h"
#include "itemstation.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

ItemLine::ItemLine(ItemStation *_startStation, ItemStation *_endStation, int _id, QColor _color, QGraphicsItem *parent):
    id(_id),
    color(_color),
    startStation(_startStation),
    endStation(_endStation),
    QGraphicsLineItem(parent)
{
    setZValue(2);
    setFlags(ItemIsSelectable/*| QGraphicsItem::ItemIsFocusable*/);//可以选择，但是不可以移动
    setAcceptHoverEvents(true);//鼠标移动到它头上

    setLine(startStation->pos().x(),
            startStation->pos().y(),
            endStation->pos().x(),
            endStation->pos().y());
    setPen(QPen(color, 1));
}

//鼠标进入
void ItemLine::hoverEnterEvent(QGraphicsSceneHoverEvent * event) {
    setPen(QPen(color.lighter(), 1));
    update();
    QGraphicsLineItem::hoverEnterEvent(event);
}

//鼠标离开
void ItemLine::hoverLeaveEvent(QGraphicsSceneHoverEvent * event) {
    setPen(QPen(color, 1));
    update();
    QGraphicsLineItem::hoverLeaveEvent(event);
}

void ItemLine::updatePosition()
{
    setLine(startStation->pos().x(),
            startStation->pos().y(),
            endStation->pos().x(),
            endStation->pos().y());
    update();
}

void ItemLine::updateColor(QColor _color)
{
    color = _color;
    setPen(QPen(color, 1));
    update();
}
