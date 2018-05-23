#ifndef MONITORMAPITEMBKG_H
#define MONITORMAPITEMBKG_H

#include <QGraphicsObject>
#include "../../data/onemap.h"

class MonitorMapItemBkg : public QGraphicsObject
{
    Q_OBJECT
public:
    MonitorMapItemBkg(MapBackground *_bkg, QGraphicsItem *parent = nullptr);
    enum { Type = UserType + 26 };
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    MapBackground *getBkg(){return bkg;}

    void setBkg(MapBackground *_bkg){
        bkg = _bkg;
        emit sig_positionChange(bkg);
    }

    void my_update();
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:
    void sig_positionChange(MapSpirit *);
public slots:

private:
    MapBackground *bkg;
    QImage img;
};

#endif // MONITORMAPITEMBKG_H
