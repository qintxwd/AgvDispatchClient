#include "mapitemstation.h"
#include "mapitemline.h"
#include "mapitemcubicbezier.h"
#include "mapitemquadraticbezier.h"
#include <QtWidgets>

MapItemStation::MapItemStation(MapPoint *_point, QGraphicsItem *parent) : QGraphicsObject(parent),
    point(_point)
{
    setZValue(4);
    setFlags(ItemIsSelectable|ItemIsMovable| QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
    setPos(point->getX(),point->getY());
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    //
    loadImg();
}

//外接区域
QRectF MapItemStation::boundingRect() const
{
    if(point->getPointType() == MapPoint::Map_Point_Type_Draw||point->getPointType() == MapPoint::Map_Point_Type_REPORT||point->getPointType() == MapPoint::Map_Point_Type_HALT){
        return QRectF(-5, -5, 5, 5);
    }else if(point->getPointType() == MapPoint::Map_Point_Type_CHARGE){
        return QRectF(-10, -10, 10, 10);
    }else if(point->getPointType() == MapPoint::Map_Point_Type_LOAD||
             point->getPointType() == MapPoint::Map_Point_Type_UNLOAD||
             point->getPointType() == MapPoint::Map_Point_Type_LOAD_UNLOAD){
        return QRectF(-10, -10, 10, 10);
    }
    return QRectF(-5, -5, 5, 5);
}

//绘制
void MapItemStation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

    if(point->getPointType() == MapPoint::Map_Point_Type_Draw||point->getPointType() == MapPoint::Map_Point_Type_REPORT||point->getPointType() == MapPoint::Map_Point_Type_HALT){
        painter->drawEllipse(-5,-5,10,10);
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
    painter->setBrush(oldBrush);
    painter->setPen(oldPen);
}

void MapItemStation::my_update()
{
    prepareGeometryChange();
    update();
}

void MapItemStation::loadImg()
{
    imgCharge = new QImage;
    imgLoad = new QImage;
    imgUnload = new QImage;
    imgLoadUnload = new QImage;
    imgCharge->load(":/images/point/ChargingStation.20x20.png");
    imgLoad->load(":/images/point/TransferStation.20x20.png");
    imgUnload->load(":/images/point/TransferStation.20x20.png");
    imgLoadUnload->load(":/images/point/TransferStation.20x20.png");
}

QVariant MapItemStation::itemChange(GraphicsItemChange change, const QVariant &value)
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

        return newPos;
    }
    return QGraphicsItem::itemChange(change, value);
}
