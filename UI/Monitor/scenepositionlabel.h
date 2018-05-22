#ifndef SCENEPOSITIONLABEL_H
#define SCENEPOSITIONLABEL_H

#include <QObject>
#include <QGraphicsObject>
#include <QPointF>

class ScenePositionLabel : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit ScenePositionLabel(QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
signals:

public slots:
    void positionChanged(QPointF _p);
private:
    QPointF p;
};

#endif // SCENEPOSITIONLABEL_H
