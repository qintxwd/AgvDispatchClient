#include "tasktablewidget.h"
#include <QMouseEvent>
#include "global.h"

TaskTableWidget::TaskTableWidget(QWidget *parent) : QTableWidget(0,5,parent)
{
    tipWidget = new QWidget;
    tipWidget->setMinimumSize(50,50);
    timer = new QTimer;
    timer->setInterval(1500);

    QStringList labels;
    labels << QStringLiteral("ID")
           << QStringLiteral("AGV")
           << QStringLiteral("Priority")
           << QStringLiteral("status")
           << QStringLiteral("produce time");

    setHorizontalHeaderLabels(labels);
    verticalHeader()->hide();
    setShowGrid(false);
    setContextMenuPolicy(Qt::CustomContextMenu);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //x先自适应宽度
    setSelectionBehavior(QAbstractItemView::SelectRows);//选中就是选中这一行
    setSelectionMode(QAbstractItemView::SingleSelection);//只能选中一行
    setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑

    connect(timer,SIGNAL(timeout()),this,SLOT(on_timer_out()));
    connect(this,SIGNAL(cellEntered(int,int)),this,SLOT(slot_cellEntered(int,int)));
    connect(&msgCenter,SIGNAL(onSubTask()),this,SLOT(updateTable()));
}

void TaskTableWidget::leaveEvent(QEvent *event)
{
    timer->stop();
    lastRow = -1;
    tipWidget->hide();
    QTableWidget::leaveEvent(event);
}

void TaskTableWidget::mouseMoveEvent(QMouseEvent *event)
{
    tipWidget->move(event->pos());
}

void TaskTableWidget::slot_cellEntered(int row, int column)
{
    if(row!=lastRow){
        timer->stop();
        timer->start();
        tipWidget->hide();
    }
    lastRow = row;
}

void TaskTableWidget::on_timer_out()
{
    tipWidget->show();
}

void TaskTableWidget::updateTable()
{
    QList<TASK_INFO> temp_taskinfos =  msgCenter.getTaskInfoModel();

    for(int i=0;i<taskinfos.length();++i){
        for(int j=0;j<temp_taskinfos.length();++j){
            if(taskinfos[i].id == temp_taskinfos[j].id){
                taskinfos[i] = temp_taskinfos[j];
            }
        }
    }

    for(int i=0;i<temp_taskinfos.length();++i){
        bool alreadyExist = false;
        for(auto t:taskinfos){
            if(t.id == temp_taskinfos[i].id){
                alreadyExist = true;
                break;
            }
        }

        if(!alreadyExist){
            taskinfos.push_back(temp_taskinfos[i]);
        }
    }

    //重新显示
    clearContents();
    while(rowCount()>0){
        removeRow(0);
    }
    for(int i=0;i<taskinfos.length();++i){
        insertRow(i);
        auto u = taskinfos.at(i);
        QTableWidgetItem *itemId = new QTableWidgetItem(tr("%1").arg(u.id));
        itemId->setTextAlignment(Qt::AlignCenter);
        setItem(i, 0, itemId);

        QTableWidgetItem *itemAgv = new QTableWidgetItem(tr("%1").arg(u.excuteAgv));
        itemAgv->setTextAlignment(Qt::AlignCenter);
        setItem(i, 1, itemAgv);

        QTableWidgetItem *itemPriority = new QTableWidgetItem(tr("%1").arg(u.priority));
        itemPriority->setTextAlignment(Qt::AlignCenter);
        setItem(i, 2, itemPriority);

        QTableWidgetItem *itemStatus = new QTableWidgetItem(tr("%1").arg(u.status));
        itemStatus->setTextAlignment(Qt::AlignCenter);
        setItem(i, 3, itemStatus);

        QTableWidgetItem *itemProduceTime = new QTableWidgetItem(tr("%1").arg(u.produceTime));
        itemProduceTime->setTextAlignment(Qt::AlignCenter);
        setItem(i, 4, itemProduceTime);
    }

}

 int TaskTableWidget::currentSelectTaskId()
 {
     if(currentRow()>=0)
        return taskinfos[currentRow()].id;
     return -1;
 }
