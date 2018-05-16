#ifndef DOCKUSERLOG_H
#define DOCKUSERLOG_H

#include <QDockWidget>
#include <QListView>
#include <QStringListModel>
#include "../../protocol.h"

class DockUserLog : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockUserLog(QWidget *parent = nullptr);

signals:

public slots:
    void appendLog(USER_LOG log);
    void onVisibilityChanged(bool v);
private:
    QListView *listview;
    QStringListModel *listModel;

    //最多显示多少行日志
    const int show_max_row = 1000;

    //超出的部分，删除前面多少行的
    const int show_cut_row = 300;
};

#endif // DOCKUSERLOG_H
