#include "mapitemline.h"
#include "mapitemstation.h"
#include <QtWidgets>

MapItemLine::MapItemLine(MapItemStation *_startStation, MapItemStation *_endStation, MapPath *_path, QGraphicsItem *parent, QObject *_parent):
    QObject(_parent),
    QGraphicsLineItem(parent),
    startStation(_startStation),
    endStation(_endStation),
    path(_path)
{
    setZValue(2);
    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
    setLine(startStation->getPoint()->getX(),
            startStation->getPoint()->getY(),
            endStation->getPoint()->getX(),
            endStation->getPoint()->getY());
    setPen(QPen(Qt::gray));
}


void MapItemLine::my_update()
{
    prepareGeometryChange();
    setLine(startStation->getPoint()->getX(),
            startStation->getPoint()->getY(),
            endStation->getPoint()->getX(),
            endStation->getPoint()->getY());
    update();
}

void MapItemLine::hoverEnterEvent(QGraphicsSceneHoverEvent * event) {
    setPen(QPen(Qt::blue));
    update();
    QGraphicsLineItem::hoverEnterEvent(event);
}

void MapItemLine::hoverLeaveEvent(QGraphicsSceneHoverEvent * event) {
    setPen(QPen(Qt::gray));
    update();
    QGraphicsLineItem::hoverLeaveEvent(event);
}


