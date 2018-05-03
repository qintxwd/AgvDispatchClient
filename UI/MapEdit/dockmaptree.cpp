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

    connect(addFloor,SIGNAL(triggered(bool)),this,SLOT(slot_addFloor()));
    connect(addPathToRoot,SIGNAL(triggered(bool)),this,SLOT(slot_addRootPath()));

    //右键菜单
    rootRighMenu = new QMenu(this);
    rootRighMenu->addAction(addFloor);
    rootRighMenu->addAction(addPathToRoot);

    connect(view, SIGNAL(customContextMenuRequested(const QPoint& )), this, SLOT(ShowContextMenu(const QPoint&)));
    connect(view,SIGNAL(clicked(QModelIndex)),this,SLOT(slot_selectChanged(QModelIndex)));
    view->header()->hide();
    setWidget(view);
    view->expandAll();

    setMaximumWidth(300);
    setWindowTitle(QStringLiteral("地图结点"));
}

void DockMapTree::slot_selectChanged(QModelIndex index)
{
    MapTreeItem *item = model->getItem(index);
    if(item != nullptr && item->getSpirit() != nullptr){
        emit sig_chooseSpirit(item->getSpirit());
    }else{
        emit sig_chooseSpirit(nullptr);
    }
}

void DockMapTree::ShowContextMenu(const QPoint& pos)
{
    QModelIndex index = view->indexAt(pos);
    MapTreeItem *item = model->getItem(index);
    if(item == nullptr){
        rootRighMenu->exec(QCursor::pos());
        return ;
    }
}

void DockMapTree::slot_addFloor()
{
    int id = onemap->getNextId();
    QString name = QString("floor_%1").arg(id);
    MapFloor *floor = new MapFloor(id,name,onemap);
    onemap->addFloor(floor);
    model->fresh();
    emit sig_addFloor(floor);
}

void DockMapTree::slot_addRootPath()
{

    //弹出对话框，列出所有的mapchange 点，选择
    //TODO

}

void DockMapTree::refresh()
{
    model->fresh();
    view->expandAll();
}

void DockMapTree::slot_chooseChanged(MapSpirit *spirit)
{
    //TODO： set select !
    view->expandAll();
    QModelIndexList pp = model->getModelIndexs();
    foreach (auto p, pp) {
        if (!p.isValid())continue;
        MapTreeItem *item = static_cast<MapTreeItem*>(p.internalPointer());
        if(item->getSpirit() == spirit){
            view->setCurrentIndex(p);
            break;
        }
    }
}



