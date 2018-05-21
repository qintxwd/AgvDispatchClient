#ifndef DOCKPROPERTY_H
#define DOCKPROPERTY_H

#include <QDockWidget>
#include <QComboBox>
#include <QLabel>
#include <QTableWidget>
#include "../../data/onemap.h"

class DockProperty : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockProperty(OneMap* _oneMap, QWidget *parent = nullptr);
    void initTabTable();
    void initTableContent();
signals:
    void sig_propertyChanged(MapSpirit *spirit);
public slots:
    void slot_shownull();
    void slot_showSpirit(MapSpirit *spirit);
    void slot_propertyChanged(MapSpirit *_spirit);
private slots:
    //point edit
    void slot_PointNameChanged(QString name);
    void slot_PointTypeChanged(int _type);
    void slot_PointXChanged(QString x);
    void slot_PointYChanged(QString y);
    void slot_PointRealXChanged(QString realx);
    void slot_PointRealYChanged(QString realy);
    void slot_PointLabelXoffsetChanged(QString labelXoffset);
    void slot_PointLabelYoffsetChanged(QString labelYoffset);
    void slot_PointMapChangeChanged(int mapchange);

    //path edit
    void slot_PathNameChanged(QString name);
    void slot_PathTypeChanged(int _type);
    //void slot_PathDirectionChanged(int _directino);
    void slot_PathP1XChanged(QString p1x);
    void slot_PathP1YChanged(QString p1y);
    void slot_PathP2XChanged(QString p2x);
    void slot_PathP2YChanged(QString p2y);
    void slot_LengthChanged(QString length);

    //floor edit
    void slot_FloorNameChanged(QString name);

    //bkg edit
    void slot_BkgNameChanged(QString name);
    void slot_BkgXChanged(QString x);
    void slot_BkgYChanged(QString y);
    void slot_BkgWidthChanged(QString w);
    void slot_BkgHeightChanged(QString h);

    //block edit
    void slot_BlockNameChanged(QString name);
private:
    void showPoint();
    void showPath();
    void showFloor();
    void showBkg();
    void showBlock();
    MapSpirit *spirit;
    QTableWidget *tableWidget_blank;//空白
    QTableWidget *tableWidget_point;//站点
    QTableWidget *tableWidget_path;//路径
    QTableWidget *tableWidget_floor;//楼层
    QTableWidget *tableWidget_bkg;//背景图片
    QTableWidget *tableWidget_block;//区块

    QTabWidget *tabwidget;

    OneMap* oneMap;

private:

    //point
    QTableWidgetItem *point_itemKeyId;
    QTableWidgetItem *point_itemValueId;
    QTableWidgetItem *point_itemKeyName;
    QLineEdit *point_nameInput;
    QTableWidgetItem *point_itemKeyType;
    QComboBox *point_comboxType;
    QTableWidgetItem *point_itemKeyX;
    QLineEdit *point_xInput;
    QTableWidgetItem *point_itemKeyY;
    QLineEdit *point_yInput;
    QTableWidgetItem *point_itemKeyRealX;
    QLineEdit *point_xRealInput;
    QTableWidgetItem *point_itemKeyRealY;
    QLineEdit *point_yRealInput;
    QTableWidgetItem *point_itemKeyLabelX;
    QLineEdit *point_xLabelInput;
    QTableWidgetItem *point_itemKeyLabelY;
    QLineEdit *point_yLabelInput;
    QTableWidgetItem *point_itemKeyMapChange;
    QComboBox *point_comboxMapChange;

    //path
    QTableWidgetItem *path_itemKeyId;
    QTableWidgetItem *path_itemValueId;
    QTableWidgetItem *path_itemKeyName;
    QLineEdit *path_nameInput;
    QTableWidgetItem *path_itemKeyType;
    QComboBox *path_comboxType;
//    QTableWidgetItem *path_itemKeyDirection;
//    QComboBox *path_comboxDirection;
    QTableWidgetItem *path_itemKeyP1X;
    QLineEdit *path_xP1Input;
    QTableWidgetItem *path_itemKeyP1Y;
    QLineEdit *path_yP1Input;
    QTableWidgetItem *path_itemKeyP2X;
    QLineEdit *path_xP2Input;
    QTableWidgetItem *path_itemKeyP2Y;
    QLineEdit *path_yP2Input;
    QTableWidgetItem *path_itemKeyLength;
    QLineEdit *path_lengthInput;

    //floor
    QTableWidgetItem *floor_itemKeyId;
    QTableWidgetItem *floor_itemValueId;
    QTableWidgetItem *floor_itemKeyName;
    QLineEdit *floor_nameInput;

    //bkg
    QTableWidgetItem *bkg_itemKeyId;
    QTableWidgetItem *bkg_itemValueId;
    QTableWidgetItem *bkg_itemKeyName;
    QLineEdit *bkg_nameInput;
    QTableWidgetItem *bkg_itemX;
    QLineEdit *bkg_xInput;
    QTableWidgetItem *bkg_itemKeyY;
    QLineEdit *bkg_yInput;
    QTableWidgetItem *bkg_itemWidth;
    QLineEdit *bkg_widthInput;
    QTableWidgetItem *bkg_itemKeyHeight;
    QLineEdit *bkg_heightInput;

    //block
    QTableWidgetItem *block_itemKeyId;
    QTableWidgetItem *block_itemValueId;
    QTableWidgetItem *block_itemKeyName;
    QLineEdit *block_nameInput;

};

#endif // DOCKPROPERTY_H
