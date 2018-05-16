#include "msgcenter.h"
#include <assert.h>
#include "global.h"
#include "base64.h"

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
    connect(&tcpClient,SIGNAL(onRead(Json::Value)),this,SLOT(push(Json::Value)));
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
void MsgCenter::push(Json::Value response)
{
    responsesMtx.lock();
    responses.push_back(response);
    condition.wakeAll();
    responsesMtx.unlock();
}

void MsgCenter::requestWaitResponse(const Json::Value &request)
{
    emit sendNewRequest();
    //qDebug()<<"send request:"<<" req->"<<request["todo"].asString().c_str()<<" queue->"<<request["queuenumber"].asString().c_str();
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
    //qDebug()<<"send response:"<<" req->"<<response["todo"].asString().c_str()<<" queue->"<<response["queuenumber"].asString().c_str();
    //错误判断和显示
    if(response["result"].asInt() == RETURN_MSG_RESULT_FAIL)
    {
        //报告错误！
        emit err(response["error_code"].asInt(),QString(response["error_info"].asString().c_str()));
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
    { MSG_TODO_MAP_SET_MAP,std::bind(&MsgCenter::response_map_set,this,std::placeholders::_1) },
    { MSG_TODO_MAP_GET_MAP,std::bind(&MsgCenter::response_map_get,this,std::placeholders::_1) },
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
    { MSG_TODO_TRAFFIC_CONTROL_STATION,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_TRAFFIC_CONTROL_LINE,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_TRAFFIC_RELEASE_STATION,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_TRAFFIC_RELEASE_LINE,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_PUB_AGV_POSITION,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_PUB_AGV_STATUS,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_PUB_LOG,std::bind(&MsgCenter::pub_agv_log,this,std::placeholders::_1) },
    { MSG_TODO_PUB_TASK,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
};

    table[response["todo"].asInt()].f(response);

    return ;
}

void MsgCenter::response_user_login(const Json::Value &response)
{
    current_user_info.id = response["user"]["id"].asInt();
    current_user_info.role = response["user"]["role"].asInt();
    current_user_info.status = response["user"]["status"].asInt();
    current_user_info.username = response["user"]["username"].asString();
    current_user_info.password = response["user"]["password"].asString();

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
    userinfos.clear();
    Json::Value users = response["users"];
    for(int i=0;i<users.size();++i)
    {
        Json::Value oneUser = users[i];
        USER_INFO user_info;
        user_info.id = oneUser["id"].asInt();
        user_info.role = oneUser["role"].asInt();
        user_info.status = oneUser["status"].asInt();
        user_info.username = oneUser["username"].asString();
        user_info.password = oneUser["password"].asString();
        userinfos.push_back(user_info);
    }

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
    emit mapSetSuccess();
}

void MsgCenter::response_map_get(const Json::Value &response)
{
    g_onemap.clear();
    //地图信息
    Json::Value mapmap = response["map"];
    int max_id = mapmap["max_id"].asInt();

    ////floors
    Json::Value floors = mapmap["floors"];

    for(int i=0;i<floors.size();++i){

        //0.floor id name
        Json::Value floor = floors[i];
        int floor_id = floor["id"].asInt();
        std::string floor_name = floor["name"].asString();
        MapFloor *map_floor = new MapFloor(floor_id,floor_name);

        //1.floor->points
        Json::Value points = floor["points"];
        for(int a = 0;a<points.size();++a){
            Json::Value point = points[a];
            int id = point["id"].asInt();
            std::string name = point["name"].asString();
            int point_type = point["point_type"].asInt();
            int x = point["x"].asInt();
            int y = point["y"].asInt();
            int realX = point["realX"].asInt();
            int realY = point["realY"].asInt();
            int labelXoffset = point["labelXoffset"].asInt();
            int labelYoffset = point["labelYoffset"].asInt();
            bool mapChange = point["mapChange"].asBool();
            bool locked = point["locked"].asBool();
            MapPoint *p = new MapPoint(id,name,(MapPoint::Map_Point_Type)point_type,x,y,realX,realY,labelXoffset,labelYoffset,mapChange,locked);
            map_floor->addPoint(p);
        }

        //2.floor->paths
        Json::Value paths = floor["paths"];
        for(int b = 0;b<paths.size();++b){
            Json::Value path = paths[b];
            int id = path["id"].asInt();
            std::string name = path["name"].asString();
            int path_type = path["path_type"].asInt();
            int start = path["start"].asInt();
            int end = path["end"].asInt();
            int p1x = path["p1x"].asInt();
            int p2x = path["p2x"].asInt();
            int p1y = path["p1y"].asInt();
            int p2y = path["p2y"].asInt();
            bool length = path["length"].asInt();
            bool locked = path["locked"].asBool();
            MapPath *p = new MapPath(id,name,start,end,(MapPath::Map_Path_Type)path_type,length,p1x,p2x,p1y,p2y,locked);
            map_floor->addPath(p);
        }

        //3.floor->bkg
        Json::Value bkg = floor["bkg"];
        int bkg_id = bkg["id"].asInt();
        std::string bkg_name = bkg["name"].asString();
        std::string filename = bkg["filename"].asString();
        std::string datas = bkg["data"].asString();
        char *imgdata = new char[datas.length()*2+1];
        int len = base64_decode(imgdata,(char *)datas.c_str(),datas.length());

        QImage img;
        img.loadFromData(QByteArray(imgdata,len));
        MapBackground *mb_bkg = new MapBackground(bkg_id,bkg_name,img,filename);
        map_floor->setBkg(mb_bkg);

        g_onemap.addFloor(map_floor);
    }

    ////4.root paths
    Json::Value rootpaths = mapmap["rootpaths"];
    for(int b = 0;b<rootpaths.size();++b){
        Json::Value path = rootpaths[b];
        int id = path["id"].asInt();
        std::string name = path["name"].asString();
        int path_type = path["path_type"].asInt();
        int start = path["start"].asInt();
        int end = path["end"].asInt();
        int p1x = path["p1x"].asInt();
        int p2x = path["p2x"].asInt();
        int p1y = path["p1y"].asInt();
        int p2y = path["p2y"].asInt();
        bool length = path["length"].asInt();
        bool locked = path["locked"].asBool();
        MapPath *p = new MapPath(id,name,start,end,(MapPath::Map_Path_Type)path_type,length,p1x,p2x,p1y,p2y,locked);
        g_onemap.addPath(p);
    }

    ////5.blocks
    Json::Value blocks = mapmap["blocks"];
    for(int b = 0;b<blocks.size();++b){
        Json::Value block = blocks[b];
        int id = block["id"].asInt();
        std::string name = block["name"].asString();

        MapBlock *map_block = new MapBlock(id,name);
        Json::Value spirits = block["spirits"];
        for(int c = 0;c<spirits.size();++c){
            int spirit = spirits[c].asInt();
            map_block->addSpirit(spirit);
        }
        g_onemap.addBlock(map_block);
    }

    ////6.groups
    Json::Value groups = mapmap["groups"];
    for(int b = 0;b<groups.size();++b){
        Json::Value group = groups[b];
        int id = group["id"].asInt();
        std::string name = group["name"].asString();

        MapGroup *map_group = new MapGroup(id,name);
        Json::Value spirits = group["spirits"];
        for(int c = 0;c<spirits.size();++c){
            int spirit = spirits[c].asInt();
            map_group->addSpirit(spirit);
        }
        Json::Value agvs = group["agvs"];
        for(int c = 0;c<agvs.size();++c){
            int agvid = agvs[c].asInt();
            map_group->addAgv(agvid);
        }
        g_onemap.addGroup(map_group);
    }


    emit mapGetSuccess();
}

void MsgCenter::response_agv_list(const Json::Value &response)
{
    agvbaseinfos.clear();
    Json::Value agvs = response["agvs"];
    for(int i=0;i<agvs.size();++i){
        Json::Value agv = agvs[i];
        AGV_BASE_INFO temp;
        temp.id = agv["id"].asInt();
        temp.ip = agv["ip"].asString();
        temp.name = agv["name"].asString();
        temp.port = agv["port"].asInt();
        agvbaseinfos.push_back(temp);
    }
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

void MsgCenter::pub_agv_log(const Json::Value &response)
{
    USER_LOG log;
    log.time = response["log"]["time"].asString();
    log.msg = response["log"]["msg"].asString();
    emit pubOneLog(log);
}

/////////////////////////////////////请求登录
void MsgCenter::login(QString username,QString password)
{
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_USER_LOGIN;
    request["username"] = username.toStdString();
    request["password"] = password.toStdString();
    requestWaitResponse(request);
}

//注销调用
void MsgCenter::logout()
{
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_USER_LOGOUT;
    requestWaitResponse(request);
}

//修改密码
void MsgCenter::changePassword(QString new_password)
{
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_USER_CHANGED_PASSWORD;
    request["password"] = new_password.toStdString();
    requestWaitResponse(request);
}

//用户列表调用(发送获取userList的请求)
void MsgCenter::userList()
{
    userinfos.clear();
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_USER_LIST;
    requestWaitResponse(request);
}

//删除用户
void MsgCenter::deleteUser(int id)
{
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_USER_DELTE;
    request["id"] = id;
    requestWaitResponse(request);
}

//添加用户
void MsgCenter::adduser(QString username, QString password, int role)
{
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_USER_ADD;
    request["username"] = username.toStdString();
    request["password"] = password.toStdString();
    request["role"] = role;
    requestWaitResponse(request);
}

//修改用户
void MsgCenter::modifyuser(int id, QString username, QString password, int role)
{
    Json::Value request;
    iniRequsttMsg(request);

    request["todo"] = MSG_TODO_USER_MODIFY;
    request["id"] = id;
    request["username"] = username.toStdString();
    request["password"] = password.toStdString();
    request["role"] = role;

    requestWaitResponse(request);
}

void MsgCenter::agvList()
{
    agvbaseinfos.clear();
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_AGV_MANAGE_LIST;
    requestWaitResponse(request);
}


void MsgCenter::deleteAgv(int id)
{
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_AGV_MANAGE_DELETE;
    request["id"] = id;
    requestWaitResponse(request);
}


void MsgCenter::addagv(QString name,QString ip,int port)
{
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_AGV_MANAGE_ADD;
    request["name"] = name.toStdString();
    request["ip"] = ip.toStdString();
    request["port"] = port;
    requestWaitResponse(request);
}


void MsgCenter::modifyagv(int id,QString name,QString ip,int port)
{
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_AGV_MANAGE_ADD;
    request["id"] = id;
    request["name"] = name.toStdString();
    request["ip"] = ip.toStdString();
    request["port"] = port;
    requestWaitResponse(request);
}


void MsgCenter::subUserLog()
{
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_SUB_LOG;
    requestWaitResponse(request);
}

void MsgCenter::cancelSubUserLog()
{
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_CANCEL_SUB_LOG;
    requestWaitResponse(request);
}
