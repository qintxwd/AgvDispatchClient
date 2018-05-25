#include "docktask.h"
#include "../../global.h"
#include "dialogaddtask.h"

DockTask::DockTask(QWidget *parent) : QDockWidget(tr("Task"),parent)
{
    setAllowedAreas(Qt::AllDockWidgetAreas);
    stack = new QStackedWidget;
    waiting = new WidgetWaiting;
    content = new QWidget;


    QPushButton *btnAddTask = new QPushButton(tr("add task"));
    QPushButton *btnCancelTask = new QPushButton(tr("cancel task"));

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addWidget(btnAddTask);
    btnsLayout->addWidget(btnCancelTask);

    tableWidget = new TaskTableWidget();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addItem(btnsLayout);
    layout->addWidget(tableWidget);

    content->setLayout(layout);

    stack->addWidget(waiting);
    stack->addWidget(content);
    stack->setCurrentIndex(1);
    setWidget(stack);


    connect(btnAddTask,SIGNAL(clicked(bool)),this,SLOT(addTask()));
    connect(btnCancelTask,SIGNAL(clicked(bool)),this,SLOT(cancelTask()));
    connect(this,SIGNAL(visibilityChanged(bool)),this,SLOT(onVisibilityChanged(bool)));
}

void DockTask::addTask()
{
    DialogAddTask *dialogAddTask = new DialogAddTask(this);
    if(dialogAddTask->exec() == QDialog::Accepted){
        //show();
    }
}

void DockTask::cancelTask()
{
    if(tableWidget->currentRow()<0){
        QMessageBox::warning(this,QStringLiteral("未选择"),QStringLiteral("请先选择你要取消的任务的行"));
        return ;
    }

    QMessageBox::StandardButton rb = QMessageBox::question(this, QStringLiteral("确认取消"), QStringLiteral("确认取消该任务?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(rb == QMessageBox::Yes)
    {
        msgCenter.cancelTask(tableWidget->currentSelectTaskId());
    }

}

void DockTask::onVisibilityChanged(bool v)
{
    if(v){
        msgCenter.subTask();
    }else{
        msgCenter.cancelSubTask();
    }
}
