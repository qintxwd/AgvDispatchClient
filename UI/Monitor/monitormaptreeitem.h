#ifndef MONITORMAPTREEITEM_H
#define MONITORMAPTREEITEM_H

#include <QVariant>
#include <QList>
#include "../../data/onemap.h"

class MonitorMapTreeItem
{
public:
    MonitorMapTreeItem(MapSpirit *_spirit, MonitorMapTreeItem *parentItem = 0,QString _str = "");

    ~MonitorMapTreeItem();

    void appendChild(MonitorMapTreeItem *child);

    MonitorMapTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    MapSpirit *getSpirit(){return spirit;}
    int row() const;
    MonitorMapTreeItem *parentItem();
private:
    MonitorMapTreeItem *m_parentItem;
    QList<MonitorMapTreeItem *> m_childItems;
    MapSpirit *spirit;
    QString str;
};

#endif // MONITORMAPTREEITEM_H
