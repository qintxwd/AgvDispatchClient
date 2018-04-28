#include "dockview.h"
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

    QList<MapSpirit *> floors = oneMap->getSpiritByType(MapSpirit::Map_Sprite_Type_Floor);

    for(int i=0;i<floors.length();++i){
        Scene *scene = new Scene(oneMap,floors[i]->getId());
        Viewer *viewer = new Viewer(oneMap,floors[i]->getId());
        viewer->setScene(scene);
        tabWidget->addTab(viewer,floors[i]->getName());
    }

    setWidget(tabWidget);
}
