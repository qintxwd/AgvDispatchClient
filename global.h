#ifndef GLOBAL_H
#define GLOBAL_H

//全局变量
#include "commonhead.h"
#include "threadpool.h"
#include "configure.h"
#include "msgcenter.h"

extern ThreadPool g_threadPool;
extern QString g_strExeRoot;
extern Configure configure;
extern MsgCenter msgCenter;
//全局函数

//非阻塞的sleep[只阻塞当前线程] 最小单位是10ms
void QyhSleep(int msec);

#endif // GLOBAL_H
