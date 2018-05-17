#include "mapitemline.h"
#include "mapitemstation.h"
#include <QtWidgets>

MapItemLine::MapItemLine(MapItemStation *_startStation, MapItemStation *_endStation, MapPath *_path, QGraphicsItem *parent) : 
    QGraphicsObject(parent),
    startStation(_startStation),
    endStation(_endStation),
    path(_path)
{
    setZValue(2);
    setFlags(ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);

}

QRectF MapItemLine::boundingRect() const
{
    int left = std::min(startStation->pos().x(),endStation->pos().x())-100;
    int top = std::min(startStation->pos().y(),endStation->pos().y())-100;
    int right = std::max(startStation->pos().x(),endStation->pos().x())+100;
    int bottom = std::max(startStation->pos().y(),endStation->pos().y())+100;

    return QRectF(left,top,right-left,bottom-top);
}

void MapItemLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    const int polygonLength = 30;
    const int polygonAngle = 20;
    double angle;

    triangle_end[0] = endStation->pos();
    angle = atan2(startStation->pos().y() - endStation->pos().y(),startStation->pos().x()- endStation->pos().x());
    triangle_end[1].setX(endStation->pos().x()+polygonLength*cos(angle-M_PI*polygonAngle/180));
    triangle_end[1].setY(endStation->pos().y()+polygonLength*sin(angle-M_PI*polygonAngle/180));
    triangle_end[2].setX(endStation->pos().x()+polygonLength*cos(angle+M_PI*polygonAngle/180));
    triangle_end[2].setY(endStation->pos().y()+polygonLength*sin(angle+M_PI*polygonAngle/180));

    triangle_start[0] = startStation->pos();
    angle = atan2(endStation->pos().y() - startStation->pos().y(),endStation->pos().x()- startStation->pos().x());
    triangle_start[1].setX(startStation->pos().x()+polygonLength*cos(angle-M_PI*polygonAngle/180));
    triangle_start[1].setY(startStation->pos().y()+polygonLength*sin(angle-M_PI*polygonAngle/180));
    triangle_start[2].setX(startStation->pos().x()+polygonLength*cos(angle+M_PI*polygonAngle/180));
    triangle_start[2].setY(startStation->pos().y()+polygonLength*sin(angle+M_PI*polygonAngle/180));

    QPen oldPen  = painter->pen();
    QBrush oldBrush  = painter->brush();

    QColor _color = Qt::gray;
    if(option->state & QStyle::State_Selected)_color = Qt::blue;
    if (option->state & QStyle::State_MouseOver)_color = Qt::blue;

    QPen share_pen(_color);
    share_pen.setWidth(1);
    painter->setPen(share_pen);
    QPainterPath myPath;
    myPath.moveTo(startStation->pos());//start
    myPath.lineTo(endStation->pos());//QPoint(endStation->pos().x()-startStation->pos().x(), endStation->pos().y()-startStation->pos().y()));
    painter->drawPath(myPath);

    //画箭头
    QBrush share_brush(_color);
    painter->setBrush(share_brush);
    if(path->getDirection() == 0 || path->getDirection() == 1){
        painter->drawPolygon(triangle_end,3,Qt::WindingFill);
    }

    if(path->getDirection() == 0 || path->getDirection() == 2){
        painter->drawPolygon(triangle_start,3,Qt::WindingFill);
    }

    painter->setPen(oldPen);
    painter->setBrush(oldBrush);
}

QPainterPath MapItemLine::shape() const
{
    QPainterPath qpath;

    qpath.moveTo(startStation->pos());
    if(path->getDirection() == 0 || path->getDirection() == 2){
        qpath.lineTo(triangle_start[1]);
        qpath.lineTo(triangle_start[2]);
        qpath.lineTo(triangle_start[0]);
    }

    qpath.lineTo(endStation->pos());
    if(path->getDirection() == 0 || path->getDirection() == 1){
        qpath.lineTo(triangle_end[1]);
        qpath.lineTo(triangle_end[2]);
        qpath.lineTo(triangle_end[0]);
    }
    return qpath;
}
void MapItemLine::my_update()
{
    prepareGeometryChange();
    update();
}
