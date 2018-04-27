#ifndef DOCKVIEW_H
#define DOCKVIEW_H

#include <QWidget>
#include <QDockWidget>
#include "../../data/onemap.h"

class DockView : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockView(OneMap* _oneMap,QWidget *parent = nullptr);

signals:

public slots:

private:
    OneMap *oneMap;
};

#endif // DOCKVIEW_H
