#include "monitormapitemstation.h"
#include "monitormapitemline.h"
#include "monitormapitemcubicbezier.h"
#include "monitormapitemquadraticbezier.h"
#include <QtWidgets>

MonitorMapItemStation::MonitorMapItemStation(MapPoint *_point, QGraphicsItem *parent) : QGraphicsObject(parent),
    point(_point)
{
    setZValue(4);
    setFlags(ItemIsSelectable/*|ItemIsMovable*/| QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
    setPos(point->getX(),point->getY());
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    //
    loadImg();
}

//外接区域
QRectF MonitorMapItemStation::boundingRect() const
{
    return QRectF(-5, -5, 10, 10);
}

//绘制
void MonitorMapItemStation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor _color = Qt::gray;
    int width = 1;

    if(option->state & QStyle::State_Selected)_color = Qt::blue;
    if (option->state & QStyle::State_MouseOver)_color = Qt::blue;
    if (option->state & QStyle::State_Selected)width = 1;

    QPen oldPen = painter->pen();
    QBrush oldBrush = painter->brush();
    QPen pen(_color);

    pen.setWidth(width);
    painter->setPen(pen);
    painter->setBrush(QBrush(_color));
    painter->drawRect(boundingRect());
    if(point->getPointType() == MapPoint::Map_Point_Type_Draw){
        painter->drawImage(boundingRect(),*imgDraw);
    }else if(point->getPointType() == MapPoint::Map_Point_Type_REPORT){
        painter->drawImage(boundingRect(),*imgReport);
    }else if(point->getPointType() == MapPoint::Map_Point_Type_HALT){
        painter->drawImage(boundingRect(),*imgHalt);
    }else if(point->getPointType() == MapPoint::Map_Point_Type_CHARGE){
        painter->drawImage(boundingRect(),*imgCharge);
    }else if(point->getPointType() == MapPoint::Map_Point_Type_LOAD){
        painter->drawImage(boundingRect(),*imgLoad);
    }
    else if(point->getPointType() == MapPoint::Map_Point_Type_UNLOAD){
        painter->drawImage(boundingRect(),*imgUnload);
    }
    else if(point->getPointType() == MapPoint::Map_Point_Type_LOAD_UNLOAD){
        painter->drawImage(boundingRect(),*imgLoadUnload);
    }
    else if(point->getPointType() == MapPoint::Map_Point_Type_ORIGIN){
        painter->drawImage(boundingRect(),*imgLoadUnload);
    }

    if(point->getMapChange()){
        painter->setBrush(oldBrush);
        painter->setPen(oldPen);
        QPen tpen(Qt::red);
        tpen.setWidth(width);
        painter->setPen(tpen);
        painter->drawEllipse(boundingRect().center(),10,10);
    }

    painter->setBrush(oldBrush);
    painter->setPen(oldPen);
}

void MonitorMapItemStation::my_update()
{
    setPos(point->getX(),point->getY());
    prepareGeometryChange();
    update();
}

void MonitorMapItemStation::loadImg()
{
    imgDraw = new QImage;
    imgReport = new QImage;
    imgHalt = new QImage;
    imgCharge = new QImage;
    imgLoad = new QImage;
    imgUnload = new QImage;
    imgLoadUnload = new QImage;
    imgDraw->load(":/images/toolbar/point-report.22.png");
    imgReport->load(":/images/toolbar/point-halt.22.png");
    imgHalt->load(":/images/toolbar/point-park.22.png");
    imgCharge->load(":/images/point/ChargingStation.20x20.png");
    imgLoad->load(":/images/point/TransferStation.20x20.png");
    imgUnload->load(":/images/point/TransferStation.20x20.png");
    imgLoadUnload->load(":/images/point/TransferStation.20x20.png");
}

QVariant MonitorMapItemStation::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange)
    {
        QPointF newPos = value.toPointF();
        point->setX(newPos.x());
        point->setY(newPos.y());
        //更新位置的同时，更新连线的起止点
        foreach (auto item, lines) {
            item->my_update();
        }
        foreach (auto item, qbs) {
            item->my_update();
        }
        foreach (auto item, cbs) {
            item->my_update();
        }

        //TODO:更新了station的位置
        emit sig_propertyChanged(point);
        return newPos;
    }
    return QGraphicsItem::itemChange(change, value);
}
