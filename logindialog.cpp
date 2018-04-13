#include "logindialog.h"
#include "global.h"


LoginDialog::LoginDialog()
{
    initUi();
}


void LoginDialog::initUi(){
    //登录界面
    loginWidget = new QWidget;
    nameInputLabel = new QLabel(QStringLiteral("用户名:"));
    nameInput = new QLineEdit(QStringLiteral(""));
    nameInput->setObjectName("nameInput");
    passwordInputLabel= new QLabel(QStringLiteral("密  码:"));
    pwdInput= new QLineEdit(QStringLiteral(""));
    pwdInput->setObjectName("pwdInput");
    tipLabel= new QLabel(QStringLiteral(""));
    configBtn = new QPushButton(QStringLiteral("配置"));
    configBtn->setObjectName("configBtn");
    loginBtn = new QPushButton(QStringLiteral("登  录"));
    loginBtn->setObjectName("loginBtn");
    QGroupBox *loginGroup = new QGroupBox(QStringLiteral("用户登录"));
    QGridLayout *loginGrid = new QGridLayout;
    loginGrid->addWidget(nameInputLabel, 0, 0);
    loginGrid->addWidget(nameInput, 0, 1);
    loginGrid->addWidget(passwordInputLabel, 1, 0);
    loginGrid->addWidget(pwdInput, 1, 1);
    loginGrid->addWidget(loginBtn, 2, 0,1,2);
    loginGrid->addWidget(tipLabel, 3, 0,1,2);
    loginGroup->setLayout(loginGrid);

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addStretch(1);
    btnsLayout->addWidget(configBtn);

    QVBoxLayout *loginWidgetLayout = new QVBoxLayout;
    loginWidgetLayout->addWidget(loginGroup);
    loginWidgetLayout->addLayout(btnsLayout);

    loginWidget->setLayout(loginWidgetLayout);

    //配置界面
    configServerWidget = new QWidget;
    ipInputLabel= new QLabel(QStringLiteral("服务器IP:"));
    ipInput= new QLineEdit(QStringLiteral(""));
    ipInput->setObjectName("ipInput");
    portInputLabel= new QLabel(QStringLiteral("服务器端口:"));
    portInput= new QLineEdit(QStringLiteral(""));
    portInput->setObjectName("portInput");
    okBtn = new QPushButton(QStringLiteral("确 认"));
    okBtn->setObjectName("okBtn");
    cancelBtn = new QPushButton(QStringLiteral("取 消"));
    cancelBtn->setObjectName("cancelBtn");
    QGroupBox *configGroup = new QGroupBox(QStringLiteral("服务器配置"));

    QGridLayout *configGrid = new QGridLayout;
    configGrid->addWidget(ipInputLabel, 0, 0);
    configGrid->addWidget(ipInput, 0, 1);
    configGrid->addWidget(portInputLabel, 1, 0);
    configGrid->addWidget(portInput, 1, 1);

    QHBoxLayout *btnsLayout2 = new QHBoxLayout;
    btnsLayout2->addWidget(okBtn);
    btnsLayout2->addWidget(cancelBtn);

    QVBoxLayout *confVLayout = new QVBoxLayout;
    confVLayout->addLayout(configGrid);
    confVLayout->addLayout(btnsLayout2);

    configGroup->setLayout(confVLayout);

    QVBoxLayout *configWidgetLayout = new QVBoxLayout;
    configWidgetLayout->addWidget(configGroup);
    configServerWidget->setLayout(configWidgetLayout);

    //切换界面
    pagesWidget = new QStackedWidget;
    pagesWidget->addWidget(loginWidget);
    pagesWidget->addWidget(configServerWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(pagesWidget);
    setLayout(mainLayout);

    QMetaObject::connectSlotsByName(this);

//    connect(g_server_connection,SIGNAL(sig_connected()),this,SLOT(onConnected()));
//    connect(g_server_connection,SIGNAL(sig_conntecting()),this,SLOT(onConnecting()));
//    connect(g_server_connection,SIGNAL(sig_disconnected()),this,SLOT(onDisconnected()));
}

void LoginDialog::onConnected()
{
    onTip(QStringLiteral("服务器已连接"));
}

void LoginDialog::onDisconnected()
{
    onTip(QStringLiteral("服务器已断开连接"));
}

void LoginDialog::onConnecting()
{
    onTip(QStringLiteral("正在连接服务器"));
}

void LoginDialog::onTip(QString tip)
{
    tipLabel->setText(tip);
}

void LoginDialog::on_nameInput_textChanged()
{
    tipLabel->setText("");
}

void LoginDialog::on_pwdInput_textChanged()
{
    tipLabel->setText("");
}

void LoginDialog::on_configBtn_clicked()
{
    ipInput->setText(g_server_connection->getIp());
    portInput->setText(QString("%1").arg(g_server_connection->getPort()));
    pagesWidget->setCurrentIndex(1);
}

void LoginDialog::on_loginBtn_clicked()
{
    //TODO:
    onTip(QStringLiteral("正在登录"));
}

void LoginDialog::on_okBtn_clicked()
{
    g_server_connection->connectToServer(ipInput->text(),portInput->text().toInt());
    pagesWidget->setCurrentIndex(0);
}

void LoginDialog::on_cancelBtn_clicked()
{
    pagesWidget->setCurrentIndex(0);
}
