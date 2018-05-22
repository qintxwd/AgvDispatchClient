#include "maptreemodel.h"

#include <QStringList>
#include <QIcon>
#include <QFileIconProvider>

MapTreeModel::MapTreeModel(OneMap *_onemap, QObject *parent)
    : QAbstractItemModel(parent),
      onemap(_onemap)
{
    rootItem = new MapTreeItem(nullptr);
    setupModelData(rootItem);
}

MapTreeModel::~MapTreeModel()
{
    delete rootItem;
}

QModelIndexList MapTreeModel::getModelIndexs()
{
    return persistentIndexList();
}

int MapTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<MapTreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

MapTreeItem *MapTreeModel::getItem(const QModelIndex &index)
{
    if (!index.isValid())
        return nullptr;
    MapTreeItem *item = static_cast<MapTreeItem*>(index.internalPointer());
    return item;
}

QVariant MapTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // 添加图标
    if(role==Qt::DecorationRole && index.isValid()){
        MapTreeItem *item = static_cast<MapTreeItem*>(index.internalPointer());
        MapSpirit *spirit = item->getSpirit();
        if(spirit == nullptr){
            QFileIconProvider iconProvider;
            return QIcon(iconProvider.icon(QFileIconProvider::Folder));
        }else{
            if(spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Point){
                return QIcon(":/images/tree/point.18x18.png");
            }

            else if(spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Path){
                return QIcon(":/images/tree/path.18x18.png");
            }

            else if(spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Floor){
                return QIcon(":/images/menu/view-grid.png");
            }

            else if(spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Background){
                return QIcon(":/images/tree/location.18x18.png");
            }
        }
    }

    if(role==Qt::DisplayRole){
        MapTreeItem *item = static_cast<MapTreeItem*>(index.internalPointer());
        return item->data(index.column());
    }
    return QVariant();

}

Qt::ItemFlags MapTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant MapTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

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

void MapTreeModel::setupModelData(MapTreeItem *root)
{
    QList<MapFloor *> floors = QList<MapFloor *>::fromStdList(onemap->getFloors());

    QList<MapPath *> independentPaths = QList<MapPath *>::fromStdList(onemap->getRootPaths());

    foreach (auto floor, floors) {
        MapTreeItem *item_floor = new MapTreeItem(floor,root);

        MapTreeItem *item_point_folder = new MapTreeItem(nullptr,item_floor,"POINTS");
        item_floor->appendChild(item_point_folder);

        MapTreeItem *item_path_folder = new MapTreeItem(nullptr,item_floor,"PATHS");
        item_floor->appendChild(item_path_folder);
        auto points = floor->getPoints();
        foreach (auto __point, points) {
            MapPoint *pp =  static_cast<MapPoint*>(onemap->getSpiritById(__point));
            if(pp==nullptr)continue;
            MapTreeItem *item_point = new MapTreeItem(pp,item_point_folder);
            item_point_folder->appendChild(item_point);
        }
        auto paths = floor->getPaths();
        foreach (auto __path, paths) {
            MapPath *pp =  static_cast<MapPath*>(onemap->getSpiritById(__path));
            if(pp==nullptr)continue;
            MapTreeItem *item_path = new MapTreeItem(pp,item_path_folder);
            item_path_folder->appendChild(item_path);
        }
        if(floor->getBkg()!=0){
            MapBackground *bkg = static_cast<MapBackground*>(onemap->getSpiritById(floor->getBkg()));
            if(bkg==nullptr)continue;
            MapTreeItem *itemBkg = new MapTreeItem(bkg,item_floor,QString::fromStdString(bkg->getName()));
            item_floor->appendChild(itemBkg);
        }

        root->appendChild(item_floor);
    }

    foreach (auto __path, independentPaths) {
        MapTreeItem *item_pp = new MapTreeItem(__path,root);
        root->appendChild(item_pp);
    }
}

void MapTreeModel::fresh()
{
    beginResetModel();
    removeRows(0,this->rowCount());
    rootItem = new MapTreeItem(nullptr);
    setupModelData(rootItem);
    endResetModel();
}
