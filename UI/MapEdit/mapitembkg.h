#ifndef MAPITEMBKG_H
#define MAPITEMBKG_H

#include <QGraphicsObject>
#include "../../data/onemap.h"

class MapItemBkg : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit MapItemBkg(MapBackground *_bkg, QGraphicsItem *parent = nullptr);
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
    }

    void my_update();
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:
    void sig_bkgPositionChange(MapBackground *);
public slots:

private:
    MapBackground *bkg;
};

#endif // MAPITEMBKG_H
