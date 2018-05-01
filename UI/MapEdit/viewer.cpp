#include "viewer.h"

#include <QWheelEvent>
#include <QDebug>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QCoreApplication>
#include <QColorDialog>
#include "stationinputdialog.h"
#include "scene.h"
#include "global.h"

Viewer::Viewer(QWidget *parent) :  QGraphicsView(parent)
{
    setBackgroundBrush(QPixmap(":/images/point/grid.png"));
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setMouseTracking(true);	// mouse track on
    setViewportUpdateMode(FullViewportUpdate);

    saveBtn = new QPushButton(QStringLiteral("保存"),this);
    saveBtn->setObjectName("saveBtn");
    loadBtn = new QPushButton(QStringLiteral("载入"),this);
    loadBtn->setObjectName("loadBtn");
    addBackgroundBtn = new QPushButton(QStringLiteral("添加背景"),this);
    addBackgroundBtn->setObjectName("addBackgroundBtn");
    addStationBtn = new QPushButton(QStringLiteral("添加站点"),this);
    addStationBtn->setObjectName("addStationBtn");
    addStationBtn->setCheckable(true);
    addLineBtn = new QPushButton(QStringLiteral("添加直线"),this);
    addLineBtn->setObjectName("addLineBtn");
    addLineBtn->setCheckable(true);
    addArcBtn = new QPushButton(QStringLiteral("添加弧线"),this);
    addArcBtn->setObjectName("addArcBtn");
    addArcBtn->setCheckable(true);
    deleteBtn = new QPushButton(QStringLiteral("删除"),this);
    deleteBtn->setObjectName("deleteBtn");
    deleteBtn->setCheckable(true);
    colorBtn = new QPushButton(QStringLiteral("设置颜色"),this);
    colorBtn->setObjectName("colorBtn");
    setBtn = new QPushButton(QStringLiteral("设置站点名称/RFID"),this);
    setBtn->setObjectName("setBtn");



    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addStretch(1);
    hlayout->addWidget(saveBtn);
    hlayout->addWidget(loadBtn);
    hlayout->addWidget(addBackgroundBtn);
    hlayout->addWidget(addStationBtn);
    hlayout->addWidget(addLineBtn);
    hlayout->addWidget(addArcBtn);
    hlayout->addWidget(deleteBtn);
    hlayout->addWidget(colorBtn);
    hlayout->addWidget(setBtn);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addItem(hlayout);
    vlayout->addStretch(1);
    setLayout(vlayout);

    mScene = new Scene(this);
    mScene->setObjectName("mScene");
    setScene(mScene);

    connect(&msgCenter,SIGNAL(changeBkgImg(QString)),mScene,SLOT(setBackgroundImagePath(QString)));

    QMetaObject::connectSlotsByName(this);
}

void Viewer::wheelEvent(QWheelEvent *event)
{
    static int kk = 0;
    double scaleFactor = 1.10;
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->delta() > 0)
        {
            if(kk<10){
                ++kk;
                scale(scaleFactor, scaleFactor);
            }
        }
        else
        {
            if(kk>-10){
                --kk;
                scale(1.0 / scaleFactor, 1.0 / scaleFactor);
            }
        }
        event->accept();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void Viewer::on_addBackgroundBtn_clicked()
{
    addStationBtn->setChecked(false);
    addLineBtn->setChecked(false);
    addArcBtn->setChecked(false);
    deleteBtn->setChecked(false);
    QString path = QFileDialog::getOpenFileName(this, QStringLiteral("Open Image"), ".", tr("Image Files(*.jpg *.png)"));
    if(path.length() != 0) {
        mScene->setBackgroundImagePath(path);
    }
}

void Viewer::on_saveBtn_clicked()
{
    addStationBtn->setChecked(false);
    addLineBtn->setChecked(false);
    addArcBtn->setChecked(false);
    deleteBtn->setChecked(false);
    //弹出等待提示
    //保存所有的站点、线路//保存背景图片
    QList<AgvLine> lines;
    QList<AgvLine> arcs;
    QList<AgvStation> stations;
    QString bkgImgPath = mScene->backgroundImagePath;

    for(QList<ItemStation *>::iterator  itr = mScene->iStations.begin();itr != mScene->iStations.end();++itr)
    {
        ItemStation *s = *itr;
        stations.push_back(s->station);
    }

    for(QList<ItemLine *>::iterator itr = mScene->iLines.begin();itr != mScene->iLines.end();++itr)
    {
        ItemLine *l = *itr;
        AgvLine line;
        line.id = l->id;
        line.startStation = l->startStation->station.id;
        line.endStation = l->endStation->station.id;
        line.color_r = l->color.red();
        line.color_g = l->color.green();
        line.color_b = l->color.blue();

        line.draw = true;
        line.line = true;


        lines.push_back(line);
    }

    for(QList<ItemArc *>::iterator itr = mScene->iArcs.begin();itr != mScene->iArcs.end();++itr)
    {
        ItemArc *l = *itr;
        AgvLine line;
        line.id = l->id;
        line.startStation = l->startStation->station.id;
        line.endStation = l->endStation->station.id;
        line.color_r = l->color.red();
        line.color_g = l->color.green();
        line.color_b = l->color.blue();

        line.draw = true;
        line.line = false;

        line.p1x = l->P2.x();
        line.p1y = l->P2.y();
        line.p2x = l->P3.x();
        line.p2y = l->P3.y();

        arcs.push_back(line);
    }

    DialogWaiting *dialog = new DialogWaiting(this);
    dialog->showMaximized();

    msgCenter.saveMap(stations,lines,arcs,bkgImgPath);

    dialog->close();
}
void Viewer::on_loadBtn_clicked()
{
    addStationBtn->setChecked(false);
    addLineBtn->setChecked(false);
    addArcBtn->setChecked(false);
    deleteBtn->setChecked(false);
    reload();
}
void Viewer::on_addStationBtn_clicked()
{
    addLineBtn->setChecked(false);
    addArcBtn->setChecked(false);
    deleteBtn->setChecked(false);

    if(addStationBtn->isChecked()){
        mScene->setCurTool(Scene::T_STATION);
    }else{
        mScene->setCurTool(Scene::T_NONE);
    }
}
void Viewer::on_addLineBtn_clicked()
{
    addStationBtn->setChecked(false);
    addArcBtn->setChecked(false);
    deleteBtn->setChecked(false);
    if(addLineBtn->isChecked()){
        mScene->setCurTool(Scene::T_LINE);
    }else{
        mScene->setCurTool(Scene::T_NONE);
    }

}
void Viewer::on_addArcBtn_clicked()
{
    addStationBtn->setChecked(false);
    addLineBtn->setChecked(false);
    //addArcBtn->setChecked(false);
    deleteBtn->setChecked(false);

    if(addArcBtn->isChecked()){
        mScene->setCurTool(Scene::T_ARC);
    }else{
        mScene->setCurTool(Scene::T_NONE);
    }

}
void Viewer::on_deleteBtn_clicked()
{
    addStationBtn->setChecked(false);
    addLineBtn->setChecked(false);
    addArcBtn->setChecked(false);
    //deleteBtn->setChecked(false);

    if(deleteBtn->isChecked()){
        mScene->setCurTool(Scene::T_ERASER);
    }else{
        mScene->setCurTool(Scene::T_NONE);
    }
}

void Viewer::on_mScene_cancelTool()
{
    addStationBtn->setChecked(false);
    addLineBtn->setChecked(false);
    addArcBtn->setChecked(false);
    deleteBtn->setChecked(false);
}

void Viewer::on_colorBtn_clicked()
{
    if(mScene->selectedItems().length()<=0){
        QMessageBox::warning(NULL,QStringLiteral(""),QStringLiteral("请先选择你要修改颜色的线路或者站点"),QMessageBox::Ok);
        return ;
    }

    QColorDialog *colorChoose = new QColorDialog;
    if( QDialog::Accepted == colorChoose->exec())
    {
        QColor color = colorChoose->selectedColor();

        QList<QGraphicsItem *> items = mScene->selectedItems();
        for(int i=0;i<items.length();++i){
            QGraphicsItem *item = items.at(i);
            if(item->type() == ItemArc::Type){
                ItemArc *newArc = qgraphicsitem_cast<ItemArc*>(item);
                newArc->setColor(color);
            }
            else if(item->type() == ItemLine::Type){
                ItemLine *newLine = qgraphicsitem_cast<ItemLine*>(item);
                newLine->updateColor(color);
            }

            if(item->type() == ItemStation::Type)
            {
                ItemStation *newStation = qgraphicsitem_cast<ItemStation*>(item);
                newStation->station.color_r = color.red();
                newStation->station.color_g = color.green();
                newStation->station.color_b = color.blue();
                newStation->update();
            }
        }
    }
}

void Viewer::on_setBtn_clicked()
{
    if(mScene->selectedItems().length()<=0 || mScene->selectedItems().at(0)->type()!= ItemStation::Type)
    {
        QMessageBox::warning(NULL,QStringLiteral(""),QStringLiteral("请先选择你要修改的站点"),QMessageBox::Ok);
        return ;
    }
    if(mScene->selectedItems().at(0)->type() == ItemStation::Type)
    {
        ItemStation *newStation = qgraphicsitem_cast<ItemStation*>(mScene->selectedItems().at(0));
        //弹出对话框，设置name和RFID
        StationInputDialog *inputdlg = new StationInputDialog(newStation->station.name,newStation->station.rfid,this);
        if(inputdlg->exec() == QDialog::Accepted)
        {
            newStation->station.name = inputdlg->getName();
            newStation->station.rfid = inputdlg->getRfid();
            newStation->update();
        }
    }
}

void Viewer::reload()
{
    mScene->load();
}
