#include "dockproperty.h"
#include<QtWidgets>

DockProperty::DockProperty(OneMap *_oneMap, QWidget *parent) : QDockWidget(parent),
    oneMap(_oneMap)
{
    setMaximumWidth(300);
    initTabTable();
    setWidget(tabwidget);
    setWindowTitle(QStringLiteral("结点属性"));
    initTableContent();
}

void DockProperty::initTabTable()
{
    tabwidget = new QTabWidget;
    tabwidget->tabBar()->hide();
    tableWidget_blank = new QTableWidget(0,2);

    tableWidget_point = new QTableWidget(10,2);
    tableWidget_path = new QTableWidget(8,2);
    tableWidget_floor = new QTableWidget(2,2);
    tableWidget_bkg = new QTableWidget(6,2);
    tableWidget_block = new QTableWidget(2,2);

    QStringList labels;
    labels << QStringLiteral("属性")
           << QStringLiteral("值");
    tableWidget_blank->setHorizontalHeaderLabels(labels);
    tableWidget_blank->verticalHeader()->hide();
    tableWidget_blank->setShowGrid(true);
    tableWidget_blank->setContextMenuPolicy(Qt::CustomContextMenu);

    tableWidget_point->setHorizontalHeaderLabels(labels);
    tableWidget_point->verticalHeader()->hide();
    tableWidget_point->setShowGrid(true);
    tableWidget_point->setContextMenuPolicy(Qt::CustomContextMenu);


    tableWidget_path->setHorizontalHeaderLabels(labels);
    tableWidget_path->verticalHeader()->hide();
    tableWidget_path->setShowGrid(true);
    tableWidget_path->setContextMenuPolicy(Qt::CustomContextMenu);


    tableWidget_floor->setHorizontalHeaderLabels(labels);
    tableWidget_floor->verticalHeader()->hide();
    tableWidget_floor->setShowGrid(true);
    tableWidget_floor->setContextMenuPolicy(Qt::CustomContextMenu);


    tableWidget_bkg->setHorizontalHeaderLabels(labels);
    tableWidget_bkg->verticalHeader()->hide();
    tableWidget_bkg->setShowGrid(true);
    tableWidget_bkg->setContextMenuPolicy(Qt::CustomContextMenu);

    tableWidget_block->setHorizontalHeaderLabels(labels);
    tableWidget_block->verticalHeader()->hide();
    tableWidget_block->setShowGrid(true);
    tableWidget_block->setContextMenuPolicy(Qt::CustomContextMenu);

    tabwidget->addTab(tableWidget_blank,"");
    tabwidget->addTab(tableWidget_point,"");
    tabwidget->addTab(tableWidget_path,"");
    tabwidget->addTab(tableWidget_floor,"");
    tabwidget->addTab(tableWidget_bkg,"");
    tabwidget->addTab(tableWidget_block,"");
    tabwidget->setCurrentIndex(0);
}

void DockProperty::initTableContent()
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
    path_comboxType->addItem(QStringLiteral("楼层间线路"));
    path_comboxType->setEnabled(false);

//    path_itemKeyDirection = new QTableWidgetItem(QStringLiteral("方向"));
//    path_itemKeyDirection->setTextAlignment(Qt::AlignCenter);
//    path_itemKeyDirection->setFlags(path_itemKeyDirection->flags()&(~Qt::ItemIsEditable));
//    path_comboxDirection = new QComboBox(); // 下拉选择框控件
//    path_comboxDirection->addItem(QStringLiteral("双向"));
//    path_comboxDirection->addItem(QStringLiteral("start-->end"));
//    path_comboxDirection->addItem(QStringLiteral("end-->start"));
//    path_comboxDirection->setEnabled(true);
//    connect(path_comboxDirection,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_PathDirectionChanged(int)));

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
    floor_itemKeyId->setTextAlignment(Qt::AlignCenter);
    floor_itemKeyId->setFlags(floor_itemKeyId->flags()&(~Qt::ItemIsEditable));
    floor_itemValueId = new QTableWidgetItem("");
    floor_itemValueId->setTextAlignment(Qt::AlignCenter);
    floor_itemValueId->setFlags(floor_itemValueId->flags()&(~Qt::ItemIsEditable));
    floor_itemKeyName = new QTableWidgetItem("NAME");
    floor_itemKeyName->setTextAlignment(Qt::AlignCenter);
    floor_itemKeyName->setFlags(floor_itemKeyName->flags()&(~Qt::ItemIsEditable));
    floor_nameInput  = new QLineEdit("");
    connect(floor_nameInput,SIGNAL(textEdited(QString)),this,SLOT(slot_FloorNameChanged(QString)));

    ///////bkg
    bkg_itemKeyId = new QTableWidgetItem("ID");
    bkg_itemKeyId->setTextAlignment(Qt::AlignCenter);
    bkg_itemKeyId->setFlags(bkg_itemKeyId->flags()&(~Qt::ItemIsEditable));
    bkg_itemValueId = new QTableWidgetItem("");
    bkg_itemValueId->setTextAlignment(Qt::AlignCenter);
    bkg_itemValueId->setFlags(bkg_itemValueId->flags()&(~Qt::ItemIsEditable));
    bkg_itemKeyName = new QTableWidgetItem("NAME");
    bkg_itemKeyName->setTextAlignment(Qt::AlignCenter);
    bkg_itemKeyName->setFlags(bkg_itemKeyName->flags()&(~Qt::ItemIsEditable));
    bkg_nameInput  = new QLineEdit("");
    connect(bkg_nameInput,SIGNAL(textEdited(QString)),this,SLOT(slot_BkgNameChanged(QString)));
    bkg_itemX = new QTableWidgetItem(QStringLiteral("X"));
    bkg_itemX->setTextAlignment(Qt::AlignCenter);
    bkg_itemX->setFlags(bkg_itemX->flags()&(~Qt::ItemIsEditable));
    bkg_xInput  = new QLineEdit(QString(""));
    connect(bkg_xInput,SIGNAL(textEdited(QString)),this,SLOT(slot_BkgXChanged(QString)));
    bkg_itemKeyY = new QTableWidgetItem(QStringLiteral("Y"));
    bkg_itemKeyY->setTextAlignment(Qt::AlignCenter);
    bkg_itemKeyY->setFlags(bkg_itemKeyY->flags()&(~Qt::ItemIsEditable));
    bkg_yInput  = new QLineEdit(QString(""));
    connect(bkg_yInput,SIGNAL(textEdited(QString)),this,SLOT(slot_BkgYChanged(QString)));
    bkg_itemWidth = new QTableWidgetItem(QStringLiteral("Width"));
    bkg_itemWidth->setTextAlignment(Qt::AlignCenter);
    bkg_itemWidth->setFlags(bkg_itemWidth->flags()&(~Qt::ItemIsEditable));
    bkg_widthInput  = new QLineEdit(QString(""));
    connect(bkg_widthInput,SIGNAL(textEdited(QString)),this,SLOT(slot_BkgWidthChanged(QString)));
    bkg_itemKeyHeight = new QTableWidgetItem(QStringLiteral("Height"));
    bkg_itemKeyHeight->setTextAlignment(Qt::AlignCenter);
    bkg_itemKeyHeight->setFlags(bkg_itemKeyHeight->flags()&(~Qt::ItemIsEditable));
    bkg_heightInput  = new QLineEdit(QString(""));
    connect(bkg_heightInput,SIGNAL(textEdited(QString)),this,SLOT(slot_BkgHeightChanged(QString)));

    ///////block
    block_itemKeyId = new QTableWidgetItem("ID");
    block_itemKeyId->setTextAlignment(Qt::AlignCenter);
    block_itemKeyId->setFlags(block_itemKeyId->flags()&(~Qt::ItemIsEditable));
    block_itemValueId = new QTableWidgetItem("");
    block_itemValueId->setTextAlignment(Qt::AlignCenter);
    block_itemValueId->setFlags(block_itemValueId->flags()&(~Qt::ItemIsEditable));
    block_itemKeyName = new QTableWidgetItem("NAME");
    block_itemKeyName->setTextAlignment(Qt::AlignCenter);
    block_itemKeyName->setFlags(block_itemKeyName->flags()&(~Qt::ItemIsEditable));
    block_nameInput  = new QLineEdit("");
    connect(block_nameInput,SIGNAL(textEdited(QString)),this,SLOT(slot_BlockNameChanged(QString)));

    ////////////add to table widget
    tableWidget_point->setItem(0, 0, point_itemKeyId);
    tableWidget_point->setItem(0, 1, point_itemValueId);
    tableWidget_point->setItem(1, 0, point_itemKeyName);
    tableWidget_point->setCellWidget(1,1,point_nameInput);
    tableWidget_point->setItem(2, 0, point_itemKeyType);
    tableWidget_point->setCellWidget(2, 1, (QWidget*)point_comboxType);
    tableWidget_point->setItem(3, 0, point_itemKeyX);
    tableWidget_point->setCellWidget(3,1,point_xInput);
    tableWidget_point->setItem(4, 0, point_itemKeyY);
    tableWidget_point->setCellWidget(4,1,point_yInput);
    tableWidget_point->setItem(5, 0, point_itemKeyRealX);
    tableWidget_point->setCellWidget(5,1,point_xRealInput);
    tableWidget_point->setItem(6, 0, point_itemKeyRealY);
    tableWidget_point->setCellWidget(6,1,point_yRealInput);
    tableWidget_point->setItem(7, 0, point_itemKeyLabelX);
    tableWidget_point->setCellWidget(7,1,point_xLabelInput);
    tableWidget_point->setItem(8, 0, point_itemKeyLabelY);
    tableWidget_point->setCellWidget(8,1,point_yLabelInput);
    tableWidget_point->setItem(9, 0, point_itemKeyMapChange);
    tableWidget_point->setCellWidget(9, 1, (QWidget*)point_comboxMapChange);

    tableWidget_path->setItem(0, 0, path_itemKeyId);
    tableWidget_path->setItem(0, 1, path_itemValueId);
    tableWidget_path->setItem(1, 0, path_itemKeyName);
    tableWidget_path->setCellWidget(1,1,path_nameInput);
    tableWidget_path->setItem(2, 0, path_itemKeyType);
    tableWidget_path->setCellWidget(2, 1, (QWidget*)path_comboxType);
//    tableWidget_path->setItem(3, 0, path_itemKeyDirection);
//    tableWidget_path->setCellWidget(3, 1, (QWidget*)path_comboxDirection);
    tableWidget_path->setItem(3, 0, path_itemKeyP1X);
    tableWidget_path->setCellWidget(3,1,path_xP1Input);
    tableWidget_path->setItem(4, 0, path_itemKeyP1Y);
    tableWidget_path->setCellWidget(4,1,path_yP1Input);
    tableWidget_path->setItem(5, 0, path_itemKeyP2X);
    tableWidget_path->setCellWidget(5,1,path_xP2Input);
    tableWidget_path->setItem(6, 0, path_itemKeyP2Y);
    tableWidget_path->setCellWidget(6,1,path_yP2Input);
    tableWidget_path->setItem(7, 0, path_itemKeyLength);
    tableWidget_path->setCellWidget(7,1,path_lengthInput);

    tableWidget_floor->setItem(0, 0, floor_itemKeyId);
    tableWidget_floor->setItem(0, 1, floor_itemValueId);
    tableWidget_floor->setItem(1, 0, floor_itemKeyName);
    tableWidget_floor->setCellWidget(1,1,floor_nameInput);

    tableWidget_bkg->setItem(0, 0, bkg_itemKeyId);
    tableWidget_bkg->setItem(0, 1, bkg_itemValueId);
    tableWidget_bkg->setItem(1, 0, bkg_itemKeyName);
    tableWidget_bkg->setCellWidget(1,1,bkg_nameInput);
    tableWidget_bkg->setItem(2, 0, bkg_itemX);
    tableWidget_bkg->setCellWidget(2,1,bkg_xInput);
    tableWidget_bkg->setItem(3, 0, bkg_itemKeyY);
    tableWidget_bkg->setCellWidget(3,1,bkg_yInput);
    tableWidget_bkg->setItem(4, 0, bkg_itemWidth);
    tableWidget_bkg->setCellWidget(4,1,bkg_widthInput);
    tableWidget_bkg->setItem(5, 0, bkg_itemKeyHeight);
    tableWidget_bkg->setCellWidget(5,1,bkg_heightInput);

    tableWidget_block->setItem(0, 0, block_itemKeyId);
    tableWidget_block->setItem(0, 1, block_itemValueId);
    tableWidget_block->setItem(1, 0, block_itemKeyName);
    tableWidget_block->setCellWidget(1,1,block_nameInput);


    //disable all edit
    tableWidget_blank->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget_point->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget_path->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget_floor->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget_bkg->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget_block->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

//地图修改的属性，这边更新
void DockProperty::slot_propertyChanged(MapSpirit *_spirit)
{
    //TODO:
    if(spirit != _spirit) return ;
    tabwidget->setCurrentIndex(0);

    //只是更新数据
    if(spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Point){
        showPoint();
    }else if(spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Path){
        showPath();
    }else if(spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Floor){
        showFloor();
    }else if(spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Background){
        showBkg();
    }else if(spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Block){
        showBlock();
    }
}

void DockProperty::slot_shownull()
{
    slot_showSpirit(nullptr);
}

void DockProperty::slot_showSpirit(MapSpirit *s)
{
    if(spirit == s) return ;
    spirit = s;

    tabwidget->setCurrentIndex(0);
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
    case MapSpirit::Map_Sprite_Type_Background:
        showBkg();
        break;
    case MapSpirit::Map_Sprite_Type_Block:
        showBlock();
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
    point_nameInput->setText(QString::fromStdString(point->getName()));
    point_comboxType->setCurrentIndex((int)(point->getPointType()));;
    point_xInput->setText(QString("%1").arg(point->getX()));
    point_yInput->setText(QString("%1").arg(point->getY()));
    point_xRealInput->setText(QString("%1").arg(point->getRealX()));
    point_yRealInput->setText(QString("%1").arg(point->getRealY()));
    point_xLabelInput->setText(QString("%1").arg(point->getLabelXoffset()));
    point_yLabelInput->setText(QString("%1").arg(point->getLabelYoffset()));
    if(point->getMapChange()){
        point_comboxMapChange->setCurrentIndex(1);
    }else{
        point_comboxMapChange->setCurrentIndex(0);

    }
    tableWidget_point->update();
    tabwidget->setCurrentIndex(1);
}

void DockProperty::showPath()
{
    if(spirit==nullptr)return ;
    //清空原来的数据
    MapPath *path = dynamic_cast<MapPath *>(spirit);
    path_itemValueId->setText(QString("%1").arg(path->getId()));
    path_nameInput->setText(QString::fromStdString(path->getName()));
    MapPath::Map_Path_Type type = path->getPathType();
    path_comboxType->setCurrentIndex((int)type);
//    int direction = path->getDirection();
//    path_comboxDirection->setCurrentIndex(direction);
    path_xP1Input->setText(QString("%1").arg(path->getP1x()));
    path_yP1Input->setText(QString("%1").arg(path->getP1y()));
    path_xP2Input->setText(QString("%1").arg(path->getP2x()));
    path_yP2Input->setText(QString("%1").arg(path->getP2y()));
    path_lengthInput->setText(QString("%1").arg(path->getLength()));
    tableWidget_path->update();
    tabwidget->setCurrentIndex(2);
}
void DockProperty::showFloor()
{
    //清空原来的数据
    if(spirit==nullptr)return ;
    MapFloor *floor = static_cast<MapFloor *>(spirit);
    floor_itemValueId->setText(QString("%1").arg(floor->getId()));
    floor_nameInput->setText(QString::fromStdString(floor->getName()));
    tableWidget_floor->update();
    tabwidget->setCurrentIndex(3);
}

void DockProperty::showBkg()
{
    //清空原来的数据
    if(spirit==nullptr)return ;
    MapBackground *bkg = static_cast<MapBackground *>(spirit);
    bkg_itemValueId->setText(QString("%1").arg(bkg->getId()));
    bkg_nameInput->setText(QString::fromStdString(bkg->getName()));
    bkg_xInput->setText(QString("%1").arg(bkg->getX()));
    bkg_yInput->setText(QString("%1").arg(bkg->getY()));
    bkg_widthInput->setText(QString("%1").arg(bkg->getWidth()));
    bkg_heightInput->setText(QString("%1").arg(bkg->getHeight()));
    tableWidget_bkg->update();
    tabwidget->setCurrentIndex(4);
}

void DockProperty::showBlock()
{
    //清空原来的数据
    if(spirit==nullptr)return ;
    MapBlock *block = static_cast<MapBlock *>(spirit);
    block_itemValueId->setText(QString("%1").arg(block->getId()));
    block_nameInput->setText(QString::fromStdString(block->getName()));
    tableWidget_block->update();
    tabwidget->setCurrentIndex(5);
}

void DockProperty::slot_PointNameChanged(QString name)
{
    if(spirit == nullptr)return ;
    spirit->setName(name.toStdString());
    emit sig_propertyChanged(spirit);
}
void DockProperty::slot_PointTypeChanged(int _type)
{
    if(spirit == nullptr)return ;
    MapPoint *p = static_cast<MapPoint *>(spirit);
    p->setPointType((MapPoint::Map_Point_Type)_type);
    emit sig_propertyChanged(spirit);
}
void DockProperty::slot_PointXChanged(QString x)
{
    if(spirit == nullptr)return ;
    (static_cast<MapPoint *>(spirit))->setX(x.toInt());
    emit sig_propertyChanged(spirit);;
}
void DockProperty::slot_PointYChanged(QString y)
{
    if(spirit == nullptr)return ;
    (static_cast<MapPoint *>(spirit))->setY(y.toInt());
    emit sig_propertyChanged(spirit);
}
void DockProperty::slot_PointRealXChanged(QString realx)
{
    if(spirit == nullptr)return ;
    (static_cast<MapPoint *>(spirit))->setRealX(realx.toInt());
    emit sig_propertyChanged(spirit);
}
void DockProperty::slot_PointRealYChanged(QString realy)
{
    if(spirit == nullptr)return ;
    (static_cast<MapPoint *>(spirit))->setRealY(realy.toInt());
    emit sig_propertyChanged(spirit);
}
void DockProperty::slot_PointLabelXoffsetChanged(QString labelXoffset)
{
    if(spirit == nullptr)return ;
    (static_cast<MapPoint *>(spirit))->setLabelXoffset(labelXoffset.toInt());
    emit sig_propertyChanged(spirit);
}
void DockProperty::slot_PointLabelYoffsetChanged(QString labelYoffset)
{
    if(spirit == nullptr)return ;
    (static_cast<MapPoint *>(spirit))->setLabelYoffset(labelYoffset.toInt());
    emit sig_propertyChanged(spirit);
}
void DockProperty::slot_PointMapChangeChanged(int mapchange)
{
    if(spirit == nullptr)return ;
    (static_cast<MapPoint *>(spirit))->setMapChange(mapchange == 1);
    emit sig_propertyChanged(spirit);
}

void DockProperty::slot_PathNameChanged(QString name)
{
    if(spirit == nullptr)return ;
    (static_cast<MapPath *>(spirit))->setName(name.toStdString());
    emit sig_propertyChanged(spirit);
}

//void DockProperty::slot_PathDirectionChanged(int _directino)
//{
//    if(spirit == nullptr)return ;
//    (static_cast<MapPath *>(spirit))->setDirection(_directino);
//    emit sig_propertyChanged(spirit);
//}

void DockProperty::slot_PathTypeChanged(int _type)
{
    if(spirit == nullptr)return ;
    (static_cast<MapPath *>(spirit))->setPathType((MapPath::Map_Path_Type)_type);
    emit sig_propertyChanged(spirit);
}
void DockProperty::slot_PathP1XChanged(QString p1x)
{
    if(spirit == nullptr)return ;
    (static_cast<MapPath *>(spirit))->setP1x(p1x.toInt());
    emit sig_propertyChanged(spirit);
}
void DockProperty::slot_PathP1YChanged(QString p1y)
{
    if(spirit == nullptr)return ;
    (static_cast<MapPath *>(spirit))->setP1y(p1y.toInt());
    emit sig_propertyChanged(spirit);
}
void DockProperty::slot_PathP2XChanged(QString p2x)
{
    if(spirit == nullptr)return ;
    (static_cast<MapPath *>(spirit))->setP2x(p2x.toInt());
    emit sig_propertyChanged(spirit);
}
void DockProperty::slot_PathP2YChanged(QString p2y)
{
    if(spirit == nullptr)return ;
    (static_cast<MapPath *>(spirit))->setP2y(p2y.toInt());
    emit sig_propertyChanged(spirit);
}
void DockProperty::slot_LengthChanged(QString length)
{
    if(spirit == nullptr)return ;
    (static_cast<MapPath *>(spirit))->setLength(length.toInt());
    emit sig_propertyChanged(spirit);
}

void DockProperty::slot_FloorNameChanged(QString name)
{
    if(spirit == nullptr)return ;
    (static_cast<MapFloor *>(spirit))->setName(name.toStdString());
    emit sig_propertyChanged(spirit);
}

void DockProperty::slot_BkgNameChanged(QString name)
{
    if(spirit == nullptr)return ;
    spirit->setName(name.toStdString());
    emit sig_propertyChanged(spirit);
}
void DockProperty::slot_BkgXChanged(QString x)
{
    if(spirit == nullptr)return ;
    (static_cast<MapBackground *>(spirit))->setX(x.toInt());
    emit sig_propertyChanged(spirit);
}
void DockProperty::slot_BkgYChanged(QString y)
{
    if(spirit == nullptr)return ;
    (static_cast<MapBackground *>(spirit))->setY(y.toInt());
    emit sig_propertyChanged(spirit);
}
void DockProperty::slot_BkgWidthChanged(QString w)
{
    if(spirit == nullptr)return ;
    (static_cast<MapBackground *>(spirit))->setWidth(w.toInt());
    emit sig_propertyChanged(spirit);
}
void DockProperty::slot_BkgHeightChanged(QString h)
{
    if(spirit == nullptr)return ;
    (static_cast<MapBackground *>(spirit))->setHeight(h.toInt());
    emit sig_propertyChanged(spirit);
}


void DockProperty::slot_BlockNameChanged(QString name)
{
    if(spirit == nullptr)return ;
    (static_cast<MapBlock *>(spirit))->setName(name.toStdString());
    emit sig_propertyChanged(spirit);
}

