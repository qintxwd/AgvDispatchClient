#ifndef MSGCENTER_H
#define MSGCENTER_H

#include <QObject>
#include <QMap>
#include <QString>
#include "common.h"
#include "protocol.h"
#include "serverconnection.h"
#include  "data/onemap.h"

typedef struct _USER_INFO
{
    int id;//id号
    int role;//角色
    QString username;//用户名
    QString password;//密码
    int status;//登录状态
}USER_INFO;

//AGV基本信息
typedef struct _AGV_BASE_INFO
{
    int id;
    QString name;
    QString ip;
    int port;
}AGV_BASE_INFO;

//AGV位置信息
typedef struct _AGV_POSITION_INFO
{
    int id;
    int x;
    int y;
    int rotation;
}AGV_POSITION_INFO;

//nodes
struct TaskNode{
    int stationid;
    int dowhat;
    QStringList params;
};


typedef struct _TASK_INFO
{
    int id;
    int excuteAgv;
    int priority;
    int status;
    QString produceTime;
    QString doTime;
    QString doneTime;
    QString cancelTime;
    int doingIndex;
    QString errorTime;
    QString errorCode;
    QString errorInfo;
    bool isCancel;
    QMap<QString,QString> extraParams;
    QList<TaskNode> nodes;
}TASK_INFO;


class MsgCenter : public QObject
{
    Q_OBJECT
public:
    explicit MsgCenter(QObject *parent = nullptr);

    ~MsgCenter();

    void init();

    void resetIpPort(QString ip,int port);

    QString getServerIp();
    int getServerPort();

    ///////////////////////////////////用户管理
    //登录调用
    void login(QString username,QString password);
    //注销调用
    void logout();
    //修改密码
    void changePassword(QString new_password);
    //用户列表调用(发送获取userList的请求)
    void userList();
    QList<USER_INFO> getUserListModel(){return userinfos;}
    //删除用户
    void deleteUser(int id);
    //添加用户
    void adduser(QString username, QString password, int role);
    //修改用户
    void modifyuser(int id, QString username, QString password, int role);

    ////////////////////////////////agv管理
    void agvList();

    QList<AGV_BASE_INFO> getAgvListModel(){return agvbaseinfos;}

    void deleteAgv(int id);

    void addagv(QString name, QString ip, int port, int station);

    void modifyagv(int id,QString name,QString ip,int port);

    void subAgvPosition();
    void cancelSubAgvPosition();

    //查询历史日志
    void queryLog(QString from,QString to,bool trace = true,bool debug = true,bool info = true,bool warn = true,bool error = true,bool fatal = true);

    ////////////////////////////////////地图
    //载入地图信息
    void mapSave(OneMap *onemap);
    void mapLoad();

    ////////////////////////////////////日志
    void subUserLog();
    void cancelSubUserLog();

    QList<USER_LOG> getLogListModel(){return loginfos;}

    /////////////////////////任务部分
    QList<TASK_INFO> getTaskInfoModel(){return agvtaskinfos;}
    void addTask(int priority,int agv,QMap<QString,QString> params,QList<TaskNode> nodes);
    void cancelTask(int taskId);
    bool getIsMapLoaded(){return isMapLoaded;}

    void subTask();
    void cancelSubTask();

    void subAgvStatus();
    void cancelSubAgvStatus();
signals:
    //连接状态改变
    void sig_connection_connected();
    void sig_connection_disconnected();
    void sig_connection_conntectting();

    //全局的 发送请求失败
    void sendRequestFail();//发送请求失败
    //全局的 等待返回超时
    void waitResponseTimeOut();//等待返回超时
    //全局的 错误提示信息
    void tip(QString tipstr);//全局的提示信息
    //全局的 返回错误显示
    void err(int errcode,QString info);
    //发送了新的请求，清空上次请求的错误信息等
    void sendNewRequest();

    ////////////////////////用户管理
    void loginSuccess(int role);
    //请求列表成功
    void listUserSuccess();
    //删除成功
    void deleteUserSuccess();
    //添加成功
    void addUserSuccess();
    //修改成功
    void modifyUserSuccess();
    //修改密码成功
    void changePasswrodSuccess();

    ////////////////////////地图管理
    void mapSetSuccess();
    void mapGetSuccess();

    ////////////////////////Agv管理
    //请求列表成功
    void listAgvSuccess();
    //删除成功
    void deleteAgvSuccess();
    //添加成功
    void addAgvSuccess();
    //修改成功
    void modifyAgvSuccess();

    ////////////////////////日志查询
    void queryLogSuccess();
    void pubOneLog(USER_LOG log);

    //////////////////////////任务
    void taskDetailSuccess();
    void onSubTask();

    //添加任务成功
    void addTaskSuccess();

    //取消任务成功
    void cancelTaskSuccess();

    //获取到一个agv位置信息
    void sig_pub_agv_position(int id,QString name,double x,double y,double theta);

    //订阅成功
    void subUserLogSuccess();
    void cancelSubUserLogSuccess();
    void subTaskSuccess();

    //取消任务订阅状态成功
    void cancelSubTaskSuccess();
    void subAgvPositionSuccess();
    void cancelSubAgvPositionSuccess();
    void subAgvStatusSuccess();
    void cancelSubStatusSuccess();
public slots:
    void push(Json::Value response);
private:
    //用户部分
    void response_user_login(const Json::Value &response);
    void response_user_logout(const Json::Value &response);
    void response_user_changePassword(const Json::Value &response);
    void response_user_list(const Json::Value &response);
    void response_user_remove(const Json::Value &response);
    void response_user_add(const Json::Value &response);
    void response_user_modify(const Json::Value &response);

    //地图部分
    void response_map_set(const Json::Value &response);
    void response_map_get(const Json::Value &response);

    //AGV 管理
    void response_agv_list(const Json::Value &response);
    void response_agv_add(const Json::Value &response);
    void response_agv_delete(const Json::Value &response);
    void response_agv_modify(const Json::Value &response);

    void pub_agv_position(const Json::Value &response);

    //任务部分
    void response_task_add(const Json::Value &response);
    void response_task_cancel(const Json::Value &response);
    void response_task_sub(const Json::Value &response);
    void response_task_cancel_sub(const Json::Value &response);
    void pub_agv_task(const Json::Value &response);

    //日志部分
    void pub_agv_log(const Json::Value &response);
    void response_subUserLog(const Json::Value &response);
    void response_cancelSubUserLog(const Json::Value &response);

    void response_subAgvPosition(const Json::Value &response);
    void response_subAgvStatus(const Json::Value &response);
    void response_subTask(const Json::Value &response);

    void response_cancelSubAgvPosition(const Json::Value &response);
    void response_cancelSubAgvStatus(const Json::Value &response);
    void response_cancelSubTask(const Json::Value &response);


    void parseOneMsg(const Json::Value &response);

    void iniRequsttMsg(Json::Value &request);

    void requestWaitResponse(const Json::Value &request);

    std::atomic_int queueNumber;

    std::atomic_bool getResponse;

    QList<USER_INFO> userinfos;

    QList<AGV_BASE_INFO> agvbaseinfos;

    QList<AGV_POSITION_INFO> agvpositioninfos;

    QList<USER_LOG> loginfos;

    QList<TASK_INFO> agvtaskinfos;

    ServerConnection tcpClient;

    QQueue<Json::Value> responses;
    QWaitCondition condition;
    QMutex responsesMtx;

    volatile bool quit;

    std::thread thread_msg_process;

    bool isMapLoaded;
};

#endif // MSGCENTER_H
