#include "itemarc.h"
#include "itemstation.h"
#include <QPainter>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QDebug>

ItemArc::ItemArc(ItemStation *_startStation, ItemStation *_endStation, int _id, QColor _color, int _width, QGraphicsItem *parent):
    id(_id),
    startStation(_startStation),
    endStation(_endStation),
    QGraphicsObject(parent),
    color(_color),
    width(_width),
    isDragingP3(false),
    isDragingP2(false)
{
    P2 = QPointF(startStation->pos().x()+(endStation->pos().x()-startStation->pos().x())/3,startStation->pos().y()+(endStation->pos().y()-startStation->pos().y())/3);
    P3 = QPointF(startStation->pos().x()+(endStation->pos().x()-startStation->pos().x())*2/3,startStation->pos().y()+(endStation->pos().y()-startStation->pos().y())*2/3);
    setZValue(3);
    setFlags(ItemIsSelectable /*| QGraphicsItem::ItemIsFocusable*/);//可以选择，但是不可以移动
    setAcceptHoverEvents(true);//鼠标移动到它头上
    setAcceptedMouseButtons(Qt::LeftButton);
}

//改变线路颜色
void ItemArc::setColor(QColor _color)
{
    color = _color;
    update();
}

//改变线路宽度
void ItemArc::setWidth(int _width)
{
    width = _width;
    update();
}

QRectF ItemArc::boundingRect() const
{
    return QRectF(-300, -300, 1580, 1020);
}

bool ItemArc::outOfLimitLine(const QPoint &grid_pos)
{
    if (grid_pos.x() < 0
            || grid_pos.x() > boundingRect().right() - 1
            || grid_pos.y() < 0
            || grid_pos.y() > boundingRect().bottom() - 1)
        return true;
    else
        return false;
}

void ItemArc::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(widget)
    QPen oldPen  = painter->pen();

    QColor _color(color);
    if (item->state & QStyle::State_MouseOver)_color = color.lighter();
    if(item->state & QStyle::State_Selected)_color = color.darker();

    QPen share_pen(_color);
    share_pen.setWidth(width);
    painter->setPen(share_pen);
    QPainterPath myPath;
    myPath.moveTo(startStation->pos());//start
    myPath.cubicTo(P2,P3, endStation->pos());//QPoint(endStation->pos().x()-startStation->pos().x(), endStation->pos().y()-startStation->pos().y()));
    painter->drawPath(myPath);
    if(item->state & QStyle::State_Selected||item->state & QStyle::State_MouseOver){
        painter->drawLine(P2.x()-2,P2.y()-2,P2.x()+2,P2.y()+2);
        painter->drawLine(P2.x()+2,P2.y()-2,P2.x()-2,P2.y()+2);
        painter->drawLine(P3.x()-2,P3.y()-2,P3.x()+2,P3.y()+2);
        painter->drawLine(P3.x()+2,P3.y()-2,P3.x()-2,P3.y()+2);
    }
    painter->setPen(oldPen);
}

QPainterPath ItemArc::shape() const
{
    QPainterPath path;
    path.moveTo(startStation->pos());
    path.cubicTo(P2,P3, endStation->pos());

    path.addRect(QRect(P2.x()-2,P2.y()-2,4,4));
    path.addRect(QRect(P3.x()-2,P3.y()-2,4,4));

//    QPolygon polygon;
//    polygon << QPoint(0, 0);
//    polygon << QPoint(5, 5);
//    polygon << QPoint(width, height);
//    polygon << QPoint(width - 5, height - 5);
//    path.addPolygon(polygon);

    return path;
}

void ItemArc::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() & Qt::LeftButton){
        //判断坐标位置
        if(sqrt(pow(event->pos().x()-P3.x(),2)+pow(event->pos().y()-P3.y(),2))<5){
            isDragingP3 = true;
        }else if(sqrt(pow(event->pos().x()-P2.x(),2)+pow(event->pos().y()-P2.y(),2))<5){
            isDragingP2 = true;
        }
    }
    QGraphicsObject::mousePressEvent(event);
}

void ItemArc::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //if(!outOfLimitLine(event->pos().toPoint())){
    if(isDragingP3){
        P3 = event->pos();
        update();
    }else if(isDragingP2){
        P2 = event->pos();
        update();
    }
    //}
    QGraphicsObject::mouseMoveEvent(event);
}

void ItemArc::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    isDragingP3 = false;
    isDragingP2 = false;
    QGraphicsObject::mouseReleaseEvent(event);
}

