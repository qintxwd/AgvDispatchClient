#include "monitormapitemagv.h"

#include "global.h"
#include <QtWidgets>

MonitorMapItemAgv::MonitorMapItemAgv(int _id, QString _name, QGraphicsItem *parent) : QGraphicsObject(parent),
    agv_id(_id),
    name(_name)
{
    int r = getRandom(255);
    int g = getRandom(255);
    int b = getRandom(255);
    color = QColor(r,g,b);

    setZValue(10);
    setFlags(ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

QRectF MonitorMapItemAgv::boundingRect() const
{
    return QRectF(-10,-5,20,10);
}

void MonitorMapItemAgv::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    QPen oldPen  = painter->pen();
    QBrush oldBrush  = painter->brush();

    QColor _color = color;
    if(option->state & QStyle::State_Selected)_color = Qt::blue;
    if (option->state & QStyle::State_MouseOver)_color = Qt::blue;

    painter->setBrush(_color);

    painter->drawRect(boundingRect());
    painter->drawLine(1,5,1,-5);
    painter->drawLine(6,5,6,-5);
    painter->drawEllipse(QPointF(8,2.5),1.8,1.8);
    painter->drawEllipse(QPointF(8,-2.5),1.8,1.8);
    painter->drawText(QRectF(-10,-5,16,10),name);

    painter->setPen(oldPen);
    painter->setBrush(oldBrush);
}

void MonitorMapItemAgv::my_update()
{
    prepareGeometryChange();
    update();
}

void MonitorMapItemAgv::slot_update_pos(int id,QString name,double x,double y,double angle)
{
    if(id!=agv_id)return ;
    setPos(x,y);
    setRotation(angle);
    my_update();
}
