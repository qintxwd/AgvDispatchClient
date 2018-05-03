﻿#include "dockview.h"
#include "scene.h"
#include "viewer.h"

DockView::DockView(OneMap *_oneMap, QWidget *parent) : QDockWidget(parent),
    oneMap(_oneMap)
{
    setWindowTitle(QStringLiteral("显示地图"));

    init();
}

void DockView::init()
{
    tabWidget = new QTabWidget;

    QList<MapFloor *> floors = oneMap->getFloors();

    foreach (auto floor, floors) {
        Scene *scene = new Scene(oneMap,floor);
        Viewer *view = new Viewer();
        view->setScene(scene);

        connect(scene,SIGNAL(sig_currentMousePos(QPointF)),this,SIGNAL(sig_currentMousePos(QPointF)));
        connect(scene,SIGNAL(sig_cancelTool()),this,SIGNAL(sig_cancelTool()));
        connect(this,SIGNAL(sig_setTool(int)),scene,SLOT(slot_setCurTool(int)));
        connect(scene,SIGNAL(sig_addStation(MapPoint*)),this,SLOT(slot_addStation(MapPoint*)));
        connect(scene,SIGNAL(sig_removeStation(MapPoint*)),this,SLOT(slot_removeStation(MapPoint*)));
        connect(scene,SIGNAL(sig_addPath(MapPath*)),this,SLOT(slot_addPath(MapPath*)));
        connect(scene,SIGNAL(sig_removePath(MapPath*)),this,SLOT(slot_removePath(MapPath*)));
        connect(scene,SIGNAL(sig_removeBkg(MapBackground*)),this,SLOT(slot_removeBkg(MapBackground*)));
        connect(scene,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
        connect(scene,SIGNAL(sig_chooseChanged(MapSpirit*)),this,SLOT(slot_chooseChanged(MapSpirit*)));
        connect(this,SIGNAL(sig_selectHand()),view,SLOT(toggleDragMode()));
        connect(this,SIGNAL(sig_selectSelect()),view,SLOT(toggleSelectMode()));

        tabWidget->addTab(view,floor->getName());
        scenes.append(scene);
    }

    setWidget(tabWidget);
}

void DockView::slot_addFloor(MapFloor *floor)
{
    Scene *scene = new Scene(oneMap,floor);
    Viewer *view = new Viewer();
    view->setScene(scene);

    connect(scene,SIGNAL(sig_currentMousePos(QPointF)),this,SIGNAL(sig_currentMousePos(QPointF)));
    connect(scene,SIGNAL(sig_cancelTool()),this,SIGNAL(sig_cancelTool()));
    connect(this,SIGNAL(sig_setTool(int)),scene,SLOT(slot_setCurTool(int)));
    connect(scene,SIGNAL(sig_addStation(MapPoint*)),this,SLOT(slot_addStation(MapPoint*)));
    connect(scene,SIGNAL(sig_removeStation(MapPoint*)),this,SLOT(slot_removeStation(MapPoint*)));
    connect(scene,SIGNAL(sig_addPath(MapPath*)),this,SLOT(slot_addPath(MapPath*)));
    connect(scene,SIGNAL(sig_removePath(MapPath*)),this,SLOT(slot_removePath(MapPath*)));
    connect(scene,SIGNAL(sig_removeBkg(MapBackground*)),this,SLOT(slot_removeBkg(MapBackground*)));
    connect(scene,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
    connect(scene,SIGNAL(sig_chooseChanged(MapSpirit*)),this,SIGNAL(sig_chooseChanged(MapSpirit*)));
    connect(this,SIGNAL(sig_selectHand()),view,SLOT(toggleDragMode()));
    connect(this,SIGNAL(sig_selectSelect()),view,SLOT(toggleSelectMode()));

    tabWidget->addTab(view,floor->getName());
    scenes.append(scene);
}

void DockView::slot_selectChanged(MapSpirit *spirit)
{
    if(spirit == nullptr) return ;
    if(spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Floor){
        for(int i=0;i<scenes.length();++i){
            if(scenes[i]->getFloor() == static_cast<MapSpirit *>(spirit)){
                tabWidget->setCurrentIndex(i);
                break;
            }
        }
    }else{
        foreach (auto s, scenes) {
            s->slot_selectItem(spirit);
        }
    }
}

//添加站点
void DockView::slot_addStation(MapPoint *spirit)
{
    Scene *scene = static_cast<Scene *>(sender());
    emit sig_addStation(scene->getFloor(),spirit);
}

//删除站点
void DockView::slot_removeStation(MapPoint *spirit)
{
    Scene *scene = static_cast<Scene *>(sender());
    emit sig_removeStation(scene->getFloor(),spirit);
}

//添加路径
void DockView::slot_addPath(MapPath *spirit)
{
    Scene *scene = static_cast<Scene *>(sender());
    emit sig_addPath(scene->getFloor(),spirit);
}

//删除路径
void DockView::slot_removePath(MapPath *spirit)
{
    Scene *scene = static_cast<Scene *>(sender());
    emit sig_removePath(scene->getFloor(),spirit);
}

//删除背景
void DockView::slot_removeBkg(MapBackground *spirit)
{
    Scene *scene = static_cast<Scene *>(sender());
    emit sig_removeBkg(scene->getFloor(),spirit);
}


