#ifndef MAPTREEMODEL_H
#define MAPTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "maptreeitem.h"
#include "../../data/onemap.h"



class MapTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit MapTreeModel(OneMap *_onemap,QObject *parent = 0);
    ~MapTreeModel();

    MapTreeItem *getItem(const QModelIndex &index);
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
public slots:
    //增加、删除、修改名字时用
    void fresh();
private:
    void setupModelData(MapTreeItem *root);
    OneMap *onemap;
    MapTreeItem *rootItem;
};

#endif // MAPTREEMODEL_H
