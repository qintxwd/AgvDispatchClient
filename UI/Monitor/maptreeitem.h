#ifndef MAPTREEITEM_H
#define MAPTREEITEM_H

#include <QVariant>
#include <QList>
#include "../../data/onemap.h"

//树形结构的节点
class MapTreeItem
{
public:
    explicit MapTreeItem(MapSpirit *_spirit, MapTreeItem *parentItem = 0,QString _str = "");

    ~MapTreeItem();

    void appendChild(MapTreeItem *child);

    MapTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    MapSpirit *getSpirit(){return spirit;}
    int row() const;
    MapTreeItem *parentItem();
private:
    MapTreeItem *m_parentItem;
    QList<MapTreeItem *> m_childItems;
    MapSpirit *spirit;
    QString str;
};

#endif // MAPTREEITEM_H
