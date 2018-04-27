#ifndef DOCKPROPERTY_H
#define DOCKPROPERTY_H

#include <QDockWidget>
#include "../../data/onemap.h"

class DockProperty : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockProperty(QWidget *parent = nullptr);

signals:

public slots:
    void showSpirit(MapSpirit *_spirit);
private:
    MapSpirit *spirit;
};

#endif // DOCKPROPERTY_H
