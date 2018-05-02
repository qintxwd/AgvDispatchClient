#ifndef DOCKVIEW_H
#define DOCKVIEW_H

#include <QWidget>
#include <QDockWidget>
#include <QTabWidget>
#include "../../data/onemap.h"

class Scene;

class DockView : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockView(OneMap* _oneMap,QWidget *parent = nullptr);

    void init();
signals:
    void sig_currentMousePos(QPointF pos);
public slots:
    void slot_addFloor(MapFloor *spirit);
    void slot_selectChanged(MapSpirit *spirit);
private:
    OneMap *oneMap;

    QTabWidget *tabWidget;

    QList<Scene *> scenes;
};

#endif // DOCKVIEW_H
