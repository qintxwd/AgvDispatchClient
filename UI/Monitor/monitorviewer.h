#ifndef MONITORVIEWER_H
#define MONITORVIEWER_H

#include <QObject>
#include <QGraphicsView>

class QPushButton;
class MonitorScene;

class MonitorViewer: public QGraphicsView
{
    Q_OBJECT
public:
    MonitorViewer(QWidget *parent = nullptr);
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
#endif // MONITORVIEWER_H
