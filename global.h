#ifndef GLOBAL_H
#define GLOBAL_H

//全局变量
#include "commonhead.h"
#include "configure.h"
#include "msgcenter.h"
#include "protocol.h"
#include "data/onemap.h"

extern QString g_strExeRoot;
extern Configure configure;
extern MsgCenter msgCenter;
extern USER_INFO current_user_info;
extern OneMap g_onemap;
//全局函数

//非阻塞的sleep[只阻塞当前线程] 最小单位是10ms
void QyhSleep(int msec);

QString getErrorString(int error_code);
#endif // GLOBAL_H
