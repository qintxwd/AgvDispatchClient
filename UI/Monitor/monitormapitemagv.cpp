#include "monitormapitemagv.h"

#include "global.h"
#include <QtWidgets>

static struct
{
    int id;
    QColor color;
} COMMON_COLOR_TABLE[] =
{
{0,Qt::red},
{1,Qt::green},
{2,Qt::blue},
{3,Qt::cyan},
{4,Qt::magenta},
{5,Qt::yellow},
};
MonitorMapItemAgv::MonitorMapItemAgv(int _id, QString _name, int _floor, QGraphicsItem *parent) : QGraphicsObject(parent),
    agv_id(_id),
    name(_name)
{
    flicktimer.setInterval(800);
    connect(&flicktimer,&QTimer::timeout,this,&MonitorMapItemAgv::slot_flick);
    color = COMMON_COLOR_TABLE[agv_id%6].color;
    floor = _floor;

    setZValue(10);
    setFlags(ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    flicktimer.start();
}

QRectF MonitorMapItemAgv::boundingRect() const
{
    return QRectF(-31,-15,62,30);
}

void MonitorMapItemAgv::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing,true);

    QPen oldPen  = painter->pen();
    QBrush oldBrush  = painter->brush();

    QColor _color = color;
    if(option->state & QStyle::State_Selected)_color = Qt::blue;
    if (option->state & QStyle::State_MouseOver)_color = Qt::blue;

    painter->setBrush(_color);

    //    painter->drawRect(boundingRect());
    //    painter->drawLine(1,5,1,-5);
    //    painter->drawLine(6,5,6,-5);
    //    painter->drawEllipse(QPointF(8,2.5),1.8,1.8);
    //    painter->drawEllipse(QPointF(8,-2.5),1.8,1.8);

    QPen pen  = painter->pen();

    pen.setColor(color);
    painter->setPen(pen);

    QFont font;
    font.setPointSize(18);
    font.setFamily("Microsoft YaHei");
    painter->setFont(font);
    painter->drawText(QRectF(-31,-15,62,30),name);

    pen.setWidth(3);
    painter->drawLine(-12.2*2, 0.0, -1.4*2, 0.0);
    painter->drawLine(-15.2*2, 3.5*2, 1.4*2, 3.5*2);
    painter->drawLine(1.4*2, 3.5*2, -1.4*2, -3.5*2);
    painter->drawLine(1.4*2, -3.5*2, 1.4*2, 3.5*2);
    painter->drawLine(1.4*2, -3.5*2, -15.2*2, -3.5*2);

    painter->setPen(oldPen);
    painter->setBrush(oldBrush);
}

void MonitorMapItemAgv::my_update()
{
    prepareGeometryChange();
    update();
}

void MonitorMapItemAgv::slot_update_pos(int id,QString name,double x,double y,double angle, QStringList occs, int _floor)
{
    if(id!=agv_id)return ;
    if(floor != _floor )
    {
        this->hide();
        return;
    }
    this->show();
    setPos(x,y);
    setRotation(angle);
    my_update();
}

void MonitorMapItemAgv::slot_flick()
{

}
