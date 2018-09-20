#ifndef MONITORMAPITEMAGV_H
#define MONITORMAPITEMAGV_H

#include <QGraphicsObject>
#include <QTimer>

class MonitorMapItemAgv : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit MonitorMapItemAgv(int _id, QString _name, int _floor, QGraphicsItem *parent = nullptr);

    enum { Type = UserType + 27};
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    int getId(){return agv_id;}

    QColor getColor(){return color;}

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void my_update();
signals:

public slots:
    void slot_update_pos(int id, QString name, double x, double y, double angle,  QStringList occs, int _floor);
    void slot_flick();
private:
    QColor color;
    int agv_id;
    QString name;
    int floor;//item shuyu nage scene

    QTimer flicktimer;
};

#endif // MONITORMAPITEMAGV_H
