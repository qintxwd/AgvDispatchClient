#include "widgetusermanage.h"

#include "global.h"
#include "dialogadduser.h"
#include "dialogmodifyuser.h"

WidgetUserManage::WidgetUserManage(QWidget *parent) : QWidget(parent)
{
    QPushButton *returnBtn = new QPushButton;
    returnBtn->setIcon(QIcon(QPixmap(":/icons/dark/appbar.undo.point.png")));
    returnBtn->setText(QStringLiteral("返回"));
    returnBtn->setToolTip(QStringLiteral("返回上级"));
    returnBtn->setObjectName("returnbtn");

    labelTitle = new QLabel(QStringLiteral("用户管理"));
    labelTitle->setFixedHeight(50);
    labelTitle->setObjectName("titlelabel");

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(labelTitle);
    hlayout->addStretch(1);
    hlayout->addWidget(returnBtn);

    btnAddUser = new QPushButton(QStringLiteral("添加用户"));
    btnDeleteUser = new QPushButton(QStringLiteral("删除用户"));
    btnModifyUser = new QPushButton(QStringLiteral("编辑用户"));
    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addWidget(btnAddUser);
    btnsLayout->addWidget(btnDeleteUser);
    btnsLayout->addWidget(btnModifyUser);
    btnsLayout->addStretch(1);
    btnsLayout->setMargin(10);
    btnsLayout->setContentsMargins(5,2,5,2);

    tableWidget = new QTableWidget(0, 5);

    QStringList labels;
    labels << QStringLiteral("ID")
           << QStringLiteral("用户名")
           << QStringLiteral("密码")
           << QStringLiteral("登录状态")
           << QStringLiteral("角色");
    tableWidget->setHorizontalHeaderLabels(labels);
    tableWidget->verticalHeader()->hide();
    tableWidget->setShowGrid(false);
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //x先自适应宽度
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//选中就是选中这一行
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);//只能选中一行
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addItem(hlayout);
    layout->addItem(btnsLayout);
    layout->addWidget(tableWidget);

    layout->addWidget(btnAddUser,0,Qt::AlignLeft|Qt::AlignVCenter);


    connect(btnDeleteUser,SIGNAL(clicked(bool)),this,SLOT(deleteUser()));
    connect(&msgCenter,SIGNAL(deleteUserSuccess()),this,SLOT(deleteSuccess()));
    connect(&msgCenter,SIGNAL(modifyAgvSuccess()),this,SLOT(modifySuccess()));
    connect(&msgCenter,SIGNAL(listUserSuccess()),this,SLOT(updateTable()));
    connect(btnAddUser,SIGNAL(clicked(bool)),this,SLOT(addUser()));
    connect(btnModifyUser,SIGNAL(clicked(bool)),this,SLOT(modifyUser()));

    connect(returnBtn,SIGNAL(clicked(bool)),this,SIGNAL(returnChoose()));
    this->setLayout(layout);

    reshow();
}
void WidgetUserManage::reshow()
{
    msgCenter.userList();
}

void WidgetUserManage::deleteUser()
{
    if(tableWidget->currentRow()<0 || tableWidget->currentRow()>=userinfos.length()){
        QMessageBox::warning(this,QStringLiteral("未选择"),QStringLiteral("请先选择你要删除的行"));
        return ;
    }

    QMessageBox::StandardButton rb = QMessageBox::question(this, QStringLiteral("确认删除"), QStringLiteral("确认删除该用户?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        msgCenter.deleteUser(userinfos.at(tableWidget->currentRow()).id);
    }

}

void WidgetUserManage::modifyUser()
{
    if(tableWidget->currentRow()<0 || tableWidget->currentRow()>=userinfos.length()){
        QMessageBox::warning(this,QStringLiteral("未选择"),QStringLiteral("请先选择你要编辑的行"));
        return ;
    }
    USER_INFO u = userinfos.at(tableWidget->currentRow());
    DialogModifyUser *modifyUserDlg = new DialogModifyUser(u.id,u.username,u.password,u.role,this);
    if(modifyUserDlg->exec() == QDialog::Accepted){
        //添加成功，刷新列表
        msgCenter.userList();
    }

}

void WidgetUserManage::modifySuccess()
{
    msgCenter.userList();
}

void WidgetUserManage::deleteSuccess()
{
    msgCenter.userList();
}

void WidgetUserManage::addUser()
{
    DialogAddUser *dialogAddUser = new DialogAddUser(this);
    if(dialogAddUser->exec() == QDialog::Accepted){
        //添加成功，刷新列表
        msgCenter.userList();
    }
}

void WidgetUserManage::updateTable()
{
    tableWidget->clearContents();
    while(    tableWidget->rowCount()>0){
        tableWidget->removeRow(0);
    }
    userinfos = msgCenter.getUserListModel();

    for(int i=0;i<userinfos.length();++i){
        tableWidget->insertRow(i);
        USER_INFO u = userinfos.at(i);
        QTableWidgetItem *itemId = new QTableWidgetItem(tr("%1").arg(u.id));
        itemId->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 0, itemId);
        QTableWidgetItem *itemUsername = new QTableWidgetItem(u.username);
        itemUsername->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 1, itemUsername);
        QTableWidgetItem *itemPassword = new QTableWidgetItem(tr("%1").arg(u.password));
        itemPassword->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 2, itemPassword);

        QString roleName = QStringLiteral("游客");
        if(u.role==USER_ROLE_VISITOR)roleName = QStringLiteral("游客");
        if(u.role==USER_ROLE_OPERATOR)roleName = QStringLiteral("操作员");
        if(u.role==USER_ROLE_ADMIN)roleName = QStringLiteral("管理员");
        if(u.role==USER_ROLE_SUPER_ADMIN)roleName = QStringLiteral("超级管理员");
        if(u.role==USER_ROLE_DEVELOP)roleName = QStringLiteral("开发人员员");
        QTableWidgetItem *itemRole = new QTableWidgetItem(tr("%1").arg(roleName));
        itemRole->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 3, itemRole);
    }
    //tableWidget->repaint();
    tableWidget->update();
}
