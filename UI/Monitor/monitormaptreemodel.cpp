#include "monitormaptreemodel.h"

#include <QStringList>
#include <QIcon>
#include <QFileIconProvider>

MonitorMapTreeModel::MonitorMapTreeModel(OneMap *_onemap, QObject *parent)
    : QAbstractItemModel(parent),
      onemap(_onemap)
{
    rootItem = new MonitorMapTreeItem(nullptr);
    setupModelData(rootItem);
}

MonitorMapTreeModel::~MonitorMapTreeModel()
{
    delete rootItem;
}

QModelIndexList MonitorMapTreeModel::getModelIndexs()
{
    return persistentIndexList();
}

int MonitorMapTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<MonitorMapTreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

MonitorMapTreeItem *MonitorMapTreeModel::getItem(const QModelIndex &index)
{
    if (!index.isValid())
        return nullptr;
    MonitorMapTreeItem *item = static_cast<MonitorMapTreeItem*>(index.internalPointer());
    return item;
}

QVariant MonitorMapTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // 添加图标
    if(role==Qt::DecorationRole && index.isValid()){
        MonitorMapTreeItem *item = static_cast<MonitorMapTreeItem*>(index.internalPointer());
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
        MonitorMapTreeItem *item = static_cast<MonitorMapTreeItem*>(index.internalPointer());
        return item->data(index.column());
    }
    return QVariant();

}

Qt::ItemFlags MonitorMapTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant MonitorMapTreeModel::headerData(int section, Qt::Orientation orientation,
                                         int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex MonitorMapTreeModel::index(int row, int column, const QModelIndex &parent)
const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    MonitorMapTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<MonitorMapTreeItem*>(parent.internalPointer());

    MonitorMapTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex MonitorMapTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    MonitorMapTreeItem *childItem = static_cast<MonitorMapTreeItem*>(index.internalPointer());
    MonitorMapTreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int MonitorMapTreeModel::rowCount(const QModelIndex &parent) const
{
    MonitorMapTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<MonitorMapTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void MonitorMapTreeModel::setupModelData(MonitorMapTreeItem *root)
{
    QList<MapFloor *> floors = QList<MapFloor *>::fromStdList(onemap->getFloors());

    QList<MapPath *> independentPaths = QList<MapPath *>::fromStdList(onemap->getRootPaths());

    foreach (auto floor, floors) {
        MonitorMapTreeItem *item_floor = new MonitorMapTreeItem(floor,root);

        MonitorMapTreeItem *item_point_folder = new MonitorMapTreeItem(nullptr,item_floor,"POINTS");
        item_floor->appendChild(item_point_folder);

        MonitorMapTreeItem *item_path_folder = new MonitorMapTreeItem(nullptr,item_floor,"PATHS");
        item_floor->appendChild(item_path_folder);
        auto points = floor->getPoints();
        foreach (auto __point, points) {
            MapPoint *pp =  static_cast<MapPoint*>(onemap->getSpiritById(__point));
            if(pp==nullptr)continue;
            MonitorMapTreeItem *item_point = new MonitorMapTreeItem(pp,item_point_folder);
            item_point_folder->appendChild(item_point);
        }
        auto paths = floor->getPaths();
        foreach (auto __path, paths) {
            MapPath *pp =  static_cast<MapPath*>(onemap->getSpiritById(__path));
            if(pp==nullptr)continue;
            MonitorMapTreeItem *item_path = new MonitorMapTreeItem(pp,item_path_folder);
            item_path_folder->appendChild(item_path);
        }
        if(floor->getBkg()!=0){
            MapBackground *bkg = static_cast<MapBackground*>(onemap->getSpiritById(floor->getBkg()));
            if(bkg!=nullptr){
                MonitorMapTreeItem *itemBkg = new MonitorMapTreeItem(bkg,item_floor,QString::fromStdString(bkg->getName()));
                item_floor->appendChild(itemBkg);
            }
        }

        root->appendChild(item_floor);
    }

    foreach (auto __path, independentPaths) {
        MonitorMapTreeItem *item_pp = new MonitorMapTreeItem(__path,root);
        root->appendChild(item_pp);
    }
}

void MonitorMapTreeModel::fresh()
{
    beginResetModel();
    removeRows(0,this->rowCount());
    rootItem = new MonitorMapTreeItem(nullptr);
    setupModelData(rootItem);
    endResetModel();
}
