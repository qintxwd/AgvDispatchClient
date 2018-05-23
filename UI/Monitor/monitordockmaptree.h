#ifndef MONITORDOCKMAPTREE_H
#define MONITORDOCKMAPTREE_H

#include <QDockWidget>
#include "../../data/onemap.h"
#include "monitormaptreemodel.h"
#include <QTreeView>
#include <QMenu>

class MonitorDockMapTree: public QDockWidget
{
    Q_OBJECT
public:
    MonitorDockMapTree(OneMap *_onemap,QWidget *parent = nullptr);

signals:
    void sig_addFloor(MapFloor *floor);
    void sig_addRootPath();

    void sig_chooseSpirit(MapSpirit *spirit);
public slots:
    void ShowContextMenu(const QPoint&pos);

    void slot_selectChanged(QModelIndex index);

    void slot_addFloor();
    void slot_addRootPath();

    void slot_add_root_path(MapPoint *from,MapPoint *to);

    void refresh();

    void slot_chooseChanged(MapSpirit *spirit);
private:
    void initUi();
    OneMap *onemap;
    QTreeView *view;
    MonitorMapTreeModel *model;

    QAction *addFloor;
    QAction *addPathToRoot;

    QMenu* rootRighMenu;
};

#endif // MONITORDOCKMAPTREE_H
