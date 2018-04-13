#include "global.h"
#include <QTime>
#include <QCoreApplication>

ThreadPool thread_pool(20);

void QyhSleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);

    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
}
