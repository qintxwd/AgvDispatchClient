#include "monitordockview.h"
#include "monitorscene.h"
#include "monitorviewer.h"

MonitorDockView::MonitorDockView(OneMap *_oneMap, QWidget *parent) : QDockWidget(parent),
    oneMap(_oneMap)
{
    setWindowTitle(QStringLiteral("显示地图"));

    init();
}

void MonitorDockView::init()
{
    tabWidget = new QTabWidget;

    auto floors = oneMap->getFloors();

    foreach (auto floor, floors) {
        MonitorScene *scene = new MonitorScene(oneMap,floor);
        MonitorViewer *view = new MonitorViewer();
        view->setScene(scene);

        connect(scene,SIGNAL(sig_currentMousePos(QPointF)),this,SIGNAL(sig_currentMousePos(QPointF)));
        connect(scene,SIGNAL(sig_cancelTool()),this,SIGNAL(sig_cancelTool()));
        connect(this,SIGNAL(sig_setTool(int)),scene,SLOT(slot_setCurTool(int)));
        connect(scene,SIGNAL(sig_add_remove_spirit()),this,SIGNAL(sig_add_remove_spirit()));
        connect(scene,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
        connect(scene,SIGNAL(sig_chooseChanged(MapSpirit*)),this,SLOT(slot_chooseChanged(MapSpirit*)));
        connect(this,SIGNAL(sig_selectHand()),view,SLOT(toggleDragMode()));
        connect(this,SIGNAL(sig_selectSelect()),view,SLOT(toggleSelectMode()));
        connect(this,SIGNAL(sig_propertyChangedFromProperty(MapSpirit *)),scene,SLOT(propertyChanged(MapSpirit *)));
        connect(this,SIGNAL(sig_propertyChangedFromProperty(MapSpirit *)),this,SLOT(slot_propertyChangedFromProperty(MapSpirit *)));

        tabWidget->addTab(view,QString::fromStdString(floor->getName()));
        scenes.append(scene);
    }

    setWidget(tabWidget);
}

void MonitorDockView::slot_addFloor(MapFloor *floor)
{
    MonitorScene *scene = new MonitorScene(oneMap,floor);
    MonitorViewer *view = new MonitorViewer();
    view->setScene(scene);

    connect(scene,SIGNAL(sig_currentMousePos(QPointF)),this,SIGNAL(sig_currentMousePos(QPointF)));
    connect(scene,SIGNAL(sig_cancelTool()),this,SIGNAL(sig_cancelTool()));
    connect(this,SIGNAL(sig_setTool(int)),scene,SLOT(slot_setCurTool(int)));
    connect(scene,SIGNAL(sig_add_remove_spirit()),this,SIGNAL(sig_add_remove_spirit()));
    connect(scene,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
    connect(scene,SIGNAL(sig_chooseChanged(MapSpirit*)),this,SIGNAL(sig_chooseChanged(MapSpirit*)));
    connect(this,SIGNAL(sig_selectHand()),view,SLOT(toggleDragMode()));
    connect(this,SIGNAL(sig_selectSelect()),view,SLOT(toggleSelectMode()));
    connect(this,SIGNAL(sig_propertyChangedFromProperty(MapSpirit *)),scene,SLOT(propertyChanged(MapSpirit *)));
    connect(this,SIGNAL(sig_propertyChangedFromProperty(MapSpirit *)),this,SLOT(slot_propertyChangedFromProperty(MapSpirit *)));

    tabWidget->addTab(view,QString::fromStdString(floor->getName()));
    scenes.append(scene);
}

void MonitorDockView::slot_addBkg(int _bkg){
    int kk = tabWidget->currentIndex();
    auto floors = QList<MapFloor *>::fromStdList(oneMap->getFloors());
    if(kk<floors.length()){
        floors[kk]->setBkg(_bkg);
        emit sig_add_remove_spirit();
        //emit sig_propertyChangedFromProperty(floors[kk]);
    }
}

void MonitorDockView::slot_selectChanged(MapSpirit *spirit)
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

void MonitorDockView::slot_propertyChangedFromProperty(MapSpirit *_spirit)
{
    //如果是floor的话，修改tab的名字
    if(_spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Floor){
        int tabIndex = -1;
        for(int i=0;i<scenes.length();++i){
            if(scenes[i]->getFloor() == _spirit){
                //TODO:
                tabIndex = i;
                break;
            }
        }

        if(tabIndex>=0){
            tabWidget->setTabText(tabIndex,QString::fromStdString(_spirit->getName()));
        }
    }
}
