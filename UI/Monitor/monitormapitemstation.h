#ifndef MONITORMAPITEMSTATION_H
#define MONITORMAPITEMSTATION_H

#include <QObject>
#include <QGraphicsObject>
#include "../../data/onemap.h"

class MonitorMapItemQuadraticBezier;
class MonitorMapItemCubicBezier;
class MonitorMapItemLine;


class MonitorMapItemStation : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit MonitorMapItemStation(MapPoint *_point,QGraphicsItem *parent = nullptr);
    enum { Type = UserType + 21 };
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    void setColor(QColor _color){color = _color;update();}

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    void addLine(MonitorMapItemLine *line){if(!lines.contains(line))lines.append(line);}
    void addQb(MonitorMapItemQuadraticBezier *arc){if(!qbs.contains(arc))qbs.append(arc);}
    void addCb(MonitorMapItemCubicBezier *arc){if(!cbs.contains(arc))cbs.append(arc);}

    void removeLine(MonitorMapItemLine *line){lines.removeAll(line);}
    void removeQb(MonitorMapItemQuadraticBezier *arc){qbs.removeAll(arc);}
    void removeCb(MonitorMapItemCubicBezier *arc){cbs.removeAll(arc);}

    MapPoint *getPoint(){return point;}
    QList<MonitorMapItemQuadraticBezier *> getQbs(){return qbs;}
    QList<MonitorMapItemCubicBezier *> getCbs(){return cbs;}
    QList<MonitorMapItemLine *> getLines(){return lines;}

    void my_update();
signals:
    void sig_propertyChanged(MapSpirit *spirit);
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    void loadImg();
    //周围的线路们
    QList<MonitorMapItemQuadraticBezier *> qbs;
    QList<MonitorMapItemCubicBezier *> cbs;
    QList<MonitorMapItemLine *> lines;

    //当前的站点
    MapPoint *point;

    QImage *imgDraw;
    QImage *imgReport;
    QImage *imgHalt;
    QImage *imgCharge;
    QImage *imgLoad;
    QImage *imgUnload;
    QImage *imgLoadUnload;

    QColor color;
};

#endif // MONITORMAPITEMSTATION_H
