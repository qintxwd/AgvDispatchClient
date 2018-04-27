#include "dockview.h"

DockView::DockView(OneMap *_oneMap, QWidget *parent) : QDockWidget(parent),
    oneMap(_oneMap)
{
    setWindowTitle(QStringLiteral("显示地图"));
}
