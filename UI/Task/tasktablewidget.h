#ifndef TASKTABLEWIDGET_H
#define TASKTABLEWIDGET_H

#include <QTableWidget>
#include <QTimer>
#include "msgcenter.h"

class TaskTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit TaskTableWidget(QWidget *parent = nullptr);

    int currentSelectTaskId();
signals:
    //void show_tip_row(int row);
    //void show_tip_close();
public slots:
    void slot_cellEntered(int row, int column);
    void on_timer_out();
    void updateTable();
protected:
    virtual void leaveEvent(QEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
private:
    QTimer *timer;
    int lastRow;
    QWidget *tipWidget;
    QList<TASK_INFO> taskinfos;
};

#endif // TASKTABLEWIDGET_H
