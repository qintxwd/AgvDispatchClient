#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "commonhead.h"

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    LoginDialog();

signals:

public slots:
    void onConnected();
    void onDisconnected();
    void onConnecting();
    void onTip(QString tip);
private slots:
    void on_nameInput_textChanged();
    void on_pwdInput_textChanged();
    void on_configBtn_clicked();
    void on_loginBtn_clicked();
    void on_okBtn_clicked();
    void on_cancelBtn_clicked();
private:
    void initUi();


    QStackedWidget *pagesWidget;//两个界面的切换

    //登录界面
    QWidget *loginWidget;
    QLabel *nameInputLabel;
    QLineEdit *nameInput;
    QLabel *passwordInputLabel;
    QLineEdit *pwdInput;
    QLabel *tipLabel;//提示语
    QPushButton *configBtn;
    QPushButton *loginBtn;

    //配置界面
    QWidget *configServerWidget;
    QLabel *ipInputLabel;
    QLineEdit *ipInput;
    QLabel *portInputLabel;
    QLineEdit *portInput;

    QPushButton *okBtn;
    QPushButton *cancelBtn;
};

#endif // LOGINDIALOG_H
