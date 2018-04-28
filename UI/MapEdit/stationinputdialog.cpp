#include "stationinputdialog.h"

#include "global.h"

StationInputDialog::StationInputDialog(QString _name, int _rfid, QWidget *parent) : QDialog(parent),name(_name),rfid(_rfid)
{
    setWindowTitle(QStringLiteral("输入站点属性"));

    okBtn = new QPushButton(QStringLiteral("确认"));
    cancelBtn = new QPushButton(QStringLiteral("取消"));

    connect(okBtn,SIGNAL(clicked(bool)),this,SLOT(onOk()));
    connect(cancelBtn,SIGNAL(clicked(bool)),this,SLOT(reject()));

    nameLabel = new QLabel(QStringLiteral("站点名称:"));
    rfidLabel = new QLabel("RFID:");
    nameinput = new QLineEdit(name);
    nameinput->setMaxLength(10);
    rfidinput = new QLineEdit(QString("%1").arg(rfid));
    rfidinput->setMaxLength(8);
    QRegExp rx("[1-9]\\d{0,7}");
    QValidator *validator = new QRegExpValidator(rx, this);
    rfidinput->setValidator(validator);

    QFormLayout *form = new QFormLayout;
    form->addRow(nameLabel,nameinput);
    form->addRow(rfidLabel,rfidinput);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(okBtn);
    hlayout->addWidget(cancelBtn);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addItem(form);
    vlayout->addItem(hlayout);

    setLayout(vlayout);
}

void StationInputDialog::onOk()
{
    if(rfidinput->text().trimmed().length()<=0){
        QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("RFID不能为空"),QMessageBox::Ok);
        return ;
    }
    name = nameinput->text();
    rfid = rfidinput->text().toInt();
    accept();
}

