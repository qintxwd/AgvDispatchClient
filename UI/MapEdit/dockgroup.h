#ifndef DOCKGROUP_H
#define DOCKGROUP_H

#include <QDockWidget>
#include <QListView>
#include <QStringListModel>
#include "../../data/onemap.h"

class DockGroup : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockGroup(OneMap* _oneMap,QWidget *parent = nullptr);

signals:
    void sig_chooseSpirit(MapSpirit *spirit);
public slots:
    void slot_selectChanged(QModelIndex index);
    void slot_onRightClicked(const QPoint &pos);

    void slot_addGroup();
    void slot_removeGroup();
    void slot_editGroup();
private:
    void init();

    QAction *addGroup;
    QAction *removeGroup;
    QAction *editGroup;
    OneMap* oneMap;
    QListView *listview;

    QMenu* righMenuItem;
    QMenu* righMenuBlank;

    QStringListModel *model;
    QStringList qsl;
};

#endif // DOCKGROUP_H
