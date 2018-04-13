#include "logindialog.h"
#include "global.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationDisplayName("HRG Dispatch");

    LoginDialog dialog;
    return dialog.exec();
}
