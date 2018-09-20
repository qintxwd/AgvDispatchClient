#include "dockgroup.h"
#include <QtWidgets>
#include "dialoggroupedit.h"

DockGroup::DockGroup(OneMap *_oneMap, QWidget *parent) : QDockWidget(parent),
    oneMap(_oneMap)
{
    init();
}

void DockGroup::init()
{
    model = new QStringListModel;
    listview = new QListView;
    setWindowTitle(QStringLiteral("地图Group"));
    listview->setContextMenuPolicy(Qt::CustomContextMenu);
    QList<MapGroup *> groups = QList<MapGroup *>::fromStdList(oneMap->getGroups());
    foreach (auto group, groups) {
        qsl<<QString("%1 -- %2").arg(group->getId()).arg(QString::fromStdString(group->getName()));
    }
    model->setStringList(qsl);
    listview->setModel(model);

    addGroup = new QAction(QStringLiteral("添加Group"),this);
    removeGroup = new QAction(QStringLiteral("删除Group"),this);
    editGroup = new QAction(QStringLiteral("编辑Group"),this);

    connect(addGroup,SIGNAL(triggered(bool)),this,SLOT(slot_addGroup()));
    connect(removeGroup,SIGNAL(triggered(bool)),this,SLOT(slot_removeGroup()));
    connect(editGroup,SIGNAL(triggered(bool)),this,SLOT(slot_editGroup()));

    righMenuItem = new QMenu(this);
    righMenuItem->addAction(editGroup);
    righMenuItem->addAction(removeGroup);

    righMenuBlank = new QMenu(this);
    righMenuBlank->addAction(addGroup);

    connect(listview, SIGNAL(customContextMenuRequested(const QPoint& )), this, SLOT(slot_onRightClicked(const QPoint&)));
    connect(listview,SIGNAL(clicked(QModelIndex)),this,SLOT(slot_selectChanged(QModelIndex)));

    setWidget(listview);
}


void DockGroup::slot_selectChanged(QModelIndex index)
{
    QList<MapGroup *> groups = QList<MapGroup *>::fromStdList(oneMap->getGroups());
    if(index.row()<groups.length()){
        emit sig_chooseSpirit(groups[index.row()]);
    }else{
        emit sig_chooseSpirit(nullptr);
    }
}

void DockGroup::slot_onRightClicked(const QPoint& pos)
{
    QModelIndex index = listview->indexAt(pos);
    if (index.isValid()){
        righMenuItem->exec(QCursor::pos());
    }else{
        righMenuBlank->exec(QCursor::pos());
    }
}

void DockGroup::slot_addGroup()
{
    int id = oneMap->getNextId();
    QString name = QString("group_%1").arg(id);
    MapGroup *group = new MapGroup(id,name.toStdString(), COMMON_GROUP);
    oneMap->addSpirit(group);
    qsl<<QString("%1 -- %2").arg(group->getId()).arg(QString::fromStdString(group->getName()));
    model->setStringList(qsl);
}

void DockGroup::slot_removeGroup()
{
    int index = listview->currentIndex().row();
    QList<MapGroup *> groups = QList<MapGroup *>::fromStdList(oneMap->getGroups());
    oneMap->removeSpirit(groups[index]);
    qsl.removeAt(index);
    groups = QList<MapGroup *>::fromStdList(oneMap->getGroups());
    foreach (auto group, groups) {
        qsl<<QString("%1 -- %2").arg(group->getId()).arg(QString::fromStdString(group->getName()));
    }
    model->setStringList(qsl);
}

void DockGroup::slot_editGroup()
{
    int index = listview->currentIndex().row();
    QList<MapGroup *> groups = QList<MapGroup *>::fromStdList(oneMap->getGroups());
    DialogGroupEdit *dbe = new DialogGroupEdit(oneMap,groups[index]);
    dbe->exec();

}
