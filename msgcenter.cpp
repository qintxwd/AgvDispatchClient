#include "msgcenter.h"

#include "global.h"

MsgCenter::MsgCenter(QObject *parent) : QObject(parent)
{

}

void MsgCenter::init()
{
    DispatchTcpClient::QyhClientReadCallback _readcallback = std::bind(&MsgCenter::push,this,std::placeholders::_1);
    DispatchTcpClient::QyhClientConnectCallback _connectcallback = std::bind(&MsgCenter::onConnected,this);
    DispatchTcpClient::QyhClientDisconnectCallback _disconnectcallback = std::bind(&MsgCenter::onDisconnected,this);
    DispatchTcpClient::QyhClientConnecttingCallback _connectingcallback = std::bind(&MsgCenter::onConnectting,this);

    tcpClient.init(configure.getValue("server/ip").toString().toStdString(),configure.getValue("server/port").toInt(),_readcallback,_connectcallback,_disconnectcallback,_connectingcallback);

    //响应消息处理的线程
    thread_pool.enqueue([&](){
        while(true){
            responsesMtx.lock();
            if(responses.size()<=0){
                responsesMtx.unlock();
                std::chrono::milliseconds dura(10);
                std::this_thread::sleep_for(dura);
                continue;
            }
            Client_Response_Msg msg = responses.front();
            responses.pop();
            responsesMtx.unlock();
            parseOneMsg(msg);
        }

    });
}

//入队响应消息
void MsgCenter::push(const Client_Response_Msg msg)
{
    responsesMtx.lock();
    responses.push(msg);
    responsesMtx.unlock();
}

void MsgCenter::onConnected()
{
    emit sig_connection_connected();
}
void MsgCenter::onDisconnected()
{
    emit sig_connection_disconnected();
}
void MsgCenter::onConnectting()
{
    emit sig_connection_conntectting();
}

void MsgCenter::requestWaitResponse(const Client_Request_Msg & msg)
{
    if(!tcpClient->send(msg)){
        emit sendRequestFail();
        return ;
    }
    int kk = 100;
    while(!getResponse&&--kk>0)
    {
        QyhSleep(10);
    }
    if(kk<=0)
        emit waitResponseTimeOut();

}

void MsgCenter::iniRequsttMsg(Client_Request_Msg &msg)
{
    memset(&msg,0,sizeof(Client_Request_Msg));
    msg.head.head = CLIENT_COMMON_HEAD_HEAD;
    msg.head.tail = CLIENT_COMMON_HEAD_TAIL;
    msg.head.queuenumber = ++queueNumber;
}

void MsgCenter::parseOneMsg(const Client_Response_Msg msg)
{
    //判断消息格式是否正确
    if(msg.head.head != CLIENT_COMMON_HEAD_HEAD || msg.head.tail!=CLIENT_COMMON_HEAD_TAIL)return ;

    //是否和发送请求的queuenumber序号相同，相同标记 得到响应
    if(msg.head.queuenumber == queueNumber)getResponse = true;

    //错误判断和显示
    if(msg.return_head.result == CLIENT_RETURN_MSG_RESULT_FAIL || msg.return_head.error_code != CLIENT_RETURN_MSG_ERROR_NO_ERROR)
    {
        //报告错误！
        emit err(msg.return_head.error_code,QString(msg.return_head.error_info));
        return ;
    }

    //没有错误，那么根据响应的指令，找到对应的处理方式处理
    typedef std::function<void(const Client_Response_Msg)> ProcessFunction;

    static struct
    {
        CLIENT_MSG_TODO t;
        ProcessFunction f;
    } table[] =
    {
    { CLIENT_MSG_TODO_USER_LOGIN,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_USER_LOGOUT,std::bind(&MsgCenter::response_user_logout,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_USER_CHANGED_PASSWORD,std::bind(&MsgCenter::response_user_changePassword,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_USER_LIST,std::bind(&MsgCenter::response_user_list,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_USER_DELTE,std::bind(&MsgCenter::response_user_remove,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_USER_ADD,std::bind(&MsgCenter::response_user_add,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_USER_MODIFY,std::bind(&MsgCenter::response_user_modify,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_MAP_CREATE_START,std::bind(&MsgCenter::response_map_create_start,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_MAP_CREATE_ADD_STATION,std::bind(&MsgCenter::response_map_create_add_station,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_MAP_CREATE_ADD_LINE,std::bind(&MsgCenter::response_map_create_add_line,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_MAP_CREATE_ADD_ARC,std::bind(&MsgCenter::response_map_create_add_arc,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_MAP_CREATE_FINISH,std::bind(&MsgCenter::response_map_create_finish,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_MAP_LIST_STATION,std::bind(&MsgCenter::response_map_list_station,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_MAP_LIST_LINE,std::bind(&MsgCenter::response_map_list_line,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_MAP_LIST_ARC,std::bind(&MsgCenter::response_map_list_arc,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_AGV_MANAGE_LIST,std::bind(&MsgCenter::response_agv_list,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_AGV_MANAGE_ADD,std::bind(&MsgCenter::response_agv_add,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_AGV_MANAGE_DELETE,std::bind(&MsgCenter::response_agv_delete,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_AGV_MANAGE_MODIFY,std::bind(&MsgCenter::response_agv_modify,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_TASK_CREATE_TO_X,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_TASK_CREATE_AGV_TO_X,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_TASK_CREATE_PASS_Y_TO_X,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_TASK_CREATE_AGV_PASS_Y_TO_X,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_TASK_QUERY_STATUS,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_TASK_CANCEL,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_TASK_LIST_UNDO,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_TASK_LIST_DOING,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_TASK_LIST_DONE_TODAY,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_TASK_LIST_DURING,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_LOG_LIST_DURING,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_SUB_AGV_POSITION,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_CANCEL_SUB_AGV_POSITION,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_SUB_AGV_STATSU,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_CANCEL_SUB_AGV_STATSU,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_SUB_LOG,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_CANCEL_SUB_LOG,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_SUB_TASK,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { CLIENT_MSG_TODO_CANCEL_SUB_TASK,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
};

    table[msg.head.todo].f(msg);

    return ;
}

void MsgCenter::response_user_login(const Client_Response_Msg msg)
{
    assert(msg.head.body_length == sizeof(USER_INFO));
    memcpy(&current_user_info,msg.body,msg.head.body_length);
    emit loginSuccess(current_user_info.role);
}

void MsgCenter::response_user_logout(const Client_Response_Msg msg)
{
    memset(&current_user_info,0,sizeof(USER_INFO));
}

void MsgCenter::response_user_changePassword(const Client_Response_Msg msg)
{
    //修改成功，需重新登录
    memset(&current_user_info,0,sizeof(USER_INFO));
    emit changePasswrodSuccess();
}

void MsgCenter::response_user_list(const Client_Response_Msg msg)
{
    assert(msg.head.body_length%sizeof(USER_INFO) == 0);
    int kk = msg.head.body_length/sizeof(USER_INFO);
    for(int i=0;i<kk;++i)
    {
        USER_INFO u;
        memcpy(&u,msg.body+i*sizeof(USER_INFO),sizeof(USER_INFO));
        userinfos.append(u);
    }
    emit listUserSuccess();
}

void MsgCenter::response_user_remove(const Client_Response_Msg msg)
{
    //删除成功
    emit deleteUserSuccess();
}

void MsgCenter::response_user_add(const Client_Response_Msg msg)
{
    //添加成功
    emit addUserSuccess();
}

void MsgCenter::response_user_modify(const Client_Response_Msg msg)
{
    //修改成功
    emit modifyUserSuccess();
}

void MsgCenter::response_map_create_start(const Client_Response_Msg msg)
{
    emit mapCreateStart();
}
void MsgCenter::response_map_create_add_line(const Client_Response_Msg msg)
{
    emit mapCreateAddStation();
}
void MsgCenter::response_map_create_add_station(const Client_Response_Msg msg)
{
    emit mapCreateAddLine();
}
void MsgCenter::response_map_create_add_arc(const Client_Response_Msg msg)
{
    emit mapCreateAddArc();
}
void MsgCenter::response_map_create_finish(const Client_Response_Msg msg)
{
    emit mapCreateFinish();
}

void MsgCenter::response_map_list_station(const Client_Response_Msg msg)
{
    int ll = msg.head.body_length/sizeof(STATION_INFO);
    for(int i=0;i<ll;++i){
        STATION_INFO temp;
        memcpy(&temp,msg.body+i*sizeof(STATION_INFO),sizeof(STATION_INFO));
        mapStations.push_back(temp);
    }
    emit mapStationListSuccess();
}

void MsgCenter::response_map_list_line(const Client_Response_Msg msg)
{
    int ll = msg.head.body_length/sizeof(AGV_LINE);
    for(int i=0;i<ll;++i){
        AGV_LINE temp;
        memcpy(&temp,msg.body+i*sizeof(AGV_LINE),sizeof(AGV_LINE));
        mapLines.push_back(temp);
    }
    emit mapLinesListSuccess();
}

void MsgCenter::response_map_list_arc(const Client_Response_Msg msg)
{
    int ll = msg.head.body_length/sizeof(AGV_ARC);
    for(int i=0;i<ll;++i){
        AGV_ARC temp;
        memcpy(&temp,msg.body+i*sizeof(AGV_ARC),sizeof(AGV_ARC));
        mapArcs.push_back(temp);
    }
    emit mapArcListSuccess();
}

void MsgCenter::response_agv_list(const Client_Response_Msg msg)
{
    int ll = msg.head.body_length/sizeof(AGV_BASE_INFO);
    for(int i=0;i<ll;++i){
        AGV_BASE_INFO temp;
        memcpy(&temp,msg.body+i*sizeof(AGV_BASE_INFO),sizeof(AGV_BASE_INFO));
        agvinfos.push_back(temp);
    }
    emit listAgvSuccess();
}

void MsgCenter::response_agv_add(const Client_Response_Msg msg)
{
    emit addAgvSuccess();
}

void MsgCenter::response_agv_delete(const Client_Response_Msg msg)
{
    emit deleteAgvSuccess();
}

void MsgCenter::response_agv_modify(const Client_Response_Msg msg)
{
    emit modifyAgvSuccess();
}
