#ifndef MONITORMAPITEMAGV_H
#define MONITORMAPITEMAGV_H

#include <QGraphicsObject>

class MonitorMapItemAgv : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit MonitorMapItemAgv(int _id,QString _name,QGraphicsItem *parent = nullptr);

    enum { Type = UserType + 27};
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void my_update();
signals:

public slots:
    void slot_update_pos(int id, QString name, double x, double y, double angle);
private:
    QColor color;
    int agv_id;
    QString name;
};

#endif // MONITORMAPITEMAGV_H
