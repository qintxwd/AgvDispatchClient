#include "dockblock.h"
#include <QMenu>
#include <QDebug>
#include "dialogblockedit.h"

DockBlock::DockBlock(OneMap *_oneMap, QWidget *parent) : QDockWidget(parent),
    oneMap(_oneMap)
{
    init();
}

void DockBlock::init()
{
    model = new QStringListModel;
    listview = new QListView;
    setWindowTitle(QStringLiteral("地图Block"));
    listview->setContextMenuPolicy(Qt::CustomContextMenu);
    QList<MapBlock *> blocks = oneMap->getBlocks();
    foreach (auto block, blocks) {
        qsl<<QString("%1 -- %2").arg(block->getId()).arg(block->getName());
    }
    model->setStringList(qsl);
    listview->setModel(model);

    addBlock = new QAction(QStringLiteral("添加BLCOK"),this);
    removeBlock = new QAction(QStringLiteral("删除BLCOK"),this);
    editBlock = new QAction(QStringLiteral("编辑BLCOK"),this);

    connect(addBlock,SIGNAL(triggered(bool)),this,SLOT(slot_addBlock()));
    connect(removeBlock,SIGNAL(triggered(bool)),this,SLOT(slot_removeBlock()));
    connect(editBlock,SIGNAL(triggered(bool)),this,SLOT(slot_editBlock()));

    righMenuItem = new QMenu(this);
    righMenuItem->addAction(removeBlock);
    righMenuItem->addAction(editBlock);
    righMenuBlank = new QMenu(this);
    righMenuBlank->addAction(addBlock);

    connect(listview, SIGNAL(customContextMenuRequested(const QPoint& )), this, SLOT(slot_onRightClicked(const QPoint&)));
    connect(listview,SIGNAL(clicked(QModelIndex)),this,SLOT(slot_selectChanged(QModelIndex)));

    setWidget(listview);
}


void DockBlock::slot_selectChanged(QModelIndex index)
{
    QList<MapBlock *> blocks = oneMap->getBlocks();
    if(index.row()<blocks.length()){
        emit sig_chooseSpirit(blocks[index.row()]);
    }else{
        emit sig_chooseSpirit(nullptr);
    }
}

void DockBlock::slot_onRightClicked(const QPoint& pos)
{
    QModelIndex index = listview->indexAt(pos);
    if (index.isValid()){
        righMenuItem->exec(QCursor::pos());
    }else{
        righMenuBlank->exec(QCursor::pos());
    }
}

void DockBlock::slot_addBlock()
{
    int id = oneMap->getNextId();
    QString name = QString("block_%1").arg(id);
    MapBlock *block = new MapBlock(id,name,oneMap);
    oneMap->addBlock(block);
    qsl<<QString("%1 -- %2").arg(block->getId()).arg(block->getName());
    model->setStringList(qsl);
}

void DockBlock::slot_removeBlock()
{
    int index = listview->currentIndex().row();
    QList<MapBlock *> blocks = oneMap->getBlocks();
    oneMap->removeBlock(blocks[index]);
    qsl.removeAt(index);
    blocks = oneMap->getBlocks();
    foreach (auto block, blocks) {
        qsl<<QString("%1 -- %2").arg(block->getId()).arg(block->getName());
    }
    model->setStringList(qsl);
}

void DockBlock::slot_editBlock()
{
    int index = listview->currentIndex().row();
    QList<MapBlock *> blocks = oneMap->getBlocks();
    DialogBlockEdit *dbe = new DialogBlockEdit(oneMap,blocks[index]);
    dbe->exec();

}
