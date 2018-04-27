#include "dockproperty.h"

DockProperty::DockProperty(QWidget *parent) : QDockWidget(parent)
{
    setMaximumWidth(300);
}

void DockProperty::showSpirit(MapSpirit *_spirit)
{
    spirit = _spirit;
}
