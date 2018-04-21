#include "logindialog.h"
#include "global.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qRegisterMetaType<MSG_Request>("MSG_Request");
    qRegisterMetaType<MSG_Response>("MSG_Response");
    //设置应用的窗口名称
    app.setApplicationDisplayName("HRG Dispatch");

    //设置全局字体
    app.setFont(QFont("Microsoft Yahei", 10));

    //设置当前目录为根目录
    g_strExeRoot = QCoreApplication::applicationDirPath();
    QDir::setCurrent(g_strExeRoot);

    //载入配置文件
    configure.load();

    //初始化到服务器的链接
    msgCenter.init();

    LoginDialog dialog;
    return dialog.exec();
}
