#ifndef ITEMBACKGROUND_H
#define ITEMBACKGROUND_H

#include <QGraphicsObject>

class ItemBackground : public QGraphicsObject
{
    Q_OBJECT
public:
    ItemBackground(QGraphicsItem *parent = Q_NULLPTR);
    enum { Type = UserType + 13 };
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    void setImg(QString _filePath){filePath=_filePath;update();}

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;


public:
    QString filePath;
};

#endif // ITEMBACKGROUND_H
