#include "itembackground.h"
#include<QPainter>
ItemBackground::ItemBackground(QGraphicsItem *parent):QGraphicsObject(parent),
    filePath("")
{

}

QRectF ItemBackground::boundingRect() const
{
    return QRectF(0, 0, 1280, 720);
}

void ItemBackground::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(item)
    Q_UNUSED(widget)
    if(filePath.length()>0)
    {
        QImage img1;
        if(img1.load(filePath))
        {
            QImage img2 = img1.scaled(boundingRect().width(),boundingRect().height());
            painter->setRenderHint(QPainter::Antialiasing, true);
            painter->drawImage(QPoint(0,0),img2);
            painter->fillRect(boundingRect(),QColor(0,0,0,100));
        }
    }
}


