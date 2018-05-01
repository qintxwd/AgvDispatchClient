#ifndef MAPITEMSTATION_H
#define MAPITEMSTATION_H

#include <QObject>
#include <QGraphicsObject>
#include "../../data/onemap.h"

class Scene;
class MapItemQuadraticBezier;
class MapItemCubicBezier;
class MapItemLine;

class MapItemStation : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit MapItemStation(MapPoint *_point,QObject *parent = nullptr);
    enum { Type = UserType + 21 };
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    void addLine(MapItemLine *line){if(!lines.contains(line))lines.append(line);}
    void addQb(MapItemQuadraticBezier *arc){if(!qbs.contains(arc))qbs.append(arc);}
    void addCb(MapItemCubicBezier *arc){if(!cbs.contains(arc))cbs.append(arc);}

    void removeLine(MapItemLine *line){lines.removeAll(line);}
    void removeQb(MapItemQuadraticBezier *arc){qbs.removeAll(arc);}
    void removeCb(MapItemCubicBezier *arc){cbs.removeAll(arc);}

    MapPoint *getPoint(){return point;}

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    //周围的线路们
    QList<MapItemQuadraticBezier *> qbs;
    QList<MapItemCubicBezier *> cbs;
    QList<MapItemLine *> lines;

    //当前的站点
    MapPoint *point;
};

#endif // MAPITEMSTATION_H
