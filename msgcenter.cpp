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
}
void MsgCenter::init()
{
    connect(&tcpClient,SIGNAL(onConnect()),this,SIGNAL(sig_connection_connected()));
    connect(&tcpClient,SIGNAL(onConnecting()),this,SIGNAL(sig_connection_conntectting()));
    connect(&tcpClient,SIGNAL(onDisconnect()),this,SIGNAL(sig_connection_conntectting()));
    connect(&tcpClient,SIGNAL(onRead(MSG_Response)),this,SLOT(push(MSG_Response)));
    tcpClient.init(configure.getValue("server/ip").toString(),configure.getValue("server/port").toInt());

    //响应消息处理的线程
    g_threadPool.enqueue([&](){
        while(!quit){
            responsesMtx.lock();
            if(responses.size()<=0){
                responsesMtx.unlock();
                std::chrono::milliseconds dura(10);
                std::this_thread::sleep_for(dura);
                continue;
            }
            MSG_Response msg = responses.front();
            responses.pop_front();
            responsesMtx.unlock();
            parseOneMsg(msg);
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
void MsgCenter::push(MSG_Response msg)
{
    responsesMtx.lock();
    responses.push_back(msg);
    responsesMtx.unlock();
}

void MsgCenter::requestWaitResponse(const MSG_Request & msg)
{
    if(!tcpClient.send(msg)){
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

void MsgCenter::iniRequsttMsg(MSG_Request &msg)
{
    memset(&msg,0,sizeof(MSG_Request));
    msg.head.head = MSG_MSG_Head_HEAD;
    msg.head.tail = MSG_MSG_Head_TAIL;
    msg.head.queuenumber = ++queueNumber;
}

void MsgCenter::parseOneMsg(const MSG_Response msg)
{
    //判断消息格式是否正确
    if(msg.head.head != MSG_MSG_Head_HEAD || msg.head.tail!=MSG_MSG_Head_TAIL)return ;

    //是否和发送请求的queuenumber序号相同，相同标记 得到响应
    if(msg.head.queuenumber == queueNumber)getResponse = true;

    //错误判断和显示
    if(msg.return_head.result == RETURN_MSG_RESULT_FAIL || msg.return_head.error_code == RETURN_MSG_ERROR_NO_ERROR)
    {
        //报告错误！
        emit err(msg.return_head.error_code,QString(msg.return_head.error_info));
        return ;
    }

    //没有错误，那么根据响应的指令，找到对应的处理方式处理
    typedef std::function<void(const MSG_Response)> ProcessFunction;

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
    { MSG_TODO_MAP_CREATE_START,std::bind(&MsgCenter::response_map_create_start,this,std::placeholders::_1) },
    { MSG_TODO_MAP_CREATE_ADD_STATION,std::bind(&MsgCenter::response_map_create_add_station,this,std::placeholders::_1) },
    { MSG_TODO_MAP_CREATE_ADD_LINE,std::bind(&MsgCenter::response_map_create_add_line,this,std::placeholders::_1) },
    { MSG_TODO_MAP_CREATE_FINISH,std::bind(&MsgCenter::response_map_create_finish,this,std::placeholders::_1) },
    { MSG_TODO_MAP_LIST_STATION,std::bind(&MsgCenter::response_map_list_station,this,std::placeholders::_1) },
    { MSG_TODO_MAP_LIST_LINE,std::bind(&MsgCenter::response_map_list_line,this,std::placeholders::_1) },
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

void MsgCenter::response_user_login(const MSG_Response msg)
{
    assert(msg.head.body_length == sizeof(USER_INFO));
    memcpy(&current_user_info,msg.body,msg.head.body_length);
    emit loginSuccess(current_user_info.role);
}

void MsgCenter::response_user_logout(const MSG_Response msg)
{
    memset(&current_user_info,0,sizeof(USER_INFO));
}

void MsgCenter::response_user_changePassword(const MSG_Response msg)
{
    //修改成功，需重新登录
    memset(&current_user_info,0,sizeof(USER_INFO));
    emit changePasswrodSuccess();
}

void MsgCenter::response_user_list(const MSG_Response msg)
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

void MsgCenter::response_user_remove(const MSG_Response msg)
{
    //删除成功
    emit deleteUserSuccess();
}

void MsgCenter::response_user_add(const MSG_Response msg)
{
    //添加成功
    emit addUserSuccess();
}

void MsgCenter::response_user_modify(const MSG_Response msg)
{
    //修改成功
    emit modifyUserSuccess();
}

void MsgCenter::response_map_create_start(const MSG_Response msg)
{
    emit mapCreateStart();
}
void MsgCenter::response_map_create_add_line(const MSG_Response msg)
{
    emit mapCreateAddStation();
}
void MsgCenter::response_map_create_add_station(const MSG_Response msg)
{
    emit mapCreateAddLine();
}
void MsgCenter::response_map_create_add_arc(const MSG_Response msg)
{
    emit mapCreateAddArc();
}
void MsgCenter::response_map_create_finish(const MSG_Response msg)
{
    emit mapCreateFinish();
}

void MsgCenter::response_map_list_station(const MSG_Response msg)
{
    int ll = msg.head.body_length/sizeof(STATION_INFO);
    for(int i=0;i<ll;++i){
        STATION_INFO temp;
        memcpy(&temp,msg.body+i*sizeof(STATION_INFO),sizeof(STATION_INFO));
        mapStations.push_back(temp);
    }
    emit mapStationListSuccess();
}

void MsgCenter::response_map_list_line(const MSG_Response msg)
{
    int ll = msg.head.body_length/sizeof(AGV_LINE);
    for(int i=0;i<ll;++i){
        AGV_LINE temp;
        memcpy(&temp,msg.body+i*sizeof(AGV_LINE),sizeof(AGV_LINE));
        mapLines.push_back(temp);
    }
    emit mapLinesListSuccess();
}

void MsgCenter::response_agv_list(const MSG_Response msg)
{
    int ll = msg.head.body_length/sizeof(AGV_BASE_INFO);
    for(int i=0;i<ll;++i){
        AGV_BASE_INFO temp;
        memcpy(&temp,msg.body+i*sizeof(AGV_BASE_INFO),sizeof(AGV_BASE_INFO));
        agvbaseinfos.push_back(temp);
    }
    emit listAgvSuccess();
}

void MsgCenter::response_agv_add(const MSG_Response msg)
{
    emit addAgvSuccess();
}

void MsgCenter::response_agv_delete(const MSG_Response msg)
{
    emit deleteAgvSuccess();
}

void MsgCenter::response_agv_modify(const MSG_Response msg)
{
    emit modifyAgvSuccess();
}

/////////////////////////////////////请求登录
void MsgCenter::login(QString username,QString password)
{
    MSG_Request request;
    iniRequsttMsg(request);
    request.head.todo = MSG_TODO_USER_LOGIN;
    memcpy_s(request.body,MSG_STRING_LEN,username.toStdString().c_str(),username.toStdString().length());
    memcpy_s(request.body+MSG_STRING_LEN,MSG_STRING_LEN,password.toStdString().c_str(),password.toStdString().length());

    requestWaitResponse(request);
}
