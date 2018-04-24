#include "dialogmodifyagv.h"
#include "global.h"
DialogModifyAgv::DialogModifyAgv(int id, QString name, QString ip, int port, QWidget *parent) : QDialog(parent),
    m_id(id)
{
    nameLabel = new QLabel(QStringLiteral("名称:"));
    nameInput = new QLineEdit;
    nameInput->setText(name);

    ipLabel = new QLabel(QStringLiteral("IP地址:"));
    ipInput = new QLineEdit;
    ipInput->setText(ip);

    portLabel = new QLabel(QStringLiteral("端口:"));
    portInput = new QLineEdit;
    portInput->setText(QString("%1").arg(port));


    tipLabel = new QLabel("");

    okBtn = new QPushButton(QStringLiteral("添加"));
    cancelBtn = new QPushButton(QStringLiteral("取消"));

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(nameLabel,0,0);
    gridLayout->addWidget(nameInput,0,1);
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
    connect(&msgCenter,SIGNAL(modifyAgvSuccess()),this,SLOT(accept()));
}

void DialogModifyAgv::onOkBtn()
{
    if(nameInput->text().trimmed().length() == 0){
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

    msgCenter.modifyagv(m_id,nameInput->text().trimmed(),ipInput->text().trimmed(),portInput->text().toInt());
}
