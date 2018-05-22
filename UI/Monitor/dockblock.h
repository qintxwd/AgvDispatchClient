#ifndef DOCKBLOCK_H
#define DOCKBLOCK_H

#include <QDockWidget>
#include <QListView>
#include <QStringListModel>
#include "../../data/onemap.h"

class DockBlock : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockBlock(OneMap* _oneMap,QWidget *parent = nullptr);

signals:
    void sig_chooseSpirit(MapSpirit *spirit);
public slots:
    void slot_selectChanged(QModelIndex index);
    void slot_onRightClicked(const QPoint &pos);

    void slot_addBlock();
    void slot_removeBlock();
    void slot_editBlock();
private:
    void init();

    QAction *addBlock;
    QAction *removeBlock;
    QAction *editBlock;
    OneMap* oneMap;
    QListView *listview;

    QMenu* righMenuItem;
    QMenu* righMenuBlank;

    QStringListModel *model;
    QStringList qsl;
};

#endif // DOCKBLOCK_H
