#include "maptreeitem.h"

MapTreeItem::MapTreeItem(MapSpirit *_spirit,  MapTreeItem *parent,QString _str):
    spirit(_spirit),
    str(_str)
{
    m_parentItem = parent;
}

MapTreeItem::~MapTreeItem(){
    qDeleteAll(m_childItems);
}

void MapTreeItem::appendChild(MapTreeItem *item)
{
    m_childItems.append(item);
}

MapTreeItem *MapTreeItem::child(int row)
{
    return m_childItems.value(row);
}

int MapTreeItem::childCount() const
{
    return m_childItems.count();
}


int MapTreeItem::columnCount() const
{
    return 1;//m_itemData.count();
}

QVariant MapTreeItem::data(int column) const
{
    Q_UNUSED(column)
    if(spirit==nullptr)return str;
    return spirit->getName();//.value(column);
}

MapTreeItem *MapTreeItem::parentItem()
{
    return m_parentItem;
}
int MapTreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<MapTreeItem*>(this));

    return 0;
}
