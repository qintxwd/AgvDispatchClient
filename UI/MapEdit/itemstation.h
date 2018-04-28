#ifndef ITEMSTATION_H
#define ITEMSTATION_H

#include <QGraphicsObject>
#include "../../data/onemap.h"
class Scene;
class ItemArc;
class ItemLine;


class ItemStation : public QGraphicsObject
{
    Q_OBJECT
public:
    ItemStation(MapPoint *point, QGraphicsItem *parent = Q_NULLPTR);

    enum { Type = UserType + 14 };

    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    bool initialize();

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    void addLine(ItemLine *line){if(!lines.contains(line))lines.append(line);}
    void addArc(ItemArc *arc){if(!arcs.contains(arc))arcs.append(arc);}
    void removeLine(ItemLine *line){lines.removeAll(line);}
    void removeArc(ItemArc *arc){arcs.removeAll(arc);}

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    Scene *myscene;
public:
    QList<ItemArc *> arcs;
    QList<ItemLine *> lines;

    MapPoint *station;
};

#endif // ITEMSTATION_H
