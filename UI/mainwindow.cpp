#include "mainwindow.h"
#include "SideBar/sidebar.h"

#include "global.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    usermanage(nullptr)
{
    createActions();
    createStatusBar();

    init();
}

void MainWindow::init()
{
    connect(&msgCenter,SIGNAL(sig_connection_connected()),this,SLOT());
    connect(&msgCenter,SIGNAL(sig_connection_disconnected()),this,SLOT());
    connect(&msgCenter,SIGNAL(sig_connection_conntectting()),this,SLOT());

    connect(&msgCenter,SIGNAL(sendRequestFail()),this,SLOT());
    connect(&msgCenter,SIGNAL(waitResponseTimeOut()),this,SLOT());
    connect(&msgCenter,SIGNAL(tip(QString)),this,SLOT());
    connect(&msgCenter,SIGNAL(err(int,QString)),this,SLOT());

}

void MainWindow::onServerConnect()
{
    statusbar_server_msg(QStringLiteral("已连接"));
}

void MainWindow::onServerDisconnect()
{
    statusbar_server_msg(QStringLiteral("连接已断开"));
}

void MainWindow::onServerConnectting()
{
    statusbar_server_msg(QStringLiteral("尝试连接服务器..."));
}

void MainWindow::onSendFail()
{
    statusbar_msg(QStringLiteral("消息:")+QStringLiteral("发送请求失败"));
}

void MainWindow::onWaitResponseTimeOut()
{
    statusbar_msg(QStringLiteral("消息:")+QStringLiteral("等待响应超时"));
}
void MainWindow::onTip(QString s)
{
    statusbar_msg(QStringLiteral("消息:")+s);
}
void MainWindow::onErr(int errcode,QString info)
{
    statusbar_msg(QStringLiteral("消息:")+getErrorString(errcode)+"  "+info);
}

void MainWindow::showUserManager(bool checked)
{
    if(usermanage==nullptr)
        usermanage = new WidgetUserManage(this);
    setCentralWidget(usermanage);
}

void MainWindow::createStatusBar()
{
    QStatusBar *statusbar = new QStatusBar(this);
    QLabel *usernamelabel = new QLabel(QStringLiteral("当前用户:")+QString::fromStdString(std::string(current_user_info.username)));
    QString role_name = "";
    switch (current_user_info.role) {
    case USER_ROLE_VISITOR:
        role_name =QStringLiteral( "未登录");
        break;
    case USER_ROLE_OPERATOR:
        role_name =QStringLiteral( "普通用户");
        break;
    case USER_ROLE_ADMIN:
        role_name =QStringLiteral( "管理员");
        break;
    case USER_ROLE_SUPER_ADMIN:
        role_name =QStringLiteral( "超级管理员");
        break;
    case USER_ROLE_DEVELOP:
        role_name =QStringLiteral( "开发人员");
        break;
    default:
        break;
    }
    QLabel *userrolelabel = new QLabel(QStringLiteral("当前角色:")+role_name);
    serverlabel = new QLabel(QStringLiteral("服务器连接:"));
    uilabel = new QLabel(QStringLiteral("当前界面:"));
    msglabel = new QLabel(QStringLiteral("消息:"));
    usernamelabel->setMinimumWidth(200);
    userrolelabel->setMinimumWidth(200);
    serverlabel->setMinimumWidth(200);
    uilabel->setMinimumWidth(200);

    statusbar->addWidget(usernamelabel);
    statusbar->addWidget(userrolelabel);
    statusbar->addWidget(serverlabel);
    statusbar->addWidget(uilabel);
    statusbar->addWidget(msglabel);

    setStatusBar(statusbar);
}

void MainWindow::createActions()
{
    QMenu *userMenu = menuBar()->addMenu(tr("UserManager"));
    QToolBar *userToolBar = addToolBar(tr("UserManager"));
    QAction *newAct = new QAction(tr("&UserManager"), this);
    newAct->setStatusTip(tr("show user manager"));
    connect(newAct, &QAction::triggered, this, &MainWindow::showUserManager);
    userMenu->addAction(newAct);
    userToolBar->addAction(newAct);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));

    QAction *aboutQtAct = helpMenu->addAction(tr("About &HRG"), this, &MainWindow::aboutHrg);
    aboutQtAct->setStatusTip(tr("Show the HRG's About box"));

}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Application"),
                       tr("The <b>HRG Dispatch client</b> connect to dispatch server,<br>"
                          "and show the ui for user to watch the status of agvs and<br>"
                          "other information about the agv dispatch system."));
}

void MainWindow::aboutHrg()
{
    QMessageBox::about(this, tr("About Hit Robot Group"),
                       tr("<b>Hit Robot Group</b><br>"
                          "we have the top robotics research scientists in<br>"
                          "the country and we are in the robot technology <br>"
                          "subject frountirer struggle,unity,hard work of<br>"
                          "high level research team.<br>"
                          "To get more information about us,you can visit our<br>"
                          " website: <a href=\'http://www.hrgrobotics.com/\'>HRG</a>"));
}

void MainWindow::statusbar_server_msg(QString msg)
{
    serverlabel->setText(QStringLiteral("服务器连接:")+msg);
}

void MainWindow::statusbar_uiChanged(QString uiname)
{
    uilabel->setText(QStringLiteral("当前界面:")+uiname);
}

void MainWindow::statusbar_msg(QString msg)
{
    uilabel->setText(QStringLiteral("消息:")+msg);
}
