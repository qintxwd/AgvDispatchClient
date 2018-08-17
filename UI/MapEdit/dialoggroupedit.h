#ifndef DIALOGGROUPEDIT_H
#define DIALOGGROUPEDIT_H

#include <QDialog>
#include <QListView>
#include <QStringListModel>
#include "../../data/onemap.h"

class DialogGroupEdit : public QDialog
{
    Q_OBJECT
public:
    explicit DialogGroupEdit(OneMap *_onemap,MapGroup *_group,QWidget *parent = nullptr);

signals:

public slots:
    void slot_btn_ok();
    void slot_btn_cancel();
    void slot_selectBtn_clicked();
    void slot_unselectBtn_clicked();
private:
    void init();
    MapGroup *group;
    OneMap *onemap;

    QListView *unselectedView;
    QListView *selectedView;

    QStringListModel *unselectedModel;
    QStringListModel *selectedModel;

    QList<int> unselectIds;
    QList<int> selectedIds;

    QStringList selectQsl;
    QStringList unselectQsl;
    QPointF pp;
};

#endif // DIALOGGROUPEDIT_H
