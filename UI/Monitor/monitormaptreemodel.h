#ifndef MONITORMAPTREEMODEL_H
#define MONITORMAPTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "monitormaptreeitem.h"
#include "../../data/onemap.h"

class MonitorMapTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit MonitorMapTreeModel(OneMap *_onemap,QObject *parent = 0);
    ~MonitorMapTreeModel();

    MonitorMapTreeItem *getItem(const QModelIndex &index);
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QModelIndexList getModelIndexs();
public slots:
    //增加、删除、修改名字时用
    void fresh();
private:
    void setupModelData(MonitorMapTreeItem *root);
    OneMap *onemap;
    MonitorMapTreeItem *rootItem;
};


#endif // MONITORMAPTREEMODEL_H
