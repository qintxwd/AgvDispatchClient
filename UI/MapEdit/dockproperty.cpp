#include "dockproperty.h"
#include<QtWidgets>

DockProperty::DockProperty(OneMap *_oneMap, QWidget *parent) : QDockWidget(parent),
    oneMap(_oneMap)
{
    setMaximumWidth(300);
    tableWidget = new QTableWidget(20,2);
    QStringList labels;
    labels << QStringLiteral("属性")
           << QStringLiteral("值");
    tableWidget->setHorizontalHeaderLabels(labels);
    tableWidget->verticalHeader()->hide();
    tableWidget->setShowGrid(true);
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    setWidget(tableWidget);
    setWindowTitle(QStringLiteral("结点属性"));
    init();
}

void DockProperty::init()
{
    /////////point
    point_itemKeyId = new QTableWidgetItem("ID");
    point_itemKeyId->setTextAlignment(Qt::AlignCenter);
    point_itemKeyId->setFlags(point_itemKeyId->flags()&(~Qt::ItemIsEditable));
    point_itemValueId = new QTableWidgetItem("");
    point_itemValueId->setTextAlignment(Qt::AlignCenter);
    point_itemValueId->setFlags(point_itemValueId->flags()&(~Qt::ItemIsEditable));
    point_itemKeyName = new QTableWidgetItem("NAME");
    point_itemKeyName->setTextAlignment(Qt::AlignCenter);
    point_itemKeyName->setFlags(point_itemKeyName->flags()&(~Qt::ItemIsEditable));
    point_nameInput  = new QLineEdit("");
    connect(point_nameInput,SIGNAL(textEdited(QString)),this,SLOT(slot_PointNameChanged(QString)));
    point_itemKeyType = new QTableWidgetItem(QStringLiteral("类型"));
    point_itemKeyType->setTextAlignment(Qt::AlignCenter);
    point_itemKeyType->setFlags(point_itemKeyType->flags()&(~Qt::ItemIsEditable));
    point_comboxType = new QComboBox(); // 下拉选择框控件
    point_comboxType->addItem(QStringLiteral("绘画点"));
    point_comboxType->addItem(QStringLiteral("报告点"));
    point_comboxType->addItem(QStringLiteral("停留点"));
    point_comboxType->addItem(QStringLiteral("充电点"));
    point_comboxType->addItem(QStringLiteral("装货点"));
    point_comboxType->addItem(QStringLiteral("卸货点"));
    point_comboxType->addItem(QStringLiteral("装卸货点"));
    connect(point_comboxType,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_PointTypeChanged(int)));
    point_itemKeyX = new QTableWidgetItem(QStringLiteral("X"));
    point_itemKeyX->setTextAlignment(Qt::AlignCenter);
    point_itemKeyX->setFlags(point_itemKeyX->flags()&(~Qt::ItemIsEditable));
    point_xInput  = new QLineEdit(QString(""));
    connect(point_xInput,SIGNAL(textEdited(QString)),this,SLOT(slot_PointXChanged(QString)));
    point_itemKeyY = new QTableWidgetItem(QStringLiteral("Y"));
    point_itemKeyY->setTextAlignment(Qt::AlignCenter);
    point_itemKeyY->setFlags(point_itemKeyY->flags()&(~Qt::ItemIsEditable));
    point_yInput  = new QLineEdit(QString(""));
    connect(point_yInput,SIGNAL(textEdited(QString)),this,SLOT(slot_PointYChanged(QString)));
    point_itemKeyRealX = new QTableWidgetItem(QStringLiteral("RealX"));
    point_itemKeyRealX->setTextAlignment(Qt::AlignCenter);
    point_itemKeyRealX->setFlags(point_itemKeyRealX->flags()&(~Qt::ItemIsEditable));
    point_xRealInput  = new QLineEdit(QString(""));
    connect(point_xRealInput,SIGNAL(textEdited(QString)),this,SLOT(slot_PointRealXChanged(QString)));
    point_itemKeyRealY = new QTableWidgetItem(QStringLiteral("RealY"));
    point_itemKeyRealY->setTextAlignment(Qt::AlignCenter);
    point_itemKeyRealY->setFlags(point_itemKeyRealY->flags()&(~Qt::ItemIsEditable));
    point_yRealInput  = new QLineEdit(QString(""));
    connect(point_yRealInput,SIGNAL(textEdited(QString)),this,SLOT(slot_PointRealYChanged(QString)));
    point_itemKeyLabelX = new QTableWidgetItem(QStringLiteral("labelXoffset"));
    point_itemKeyLabelX->setTextAlignment(Qt::AlignCenter);
    point_itemKeyLabelX->setFlags(point_itemKeyLabelX->flags()&(~Qt::ItemIsEditable));
    point_xLabelInput  = new QLineEdit(QString(""));
    connect(point_xLabelInput,SIGNAL(textEdited(QString)),this,SLOT(slot_PointLabelXoffsetChanged(QString)));
    point_itemKeyLabelY = new QTableWidgetItem(QStringLiteral("labelYoffset"));
    point_itemKeyLabelY->setTextAlignment(Qt::AlignCenter);
    point_itemKeyLabelY->setFlags(point_itemKeyLabelY->flags()&(~Qt::ItemIsEditable));
    point_yLabelInput  = new QLineEdit(QString(""));
    connect(point_yLabelInput,SIGNAL(textEdited(QString)),this,SLOT(slot_PointLabelYoffsetChanged(QString)));
    point_itemKeyMapChange = new QTableWidgetItem(QStringLiteral("楼层切换点"));
    point_itemKeyMapChange->setTextAlignment(Qt::AlignCenter);
    point_itemKeyMapChange->setFlags(point_itemKeyMapChange->flags()&(~Qt::ItemIsEditable));
    point_comboxMapChange = new QComboBox(); // 下拉选择框控件
    point_comboxMapChange->addItem(QStringLiteral("FALSE"));
    point_comboxMapChange->addItem(QStringLiteral("TRUE"));
    point_comboxMapChange->setCurrentIndex(0);
    connect(point_comboxMapChange,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_PointMapChangeChanged(int)));

    ////////////path
    path_itemKeyId = new QTableWidgetItem("ID");
    path_itemKeyId->setTextAlignment(Qt::AlignCenter);
    path_itemKeyId->setFlags(path_itemKeyId->flags()&(~Qt::ItemIsEditable));
    path_itemValueId = new QTableWidgetItem(QString(""));
    path_itemValueId->setTextAlignment(Qt::AlignCenter);
    path_itemValueId->setFlags(path_itemValueId->flags()&(~Qt::ItemIsEditable));
    path_itemKeyName = new QTableWidgetItem("NAME");
    path_itemKeyName->setTextAlignment(Qt::AlignCenter);
    path_itemKeyName->setFlags(path_itemKeyName->flags()&(~Qt::ItemIsEditable));
    path_nameInput  = new QLineEdit("");
    connect(path_nameInput,SIGNAL(textEdited(QString)),this,SLOT(slot_PathNameChanged(QString)));
    path_itemKeyType = new QTableWidgetItem(QStringLiteral("类型"));
    path_itemKeyType->setTextAlignment(Qt::AlignCenter);
    path_itemKeyType->setFlags(path_itemKeyType->flags()&(~Qt::ItemIsEditable));
    path_comboxType = new QComboBox(); // 下拉选择框控件
    path_comboxType->addItem(QStringLiteral("直线"));
    path_comboxType->addItem(QStringLiteral("二次贝赛尔曲线"));
    path_comboxType->addItem(QStringLiteral("三次贝赛尔曲线"));
    connect(path_comboxType,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_PathTypeChanged(int)));
    path_itemKeyP1X = new QTableWidgetItem(QStringLiteral("P1X"));
    path_itemKeyP1X->setTextAlignment(Qt::AlignCenter);
    path_itemKeyP1X->setFlags(path_itemKeyP1X->flags()&(~Qt::ItemIsEditable));
    path_xP1Input  = new QLineEdit(QString(""));
    connect(path_xP1Input,SIGNAL(textEdited(QString)),this,SLOT(slot_PathP1XChanged(QString)));
    path_itemKeyP1Y = new QTableWidgetItem(QStringLiteral("P1Y"));
    path_itemKeyP1Y->setTextAlignment(Qt::AlignCenter);
    path_itemKeyP1Y->setFlags(path_itemKeyP1Y->flags()&(~Qt::ItemIsEditable));
    path_yP1Input  = new QLineEdit(QString(""));
    connect(path_yP1Input,SIGNAL(textEdited(QString)),this,SLOT(slot_PathP1YChanged(QString)));
    path_itemKeyP2X = new QTableWidgetItem(QStringLiteral("P2X"));
    path_itemKeyP2X->setTextAlignment(Qt::AlignCenter);
    path_itemKeyP2X->setFlags(path_itemKeyP2X->flags()&(~Qt::ItemIsEditable));
    path_xP2Input  = new QLineEdit(QString(""));
    connect(path_xP2Input,SIGNAL(textEdited(QString)),this,SLOT(slot_PathP2XChanged(QString)));
    path_itemKeyP2Y = new QTableWidgetItem(QStringLiteral("P2Y"));
    path_itemKeyP2Y->setTextAlignment(Qt::AlignCenter);
    path_itemKeyP2Y->setFlags(path_itemKeyP2Y->flags()&(~Qt::ItemIsEditable));
    path_yP2Input  = new QLineEdit(QString(""));
    connect(path_yP2Input,SIGNAL(textEdited(QString)),this,SLOT(slot_PathP2YChanged(QString)));
    path_itemKeyLength = new QTableWidgetItem(QStringLiteral("Length"));
    path_itemKeyLength->setTextAlignment(Qt::AlignCenter);
    path_itemKeyLength->setFlags(path_itemKeyLength->flags()&(~Qt::ItemIsEditable));
    path_lengthInput  = new QLineEdit(QString(""));
    connect(path_lengthInput,SIGNAL(textEdited(QString)),this,SLOT(slot_LengthChanged(QString)));

    ///////floor
    floor_itemKeyId = new QTableWidgetItem("ID");
    floor_itemValueId = new QTableWidgetItem("");
    floor_itemKeyName = new QTableWidgetItem("NAME");
    floor_nameInput  = new QLineEdit("");
    connect(floor_nameInput,SIGNAL(textEdited(QString)),this,SLOT(slot_FloorNameChanged(QString)));

    ////////////add to table widget
    tableWidget->setItem(0, 0, point_itemKeyId);
    tableWidget->setItem(0, 1, point_itemValueId);
    tableWidget->setItem(1, 0, point_itemKeyName);
    tableWidget->setCellWidget(1,1,point_nameInput);
    tableWidget->setItem(2, 0, point_itemKeyType);
    tableWidget->setCellWidget(2, 1, (QWidget*)point_comboxType);
    tableWidget->setItem(3, 0, point_itemKeyX);
    tableWidget->setCellWidget(3,1,point_xInput);
    tableWidget->setItem(4, 0, point_itemKeyY);
    tableWidget->setCellWidget(4,1,point_yInput);
    tableWidget->setItem(5, 0, point_itemKeyRealX);
    tableWidget->setCellWidget(5,1,point_xRealInput);
    tableWidget->setItem(6, 0, point_itemKeyRealY);
    tableWidget->setCellWidget(6,1,point_yRealInput);
    tableWidget->setItem(7, 0, point_itemKeyLabelX);
    tableWidget->setCellWidget(7,1,point_xLabelInput);
    tableWidget->setItem(8, 0, point_itemKeyLabelY);
    tableWidget->setCellWidget(8,1,point_yLabelInput);
    tableWidget->setItem(9, 0, point_itemKeyMapChange);
    tableWidget->setCellWidget(9, 1, (QWidget*)point_comboxMapChange);

    tableWidget->setItem(10, 0, path_itemKeyId);
    tableWidget->setItem(10, 1, path_itemValueId);
    tableWidget->setItem(11, 0, path_itemKeyName);
    tableWidget->setCellWidget(11,1,path_nameInput);
    tableWidget->setItem(12, 0, path_itemKeyType);
    tableWidget->setCellWidget(12, 1, (QWidget*)path_comboxType);
    tableWidget->setItem(13, 0, path_itemKeyP1X);
    tableWidget->setCellWidget(13,1,path_xP1Input);
    tableWidget->setItem(14, 0, path_itemKeyP1Y);
    tableWidget->setCellWidget(14,1,path_yP1Input);
    tableWidget->setItem(15, 0, path_itemKeyP2X);
    tableWidget->setCellWidget(15,1,path_xP2Input);
    tableWidget->setItem(16, 0, path_itemKeyP2Y);
    tableWidget->setCellWidget(16,1,path_yP2Input);
    tableWidget->setItem(17, 0, path_itemKeyLength);
    tableWidget->setCellWidget(17,1,path_lengthInput);

    tableWidget->setItem(18, 0, floor_itemKeyId);
    tableWidget->setItem(18, 1, floor_itemValueId);
    tableWidget->setItem(19, 0, floor_itemKeyName);
    tableWidget->setCellWidget(19,1,floor_nameInput);

    for(int i=0;i<20;++i){
        tableWidget->setRowHidden(i,true);
    }

}

//地图修改的属性，这边更新
void DockProperty::slot_propertyChanged(MapSpirit *_spirit)
{
    //TODO:
    if(spirit != _spirit) return ;

    for(int i=0;i<20;++i){
        tableWidget->setRowHidden(i,i>9);
    }

    //只是更新数据
    if(spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Point){
        showPoint();
    }else if(spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Path){
        showPath();
    }else if(spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Floor){
        showFloor();
    }
}

void DockProperty::slot_showSpirit(MapSpirit *s)
{
    if(spirit == s) return ;
    spirit = s;

    tableWidget->update();
    if(spirit == nullptr)return ;
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
        break;
    }
}

void DockProperty::showPoint()
{
    if(spirit==nullptr)return ;
    //清空原来的数据
    MapPoint *point = static_cast<MapPoint *>(spirit);
    point_itemValueId->setText(QString("%1").arg(point->getId()));
    point_nameInput->setText(point->getName());
    point_comboxType->setCurrentIndex((int)(point->getPointType()));;
    point_xInput->setText(QString("%1").arg(point->getX()));
    point_yInput->setText(QString("%1").arg(point->getY()));
    point_xRealInput->setText(QString("%1").arg(point->getRealX()));
    point_yRealInput->setText(QString("%1").arg(point->getRealY()));
    point_xLabelInput->setText(QString("%1").arg(point->getLabelXoffset()));
    point_yLabelInput->setText(QString("%1").arg(point->getLabelYoffset()));
    point_comboxMapChange->setCurrentIndex(0);
    if(point->getMapChange()){
        point_comboxMapChange->setCurrentIndex(1);
    }
    for(int i=0;i<20;++i){
        tableWidget->setRowHidden(i,i>9);
    }

    tableWidget->update();
}

void DockProperty::showPath()
{
    if(spirit==nullptr)return ;
    //清空原来的数据
    MapPath *path = static_cast<MapPath *>(spirit);
    path_itemValueId->setText(QString("%1").arg(path->getId()));
    path_nameInput->setText(path->getName());
    path_comboxType->setCurrentIndex((int)(path->getPathType()));
    path_xP1Input->setText(QString("%1").arg(path->getP1x()));
    path_yP1Input->setText(QString("%1").arg(path->getP1y()));
    path_xP2Input->setText(QString("%1").arg(path->getP2x()));
    path_yP2Input->setText(QString("%1").arg(path->getP2y()));
    path_lengthInput->setText(QString("%1").arg(path->getLength()));
    for(int i=0;i<20;++i){
        tableWidget->setRowHidden(i,i<=9 || i > 17);
    }
    tableWidget->update();
}
void DockProperty::showFloor()
{
    //清空原来的数据
    if(spirit==nullptr)return ;
    MapFloor *floor = static_cast<MapFloor *>(spirit);
    floor_itemValueId->setText(QString("%1").arg(floor->getId()));
    floor_nameInput->setText(floor->getName());
    for(int i=0;i<20;++i){
        tableWidget->setRowHidden(i,i <= 17);
    }
    tableWidget->update();
}

void DockProperty::slot_PointNameChanged(QString name)
{
    spirit->setName(name);
    emit sig_PointNameChanged(spirit,name);
}
void DockProperty::slot_PointTypeChanged(int _type)
{
    MapPoint *p = static_cast<MapPoint *>(spirit);
    p->setPointType((MapPoint::Map_Point_Type)_type);
    emit sig_PointTypeChanged(spirit,_type);
}
void DockProperty::slot_PointXChanged(QString x)
{
    (static_cast<MapPoint *>(spirit))->setX(x.toInt());
    emit sig_PointXChanged(spirit,x);
}
void DockProperty::slot_PointYChanged(QString y)
{
    (static_cast<MapPoint *>(spirit))->setY(y.toInt());
    emit sig_PointYChanged(spirit,y);
}
void DockProperty::slot_PointRealXChanged(QString realx)
{
    (static_cast<MapPoint *>(spirit))->setRealX(realx.toInt());
    emit sig_PointRealXChanged(spirit,realx);
}
void DockProperty::slot_PointRealYChanged(QString realy)
{
    (static_cast<MapPoint *>(spirit))->setRealY(realy.toInt());
    emit sig_PointRealYChanged(spirit,realy);
}
void DockProperty::slot_PointLabelXoffsetChanged(QString labelXoffset)
{
    (static_cast<MapPoint *>(spirit))->setLabelXoffset(labelXoffset.toInt());
    emit sig_PointLabelXoffsetChanged(spirit,labelXoffset);
}
void DockProperty::slot_PointLabelYoffsetChanged(QString labelYoffset)
{
    (static_cast<MapPoint *>(spirit))->setLabelYoffset(labelYoffset.toInt());
    emit sig_PointLabelYoffsetChanged(spirit,labelYoffset);
}
void DockProperty::slot_PointMapChangeChanged(int mapchange)
{
    (static_cast<MapPoint *>(spirit))->setMapChange(mapchange == 1);
    emit sig_PointMapChangeChanged(spirit,mapchange);
}

void DockProperty::slot_PathNameChanged(QString name)
{
    (static_cast<MapPath *>(spirit))->setName(name);
    emit sig_PathNameChanged(spirit,name);
}

void DockProperty::slot_PathTypeChanged(int _type)
{
    (static_cast<MapPath *>(spirit))->setPathType((MapPath::Map_Path_Type)_type);
    emit sig_PathTypeChanged(spirit,_type);
}
void DockProperty::slot_PathP1XChanged(QString p1x)
{
    (static_cast<MapPath *>(spirit))->setP1x(p1x.toInt());
    emit sig_PathP1XChanged(spirit,p1x);
}
void DockProperty::slot_PathP1YChanged(QString p1y)
{
    (static_cast<MapPath *>(spirit))->setP1y(p1y.toInt());
    emit sig_PathP1YChanged(spirit,p1y);
}
void DockProperty::slot_PathP2XChanged(QString p2x)
{
    (static_cast<MapPath *>(spirit))->setP2x(p2x.toInt());
    emit sig_PathP2XChanged(spirit,p2x);
}
void DockProperty::slot_PathP2YChanged(QString p2y)
{
    (static_cast<MapPath *>(spirit))->setP2y(p2y.toInt());
    emit sig_PathP2YChanged(spirit,p2y);
}
void DockProperty::slot_LengthChanged(QString length)
{
    (static_cast<MapPath *>(spirit))->setLength(length.toInt());
    emit sig_LengthChanged(spirit,length);
}

void DockProperty::slot_FloorNameChanged(QString name)
{
    (static_cast<MapFloor *>(spirit))->setName(name);
    emit sig_FloorNameChanged(spirit,name);
}
