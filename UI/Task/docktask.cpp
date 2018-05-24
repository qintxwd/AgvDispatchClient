#include "docktask.h"
#include "../../global.h"


DockTask::DockTask(QWidget *parent) : QDockWidget(tr("Task"),parent)
{
    setAllowedAreas(Qt::AllDockWidgetAreas);
    stack = new QStackedWidget;
    waiting = new WidgetWaiting;
    content = new QWidget;


    QPushButton *btnAddTask = new QPushButton(tr("add task"));
    QPushButton *btnCancelTask = new QPushButton(tr("cancel task"));
    QPushButton *DetailTask = new QPushButton(tr("show detail"));

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addWidget(btnAddTask);
    btnsLayout->addWidget(btnCancelTask);
    btnsLayout->addWidget(DetailTask);

    tableWidget = new QTableWidget(0, 12);
    QStringList labels;
    labels << QStringLiteral("ID")
           << QStringLiteral("username")
           << QStringLiteral("password")
           << QStringLiteral("status")
           << QStringLiteral("role")
           << QStringLiteral("ID")
           << QStringLiteral("username")
           << QStringLiteral("password")
           << QStringLiteral("status")
           << QStringLiteral("role")
           << QStringLiteral("ID")
           << QStringLiteral("username");
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
    stack->setCurrentIndex(2);
    setWidget(stack);


    connect(btnAddTask,SIGNAL(clicked(bool)),this,SLOT(addTask()));
    connect(btnCancelTask,SIGNAL(clicked(bool)),this,SLOT(cancelTask()));

//    connect(&msgCenter,SIGNAL(deleteUserSuccess()),this,SLOT(deleteSuccess()));
//    connect(&msgCenter,SIGNAL(modifyAgvSuccess()),this,SLOT(modifySuccess()));
//    connect(&msgCenter,SIGNAL(listUserSuccess()),this,SLOT(updateTable()));

    connect(this,SIGNAL(visibilityChanged(bool)),this,SLOT(onVisibilityChanged(bool)));

}

void DockTask::addTask()
{
//    DialogAddUser *dialogAddUser = new DialogAddUser(this);
//    if(dialogAddUser->exec() == QDialog::Accepted){
//        //添加成功，刷新列表
//        show();
//    }
}

void DockTask::cancelTask()
{
    if(tableWidget->currentRow()<0){
        QMessageBox::warning(this,QStringLiteral("未选择"),QStringLiteral("请先选择你要取消的任务的行"));
        return ;
    }

    QMessageBox::StandardButton rb = QMessageBox::question(this, QStringLiteral("确认取消"), QStringLiteral("确认取消该任务?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        //msgCenter.deleteUser(userinfos.at(tableWidget->currentRow()).id);
    }

}

void DockTask::onVisibilityChanged(bool v)
{
    if(v){
        stack->setCurrentIndex(0);
        //msgCenter.userList();
    }
}

void DockTask::updateTable()
{
    tableWidget->clearContents();
    while(    tableWidget->rowCount()>0){
        tableWidget->removeRow(0);
    }
//    userinfos = msgCenter.getUserListModel();

//    for(int i=0;i<userinfos.length();++i){
//        tableWidget->insertRow(i);
//        USER_INFO u = userinfos.at(i);
//        QTableWidgetItem *itemId = new QTableWidgetItem(tr("%1").arg(u.id));
//        itemId->setTextAlignment(Qt::AlignCenter);
//        tableWidget->setItem(i, 0, itemId);
//        QTableWidgetItem *itemUsername = new QTableWidgetItem(QString::fromStdString(u.username));
//        itemUsername->setTextAlignment(Qt::AlignCenter);
//        tableWidget->setItem(i, 1, itemUsername);
//        QTableWidgetItem *itemPassword = new QTableWidgetItem(tr("%1").arg(QString::fromStdString(u.password)));
//        itemPassword->setTextAlignment(Qt::AlignCenter);
//        tableWidget->setItem(i, 2, itemPassword);

//        QString online = tr("offline");
//        if(u.status!=0)online = tr("online");
//        QTableWidgetItem *itemStatus = new QTableWidgetItem(online);
//        itemStatus->setTextAlignment(Qt::AlignCenter);
//        tableWidget->setItem(i, 3, itemStatus);

//        QString roleName = tr("visitor");
//        if(u.role==USER_ROLE_VISITOR)roleName = tr("visitor");
//        if(u.role==USER_ROLE_OPERATOR)roleName = tr("operator");
//        if(u.role==USER_ROLE_ADMIN)roleName = tr("admin");
//        if(u.role==USER_ROLE_SUPER_ADMIN)roleName = tr("super admin");
//        if(u.role==USER_ROLE_DEVELOP)roleName = tr("developer");
//        QTableWidgetItem *itemRole = new QTableWidgetItem(roleName);
//        itemRole->setTextAlignment(Qt::AlignCenter);
//        tableWidget->setItem(i, 4, itemRole);
//    }
    tableWidget->update();
    stack->setCurrentIndex(1);
}
