#ifndef VIEWER_H
#define VIEWER_H

#include <QObject>
#include <QGraphicsView>

class QPushButton;
class Scene;
class Viewer : public QGraphicsView
{
    Q_OBJECT
public:
    Viewer(QWidget *parent = nullptr);
public slots:
    void toggleSelectMode();
    void toggleDragMode();
protected:
    virtual void wheelEvent(QWheelEvent *event);
signals:

public slots:
private slots:

private:

};

#endif // VIEWER_H
