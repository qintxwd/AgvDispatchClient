#include "dialogadduser.h"

#include "global.h"

DialogAddUser::DialogAddUser(QWidget *parent) : QDialog(parent)
{
    //TODO:设置输入内容的正则表达式
    usernameLabel = new QLabel(QStringLiteral("用户名:"));
    usernameInput = new QLineEdit;

    passwordLabel = new QLabel(QStringLiteral("密码:"));
    passwordInput = new QLineEdit;
    passwordInput->setEchoMode(QLineEdit::Password);

    passwordConfirmLabel = new QLabel(QStringLiteral("确认密码:"));
    passwordConfirmInput = new QLineEdit();
    passwordConfirmInput->setEchoMode(QLineEdit::Password);

    roleLabel = new QLabel(QStringLiteral("角色"));
    roleGroup = new QHBoxLayout;
    visitorRadio = new QRadioButton(QStringLiteral("游客"));
    operatorRadio = new QRadioButton(QStringLiteral("操作员"));
    operatorRadio->setChecked(true);
    managerRadio = new QRadioButton(QStringLiteral("管理员"));
    sysManagerRadio = new QRadioButton(QStringLiteral("系统管理员"));
    developerRadio = new QRadioButton(QStringLiteral("开发者"));
    roleGroup->addWidget(visitorRadio);
    roleGroup->addWidget(operatorRadio);
    roleGroup->addWidget(managerRadio);
    roleGroup->addWidget(sysManagerRadio);
    roleGroup->addWidget(developerRadio);
    tipLabel = new QLabel("");

    okBtn = new QPushButton(QStringLiteral("添加"));
    cancelBtn = new QPushButton(QStringLiteral("取消"));

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(usernameLabel,0,0);
    gridLayout->addWidget(usernameInput,0,1);
    gridLayout->addWidget(passwordLabel,1,0);
    gridLayout->addWidget(passwordInput,1,1);
    gridLayout->addWidget(passwordConfirmLabel,2,0);
    gridLayout->addWidget(passwordConfirmInput,2,1);
    gridLayout->addWidget(roleLabel,3,0);
    gridLayout->addItem(roleGroup,3,1);

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(okBtn);
    hboxLayout->addWidget(cancelBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addItem(gridLayout);
    mainLayout->addItem(hboxLayout);
    mainLayout->addWidget(tipLabel);

    setLayout(mainLayout);

    connect(okBtn,SIGNAL(clicked(bool)),this,SLOT(onOkBtn()));
    connect(cancelBtn,SIGNAL(clicked(bool)),this,SLOT(close()));

    connect(&msgCenter,SIGNAL(tip(QString)),tipLabel,SLOT(setText(QString)));
    connect(&msgCenter,SIGNAL(addUserSuccess()),this,SLOT(accept()));
}

void DialogAddUser::onOkBtn()
{
    if(usernameInput->text().trimmed().length() == 0){
        QMessageBox::warning(this,QStringLiteral("填写不合法"),QStringLiteral("用户名不能为空"));
        return ;
    }
    if(passwordInput->text().trimmed().length() == 0){
        QMessageBox::warning(this,QStringLiteral("填写不合法"),QStringLiteral("密码不能为空"));
        return ;
    }
    if(passwordInput->text()!= passwordConfirmInput->text()){
        QMessageBox::warning(this,QStringLiteral("填写不合法"),QStringLiteral("两次密码不相同"));
        return ;
    }

    //其他的

    //角色
    int role = 0;
    if(visitorRadio->isChecked())role = 0;
    if(operatorRadio->isChecked())role = 1;
    if(managerRadio->isChecked())role = 2;
    if(sysManagerRadio->isChecked())role = 3;
    if(developerRadio->isChecked())role = 4;
    msgCenter.adduser(usernameInput->text().trimmed(),passwordInput->text().trimmed(),role);
}
