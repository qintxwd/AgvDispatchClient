#include "dialogaddagv.h"

#include "global.h"

#include <QtWidgets>

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

    stationLabel = new QLabel(QStringLiteral("位置:"));;
    stationCbox = new QComboBox();

    auto ae = g_onemap.getAllElement();

    for(auto e:ae){
        if(e->getSpiritType() == MapSpirit::Map_Sprite_Type_Point){
            stationCbox->addItem(QString::fromStdString(e->getName()));
            stationIds.append(e->getId());
        }
    }

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(agvnameLabel,0,0);
    gridLayout->addWidget(agvnameInput,0,1);
    gridLayout->addWidget(ipLabel,1,0);
    gridLayout->addWidget(ipInput,1,1);
    gridLayout->addWidget(portLabel,2,0);
    gridLayout->addWidget(portInput,2,1);
    gridLayout->addWidget(stationLabel,3,0);
    gridLayout->addWidget(stationCbox,3,1);


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

    if(stationCbox->currentIndex()<0){
        QMessageBox::warning(this,QStringLiteral("选择站点"),QStringLiteral("请选择站点"));
        return ;
    }
    int stationId = stationIds[stationCbox->currentIndex()];

    msgCenter.addagv(agvnameInput->text().trimmed(),ipInput->text().trimmed(),portInput->text().toInt(),stationId);
}
