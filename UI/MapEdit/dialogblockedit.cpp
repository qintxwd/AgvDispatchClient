#include "dialogblockedit.h"
#include <QtWidgets>

DialogBlockEdit::DialogBlockEdit(OneMap *_onemap, MapBlock *_block, QWidget *parent) : QDialog(parent),
    onemap(_onemap),
    block(_block)
{
    init();
}

void DialogBlockEdit::init()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QHBoxLayout *centerlayout = new QHBoxLayout;

    unselectedView = new QListView;
    selectedView = new QListView;

    unselectedModel = new QStringListModel;
    selectedModel = new QStringListModel;

    //1.already select list
    selectedIds = block->getSpirits();
    //2.all points paths list
    auto pp = onemap->getRootPaths();
    foreach (auto p, pp) {
        unselectIds.append(p->getId());
    }

    QList<MapFloor *> floors = onemap->getFloors();
    foreach (auto floor, floors) {
        auto points = floor->getPoints();
        auto paths = floor->getPaths();
        foreach (auto p, points) {
            unselectIds.append(p->getId());
        }
        foreach (auto p, paths) {
            unselectIds.append(p->getId());
        }
    }
    foreach (auto id, selectedIds) {
        if(unselectIds.contains(id))unselectIds.removeAll(id);
    }

    //set model
    foreach (auto id, selectedIds) {
        auto item = onemap->getSpiritById(id);
        QString type = "";
        if(item->getSpiritType() == MapSpirit::Map_Sprite_Type_Path){
            type = "PPATH";
        }else if(item->getSpiritType() == MapSpirit::Map_Sprite_Type_Point){
            type = "POINT";
        }
        selectQsl.append(QString("%1 -- %2 --%3").arg(type).arg(item->getId()).arg(item->getName()));
    }
    selectedModel->setStringList(selectQsl);

    //set model
    foreach (auto id, unselectIds) {
        auto item = onemap->getSpiritById(id);
        QString type = "";
        if(item->getSpiritType() == MapSpirit::Map_Sprite_Type_Path){
            type = "PPATH";
        }else if(item->getSpiritType() == MapSpirit::Map_Sprite_Type_Point){
            type = "POINT";
        }
        unselectQsl.append(QString("%1 -- %2 --%3").arg(type).arg(item->getId()).arg(item->getName()));
    }
    unselectedModel->setStringList(unselectQsl);

    unselectedView->setModel(unselectedModel);
    selectedView->setModel(selectedModel);

    QVBoxLayout *left = new QVBoxLayout;
    QVBoxLayout *center = new QVBoxLayout;
    QVBoxLayout *right = new QVBoxLayout;

    left->addWidget(new QLabel("unselected"));
    left->addWidget(unselectedView);

    QPushButton *selectBtn = new QPushButton(">>");
    QPushButton *unselectBtn = new QPushButton("<<");
    center->addWidget(selectBtn);
    center->addWidget(unselectBtn);

    right->addWidget(new QLabel("selected"));
    right->addWidget(selectedView);

    centerlayout->addItem(left);
    centerlayout->addItem(center);
    centerlayout->addItem(right);


    QHBoxLayout *bomLayout = new QHBoxLayout;
    QPushButton *btn_ok = new QPushButton("OK");
    btn_ok->setDefault(true);
    QPushButton *btn_cancel = new QPushButton("Cancel");
    bomLayout->addStretch();
    bomLayout->addWidget(btn_ok);
    bomLayout->addStretch();
    bomLayout->addWidget(btn_cancel);
    bomLayout->addStretch();

    mainLayout->addItem(centerlayout);
    mainLayout->addItem(bomLayout);

    setLayout(mainLayout);

    connect(btn_ok,SIGNAL(clicked(bool)),this,SLOT(slot_btn_ok()));
    connect(btn_cancel,SIGNAL(clicked(bool)),this,SLOT(slot_btn_cancel()));
    connect(selectBtn,SIGNAL(clicked(bool)),this,SLOT(slot_selectBtn_clicked()));
    connect(unselectBtn,SIGNAL(clicked(bool)),this,SLOT(slot_unselectBtn_clicked()));
}

void DialogBlockEdit::slot_btn_ok()
{
    block->clear();
    foreach (auto s, selectedIds) {
        auto p = onemap->getSpiritById(s);
        if(p==nullptr)continue;
        block->addSpirit(s);
    }
    accept();
}

void DialogBlockEdit::slot_btn_cancel()
{
    reject();
}

void DialogBlockEdit::slot_selectBtn_clicked()
{
    int index = unselectedView->currentIndex().row();
    if(index < unselectIds.length() && index>=0){
        int selectId = unselectIds[index];
        QString selectString  = unselectQsl[index];
        unselectQsl.removeAt(index);
        selectQsl.append(selectString);
        unselectIds.removeAt(index);
        selectedIds.append(selectId);
        unselectedModel->setStringList(unselectQsl);
        selectedModel->setStringList(selectQsl);
    }
}

void DialogBlockEdit::slot_unselectBtn_clicked()
{
    int index = selectedView->currentIndex().row();
    if(index < selectedIds.length() && index>=0){
        int selectId = selectedIds[index];
        QString selectString  = selectQsl[index];
        selectQsl.removeAt(index);
        unselectQsl.append(selectString);
        selectedIds.removeAt(index);
        unselectIds.append(selectId);
        unselectedModel->setStringList(unselectQsl);
        selectedModel->setStringList(selectQsl);
    }
}
