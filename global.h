#ifndef GLOBAL_H
#define GLOBAL_H

//全局变量

#include "threadpool.h"

extern ThreadPool thread_pool;

//全局函数

//非阻塞的sleep[只阻塞当前线程] 最小单位是10ms
void QyhSleep(int msec);

#endif // GLOBAL_H
