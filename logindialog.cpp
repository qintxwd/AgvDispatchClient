#include "logindialog.h"
#include "global.h"


LoginDialog::LoginDialog()
{
    initUi();
}


void LoginDialog::initUi(){

    //登录界面
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

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

    connect(&msgCenter,SIGNAL(sig_connection_connected()),this,SLOT(onConnected()));
    connect(&msgCenter,SIGNAL(sig_connection_disconnected()),this,SLOT(onConnecting()));
    connect(&msgCenter,SIGNAL(sig_connection_conntectting()),this,SLOT(onDisconnected()));
    connect(&msgCenter,SIGNAL(loginSuccess(int)),this,SLOT(onLoginSuccess(int)));

    connect(&msgCenter,SIGNAL(sendRequestFail()),this,SLOT(onSendFail()));
    connect(&msgCenter,SIGNAL(waitResponseTimeOut()),this,SLOT(onWaitTimeOut()));
    connect(&msgCenter,SIGNAL(tip(QString)),this,SLOT(onTip(QString)));
    connect(&msgCenter,SIGNAL(err(int,QString)),this,SLOT(onErr(int,QString)));
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

void LoginDialog::onSendFail()
{
    onTip(QStringLiteral("发送请求失败，请检查链接"));
}

void LoginDialog::onWaitTimeOut()
{
    onTip(QStringLiteral("等待服务器响应超时"));
}

void LoginDialog::onErr(int code,QString info)
{
    Q_UNUSED(info);
    onTip(getErrorString(code));
}

void LoginDialog::onLoginSuccess(int role)
{
    //根据role的不同，显示不同的界面
    //TODO
    onTip(QStringLiteral("登录成功"));
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
    ipInput->setText(msgCenter.getServerIp());
    portInput->setText(QString("%1").arg(msgCenter.getServerPort()));
    pagesWidget->setCurrentIndex(1);
}

void LoginDialog::on_loginBtn_clicked()
{
    //TODO:
    onTip(QStringLiteral("正在登录"));
    msgCenter.login(nameInput->text(),pwdInput->text());
}

void LoginDialog::on_okBtn_clicked()
{
    msgCenter.resetIpPort(ipInput->text(),portInput->text().toInt());
    pagesWidget->setCurrentIndex(0);
}

void LoginDialog::on_cancelBtn_clicked()
{
    pagesWidget->setCurrentIndex(0);
}
