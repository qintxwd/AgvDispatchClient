#include "mapitemstationname.h"


MapItemStationName::MapItemStationName(MapItemStation *_station,MapPoint *_point,QObject *parent = nullptr):
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

void MapItemStationName::update()
{
    setPos(point->getX()+point->getLabelXoffset(),point->getY()+point->getLabelYoffset());
    setPlainText(point->getName());
    setDefaultTextColor(Qt::gray);
}

QVariant MapItemStationName::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange)
    {
        QPointF newPos = value.toPointF();
        point->setLabelXoffset(newPos.x() - point->getX());
        point->setLabelYoffset(newPos.() - point->getY());
        return newPos;
    }
    return QGraphicsItem::itemChange(change, value);
}
