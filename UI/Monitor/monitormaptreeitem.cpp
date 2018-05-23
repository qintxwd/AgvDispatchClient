#include "monitormaptreeitem.h"

MonitorMapTreeItem::MonitorMapTreeItem(MapSpirit *_spirit,  MonitorMapTreeItem *parent,QString _str):
    spirit(_spirit),
    str(_str)
{
    m_parentItem = parent;
}

MonitorMapTreeItem::~MonitorMapTreeItem(){
    qDeleteAll(m_childItems);
}

void MonitorMapTreeItem::appendChild(MonitorMapTreeItem *item)
{
    m_childItems.append(item);
}

MonitorMapTreeItem *MonitorMapTreeItem::child(int row)
{
    return m_childItems.value(row);
}

int MonitorMapTreeItem::childCount() const
{
    return m_childItems.count();
}


int MonitorMapTreeItem::columnCount() const
{
    return 1;//m_itemData.count();
}

QVariant MonitorMapTreeItem::data(int column) const
{
    Q_UNUSED(column)
    if(spirit==nullptr)return str;
    return QString::fromStdString(spirit->getName());//.value(column);
}

MonitorMapTreeItem *MonitorMapTreeItem::parentItem()
{
    return m_parentItem;
}
int MonitorMapTreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<MonitorMapTreeItem*>(this));

    return 0;
}
