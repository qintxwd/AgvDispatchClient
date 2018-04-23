#ifndef WIDGETUSERMANAGE_H
#define WIDGETUSERMANAGE_H

#include "commonhead.h"
#include "protocol.h"

class WidgetUserManage : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetUserManage(QWidget *parent = nullptr);

signals:
    void readyToShow(int);//已经准备好重新显示ile
    void returnChoose();
public slots:
    void reshow();//准备重新显示，相当于刷新了,也相当于重新显示
    void updateTable();
    void addUser();
    void deleteUser();
    void modifyUser();
    void deleteSuccess();
    void modifySuccess();
private:
    QLabel *labelTitle;
    QPushButton *btnAddUser;
    QPushButton *btnDeleteUser;
    QPushButton *btnModifyUser;
    QTableWidget *tableWidget;

    QList<USER_INFO> userinfos;

    QPushButton *returnBtn;
};

#endif // WIDGETUSERMANAGE_H
