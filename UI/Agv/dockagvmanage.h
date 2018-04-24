#ifndef DOCKAGVMANAGE_H
#define DOCKAGVMANAGE_H

#include <QtWidgets>
#include "../widgetwaiting.h"
#include "protocol.h"

class DockAgvManage : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockAgvManage(QWidget *parent = nullptr);
    void reshow();

signals:

public slots:
    void onVisibilityChanged(bool v);
    void updateTable();
    void addAgv();
    void deleteAgv();
    void modifyAgv();
    void deleteSuccess();
    void modifySuccess();
private:
    //显示载入中
    void showWaiting();
    //显示载入后的内容
    void showContent();

    QStackedWidget *stack;

    WidgetWaiting *waiting;
    QWidget *content;

    QTableWidget *tableWidget;

    QList<AGV_BASE_INFO> agvinfos;
};

#endif // DOCKAGVMANAGE_H
