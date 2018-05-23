#include "monitormapitemquadraticbezier.h"
#include "monitormapitemstation.h"

#include <QtWidgets>

MonitorMapItemQuadraticBezier::MonitorMapItemQuadraticBezier(MonitorMapItemStation *_startStation, MonitorMapItemStation *_endStation, MapPath *_path, QGraphicsItem *parent) : QGraphicsObject(parent),
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

QRectF MonitorMapItemQuadraticBezier::boundingRect() const
{
    int left = std::min(startStation->pos().x(),endStation->pos().x()) - 1000;
    int top = std::min(startStation->pos().y(),endStation->pos().y()) - 1000;
    int right = std::max(startStation->pos().x(),endStation->pos().x()) + 1000;
    int bottom = std::max(startStation->pos().y(),endStation->pos().y()) + 1000;

    return QRectF(left,top,right-left,bottom-top);
}

void MonitorMapItemQuadraticBezier::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)


    const int polygonLength = 30;
    const int polygonAngle = 20;
    double angle;

    triangle_end[0] = endStation->pos();
    angle = atan2(P1.y() - endStation->pos().y(),P1.x()- endStation->pos().x());
    triangle_end[1].setX(endStation->pos().x()+polygonLength*cos(angle-M_PI*polygonAngle/180));
    triangle_end[1].setY(endStation->pos().y()+polygonLength*sin(angle-M_PI*polygonAngle/180));
    triangle_end[2].setX(endStation->pos().x()+polygonLength*cos(angle+M_PI*polygonAngle/180));
    triangle_end[2].setY(endStation->pos().y()+polygonLength*sin(angle+M_PI*polygonAngle/180));

    //    triangle_start[0] = startStation->pos();
    //    angle = atan2(P1.y() - startStation->pos().y(),P1.x()- startStation->pos().x());
    //    triangle_start[1].setX(startStation->pos().x()+polygonLength*cos(angle-M_PI*polygonAngle/180));
    //    triangle_start[1].setY(startStation->pos().y()+polygonLength*sin(angle-M_PI*polygonAngle/180));
    //    triangle_start[2].setX(startStation->pos().x()+polygonLength*cos(angle+M_PI*polygonAngle/180));
    //    triangle_start[2].setY(startStation->pos().y()+polygonLength*sin(angle+M_PI*polygonAngle/180));

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
    myPath.quadTo(P1, endStation->pos());//QPoint(endStation->pos().x()-startStation->pos().x(), endStation->pos().y()-startStation->pos().y()));
    painter->drawPath(myPath);
    if(option->state & QStyle::State_Selected||option->state & QStyle::State_MouseOver){
        painter->drawLine(P1.x()-5,P1.y()-5,P1.x()+5,P1.y()+5);
        painter->drawLine(P1.x()+5,P1.y()-5,P1.x()-5,P1.y()+5);
    }

    //画箭头
    QBrush share_brush(_color);
    painter->setBrush(share_brush);
    //    if(path->getDirection() == 0 || path->getDirection() == 1){
    painter->drawPolygon(triangle_end,3,Qt::WindingFill);
    //    }

    //    if(path->getDirection() == 0 || path->getDirection() == 2){
    //        painter->drawPolygon(triangle_start,3,Qt::WindingFill);
    //    }

    painter->setPen(oldPen);
    painter->setBrush(oldBrush);
}

QPainterPath MonitorMapItemQuadraticBezier::shape() const
{
    QPainterPath qpath;
    qpath.moveTo(startStation->pos());
    //    if(path->getDirection() == 0 || path->getDirection() == 2){
    //        qpath.lineTo(triangle_start[1]);
    //        qpath.lineTo(triangle_start[2]);
    //        qpath.lineTo(triangle_start[0]);
    //    }
    qpath.quadTo(P1, endStation->pos());
    //if(path->getDirection() == 0 || path->getDirection() == 1){
        qpath.lineTo(triangle_end[1]);
        qpath.lineTo(triangle_end[2]);
        qpath.lineTo(triangle_end[0]);
    //}
    qpath.addRect(QRect(P1.x()-5,P1.y()-5,10,10));

    return qpath;
}

void MonitorMapItemQuadraticBezier::my_update()
{
    P1 = QPointF(path->getP1x(),path->getP1y());
    prepareGeometryChange();
    update();
}

//void MonitorMapItemQuadraticBezier::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    if(event->button() & Qt::LeftButton){
//        //判断坐标位置
//        if(sqrt(pow(event->pos().x()-P1.x(),2)+pow(event->pos().y()-P1.y(),2))<10){
//            isDragingP1 = true;
//        }
//    }
//    QGraphicsObject::mousePressEvent(event);
//}

//void MonitorMapItemQuadraticBezier::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
//{
//    if(isDragingP1){
//        P1 = event->pos();
//        path->setP1x(P1.x());
//        path->setP1y(P1.y());
//        update();
//        emit sig_propertyChanged(path);
//    }
//    QGraphicsObject::mouseMoveEvent(event);
//}

//void MonitorMapItemQuadraticBezier::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
//{
//    isDragingP1 = false;
//    QGraphicsObject::mouseReleaseEvent(event);
//}
