#ifndef MSGCENTER_H
#define MSGCENTER_H

#include <QObject>
#include "common.h"
#include "protocol.h"
#include "serverconnection.h"

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
    void changePassword(QString old_password,QString new_password);
    //用户列表调用(发送获取userList的请求)
    void userList();
    QList<USER_INFO> getUserListModel(){return userinfos;}
    //删除用户
    void deleteUser(int32_t id);
    //添加用户
    void adduser(QString username, QString password, int32_t role);
    //修改用户
    void modifyuser(int32_t id, QString username, QString password, int32_t role);

    ////////////////////////////////agv管理
    void agvList();

    QList<AGV_BASE_INFO> getAgvListModel(){return agvbaseinfos;}

    void deleteAgv(int id);

    void addagv(QString name,QString ip,int port);

    void modifyagv(int id,QString name,QString ip,int port);

    //查询历史日志
    void queryLog(QString from,QString to,bool trace = true,bool debug = true,bool info = true,bool warn = true,bool error = true,bool fatal = true);

    ////////////////////////////////////地图
    //载入地图信息
    void loadMap();

    void mapStationList();

    void mapLinesList();

    void loadMapImg();

    QList<USER_LOG> getLogListModel(){return loginfos;}
    //获取线路
    QList<AGV_LINE> getMapLineListModel(){return mapLines;}

    //获取站点
    QList<STATION_INFO> getMapStationListModel(){return mapStations;}

    //保存地图
    void saveMap(QList<STATION_INFO> stations,QList<AGV_LINE> lines);

    /////////////////////////任务部分
    void taskAgvGoB(int agvId,int stationId);
    void taskGoB(int stationId);
    void taskAgvPassAGoB(int agvId,int stationAId,int stationBId);
    void taskPassAGoB(int stationAId,int stationBId);
    void taskAgvPassAGoBCircle(int agvId,int stationAId,int stationBId);

    void taskCancel(int taskId);
//    void taskDetail(int taskId);
//    QList<TaskNode> getAgvTaskDetail(){return agvtasknodes;}

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
    void mapCreateStart();
    void mapCreateAddStation();
    void mapCreateAddLine();
    void mapCreateAddArc();
    void mapCreateFinish();
    void mapStationListSuccess();
    void mapLinesListSuccess();
    void mapArcListSuccess();

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

    ////////////////////////地图信息
    void mapStationsSuccess();
    void mapLinesSuccess();
    void changeBkgImg(QString);

    //任务
    void taskDetailSuccess();
public slots:
    void push(const MSG_Response msg);
private:
    //用户部分
    void response_user_login(const MSG_Response msg);
    void response_user_logout(const MSG_Response msg);
    void response_user_changePassword(const MSG_Response msg);
    void response_user_list(const MSG_Response msg);
    void response_user_remove(const MSG_Response msg);
    void response_user_add(const MSG_Response msg);
    void response_user_modify(const MSG_Response msg);

    //地图部分
    void response_map_create_start(const MSG_Response msg);
    void response_map_create_add_line(const MSG_Response msg);
    void response_map_create_add_station(const MSG_Response msg);
    void response_map_create_add_arc(const MSG_Response msg);
    void response_map_create_finish(const MSG_Response msg);
    void response_map_list_station(const MSG_Response msg);
    void response_map_list_line(const MSG_Response msg);
    void response_map_list_arc(const MSG_Response msg);

    //AGV 管理
    void response_agv_list(const MSG_Response msg);
    void response_agv_add(const MSG_Response msg);
    void response_agv_delete(const MSG_Response msg);
    void response_agv_modify(const MSG_Response msg);

    void parseOneMsg(const MSG_Response msg);

    void iniRequsttMsg(MSG_Request &msg);

    void requestWaitResponse(const MSG_Request &msg);

    std::atomic_int queueNumber;

    std::atomic_bool getResponse;

    QList<USER_INFO> userinfos;

    QList<AGV_BASE_INFO> agvbaseinfos;

    QList<AGV_POSITION_INFO> agvpositioninfos;

    QList<STATION_INFO> mapStations;

    QList<AGV_LINE> mapLines;

    QList<USER_LOG> loginfos;

    QList<TASK_INFO> agvtasknodes;

    //AgvTaskBriefInfo detailTaskinfo;

    ServerConnection tcpClient;

    QQueue<MSG_Response> responses;
    QWaitCondition condition;
    QMutex responsesMtx;

    volatile bool quit;

    std::thread thread_msg_process;
};

#endif // MSGCENTER_H
