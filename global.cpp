#include "global.h"
#include <QTime>
#include <QCoreApplication>

ThreadPool g_threadPool(10);
QString g_strExeRoot;
Configure configure;
MsgCenter msgCenter;

void QyhSleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);

    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
}
