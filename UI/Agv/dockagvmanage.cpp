#include "dockagvmanage.h"
#include "global.h"
#include "dialogaddagv.h"
#include "dialogmodifyagv.h"

DockAgvManage::DockAgvManage(QWidget *parent) : QDockWidget(tr("AgvManage"),parent)
{
    setAllowedAreas(Qt::AllDockWidgetAreas);
    stack = new QStackedWidget;
    waiting = new WidgetWaiting;
    content = new QWidget;

    QPushButton *btnAddAgv = new QPushButton(tr("add agv"));
    QPushButton *btnDeleteAgv = new QPushButton(tr("delete agv"));
    QPushButton *btnModifyAgv = new QPushButton(tr("edit agv"));
    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addWidget(btnAddAgv);
    btnsLayout->addWidget(btnDeleteAgv);
    btnsLayout->addWidget(btnModifyAgv);

    tableWidget = new QTableWidget(0, 4);
    QStringList labels;
    labels << QStringLiteral("ID")
           << QStringLiteral("agv name")
           << QStringLiteral("agv ip")
           << QStringLiteral("agv port");
    tableWidget->setHorizontalHeaderLabels(labels);
    tableWidget->verticalHeader()->hide();
    tableWidget->setShowGrid(false);
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //x先自适应宽度
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//选中就是选中这一行
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);//只能选中一行
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addItem(btnsLayout);
    layout->addWidget(tableWidget);

    content->setLayout(layout);

    stack->addWidget(waiting);
    stack->addWidget(content);
    stack->setCurrentIndex(0);
    setWidget(stack);

    connect(btnDeleteAgv,SIGNAL(clicked(bool)),this,SLOT(deleteAgv()));
    connect(btnAddAgv,SIGNAL(clicked(bool)),this,SLOT(addAgv()));
    connect(btnModifyAgv,SIGNAL(clicked(bool)),this,SLOT(modifyAgv()));

    connect(&msgCenter,SIGNAL(deleteAgvSuccess()),this,SLOT(deleteSuccess()));
    connect(&msgCenter,SIGNAL(modifyAgvSuccess()),this,SLOT(modifySuccess()));
    connect(&msgCenter,SIGNAL(listAgvSuccess()),this,SLOT(updateTable()));

    connect(this,SIGNAL(visibilityChanged(bool)),this,SLOT(onVisibilityChanged(bool)));
}

void DockAgvManage::onVisibilityChanged(bool v)
{
    if(v){
        stack->setCurrentIndex(0);
        msgCenter.agvList();
    }
}


void DockAgvManage::deleteAgv()
{
    if(tableWidget->currentRow()<0 || tableWidget->currentRow()>=agvinfos.length()){
        QMessageBox::warning(this,QStringLiteral("未选择"),QStringLiteral("请先选择你要删除的行"));
        return ;
    }

    QMessageBox::StandardButton rb = QMessageBox::question(this, QStringLiteral("确认删除"), QStringLiteral("确认删除该条?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        msgCenter.deleteAgv(agvinfos.at(tableWidget->currentRow()).id);
    }

}

void DockAgvManage::modifyAgv()
{
    if(tableWidget->currentRow()<0 || tableWidget->currentRow()>=agvinfos.length()){
        QMessageBox::warning(this,QStringLiteral("未选择"),QStringLiteral("请先选择你要编辑的行"));
        return ;
    }
    AGV_BASE_INFO u = agvinfos.at(tableWidget->currentRow());
    DialogModifyAgv *modifyAgvDlg = new DialogModifyAgv(u.id,u.name,u.ip,u.port,this);
    if(modifyAgvDlg->exec() == QDialog::Accepted){
        msgCenter.agvList();
    }

}

void DockAgvManage::modifySuccess()
{
    msgCenter.agvList();
}

void DockAgvManage::deleteSuccess()
{
    msgCenter.agvList();
}

void DockAgvManage::addAgv()
{
    DialogAddAgv *dialogAddAgv = new DialogAddAgv(this);
    if(dialogAddAgv->exec() == QDialog::Accepted){
        msgCenter.agvList();
    }
}

void DockAgvManage::updateTable()
{
    tableWidget->clearContents();
    while(tableWidget->rowCount()>0){
        tableWidget->removeRow(0);
    }
    agvinfos = msgCenter.getAgvListModel();

    for(int i=0;i<agvinfos.length();++i){
        tableWidget->insertRow(i);
        AGV_BASE_INFO u = agvinfos.at(i);
        QTableWidgetItem *itemId = new QTableWidgetItem(tr("%1").arg(u.id));
        itemId->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 0, itemId);
        QTableWidgetItem *itemAgvname = new QTableWidgetItem(QString::fromStdString(std::string(u.name)));
        itemAgvname->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 1, itemAgvname);
        QTableWidgetItem *itemIp = new QTableWidgetItem(QString::fromStdString(std::string(u.ip)));
        itemIp->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 2, itemIp);
        QTableWidgetItem *itemport = new QTableWidgetItem(QString("%1").arg(u.port));
        itemport->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 3, itemport);
    }
    tableWidget->update();
    stack->setCurrentIndex(1);
}



