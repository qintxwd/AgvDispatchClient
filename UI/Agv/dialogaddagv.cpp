#include "dialogaddagv.h"

#include "global.h"

DialogAddAgv::DialogAddAgv(QWidget *parent) : QDialog(parent)
{
    agvnameLabel = new QLabel(QStringLiteral("名称:"));
    agvnameInput = new QLineEdit;

    ipLabel = new QLabel(QStringLiteral("IP地址:"));
    ipInput = new QLineEdit;

    portLabel = new QLabel(QStringLiteral("端口:"));;
    portInput = new QLineEdit;

    tipLabel = new QLabel("");

    okBtn = new QPushButton(QStringLiteral("添加"));
    cancelBtn = new QPushButton(QStringLiteral("取消"));
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(agvnameLabel,0,0);
    gridLayout->addWidget(agvnameInput,0,1);
    gridLayout->addWidget(ipLabel,1,0);
    gridLayout->addWidget(ipInput,1,1);
    gridLayout->addWidget(portLabel,2,0);
    gridLayout->addWidget(portInput,2,1);

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
    connect(&msgCenter,SIGNAL(addAgvSuccess()),this,SLOT(accept()));
}

void DialogAddAgv::onOkBtn()
{
    if(agvnameInput->text().trimmed().length() == 0){
        QMessageBox::warning(this,QStringLiteral("填写不合法"),QStringLiteral("名称不能为空"));
        return ;
    }
    if(ipInput->text().trimmed().length() == 0){
        QMessageBox::warning(this,QStringLiteral("填写不合法"),QStringLiteral("IP不能为空"));
        return ;
    }

    if(portInput->text().trimmed().length() == 0){
        QMessageBox::warning(this,QStringLiteral("填写不合法"),QStringLiteral("端口不能为空"));
        return ;
    }

    msgCenter.addagv(agvnameInput->text().trimmed(),ipInput->text().trimmed(),portInput->text().toInt());
}
