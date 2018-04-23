#include "sidebar.h"

SideBar::SideBar(QWidget *parent) : QTreeView(parent)
{
    init();
}

void SideBar::init()
{
    QStandardItemModel* model = new QStandardItemModel(this);

    QStandardItem* usermanage = new QStandardItem(QStringLiteral("用户管理"));
    usermanage->setEditable(false);
    model->appendRow(usermanage);

    QStandardItem* agvmanage = new QStandardItem(QStringLiteral("AGV管理"));
    agvmanage->setEditable(false);
    model->appendRow(agvmanage);
    QStandardItem* itemChild5 = new QStandardItem(QStringLiteral("AGV列表"));
    QStandardItem* itemChild6 = new QStandardItem(QStringLiteral("添加AGV"));
    QStandardItem* itemChild7 = new QStandardItem(QStringLiteral("删除AGV"));
    QStandardItem* itemChild8 = new QStandardItem(QStringLiteral("修改AGV"));
    itemChild5->setEditable(false);
    itemChild6->setEditable(false);
    itemChild7->setEditable(false);
    itemChild8->setEditable(false);
    agvmanage->appendRow(itemChild5);
    agvmanage->appendRow(itemChild6);
    agvmanage->appendRow(itemChild7);
    agvmanage->appendRow(itemChild8);


    setModel(model);
    setHeaderHidden(true);
    setExpandsOnDoubleClick(true);

    setMaximumWidth(150);
}

void SideBar::mousePressEvent(QMouseEvent *event)
{
    QTreeView::mousePressEvent(event);

    QModelIndex index = currentIndex();

    if(event->buttons() == Qt::LeftButton) {
        qDebug() << "index= "<<index;
    }
}

