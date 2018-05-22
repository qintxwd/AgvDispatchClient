#include "scenepositionlabel.h"
#include <QFont>
#include <QPainter>

ScenePositionLabel::ScenePositionLabel(QGraphicsItem *parent) : QGraphicsObject(parent)
{
    setZValue(1000);
}

QRectF ScenePositionLabel::boundingRect() const
{
    return QRectF(0, 0, 120, 40);
}

void ScenePositionLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPen oldPen = painter->pen();
    painter->setPen(QPen(Qt::red, 1));
    QFont font("Times", 10);
    font.setStyleStrategy(QFont::ForceOutline);
    painter->setFont(font);
    painter->drawText(0, 0, QString("%1  %2").arg(p.x()).arg(p.y()));
    painter->setPen(oldPen);
}

void ScenePositionLabel::positionChanged(QPointF _p)
{
    p=_p;
    update();
}
