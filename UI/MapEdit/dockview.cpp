#include "dockview.h"
#include "scene.h"
#include <QGraphicsView>

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
        QGraphicsView *view = new QGraphicsView(scene);

        view->setBackgroundBrush(QPixmap("qrc:/images/point/ChargingStation.20x20.png"));
        view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

        tabWidget->addTab(view,floor->getName());
        scenes.append(scene);
    }

    setWidget(tabWidget);
}

void DockView::slot_addFloor(MapFloor *spirit)
{
    Scene *scene = new Scene(oneMap,spirit);
    QGraphicsView *view = new QGraphicsView(scene);
    tabWidget->addTab(view,spirit->getName());
    scenes.append(scene);
}

void DockView::slot_selectChanged(MapSpirit *spirit)
{
    if(spirit == nullptr) return ;
    if(spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Floor){
        for(int i=0;i<scenes.length();++i){
            if(scenes[i]->getFloor() == dynamic_cast<MapSpirit *>(spirit)){
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

