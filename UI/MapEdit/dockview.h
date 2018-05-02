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
    void sig_cancelTool();
    void sig_setTool(int);

    //添加站点
    void sig_addStation(MapFloor *floor, MapPoint *spirit);

    //删除站点
    void sig_removeStation(MapFloor *floor,MapPoint *spirit);

    //添加路径
    void sig_addPath(MapFloor *floor,MapPath *spirit);

    //删除路径
    void sig_removePath(MapFloor *floor,MapPath *spirit);

    //删除背景
    void sig_removeBkg(MapFloor *floor,MapBackground *spirit);

    //站点属性修改
    void sig_stationPropertyChanged(MapFloor *floor,MapPoint *spirit);

    //路径属性修改
    void sig_pathPropertyChanged(MapFloor *floor,MapPath *spirit);

    //背景图片属性修改
    void sig_bgkPropertyChanged(MapFloor *floor,MapBackground *spirit);

    //选择修改
    void sig_chooseChanged(MapFloor *floor,MapSpirit *spirit);

public slots:

    void slot_addFloor(MapFloor *floor);

    void slot_selectChanged(MapSpirit *spirit);


    //添加站点
    void slot_addStation(MapPoint *spirit);

    //删除站点
    void slot_removeStation(MapPoint *spirit);

    //添加路径
    void slot_addPath(MapPath *spirit);

    //删除路径
    void slot_removePath(MapPath *spirit);

    //删除背景
    void slot_removeBkg(MapBackground *spirit);

    //站点属性修改
    void slot_stationPropertyChanged(MapPoint *spirit);

    //路径属性修改
    void slot_pathPropertyChanged(MapPath *spirit);

    //背景图片属性修改
    void slot_bgkPropertyChanged(MapBackground *spirit);

    //选择修改
    void slot_chooseChanged(MapSpirit *spirit);
private:
    OneMap *oneMap;

    QTabWidget *tabWidget;

    QList<Scene *> scenes;
};

#endif // DOCKVIEW_H
