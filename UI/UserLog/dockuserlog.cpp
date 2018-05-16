#include "dockuserlog.h"
#include "../../global.h"

DockUserLog::DockUserLog(QWidget *parent) : QDockWidget(tr("UserLog"),parent)
{
    setAllowedAreas(Qt::AllDockWidgetAreas);
    listview = new QListView;
    listview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listModel = new QStringListModel;
    listview->setModel(listModel);
    setWidget(listview);
    connect(this,SIGNAL(visibilityChanged(bool)),this,SLOT(onVisibilityChanged(bool)));
    connect(&msgCenter,SIGNAL(pubOneLog(USER_LOG)),this,SLOT(appendLog(USER_LOG)));
}

void DockUserLog::appendLog(USER_LOG log)
{
    qDebug()<<"log==>"<<QString::fromUtf8(log.msg.c_str(),log.msg.length());

    int row = listModel->rowCount();
    if(row<show_max_row){
        listModel->insertRows(row, 1);
        QModelIndex index = listModel->index(row);
        listModel->setData(index, QString("%1\t%2").arg(log.time.c_str()).arg(QString::fromUtf8(log.msg.c_str(),log.msg.length())));
    }else{
        for(int i=0;i<show_cut_row;++i)listModel->removeRow(0);
        row = listModel->rowCount();
        listModel->insertRows(row, 1);
        QModelIndex index = listModel->index(row);
        listModel->setData(index, QString("%1\t%2").arg(log.time.c_str()).arg(QString::fromUtf8(log.msg.c_str(),log.msg.length())));
    }
    listview->scrollToBottom();
}

void DockUserLog::onVisibilityChanged(bool v)
{
    if(v){
        msgCenter.subUserLog();
    }else{
        msgCenter.cancelSubUserLog();
        while(listModel->rowCount()>0)listModel->removeRow(0);
    }
}
