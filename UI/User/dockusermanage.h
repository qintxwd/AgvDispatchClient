#ifndef DOCKUSERMANAGE_H
#define DOCKUSERMANAGE_H

#include <QtWidgets>
#include "../widgetwaiting.h"
#include "protocol.h"

class DockUserManage : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockUserManage(QWidget *parent = nullptr);
signals:

public slots:
    void onVisibilityChanged(bool v);
    void updateTable();
    void addUser();
    void deleteUser();
    void modifyUser();
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

    QList<USER_INFO> userinfos;
};

#endif // DOCKUSERMANAGE_H
