#include "dockmaptree.h"
#include <QHeaderView>
#include <QVariant>

DockMapTree::DockMapTree(OneMap *_onemap, QWidget *parent) : QDockWidget(parent),
    onemap(_onemap)
{
    model = new MapTreeModel(onemap);
    initUi();
}

void DockMapTree::initUi()
{
    view = new QTreeView;
    view->setModel(model);
    //支持右键菜单
    view->setContextMenuPolicy(Qt::CustomContextMenu);

    addFloor = new QAction(QStringLiteral("添加楼层"),this);;
    addPathToRoot = new QAction(QStringLiteral("添加线路"),this);;
    addPath = new QAction(QStringLiteral("添加线路"),this);;
    addPoint = new QAction(QStringLiteral("添加站点"),this);;

    //右键菜单
    rootRighMenu = new QMenu(this);
    rootRighMenu->addAction(addFloor);
    rootRighMenu->addAction(addPathToRoot);

    pointFolderRightMenu = new QMenu(this);
    pointFolderRightMenu->addAction(addPoint);

    pathFolderRightMenu = new QMenu(this);
    pathFolderRightMenu->addAction(addPath);

    connect(view, SIGNAL(customContextMenuRequested(const QPoint& )), this, SLOT(ShowContextMenu(const QPoint&)));

    view->header()->hide();
    setWidget(view);

    setMaximumWidth(300);
}

void DockMapTree::ShowContextMenu(const QPoint& pos)
{
    QModelIndex index = view->indexAt(pos);
    MapTreeItem *item = model->getItem(index);
    if(item == nullptr){
        rootRighMenu->exec(QCursor::pos());
        return ;
    }

    if(item->getSpirit() == nullptr && item->data(0).toString() == "POINTS"){
        pointFolderRightMenu->exec(QCursor::pos());
    }else if(item->getSpirit() == nullptr && item->data(0).toString() == "PATHS"){
        pathFolderRightMenu->exec(QCursor::pos());
    }
}
