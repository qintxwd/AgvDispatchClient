#ifndef MSGCENTER_H
#define MSGCENTER_H

#include <QObject>
#include "common.h"
#include "protocol.h"
#include "dispatchtcpclient.h"

class MsgCenter : public QObject
{
    Q_OBJECT
public:
    explicit MsgCenter(QObject *parent = nullptr);

    void init();

    //和TCP连接相关函数
    void push(const Client_Response_Msg msg);
    void onConnected();
    void onDisconnected();
    void onConnectting();

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
    void deleteUser(int id);
    //添加用户
    void adduser(QString username, QString password, int role);
    //修改用户
    void modifyuser(int id, QString username, QString password, int role);

    ////////////////////////////////agv管理
    //用户列表调用(发送获取userList的请求)
    void agvList();
    //服务器的列表已经返回后，qml调用该函数获取model
    QList<AgvInfo> getAgvListModel(){return agvinfos;}
    //删除用户
    void deleteAgv(int id);
    //添加用户
    void addagv(QString name,QString ip);
    //修改用户
    void modifyagv(int id,QString name,QString ip);

    //查询历史日志
    void queryLog(QString from,QString to,bool trace = true,bool debug = true,bool info = true,bool warn = true,bool error = true,bool fatal = true);

    ////////////////////////////////////地图
    //载入地图信息
    void loadMap();

    void mapStationList();

    void mapLinesList();

    void loadMapImg();

    QList<AGV_LOG> getLogListModel(){return loginfos;}
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

    void parseOneMsg(const Client_Response_Msg msg);

    //用户部分
    void response_user_login(const Client_Response_Msg msg);
    void response_user_logout(const Client_Response_Msg msg);
    void response_user_changePassword(const Client_Response_Msg msg);
    void response_user_list(const Client_Response_Msg msg);
    void response_user_remove(const Client_Response_Msg msg);
    void response_user_add(const Client_Response_Msg msg);
    void response_user_modify(const Client_Response_Msg msg);

    //地图部分
    void response_map_create_start(const Client_Response_Msg msg);
    void response_map_create_add_line(const Client_Response_Msg msg);
    void response_map_create_add_station(const Client_Response_Msg msg);
    void response_map_create_add_arc(const Client_Response_Msg msg);
    void response_map_create_finish(const Client_Response_Msg msg);
    void response_map_list_station(const Client_Response_Msg msg);
    void response_map_list_line(const Client_Response_Msg msg);
    void response_map_list_arc(const Client_Response_Msg msg);

    //AGV 管理
    void response_agv_list(const Client_Response_Msg msg);
    void response_agv_add(const Client_Response_Msg msg);
    void response_agv_delete(const Client_Response_Msg msg);
    void response_agv_modify(const Client_Response_Msg msg);

private:
    void iniRequsttMsg(Client_Request_Msg &msg);

    void requestWaitResponse(const Client_Request_Msg &msg);

    std::atomic_int queueNumber;

    std::atomic_bool getResponse;

    QList<USER_INFO> userinfos;

    QList<AGV_BASE_INFO> agvinfos;

    QList<AGV_POSITION_INFO> agvinfos;

    QList<STATION_INFO> mapStations;

    QList<AGV_LINE> mapLines;

    QList<AGV_ARC> mapArcs;

    QList<AGV_LOG> loginfos;

    QList<TASK_INFO> agvtasknodes;

    //AgvTaskBriefInfo detailTaskinfo;

    USER_INFO current_user_info;

    DispatchTcpClient tcpClient;

    std::queue<Client_Response_Msg> responses;
    std::mutex responsesMtx;

};

#endif // MSGCENTER_H
