#include "itemstation.h"
#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QDebug>
#include "itemarc.h"
#include "itemline.h"
#include "scene.h"

ItemStation::ItemStation(MapPoint *point, QGraphicsItem *parent):QGraphicsObject(parent),
    station(point),
    myscene(NULL)
{
    setZValue(4);
    setFlags(ItemIsSelectable|ItemIsMovable| QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);//鼠标移动到它头上
    setPos(station->getX(),station->getY());
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}
bool ItemStation::initialize()
{
    myscene = dynamic_cast<Scene *>(this->scene());
    if (myscene == NULL)
    {
        qDebug() << "!!! Spirit: get scene failed, make sure it's add to a scene!";
        return false;
    }
    return true;
}
//外接区域
QRectF ItemStation::boundingRect() const
{
    //    Map_Point_Type_Draw = 0,
    //    Map_Point_Type_REPORT,
    //    Map_Point_Type_HALT,
    //    Map_Point_Type_CHARGE,
    //    Map_Point_Type_LOAD,
    //    Map_Point_Type_UNLOAD,
    //    Map_Point_Type_LOAD_UNLOAD,

    if(station->getPointType() == MapPoint::Map_Point_Type_Draw){
        return QRectF(-3, -3, 3, 3);
    }else if(station->getPointType() == MapPoint::Map_Point_Type_REPORT){
        return QRectF(-5, -5, 5, 5);
    }else if(station->getPointType() == MapPoint::Map_Point_Type_HALT){
        return QRectF(-8, -8, 8, 8);
    }else if(station->getPointType() == MapPoint::Map_Point_Type_CHARGE){
        return QRectF(-8, -8, 8, 8);
    }else if(station->getPointType() == MapPoint::Map_Point_Type_LOAD||
             station->getPointType() == MapPoint::Map_Point_Type_UNLOAD||
             station->getPointType() == MapPoint::Map_Point_Type_LOAD_UNLOAD){
        return QRectF(-10, -10, 10, 10);
    }
    return QRectF(-3, -3, 3, 3);
}

//绘制
void ItemStation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    if(station->getPointType() == MapPoint::Map_Point_Type_Draw){

    }else if(station->getPointType() == MapPoint::Map_Point_Type_REPORT){

    }else if(station->getPointType() == MapPoint::Map_Point_Type_HALT){

    }else if(station->getPointType() == MapPoint::Map_Point_Type_CHARGE){

    }else if(station->getPointType() == MapPoint::Map_Point_Type_LOAD||
             station->getPointType() == MapPoint::Map_Point_Type_UNLOAD||
             station->getPointType() == MapPoint::Map_Point_Type_LOAD_UNLOAD){

    }

    QColor _color(station.color_r,station.color_g,station.color_b);
    if(option->state & QStyle::State_Selected)_color = _color.darker();
    if (option->state & QStyle::State_MouseOver)_color = _color.lighter();
    QPen oldPen = painter->pen();
    QBrush oldBrush = painter->brush();
    QPen pen(_color);
    int width = 0;
    if (option->state & QStyle::State_Selected)width = 1;
    pen.setWidth(width);
    painter->setPen(pen);
    painter->setBrush(QBrush(_color));
    painter->drawEllipse(-2, -2,4, 4);
    painter->drawText(QPoint(10,-10),QString("%1[%2]").arg(station.name).arg(station.rfid));
    painter->setBrush(oldBrush);
    painter->setPen(oldPen);
    //setPos(mpos);
}

QVariant ItemStation::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && myscene)
    {
        //限定station不能出界
        QPointF newPos = value.toPointF();
        if(myscene->outOfLimitLine(newPos)){
            newPos.setX(qMin(0.0+myscene->width() * Scene::GRID_SIZE, qMax(newPos.x(), 0.0)));
            newPos.setY(qMin(0.0+myscene->height() * Scene::GRID_SIZE, qMax(newPos.y(), 0.0)));
        }

        //更新位置的同时，更新连线的起止点
        for(QList<ItemArc *>::iterator itr =  arcs.begin();itr!=arcs.end();++itr)
        {
            ItemArc *arc = (*itr);
            arc->update();
        }

        for(QList<ItemLine *>::iterator itr =  lines.begin();itr!=lines.end();++itr)
        {
            ItemLine *line = (*itr);
            line->updatePosition();
        }

        station.x = newPos.x();
        station.y = newPos.y();

        return newPos;
    }
    return QGraphicsItem::itemChange(change, value);
}
