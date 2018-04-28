#ifndef DOCKVIEW_H
#define DOCKVIEW_H

#include <QWidget>
#include <QDockWidget>
#include <QTabWidget>
#include "../../data/onemap.h"

class DockView : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockView(OneMap* _oneMap,QWidget *parent = nullptr);

    void init();
signals:

public slots:

private:
    OneMap *oneMap;

    QTabWidget *tabWidget;
};

#endif // DOCKVIEW_H
