#include "msgcenter.h"
#include <assert.h>
#include "global.h"


MsgCenter::MsgCenter(QObject *parent) : QObject(parent),
    quit(false)
{

}
MsgCenter::~MsgCenter()
{
    quit =true;
    condition.wakeAll();
    thread_msg_process.join();
}
void MsgCenter::init()
{
    connect(&tcpClient,SIGNAL(onConnect()),this,SIGNAL(sig_connection_connected()));
    connect(&tcpClient,SIGNAL(onConnecting()),this,SIGNAL(sig_connection_conntectting()));
    connect(&tcpClient,SIGNAL(onDisconnect()),this,SIGNAL(sig_connection_conntectting()));
    connect(&tcpClient,SIGNAL(onRead(MSG_Response)),this,SLOT(push(MSG_Response)));
    tcpClient.init(configure.getValue("server/ip").toString(),configure.getValue("server/port").toInt());

    //响应消息处理的线程
    thread_msg_process = std::thread([&](){
        while(!quit){
            responsesMtx.lock();
            if(responses.size()<=0){
                condition.wait(&responsesMtx);
            }
            if(quit){
                responsesMtx.unlock();
                break;
            }
            Json::Value response = responses.front();
            responses.pop_front();
            responsesMtx.unlock();
            parseOneMsg(response);
        }
    });
}

void MsgCenter::resetIpPort(QString ip,int port)
{
    configure.setValue("server/ip",ip);
    configure.setValue("server/port",port);
    tcpClient.reset(configure.getValue("server/ip").toString(),configure.getValue("server/port").toInt());
}

QString MsgCenter::getServerIp()
{
    return configure.getValue("server/ip").toString();
}
int MsgCenter::getServerPort()
{
    return configure.getValue("server/port").toInt();
}

//入队响应消息
void MsgCenter::push(const Json::Value &response)
{
    responsesMtx.lock();
    responses.push_back(response);
    condition.wakeAll();
    responsesMtx.unlock();
}

void MsgCenter::requestWaitResponse(const Json::Value &request)
{
    emit sendNewRequest();
    qDebug()<<"send request:"<<" req->"<<request["todo"]<<" queue->"<<request["queuenumber"];
    if(!tcpClient.send(request)){
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

void MsgCenter::iniRequsttMsg(Json::Value &request)
{
    request["queuenumber"] = ++queueNumber;
    request["type"] = MSG_TYPE_REQUEST;
}

void MsgCenter::parseOneMsg(const Json::Value &response)
{
    //判断消息格式是否正确
    if(response["type"].isNull()||response["queuenumber"].isNull()||response["todo"].isNull()){
        return ;
    }

    //是否和发送请求的queuenumber序号相同，相同标记 得到响应
    if(response["queuenumber"].asInt() == queueNumber)getResponse = true;
    qDebug()<<"send response:"<<" req->"<<response["todo"]<<" queue->"<<response["queuenumber"];
    //错误判断和显示
    if(msg.return_head.result == RETURN_MSG_RESULT_FAIL || msg.return_head.error_code != RETURN_MSG_ERROR_NO_ERROR)
    {
        //报告错误！
        emit err(msg.return_head.error_code,QString(msg.return_head.error_info));
        return ;
    }

    //没有错误，那么根据响应的指令，找到对应的处理方式处理
    typedef std::function<void(const Json::Value &)> ProcessFunction;

    static struct
    {
        MSG_TODO t;
        ProcessFunction f;
    } table[] =
    {
    { MSG_TODO_USER_LOGIN,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_USER_LOGOUT,std::bind(&MsgCenter::response_user_logout,this,std::placeholders::_1) },
    { MSG_TODO_USER_CHANGED_PASSWORD,std::bind(&MsgCenter::response_user_changePassword,this,std::placeholders::_1) },
    { MSG_TODO_USER_LIST,std::bind(&MsgCenter::response_user_list,this,std::placeholders::_1) },
    { MSG_TODO_USER_DELTE,std::bind(&MsgCenter::response_user_remove,this,std::placeholders::_1) },
    { MSG_TODO_USER_ADD,std::bind(&MsgCenter::response_user_add,this,std::placeholders::_1) },
    { MSG_TODO_USER_MODIFY,std::bind(&MsgCenter::response_user_modify,this,std::placeholders::_1) },
    { MSG_TODO_MAP_SET_MAP,std::bind(&MsgCenter::response_map_create_start,this,std::placeholders::_1) },
    { MSG_TODO_MAP_GET_MAP,std::bind(&MsgCenter::response_map_list_station,this,std::placeholders::_1) },
    { MSG_TODO_AGV_MANAGE_LIST,std::bind(&MsgCenter::response_agv_list,this,std::placeholders::_1) },
    { MSG_TODO_AGV_MANAGE_ADD,std::bind(&MsgCenter::response_agv_add,this,std::placeholders::_1) },
    { MSG_TODO_AGV_MANAGE_DELETE,std::bind(&MsgCenter::response_agv_delete,this,std::placeholders::_1) },
    { MSG_TODO_AGV_MANAGE_MODIFY,std::bind(&MsgCenter::response_agv_modify,this,std::placeholders::_1) },
    { MSG_TODO_TASK_CREATE,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_TASK_QUERY_STATUS,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_TASK_CANCEL,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_TASK_LIST_UNDO,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_TASK_LIST_DOING,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_TASK_LIST_DONE_TODAY,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_TASK_LIST_DURING,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_LOG_LIST_DURING,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_SUB_AGV_POSITION,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_CANCEL_SUB_AGV_POSITION,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_SUB_AGV_STATSU,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_CANCEL_SUB_AGV_STATSU,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_SUB_LOG,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_CANCEL_SUB_LOG,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_SUB_TASK,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_CANCEL_SUB_TASK,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
};

    table[msg.head.todo].f(msg);

    return ;
}

void MsgCenter::response_user_login(const Json::Value &response)
{
    assert(msg.head.body_length == sizeof(USER_INFO));
    memcpy(&current_user_info,msg.body,msg.head.body_length);
    emit loginSuccess(current_user_info.role);
}

void MsgCenter::response_user_logout(const Json::Value &response)
{
    memset(&current_user_info,0,sizeof(USER_INFO));
}

void MsgCenter::response_user_changePassword(const Json::Value &response)
{
    //修改成功，需重新登录
    memset(&current_user_info,0,sizeof(USER_INFO));
    emit changePasswrodSuccess();
}

void MsgCenter::response_user_list(const Json::Value &response)
{
    assert(msg.head.body_length%sizeof(USER_INFO) == 0);
    int kk = msg.head.body_length/sizeof(USER_INFO);
    for(int i=0;i<kk;++i)
    {
        USER_INFO u;
        memcpy(&u,msg.body+i*sizeof(USER_INFO),sizeof(USER_INFO));
        userinfos.append(u);
    }
    if(msg.head.flag==0)
        emit listUserSuccess();
}

void MsgCenter::response_user_remove(const Json::Value &response)
{
    //删除成功
    emit deleteUserSuccess();
}

void MsgCenter::response_user_add(const Json::Value &response)
{
    //添加成功
    emit addUserSuccess();
}

void MsgCenter::response_user_modify(const Json::Value &response)
{
    //修改成功
    emit modifyUserSuccess();
}

void MsgCenter::response_map_set(const Json::Value &response)
{
    emit mapCreateStart();
}

void MsgCenter::response_map_get(const Json::Value &response)
{
    int ll = msg.head.body_length/sizeof(STATION_INFO);
    for(int i=0;i<ll;++i){
        STATION_INFO temp;
        memcpy(&temp,msg.body+i*sizeof(STATION_INFO),sizeof(STATION_INFO));
        mapStations.push_back(temp);
    }
    emit mapStationListSuccess();
}


void MsgCenter::response_agv_list(const Json::Value &response)
{
    int ll = msg.head.body_length/sizeof(AGV_BASE_INFO);
    for(int i=0;i<ll;++i){
        AGV_BASE_INFO temp;
        memcpy(&temp,msg.body+i*sizeof(AGV_BASE_INFO),sizeof(AGV_BASE_INFO));
        agvbaseinfos.push_back(temp);
    }
    if(msg.head.flag==0)
        emit listAgvSuccess();
}

void MsgCenter::response_agv_add(const Json::Value &response)
{
    emit addAgvSuccess();
}

void MsgCenter::response_agv_delete(const Json::Value &response)
{
    emit deleteAgvSuccess();
}

void MsgCenter::response_agv_modify(const Json::Value &response)
{
    emit modifyAgvSuccess();
}

/////////////////////////////////////请求登录
void MsgCenter::login(QString username,QString password)
{
    Json::Value request;
    iniRequsttMsg(request);
    request.head.todo = MSG_TODO_USER_LOGIN;
    request.head.body_length = MSG_STRING_LEN*2;
    memcpy_s(request.body,MSG_STRING_LEN,username.toStdString().c_str(),username.toStdString().length());
    memcpy_s(request.body+MSG_STRING_LEN,MSG_STRING_LEN,password.toStdString().c_str(),password.toStdString().length());

    requestWaitResponse(request);
}

//注销调用
void MsgCenter::logout()
{
    Json::Value request;
    iniRequsttMsg(request);
    request.head.todo = MSG_TODO_USER_LOGOUT;
    request.head.body_length = 0;
    requestWaitResponse(request);
}

//修改密码
void MsgCenter::changePassword(QString old_password,QString new_password)
{
    Json::Value request;
    iniRequsttMsg(request);
    request.head.todo = MSG_TODO_USER_CHANGED_PASSWORD;
    request.head.body_length = MSG_STRING_LEN;
    memcpy_s(request.body,MSG_STRING_LEN,new_password.toStdString().c_str(),new_password.toStdString().length());
    requestWaitResponse(request);
}

//用户列表调用(发送获取userList的请求)
void MsgCenter::userList()
{
    userinfos.clear();
    Json::Value request;
    iniRequsttMsg(request);
    request.head.todo = MSG_TODO_USER_LIST;
    requestWaitResponse(request);
}

//删除用户
void MsgCenter::deleteUser(int32_t id)
{
    Json::Value request;
    iniRequsttMsg(request);
    request.head.todo = MSG_TODO_USER_DELTE;
    request.head.body_length = sizeof(int32_t);
    memcpy_s(request.body,MSG_REQUEST_BODY_MAX_SIZE,&id,sizeof(int32_t));
    requestWaitResponse(request);
}

//添加用户
void MsgCenter::adduser(QString username, QString password, int32_t role)
{
    Json::Value request;
    iniRequsttMsg(request);
    request.head.todo = MSG_TODO_USER_ADD;
    USER_INFO u;
    request.head.body_length = sizeof(USER_INFO);
    snprintf(u.username,MSG_STRING_LEN,"%s",username.toStdString().c_str());
    snprintf(u.password,MSG_STRING_LEN,"%s",password.toStdString().c_str());
    u.role = role;
    u.status = 0;
    u.id = 0;
    memcpy_s(request.body,MSG_REQUEST_BODY_MAX_SIZE,&u,sizeof(USER_INFO));
    requestWaitResponse(request);
}

//修改用户
void MsgCenter::modifyuser(int32_t id, QString username, QString password, int32_t role)
{
    Json::Value request;
    iniRequsttMsg(request);
    request.head.todo = MSG_TODO_USER_MODIFY;
    USER_INFO u;
    request.head.body_length = sizeof(USER_INFO);
    snprintf(u.username,MSG_STRING_LEN,"%s",username.toStdString().c_str());
    snprintf(u.password,MSG_STRING_LEN,"%s",password.toStdString().c_str());
    u.role = role;
    u.status = 0;
    u.id = id;
    memcpy_s(request.body,MSG_REQUEST_BODY_MAX_SIZE,&u,sizeof(USER_INFO));
    requestWaitResponse(request);
}

void MsgCenter::agvList()
{
    agvbaseinfos.clear();
    Json::Value request;
    iniRequsttMsg(request);
    request.head.todo = MSG_TODO_AGV_MANAGE_LIST;
    requestWaitResponse(request);
}


void MsgCenter::deleteAgv(int id)
{
    Json::Value request;
    iniRequsttMsg(request);
    request.head.todo = MSG_TODO_AGV_MANAGE_DELETE;
    request.head.body_length = sizeof(int32_t);
    memcpy_s(request.body,MSG_REQUEST_BODY_MAX_SIZE,&id,sizeof(int32_t));
    requestWaitResponse(request);
}


void MsgCenter::addagv(QString name,QString ip,int port)
{
    Json::Value request;
    iniRequsttMsg(request);
    request.head.todo = MSG_TODO_AGV_MANAGE_ADD;
    AGV_BASE_INFO baseinfo;
    baseinfo.id = 0;
    snprintf(baseinfo.name,MSG_STRING_LEN,"%s",name.toStdString().c_str());
    snprintf(baseinfo.ip,MSG_STRING_LEN,"%s",ip.toStdString().c_str());
    baseinfo.port = port;
    request.head.body_length = sizeof(baseinfo);
    memcpy_s(request.body,MSG_REQUEST_BODY_MAX_SIZE,&baseinfo,sizeof(baseinfo));
    requestWaitResponse(request);
}


void MsgCenter::modifyagv(int id,QString name,QString ip,int port)
{
    Json::Value request;
    iniRequsttMsg(request);
    request.head.todo = MSG_TODO_AGV_MANAGE_MODIFY;
    AGV_BASE_INFO baseinfo;
    baseinfo.id = id;
    snprintf(baseinfo.name,MSG_STRING_LEN,"%s",name.toStdString().c_str());
    snprintf(baseinfo.ip,MSG_STRING_LEN,"%s",ip.toStdString().c_str());
    baseinfo.port = port;
    request.head.body_length = sizeof(baseinfo);
    memcpy_s(request.body,MSG_REQUEST_BODY_MAX_SIZE,&baseinfo,sizeof(baseinfo));
    requestWaitResponse(request);
}
