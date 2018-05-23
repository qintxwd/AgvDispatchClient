#ifndef MONITORDOCKBLOCK_H
#define MONITORDOCKBLOCK_H


#include <QDockWidget>
#include <QListView>
#include <QStringListModel>
#include "../../data/onemap.h"

class MonitorDockBlock: public QDockWidget
{
    Q_OBJECT
public:
    MonitorDockBlock(OneMap* _oneMap,QWidget *parent = nullptr);

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

#endif // MONITORDOCKBLOCK_H
