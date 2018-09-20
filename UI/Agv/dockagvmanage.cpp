#include "dockagvmanage.h"
#include "global.h"
#include "dialogaddagv.h"
#include "dialogmodifyagv.h"

//状态
enum {
    AGV_STATUS_HANDING = -1,//手动模式中，不可用
    AGV_STATUS_IDLE = 0,//空闲可用
    AGV_STATUS_UNCONNECT = 1,//未连接
    AGV_STATUS_TASKING = 2,//正在执行任务
    AGV_STATUS_POWER_LOW = 3,//电量低
    AGV_STATUS_ERROR = 4,//故障
    AGV_STATUS_GO_CHARGING = 5,//返回充电中
    AGV_STATUS_CHARGING = 6,//正在充电
    AGV_STATUS_NOTREADY = 7 //刚连接，尚未上报位置
};

DockAgvManage::DockAgvManage(QWidget *parent) : QDockWidget(tr("AgvManage"),parent)
{
    setAllowedAreas(Qt::AllDockWidgetAreas);
    stack = new QStackedWidget;
    waiting = new WidgetWaiting;
    content = new QWidget;

    QPushButton *btnAddAgv = new QPushButton(tr("add agv"));
    QPushButton *btnDeleteAgv = new QPushButton(tr("delete agv"));
    QPushButton *btnModifyAgv = new QPushButton(tr("edit agv"));
    QPushButton *btnStopAgv = new QPushButton(tr("stop"));
    QPushButton *btnPauseAgv = new QPushButton(tr("pause"));
    QPushButton *btnResumeAgv = new QPushButton(tr("resume"));

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addWidget(btnAddAgv);
    btnsLayout->addWidget(btnDeleteAgv);
    btnsLayout->addWidget(btnModifyAgv);
    btnsLayout->addWidget(btnStopAgv);
    btnsLayout->addWidget(btnPauseAgv);
    btnsLayout->addWidget(btnResumeAgv);

    tableWidget = new QTableWidget(0, 6);
    QStringList labels;
    labels << QStringLiteral("ID")
           << QStringLiteral("agv name")
              << QStringLiteral("agv status")
           << QStringLiteral("agv ip")
           << QStringLiteral("agv port")
           << QStringLiteral("agv station");
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
    connect(btnStopAgv, SIGNAL(clicked(bool)), this, SLOT(stopAgv()));
    connect(btnPauseAgv, SIGNAL(clicked(bool)), this, SLOT(pauseAgv()));
    connect(btnResumeAgv, SIGNAL(clicked(bool)), this, SLOT(resumeAgv()));

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
    DialogModifyAgv *modifyAgvDlg = new DialogModifyAgv(u.id,u.name,u.ip,u.port,u.station, this);
    if(modifyAgvDlg->exec() == QDialog::Accepted){
        msgCenter.agvList();
    }

}

void DockAgvManage::controlAgv(int params)
{
    int id = 0;
    if(tableWidget->currentRow()<0 || tableWidget->currentRow()>=agvinfos.length()){

    }
    else
    {
        id = agvinfos.at(tableWidget->currentRow()).id;
    }
    msgCenter.controlagv(id, params);
}

void DockAgvManage::stopAgv()
{
    controlAgv(2);
}

void DockAgvManage::pauseAgv()
{
    controlAgv(1);
}

void DockAgvManage::resumeAgv()
{
    controlAgv(0);
}

void DockAgvManage::modifySuccess()
{
    stack->setCurrentIndex(0);
    msgCenter.agvList();
}

void DockAgvManage::deleteSuccess()
{
    stack->setCurrentIndex(0);
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
        QTableWidgetItem *itemAgvname = new QTableWidgetItem(u.name);
        itemAgvname->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 1, itemAgvname);

        QString statusStr = "";
        if(u.status ==  AGV_STATUS_HANDING){
            //手动模式中，不可用
            statusStr = QStringLiteral("手动");
        }else if(u.status ==  AGV_STATUS_IDLE){
            //空闲可用
            statusStr = QStringLiteral("空闲");
        }else if(u.status ==  AGV_STATUS_UNCONNECT){
            //未连接
            statusStr = QStringLiteral("未连接");
        }else if(u.status ==  AGV_STATUS_TASKING){
            //正在执行任务
            statusStr = QStringLiteral("执行任务");
        }else if(u.status ==  AGV_STATUS_POWER_LOW){
            //电量低
            statusStr = QStringLiteral("电量低");
        }else if(u.status ==  AGV_STATUS_ERROR){
            //故障
            statusStr = QStringLiteral("故障");
        }else if(u.status ==  AGV_STATUS_GO_CHARGING){
            //返回充电中
            statusStr = QStringLiteral("返回充电");
        }else if(u.status ==  AGV_STATUS_CHARGING){
            //正在充电
            statusStr = QStringLiteral("正在充电");
        }else if(u.status ==  AGV_STATUS_NOTREADY){
            //刚连接，尚未上报位置
            statusStr = QStringLiteral("刚连接");
        }


        QTableWidgetItem *itemAgvStatus = new QTableWidgetItem(statusStr);
        itemAgvStatus->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 2, itemAgvStatus);
        QTableWidgetItem *itemIp = new QTableWidgetItem(u.ip);
        itemIp->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 3, itemIp);
        QTableWidgetItem *itemport = new QTableWidgetItem(QString("%1").arg(u.port));
        itemport->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 4, itemport);
        QTableWidgetItem *itemstation = new QTableWidgetItem(QString("%1").arg(u.station));
        itemport->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 5, itemstation);
    }
    tableWidget->update();
    stack->setCurrentIndex(1);
}



