#include "tasktablewidget.h"
#include <QMouseEvent>
#include "global.h"

/////////任务状态
enum {
    AGV_TASK_STATUS_UNEXIST = -3,//不存在
    AGV_TASK_STATUS_UNEXCUTE = -2,//未执行
    AGV_TASK_STATUS_EXCUTING = -1,//正在执行
    AGV_TASK_STATUS_DONE = 0,//完成
    AGV_TASK_STATUS_FAIL = 1,//失败
    AGV_TASK_STATUS_CANCEL = 2//取消
};

///////////任务优先级
enum {
    PRIORITY_VERY_LOW = 0,//最低的优先级
    PRIORITY_LOW = 1,//低优先级
    PRIORITY_NORMAL = 2,//普通优先级
    PRIORITY_HIGH = 3,//高优先级
    PRIORITY_VERY_HIGH = 4,//最高优先级
    PRIORITY_LENGTH = 5,
};

TaskTableWidget::TaskTableWidget(QWidget *parent) : QTableWidget(0,6,parent)
{
    tipWidget = new QWidget;

    //tip widget 固定内容:
    //    ID/优先级/agv/producetime/cancelTime/doTime/DoingInde/doneTime/errorCode/errorInfo/errorTime/isCancel/status
    //            node列表
    tipWidget->setMinimumSize(50,50);
    tipWidget->hide();
    timer = new QTimer;
    timer->setInterval(1500);

    QStringList labels;
    labels << QStringLiteral("ID")
           << QStringLiteral("AGV")
           << QStringLiteral("Priority")
           << QStringLiteral("status")
           << QStringLiteral("produce time")
           << QStringLiteral("task describe");

    setHorizontalHeaderLabels(labels);
    verticalHeader()->hide();
    setShowGrid(false);
    setContextMenuPolicy(Qt::CustomContextMenu);
//    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //x先自适应宽度
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
    //TODO  lastRow
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

        QString priorityStr = "";

        if(u.priority == PRIORITY_VERY_LOW){
            priorityStr = QStringLiteral("很低");
        }else if(u.priority == PRIORITY_LOW){
            priorityStr = QStringLiteral("低");
        }else if(u.priority == PRIORITY_NORMAL){
            priorityStr = QStringLiteral("一般");
        }else if(u.priority == PRIORITY_HIGH){
            priorityStr = QStringLiteral("高");
        }else if(u.priority == PRIORITY_VERY_HIGH){
            priorityStr = QStringLiteral("很高");
        }

        QTableWidgetItem *itemPriority = new QTableWidgetItem(tr("%1").arg(priorityStr));
        itemPriority->setTextAlignment(Qt::AlignCenter);
        setItem(i, 2, itemPriority);

        QString statusStr = "";
        if(u.status == AGV_TASK_STATUS_UNEXCUTE){
            statusStr = QStringLiteral("未执行");
        }else if(u.status == AGV_TASK_STATUS_EXCUTING){
            statusStr = QStringLiteral("正在执行");
        }else if(u.status == AGV_TASK_STATUS_DONE){
            statusStr = QStringLiteral("已完成");
        }else if(u.status == AGV_TASK_STATUS_FAIL){
            statusStr = QStringLiteral("失败");
        }else if(u.status == AGV_TASK_STATUS_CANCEL){
            statusStr = QStringLiteral("已取消");
        }

        QTableWidgetItem *itemStatus = new QTableWidgetItem(tr("%1").arg(statusStr));
        itemStatus->setTextAlignment(Qt::AlignCenter);
        setItem(i, 3, itemStatus);

        QTableWidgetItem *itemProduceTime = new QTableWidgetItem(tr("%1").arg(u.produceTime));
        itemProduceTime->setTextAlignment(Qt::AlignCenter);
        setItem(i, 4, itemProduceTime);

        QTableWidgetItem *itemDescribe = new QTableWidgetItem(tr("%1").arg(u.describe));
        itemProduceTime->setTextAlignment(Qt::AlignCenter);
        setItem(i, 5, itemDescribe);
    }

}

int TaskTableWidget::currentSelectTaskId()
{
    if(currentRow()>=0)
        return taskinfos[currentRow()].id;
    return -1;
}
