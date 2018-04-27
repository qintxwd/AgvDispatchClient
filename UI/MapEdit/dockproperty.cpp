#include "dockproperty.h"
#include<QtWidgets>

DockProperty::DockProperty(OneMap *_oneMap, QWidget *parent) : QDockWidget(parent),
    oneMap(_oneMap)
{
    setMaximumWidth(300);
    tableWidget = new QTableWidget();
    tableWidget->setColumnCount(2);
    QStringList labels;
    labels << QStringLiteral("属性")
           << QStringLiteral("值");
    tableWidget->setHorizontalHeaderLabels(labels);
    tableWidget->verticalHeader()->hide();
    tableWidget->setShowGrid(true);
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    setWidget(tableWidget);
    setWindowTitle(QStringLiteral("结点属性"));
}

void DockProperty::showSpirit(int id)
{
    MapSpirit *newSpirit = oneMap->getSpiritById(id);
    if(spirit == newSpirit) return ;
    spirit = newSpirit;
    if(spirit == nullptr){
        while(tableWidget->rowCount()>0){
            tableWidget->removeRow(0);
        }
        tableWidget->update();
        return ;
    }
    switch (spirit->getSpiritType()) {
    case MapSpirit::Map_Sprite_Type_Point:
        showPoint();
        break;
    case MapSpirit::Map_Sprite_Type_Path:
        showPath();
        break;
    case MapSpirit::Map_Sprite_Type_Floor:
        showFloor();
        break;
    default:
        while(tableWidget->rowCount()>0){
            tableWidget->removeRow(0);
        }
        tableWidget->update();
        break;
    }
}

void DockProperty::showPoint()
{
    //清空原来的数据
    while(tableWidget->rowCount()>0){
        tableWidget->removeRow(0);
    }
    tableWidget->update();
    MapPoint *point = static_cast<MapPoint *>(spirit);
    tableWidget->setRowCount(10);

    //对象ID
    QTableWidgetItem *itemKeyId = new QTableWidgetItem("ID");
    itemKeyId->setTextAlignment(Qt::AlignCenter);
    itemKeyId->setFlags(itemKeyId->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(0, 0, itemKeyId);
    QTableWidgetItem *itemValueId = new QTableWidgetItem(QString("%1").arg(point->getId()));
    itemValueId->setTextAlignment(Qt::AlignCenter);
    itemValueId->setFlags(itemValueId->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(0, 1, itemValueId);

    //对象NAME
    QTableWidgetItem *itemKeyName = new QTableWidgetItem("NAME");
    itemKeyName->setTextAlignment(Qt::AlignCenter);
    itemKeyName->setFlags(itemKeyName->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(1, 0, itemKeyName);
    QLineEdit *nameInput  = new QLineEdit(point->getName());
    connect(nameInput,SIGNAL(textChanged(QString)),this,SLOT(slot_PointNameChanged(QString)));
    tableWidget->setCellWidget(1,1,nameInput);

    //对象TYPE
    QTableWidgetItem *itemKeyType = new QTableWidgetItem(QStringLiteral("类型"));
    itemKeyType->setTextAlignment(Qt::AlignCenter);
    itemKeyType->setFlags(itemKeyType->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(2, 0, itemKeyType);
    QComboBox *comboxType = new QComboBox(); // 下拉选择框控件
    comboxType->addItem(QStringLiteral("绘画点"));
    comboxType->addItem(QStringLiteral("报告点"));
    comboxType->addItem(QStringLiteral("停留点"));
    comboxType->addItem(QStringLiteral("充点点"));
    comboxType->addItem(QStringLiteral("装货点"));
    comboxType->addItem(QStringLiteral("卸货点"));
    comboxType->addItem(QStringLiteral("装卸货点"));
    comboxType->setCurrentIndex((int)(point->getPointType()));
    connect(comboxType,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_PointTypeChanged(int)));
    tableWidget->setCellWidget(2, 1, (QWidget*)comboxType);

    QTableWidgetItem *itemKeyX = new QTableWidgetItem(QStringLiteral("X"));
    itemKeyX->setTextAlignment(Qt::AlignCenter);
    itemKeyX->setFlags(itemKeyX->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(3, 0, itemKeyX);
    QLineEdit *xInput  = new QLineEdit(QString("%1").arg(point->getX()));
    connect(xInput,SIGNAL(textChanged(QString)),this,SLOT(slot_PointXChanged(QString)));
    tableWidget->setCellWidget(3,1,xInput);

    QTableWidgetItem *itemKeyY = new QTableWidgetItem(QStringLiteral("Y"));
    itemKeyY->setTextAlignment(Qt::AlignCenter);
    itemKeyY->setFlags(itemKeyY->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(4, 0, itemKeyY);
    QLineEdit *yInput  = new QLineEdit(QString("%1").arg(point->getY()));
    connect(yInput,SIGNAL(textChanged(QString)),this,SLOT(slot_PointYChanged(QString)));
    tableWidget->setCellWidget(4,1,yInput);

    QTableWidgetItem *itemKeyRealX = new QTableWidgetItem(QStringLiteral("RealX"));
    itemKeyRealX->setTextAlignment(Qt::AlignCenter);
    itemKeyRealX->setFlags(itemKeyRealX->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(5, 0, itemKeyRealX);
    QLineEdit *xRealInput  = new QLineEdit(QString("%1").arg(point->getRealX()));
    connect(xRealInput,SIGNAL(textChanged(QString)),this,SLOT(slot_PointRealXChanged(QString)));
    tableWidget->setCellWidget(5,1,xRealInput);

    QTableWidgetItem *itemKeyRealY = new QTableWidgetItem(QStringLiteral("RealY"));
    itemKeyRealY->setTextAlignment(Qt::AlignCenter);
    itemKeyRealY->setFlags(itemKeyRealY->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(6, 0, itemKeyRealY);
    QLineEdit *yRealInput  = new QLineEdit(QString("%1").arg(point->getRealY()));
    connect(yRealInput,SIGNAL(textChanged(QString)),this,SLOT(slot_PointRealYChanged(QString)));
    tableWidget->setCellWidget(6,1,yRealInput);

    QTableWidgetItem *itemKeyLabelX = new QTableWidgetItem(QStringLiteral("labelXoffset"));
    itemKeyLabelX->setTextAlignment(Qt::AlignCenter);
    itemKeyLabelX->setFlags(itemKeyLabelX->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(7, 0, itemKeyLabelX);
    QLineEdit *xLabelInput  = new QLineEdit(QString("%1").arg(point->getLabelXoffset()));
    connect(xLabelInput,SIGNAL(textChanged(QString)),this,SLOT(slot_PointLabelXoffsetChanged(QString)));
    tableWidget->setCellWidget(7,1,xLabelInput);

    QTableWidgetItem *itemKeyLabelY = new QTableWidgetItem(QStringLiteral("labelYoffset"));
    itemKeyLabelY->setTextAlignment(Qt::AlignCenter);
    itemKeyLabelY->setFlags(itemKeyLabelY->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(8, 0, itemKeyLabelY);
    QLineEdit *yLabelInput  = new QLineEdit(QString("%1").arg(point->getLabelYoffset()));
    connect(yLabelInput,SIGNAL(textChanged(QString)),this,SLOT(slot_PointLabelYoffsetChanged(QString)));
    tableWidget->setCellWidget(8,1,yLabelInput);

    QTableWidgetItem *itemKeyMapChange = new QTableWidgetItem(QStringLiteral("楼层切换点"));
    itemKeyMapChange->setTextAlignment(Qt::AlignCenter);
    itemKeyMapChange->setFlags(itemKeyMapChange->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(9, 0, itemKeyMapChange);
    QComboBox *comboxMapChange = new QComboBox(); // 下拉选择框控件
    comboxMapChange->addItem(QStringLiteral("FALSE"));
    comboxMapChange->addItem(QStringLiteral("TRUE"));
    comboxMapChange->setCurrentIndex(0);
    if(point->getMapChange()){
        comboxMapChange->setCurrentIndex(1);
    }
    connect(comboxType,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_PointMapChangeChanged(int)));
    tableWidget->setCellWidget(9, 1, (QWidget*)comboxType);

    tableWidget->insertRow(0);
}

void DockProperty::showPath()
{
    //清空原来的数据
    while(tableWidget->rowCount()>0){
        tableWidget->removeRow(0);
    }
    tableWidget->update();
    MapPath *path = static_cast<MapPath *>(spirit);
    tableWidget->setRowCount(8);

    //对象ID
    QTableWidgetItem *itemKeyId = new QTableWidgetItem("ID");
    itemKeyId->setTextAlignment(Qt::AlignCenter);
    itemKeyId->setFlags(itemKeyId->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(0, 0, itemKeyId);
    QTableWidgetItem *itemValueId = new QTableWidgetItem(QString("%1").arg(path->getId()));
    itemValueId->setTextAlignment(Qt::AlignCenter);
    itemValueId->setFlags(itemValueId->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(0, 1, itemValueId);

    //对象NAME
    QTableWidgetItem *itemKeyName = new QTableWidgetItem("NAME");
    itemKeyName->setTextAlignment(Qt::AlignCenter);
    itemKeyName->setFlags(itemKeyName->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(1, 0, itemKeyName);
    QLineEdit *nameInput  = new QLineEdit(path->getName());
    connect(nameInput,SIGNAL(textChanged(QString)),this,SLOT(slot_PathNameChanged(QString)));
    tableWidget->setCellWidget(1,1,nameInput);

    //对象TYPE
    QTableWidgetItem *itemKeyType = new QTableWidgetItem(QStringLiteral("类型"));
    itemKeyType->setTextAlignment(Qt::AlignCenter);
    itemKeyType->setFlags(itemKeyType->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(2, 0, itemKeyType);
    QComboBox *comboxType = new QComboBox(); // 下拉选择框控件
    comboxType->addItem(QStringLiteral("直线"));
    comboxType->addItem(QStringLiteral("二次贝赛尔曲线"));
    comboxType->addItem(QStringLiteral("三次贝赛尔曲线"));
    comboxType->setCurrentIndex((int)(path->getPathType()));
    connect(comboxType,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_PathTypeChanged(int)));
    tableWidget->setCellWidget(2, 1, (QWidget*)comboxType);

    //p1x
    QTableWidgetItem *itemKeyP1X = new QTableWidgetItem(QStringLiteral("P1X"));
    itemKeyP1X->setTextAlignment(Qt::AlignCenter);
    itemKeyP1X->setFlags(itemKeyP1X->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(3, 0, itemKeyP1X);
    QLineEdit *xP1Input  = new QLineEdit(QString("%1").arg(path->getP1x()));
    connect(xP1Input,SIGNAL(textChanged(QString)),this,SLOT(slot_PathP1XChanged(QString)));
    tableWidget->setCellWidget(3,1,xP1Input);

    //p1y
    QTableWidgetItem *itemKeyP1Y = new QTableWidgetItem(QStringLiteral("P1Y"));
    itemKeyP1Y->setTextAlignment(Qt::AlignCenter);
    itemKeyP1Y->setFlags(itemKeyP1Y->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(4, 0, itemKeyP1Y);
    QLineEdit *yP1Input  = new QLineEdit(QString("%1").arg(path->getP1y()));
    connect(yP1Input,SIGNAL(textChanged(QString)),this,SLOT(slot_PathP1YChanged(QString)));
    tableWidget->setCellWidget(4,1,yP1Input);

    //p2x
    QTableWidgetItem *itemKeyP2X = new QTableWidgetItem(QStringLiteral("P2X"));
    itemKeyP2X->setTextAlignment(Qt::AlignCenter);
    itemKeyP2X->setFlags(itemKeyP2X->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(5, 0, itemKeyP2X);
    QLineEdit *xP2Input  = new QLineEdit(QString("%1").arg(path->getP2x()));
    connect(xP2Input,SIGNAL(textChanged(QString)),this,SLOT(slot_PathP2XChanged(QString)));
    tableWidget->setCellWidget(5,1,xP2Input);

    //p2y
    QTableWidgetItem *itemKeyP2Y = new QTableWidgetItem(QStringLiteral("P2Y"));
    itemKeyP2Y->setTextAlignment(Qt::AlignCenter);
    itemKeyP2Y->setFlags(itemKeyP2Y->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(6, 0, itemKeyP2Y);
    QLineEdit *yP2Input  = new QLineEdit(QString("%1").arg(path->getP2y()));
    connect(yP2Input,SIGNAL(textChanged(QString)),this,SLOT(slot_PathP2YChanged(QString)));
    tableWidget->setCellWidget(6,1,yP2Input);

    //length
    QTableWidgetItem *itemKeyLength = new QTableWidgetItem(QStringLiteral("Length"));
    itemKeyLength->setTextAlignment(Qt::AlignCenter);
    itemKeyLength->setFlags(itemKeyLength->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(7, 0, itemKeyLength);
    QLineEdit *lengthInput  = new QLineEdit(QString("%1").arg(path->getLength()));
    connect(lengthInput,SIGNAL(textChanged(QString)),this,SLOT(slot_LengthChanged(QString)));
    tableWidget->setCellWidget(7,1,lengthInput);


}
void DockProperty::showFloor()
{
    //清空原来的数据
    while(tableWidget->rowCount()>0){
        tableWidget->removeRow(0);
    }
    tableWidget->update();
    MapFloor *floor = static_cast<MapFloor *>(spirit);
    tableWidget->setRowCount(2);

    //对象ID
    QTableWidgetItem *itemKeyId = new QTableWidgetItem("ID");
    itemKeyId->setTextAlignment(Qt::AlignCenter);
    itemKeyId->setFlags(itemKeyId->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(0, 0, itemKeyId);
    QTableWidgetItem *itemValueId = new QTableWidgetItem(QString("%1").arg(floor->getId()));
    itemValueId->setTextAlignment(Qt::AlignCenter);
    itemValueId->setFlags(itemValueId->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(0, 1, itemValueId);

    //对象NAME
    QTableWidgetItem *itemKeyName = new QTableWidgetItem("NAME");
    itemKeyName->setTextAlignment(Qt::AlignCenter);
    itemKeyName->setFlags(itemKeyName->flags()&~Qt::ItemIsEditable&~Qt::ItemIsEnabled);//不可编译
    tableWidget->setItem(1, 0, itemKeyName);
    QLineEdit *nameInput  = new QLineEdit(floor->getName());
    connect(nameInput,SIGNAL(textChanged(QString)),this,SLOT(slot_FloorNameChanged(QString)));
    tableWidget->setCellWidget(1,1,nameInput);
}

void DockProperty::slot_PointNameChanged(QString name)
{
    spirit->setName(name);
}
void DockProperty::slot_PointTypeChanged(int type)
{
    spirit->setSpiritType((MapSpirit::Map_Spirit_Type)type);
}
void DockProperty::slot_PointXChanged(QString x)
{
    (static_cast<MapPoint *>(spirit))->setX(x.toInt());
    //TODO:
}
void DockProperty::slot_PointYChanged(QString y)
{
    (static_cast<MapPoint *>(spirit))->setY(y.toInt());
}
void DockProperty::slot_PointRealXChanged(QString realx)
{
    (static_cast<MapPoint *>(spirit))->setRealX(realx.toInt());
}
void DockProperty::slot_PointRealYChanged(QString realy)
{
    (static_cast<MapPoint *>(spirit))->setRealY(realy.toInt());
}
void DockProperty::slot_PointLabelXoffsetChanged(QString labelXoffset)
{
    (static_cast<MapPoint *>(spirit))->setLabelXoffset(labelXoffset.toInt());
}
void DockProperty::slot_PointLabelYoffsetChanged(QString labelYoffset)
{
    (static_cast<MapPoint *>(spirit))->setLabelYoffset(labelYoffset.toInt());
}
void DockProperty::slot_PointMapChangeChanged(int mapchange)
{
    (static_cast<MapPoint *>(spirit))->setMapChange(mapchange == 1);
}

void DockProperty::slot_PathNameChanged(QString name)
{
    (static_cast<MapPath *>(spirit))->setName(name);
}

void DockProperty::slot_PathTypeChanged(int type)
{
    (static_cast<MapPath *>(spirit))->setPathType((MapPath::Map_Path_Type)type);
}
void DockProperty::slot_PathP1XChanged(QString p1x)
{
    (static_cast<MapPath *>(spirit))->setP1x(p1x.toInt());
}
void DockProperty::slot_PathP1YChanged(QString p1y)
{
    (static_cast<MapPath *>(spirit))->setP1y(p1y.toInt());
}
void DockProperty::slot_PathP2XChanged(QString p2x)
{
    (static_cast<MapPath *>(spirit))->setP2x(p2x.toInt());
}
void DockProperty::slot_PathP2YChanged(QString p2y)
{
    (static_cast<MapPath *>(spirit))->setP2y(p2y.toInt());
}
void DockProperty::slot_LengthChanged(QString length)
{
    (static_cast<MapPath *>(spirit))->setLength(length.toInt());
}

void DockProperty::slot_FloorNameChanged(QString name)
{
    (static_cast<MapFloor *>(spirit))->setName(name);
}
