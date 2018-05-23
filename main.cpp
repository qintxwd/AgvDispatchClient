#include "logindialog.h"
#include "global.h"
#include "UI/mainwindow.h"
#include <QApplication>

#include "UI/MapEdit/mapeditwindow.h"
#include <QProgressDialog>

#define HRG_DISPATCH_VERSION_STR "1.0.1"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qRegisterMetaType<Json::Value>("Json::Value");
    qRegisterMetaType<USER_LOG>("USER_LOG");
    //设置应用的窗口名称
    app.setApplicationDisplayName("HRG Dispatch client");
    QCoreApplication::setOrganizationName("HRG(Hit Robot Group)");
    QCoreApplication::setApplicationName("HRG Dispatch client");
    QCoreApplication::setApplicationVersion(HRG_DISPATCH_VERSION_STR);

    //设置全局字体
    app.setFont(QFont("Microsoft Yahei", 10));

    //设置当前目录为根目录
    g_strExeRoot = QCoreApplication::applicationDirPath();
    QDir::setCurrent(g_strExeRoot);

    //载入配置文件
    configure.load();

    //初始化到服务器的链接
    msgCenter.init();

    //    MainWindow *mainWindow = new MainWindow;
    //    mainWindow->showMaximized();

    LoginDialog *dialog = new LoginDialog;

    if ( dialog->exec() == QDialog::Accepted ) {

        //载入地图，显示一个3秒的进度条，如果载入成功，显示主窗口。否则提示并退出
        QProgressDialog pd(("load from server"),("cancel"),0,5000);
        pd.setWindowTitle(("loading"));
        pd.show();
        msgCenter.mapLoad();
        for(int i = 0;i < 5000;++i)
        {
            pd.setValue(i);
            QyhSleep(1);
            if(pd.wasCanceled()) break;
        }
        pd.setValue(50000);
        if(pd.wasCanceled())return 0;
        if(msgCenter.getIsMapLoaded()){
            MainWindow *mainWindow = new MainWindow();
            mainWindow->showMaximized();
        }else{
            QMessageBox::warning(nullptr,QStringLiteral("载入失败"),QStringLiteral("载入失败"));
            return 0;
        }
    }else{
        return 0;
    }

    return app.exec();
}
