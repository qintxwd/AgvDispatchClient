#include "mapitemcubicbezier.h"
#include "mapitemstation.h"

#include <QtWidgets>

MapItemCubicBezier::MapItemCubicBezier(MapItemStation *_startStation, MapItemStation *_endStation, MapPath *_path, QGraphicsItem *parent) : QGraphicsObject(parent),
    startStation(_startStation),
    endStation(_endStation),
    path(_path),
    isDragingP1(false),
    isDragingP2(false)
{
    P1 = QPointF(path->getP1x(),path->getP1y());
    P2 = QPointF(path->getP2x(),path->getP2y());
    setZValue(3);
    setFlags(ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

QRectF MapItemCubicBezier::boundingRect() const
{
    int left = std::min(startStation->pos().x(),endStation->pos().x()) - 1000;
    int top = std::min(startStation->pos().y(),endStation->pos().y()) - 1000;
    int right = std::max(startStation->pos().x(),endStation->pos().x()) + 1000;
    int bottom = std::max(startStation->pos().y(),endStation->pos().y()) + 1000;

    return QRectF(left,top,right-left,bottom-top);
}

void MapItemCubicBezier::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    QPen oldPen  = painter->pen();

    QColor _color = Qt::gray;
    if(option->state & QStyle::State_Selected)_color = Qt::blue;
    if (option->state & QStyle::State_MouseOver)_color = Qt::blue;

    QPen share_pen(_color);
    share_pen.setWidth(1);
    painter->setPen(share_pen);
    QPainterPath myPath;
    myPath.moveTo(startStation->pos());//start
    myPath.cubicTo(P1,P2, endStation->pos());//QPoint(endStation->pos().x()-startStation->pos().x(), endStation->pos().y()-startStation->pos().y()));
    painter->drawPath(myPath);
    if(option->state & QStyle::State_Selected||option->state & QStyle::State_MouseOver){
        painter->drawLine(P1.x()-5,P1.y()-5,P1.x()+5,P1.y()+5);
        painter->drawLine(P1.x()+5,P1.y()-5,P1.x()-5,P1.y()+5);
        painter->drawLine(P2.x()-5,P2.y()-5,P2.x()+5,P2.y()+5);
        painter->drawLine(P2.x()+5,P2.y()-5,P2.x()-5,P2.y()+5);
    }
    painter->setPen(oldPen);
}

QPainterPath MapItemCubicBezier::shape() const
{
    QPainterPath path;
    path.moveTo(startStation->pos());
    path.cubicTo(P1,P2, endStation->pos());

    path.addRect(QRect(P1.x()-5,P1.y()-5,10,10));
    path.addRect(QRect(P2.x()-5,P2.y()-5,10,10));

    return path;
}

void MapItemCubicBezier::my_update()
{
    P1 = QPointF(path->getP1x(),path->getP1y());
    P2 = QPointF(path->getP2x(),path->getP2y());
    prepareGeometryChange();
    update();
}


void MapItemCubicBezier::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() & Qt::LeftButton){
        //判断坐标位置
        if(sqrt(pow(event->pos().x()-P1.x(),2)+pow(event->pos().y()-P1.y(),2))<10){
            isDragingP1 = true;
        }else if(sqrt(pow(event->pos().x()-P2.x(),2)+pow(event->pos().y()-P2.y(),2))<10){
            isDragingP2 = true;
        }
    }
    QGraphicsObject::mousePressEvent(event);
}

void MapItemCubicBezier::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(isDragingP1){
        P1 = event->pos();
        path->setP1x(P1.x());
        path->setP1y(P1.y());
        update();
        emit sig_propertyChanged(path);
    }else if(isDragingP2){
        P2 = event->pos();
        path->setP1x(P2.x());
        path->setP1y(P2.y());
        update();
        emit sig_propertyChanged(path);
    }
    QGraphicsObject::mouseMoveEvent(event);
}

void MapItemCubicBezier::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    isDragingP1 = false;
    isDragingP2 = false;
    QGraphicsObject::mouseReleaseEvent(event);
}
