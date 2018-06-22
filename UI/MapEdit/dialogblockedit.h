#ifndef DIALOGBLOCKEDIT_H
#define DIALOGBLOCKEDIT_H

#include <QDialog>
#include <QListView>
#include <QStringListModel>
#include "../../data/onemap.h"

class DialogBlockEdit : public QDialog
{
    Q_OBJECT
public:
    explicit DialogBlockEdit(OneMap *_onemap,MapBlock *_block,QWidget *parent = nullptr);

signals:

public slots:
    void slot_btn_ok();
    void slot_btn_cancel();
    void slot_selectBtn_clicked();
    void slot_unselectBtn_clicked();
private:
    void init();
    MapBlock *block;
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

#endif // DIALOGBLOCKEDIT_H
