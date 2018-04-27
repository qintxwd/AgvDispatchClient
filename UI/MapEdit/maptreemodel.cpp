#include "maptreemodel.h"

#include <QStringList>
#include <QIcon>

//! [0]
MapTreeModel::MapTreeModel(OneMap *_onemap, QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new MapTreeItem(nullptr);
    setupModelData(_onemap, rootItem);
}
//! [0]

//! [1]
MapTreeModel::~MapTreeModel()
{
    delete rootItem;
}
//! [1]

//! [2]
int MapTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<MapTreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}
//! [2]
MapTreeItem *MapTreeModel::getItem(const QModelIndex &index)
{
    if (!index.isValid())
        return nullptr;
    MapTreeItem *item = static_cast<MapTreeItem*>(index.internalPointer());
    return item;
}
//! [3]
QVariant MapTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // 添加图标
    if(role==Qt::DecorationRole)
        return QIcon(":/images/panel/battery-100-2.png");

    if(role==Qt::DisplayRole){
        MapTreeItem *item = static_cast<MapTreeItem*>(index.internalPointer());
        return item->data(index.column());
    }
    return QVariant();

}
//! [3]

//! [4]
Qt::ItemFlags MapTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}
//! [4]

//! [5]
QVariant MapTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}
//! [5]

//! [6]
QModelIndex MapTreeModel::index(int row, int column, const QModelIndex &parent)
const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    MapTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<MapTreeItem*>(parent.internalPointer());

    MapTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
//! [6]

//! [7]
QModelIndex MapTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    MapTreeItem *childItem = static_cast<MapTreeItem*>(index.internalPointer());
    MapTreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}
//! [7]

//! [8]
int MapTreeModel::rowCount(const QModelIndex &parent) const
{
    MapTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<MapTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}
//! [8]

void MapTreeModel::setupModelData(OneMap *onemap, MapTreeItem *root)
{
    QList<MapSpirit *> floors = onemap->getSpiritByType(MapSpirit::Map_Sprite_Type_Floor);

    QList<MapSpirit *> paths = onemap->getSpiritByType(MapSpirit::Map_Sprite_Type_Path);

    QList<MapSpirit *> independentPaths;

    foreach (auto __path, paths) {
        bool independent = true;
        foreach(auto _floor,floors){
            MapFloor *floor = static_cast<MapFloor *>(_floor);
            for(auto p:floor->getPaths()){
                if(__path->getId() == p){
                    independent = false;
                    break;
                }
            }
            if(!independent)break;
        }
        if(independent){
            independentPaths.push_back(__path);
        }
    }

    foreach (auto _floor, floors) {
        MapFloor *floor = static_cast<MapFloor *>(_floor);
        MapTreeItem *item_floor = new MapTreeItem(_floor,root);

        MapTreeItem *item_point_folder = new MapTreeItem(nullptr,item_floor,"POINTS");
        item_floor->appendChild(item_point_folder);

        MapTreeItem *item_path_folder = new MapTreeItem(nullptr,item_floor,"PATHS");
        item_floor->appendChild(item_path_folder);

        foreach (auto __point, floor->getPoints()) {
            MapTreeItem *item_point = new MapTreeItem(onemap->getSpiritById(__point),item_point_folder);
            item_point_folder->appendChild(item_point);
        }

        foreach (auto __path, floor->getPaths()) {
            MapTreeItem *item_path = new MapTreeItem(onemap->getSpiritById(__path),item_path_folder);
            item_path_folder->appendChild(item_path);
        }

        root->appendChild(item_floor);
    }

    foreach (auto __path, independentPaths) {
        MapTreeItem *item_pp = new MapTreeItem(__path,root);
        root->appendChild(item_pp);
    }
}
