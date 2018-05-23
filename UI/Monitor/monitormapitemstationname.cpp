
#include "monitormapitemstationname.h"

#include <QtWidgets>

MonitorMapItemStationName::MonitorMapItemStationName(MonitorMapItemStation *_station, MapPoint *_point, QGraphicsItem *parent):
    QGraphicsTextItem(parent),
    point(_point),
    station(_station)
{
    setZValue(4);
    setFlags(ItemIsSelectable/*|ItemIsMovable*/| QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

    setPos(point->getX()+point->getLabelXoffset(),point->getY()+point->getLabelYoffset());
    setPlainText(QString::fromStdString(point->getName()));
    setDefaultTextColor(Qt::black);
}

void MonitorMapItemStationName::my_update()
{
    prepareGeometryChange();
    setPos(point->getX()+point->getLabelXoffset(),point->getY()+point->getLabelYoffset());
    setPlainText(QString::fromStdString(point->getName()));
    setDefaultTextColor(Qt::black);
    update();
}

void MonitorMapItemStationName::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    setDefaultTextColor(Qt::blue);
    update();
    QGraphicsTextItem::hoverEnterEvent(event);
}

void MonitorMapItemStationName::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    setDefaultTextColor(Qt::black);
    update();
    QGraphicsTextItem::hoverEnterEvent(event);
}

QVariant MonitorMapItemStationName::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange)
    {
        QPointF newPos = value.toPointF();
        point->setLabelXoffset(newPos.x() - point->getX());
        point->setLabelYoffset(newPos.y() - point->getY());
        emit sig_propertyChanged(point);
        return newPos;
    }
    return QGraphicsItem::itemChange(change, value);
}
