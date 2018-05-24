#ifndef DOCKTASK_H
#define DOCKTASK_H

#include <QDockWidget>
#include <QtWidgets>
#include "../widgetwaiting.h"
#include "../../protocol.h"
#include "../../msgcenter.h"

class DockTask : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockTask(QWidget *parent = nullptr);

signals:

public slots:
    void onVisibilityChanged(bool v);
    void updateTable();
    void addTask();
    void cancelTask();
//    void deleteSuccess();
//    void modifySuccess();
private:
    //显示载入中
    void showWaiting();
    //显示载入后的内容
    void showContent();

    QStackedWidget *stack;

    WidgetWaiting *waiting;
    QWidget *content;

    QTableWidget *tableWidget;

    //QList<USER_INFO> userinfos;
};

#endif // DOCKTASK_H
