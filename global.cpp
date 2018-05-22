#include "global.h"
#include <QTime>
#include <QCoreApplication>

QString g_strExeRoot;
Configure configure;
MsgCenter msgCenter;
USER_INFO current_user_info;
OneMap g_onemap;
QMutex g_onemap_mtx;

void QyhSleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);

    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
}

QString getErrorString(int error_code)
{
    QString ss = QStringLiteral("");;
    switch (error_code) {
    case RETURN_MSG_ERROR_CODE_UNKNOW:
        ss = QStringLiteral("未知错误");
        break;
//    case RETURN_MSG_ERROR_CODE_LENGTH:
//        ss = QStringLiteral("请求数据长度不正确");
//        break;
    case RETURN_MSG_ERROR_CODE_PERMISSION_DENIED:
        ss = QStringLiteral("无权限执行该操作");
        break;
    case RETURN_MSG_ERROR_CODE_USERNAME_NOT_EXIST:
        ss = QStringLiteral("用户名不存在");
        break;
    case RETURN_MSG_ERROR_CODE_PASSWORD_ERROR:
        ss = QStringLiteral("密码错误");
        break;
    case RETURN_MSG_ERROR_CODE_NOT_LOGIN:
        ss = QStringLiteral("尚未登录");
        break;
    case RETURN_MSG_ERROR_CODE_QUERY_SQL_FAIL:
        ss = QStringLiteral("查询数据库失败");
        break;
    case RETURN_MSG_ERROR_CODE_SAVE_SQL_FAIL:
        ss = QStringLiteral("保存数据库失败");
        break;
    case RETURN_MSG_ERROR_CODE_TASKING:
        ss = QStringLiteral("有任务正在执行");
        break;
    case RETURN_MSG_ERROR_CODE_NOT_CTREATING:
        ss = QStringLiteral("尚未开始创建地图");
        break;
    case RETURN_MSG_ERROR_CODE_CTREATING:
        ss = QStringLiteral("正在创建地图");
        break;
    case RETURN_MSG_ERROR_NO_ERROR:
    default:
        break;
    }
    return ss;
}
