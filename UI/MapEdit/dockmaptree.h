#ifndef DOCKMAPTREE_H
#define DOCKMAPTREE_H

#include <QDockWidget>
#include "../../data/onemap.h"
#include "maptreemodel.h"
#include <QTreeView>
#include <QMenu>

class DockMapTree : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockMapTree(OneMap *_onemap,QWidget *parent = nullptr);

signals:

public slots:
    void ShowContextMenu(const QPoint&pos);
private:
    void initUi();
    OneMap *onemap;
    QTreeView *view;
    MapTreeModel *model;

    QAction *addFloor;
    QAction *addPathToRoot;
    QAction *addPath;
    QAction *addPoint;

    QMenu* rootRighMenu;
    QMenu* pointFolderRightMenu;
    QMenu* pathFolderRightMenu;
};

#endif // DOCKMAPTREE_H
