#include "mapitemstationname.h"


MapItemStationName::MapItemStationName(MapItemStation *_station, MapPoint *_point, QGraphicsItem *parent):
    QGraphicsTextItem(parent),
    point(_point),
    station(_station)
{
    setZValue(4);
    setFlags(ItemIsSelectable|ItemIsMovable| QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

    setPos(point->getX()+point->getLabelXoffset(),point->getY()+point->getLabelYoffset());
    setPlainText(point->getName());
    setDefaultTextColor(Qt::gray);
}

void MapItemStationName::my_update()
{
    prepareGeometryChange();
    setPos(point->getX()+point->getLabelXoffset(),point->getY()+point->getLabelYoffset());
    setPlainText(point->getName());
    setDefaultTextColor(Qt::gray);
    update();
}

QVariant MapItemStationName::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange)
    {
        QPointF newPos = value.toPointF();
        point->setLabelXoffset(newPos.x() - point->getX());
        point->setLabelYoffset(newPos.y() - point->getY());
        return newPos;
    }
    return QGraphicsItem::itemChange(change, value);
}
