#include "mapitembkg.h"
#include <QtWidgets>

MapItemBkg::MapItemBkg(MapBackground *_bkg, QGraphicsItem *parent) : QGraphicsObject(parent),
    bkg(_bkg)
{
    setZValue(1);
    setFlags(ItemIsSelectable| QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
    setPos(bkg->getX(),bkg->getY());
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

//外接区域
QRectF MapItemBkg::boundingRect() const
{
    return QRectF(bkg->getX(), bkg->getY(), bkg->getWidth(), bkg->getHeight());
}

void MapItemBkg::my_update()
{
    //setPos(bkg->getX(),bkg->getY());
    prepareGeometryChange();
    update();
}

//绘制
void MapItemBkg::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor _color = Qt::gray;

    if(option->state & QStyle::State_Selected)_color = Qt::blue;
    if (option->state & QStyle::State_MouseOver)_color = Qt::blue;

    QPen oldPen = painter->pen();
    QPen pen(_color);

    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawRect(boundingRect());

    if(img.isNull()){
        if(bkg!=nullptr && bkg->getImgData()!=nullptr && bkg->getImgDataLen()>0){
            QImage img;
            QByteArray ba(bkg->getImgData(),bkg->getImgDataLen());
            QDataStream ds(&ba,QIODevice::ReadOnly);
            ds>>img;
            if(!img.isNull()){
                painter->drawImage(boundingRect(),img);
            }
        }
    }else{
        painter->drawImage(boundingRect(),img);
    }

    painter->setPen(oldPen);
}

QVariant MapItemBkg::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange)
    {
        QPointF newPos = value.toPointF();
        //更新位置的同时，更新连线的起止点
        bkg->setX(newPos.x());
        bkg->setY(newPos.y());
        //TODO:
        emit sig_positionChange(bkg);
        return newPos;
    }
    return QGraphicsItem::itemChange(change, value);
}
