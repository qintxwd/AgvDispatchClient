#ifndef VIEWER_H
#define VIEWER_H

#include <QObject>
#include <QGraphicsView>

class QPushButton;
class Scene;
class Viewer : public QGraphicsView
{
    Q_OBJECT
public:
    Viewer(QWidget *parent = nullptr);
    void reload();
public slots:

protected:
    virtual void wheelEvent(QWheelEvent *event);
signals:

public slots:
private slots:
    void on_addBackgroundBtn_clicked();
    void on_saveBtn_clicked();
    void on_loadBtn_clicked();
    void on_addStationBtn_clicked();
    void on_addLineBtn_clicked();
    void on_addArcBtn_clicked();
    void on_deleteBtn_clicked();
    void on_mScene_cancelTool();
    void on_colorBtn_clicked();
    void on_setBtn_clicked();
private:
    QPushButton *addBackgroundBtn;//添加背景
    QPushButton *saveBtn;//保存地图按钮
    QPushButton *loadBtn;//载入现有地图
    QPushButton *addStationBtn;//添加站点
    QPushButton *addLineBtn;//添加直线按钮
    QPushButton *addArcBtn;//弧线
    QPushButton *deleteBtn;//删除按钮
    QPushButton *colorBtn;//颜色按钮
    QPushButton *setBtn;//设置站点名称RFID按钮

    Scene *mScene;
};

#endif // VIEWER_H
