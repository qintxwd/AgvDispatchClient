#include "mapitemquadraticbezier.h"
#include "mapitemstation.h"

#include <QtWidgets>

MapItemQuadraticBezier::MapItemQuadraticBezier(MapItemStation *_startStation, MapItemStation *_endStation, MapPath *_path, QGraphicsItem *parent) : QGraphicsObject(parent),
    startStation(_startStation),
    endStation(_endStation),
    path(_path),
    isDragingP1(false)
{
    P1 = QPointF(path->getP1x(),path->getP1y());
    setZValue(3);
    setFlags(ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

QRectF MapItemQuadraticBezier::boundingRect() const
{
    int left = std::min(startStation->pos().x(),endStation->pos().x()) - 1000;
    int top = std::min(startStation->pos().y(),endStation->pos().y()) - 1000;
    int right = std::max(startStation->pos().x(),endStation->pos().x()) + 1000;
    int bottom = std::max(startStation->pos().y(),endStation->pos().y()) + 1000;

    return QRectF(left,top,right-left,bottom-top);
}

void MapItemQuadraticBezier::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    myPath.quadTo(P1, endStation->pos());//QPoint(endStation->pos().x()-startStation->pos().x(), endStation->pos().y()-startStation->pos().y()));
    painter->drawPath(myPath);
    if(option->state & QStyle::State_Selected||option->state & QStyle::State_MouseOver){
        painter->drawLine(P1.x()-5,P1.y()-5,P1.x()+5,P1.y()+5);
        painter->drawLine(P1.x()+5,P1.y()-5,P1.x()-5,P1.y()+5);
    }
    painter->setPen(oldPen);
}

QPainterPath MapItemQuadraticBezier::shape() const
{
    QPainterPath path;
    path.moveTo(startStation->pos());
    path.quadTo(P1, endStation->pos());

    path.addRect(QRect(P1.x()-5,P1.y()-5,10,10));

    return path;
}

void MapItemQuadraticBezier::my_update()
{
    P1 = QPointF(path->getP1x(),path->getP1y());
    prepareGeometryChange();
    update();
}

void MapItemQuadraticBezier::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() & Qt::LeftButton){
        //判断坐标位置
        if(sqrt(pow(event->pos().x()-P1.x(),2)+pow(event->pos().y()-P1.y(),2))<10){
            isDragingP1 = true;
        }
    }
    QGraphicsObject::mousePressEvent(event);
}

void MapItemQuadraticBezier::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(isDragingP1){
        P1 = event->pos();
        path->setP1x(P1.x());
        path->setP1y(P1.y());
        update();
        emit sig_propertyChanged(path);
    }
    QGraphicsObject::mouseMoveEvent(event);
}

void MapItemQuadraticBezier::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    isDragingP1 = false;
    QGraphicsObject::mouseReleaseEvent(event);
}
