#ifndef MONITORDOCKVIEW_H
#define MONITORDOCKVIEW_H

#include <QWidget>
#include <QDockWidget>
#include <QTabWidget>
#include "../../data/onemap.h"

class MonitorScene;

class MonitorDockView : public QDockWidget
{
    Q_OBJECT
public:
    MonitorDockView(OneMap* _oneMap,QWidget *parent = nullptr);

    void init();
signals:
    void sig_currentMousePos(QPointF pos);
    void sig_cancelTool();
    void sig_setTool(int);


    void sig_add_remove_spirit();

    //属性修改[map --> property & map --> tree]
    void sig_propertyChanged(MapSpirit *spirit);

    //选择修改
    void sig_chooseChanged(MapSpirit *spirit);

    void sig_selectSelect();

    void sig_selectHand();

    void sig_propertyChangedFromProperty(MapSpirit *spirit);

public slots:

    void slot_addFloor(MapFloor *floor);

    void slot_selectChanged(MapSpirit *spirit);

    void slot_propertyChangedFromProperty(MapSpirit *_spirit);

    void slot_addBkg(int _bkg);
private:
    OneMap *oneMap;

    QTabWidget *tabWidget;

    QList<MonitorScene *> scenes;
};

#endif // MONITORDOCKVIEW_H
