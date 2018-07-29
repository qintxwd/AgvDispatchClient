#include "msgcenter.h"
#include <assert.h>
#include "global.h"
#include "base64.h"
#include <iostream>

MsgCenter::MsgCenter(QObject *parent) : QObject(parent),
    quit(false),
    isMapLoaded(false)
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
    int kk = 200;
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

    //错误判断和显示
    if(!response["result"].isNull() &&  response["result"].asInt() == RETURN_MSG_RESULT_FAIL)
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
    { MSG_TODO_TASK_CREATE,std::bind(&MsgCenter::response_task_add,this,std::placeholders::_1) },
    { MSG_TODO_TASK_QUERY_STATUS,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_TASK_CANCEL,std::bind(&MsgCenter::response_task_cancel,this,std::placeholders::_1) },
    { MSG_TODO_TASK_LIST_UNDO,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_TASK_LIST_DOING,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_TASK_LIST_DONE_TODAY,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_TASK_LIST_DURING,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_LOG_LIST_DURING,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_SUB_AGV_POSITION,std::bind(&MsgCenter::response_subAgvPosition,this,std::placeholders::_1) },
    { MSG_TODO_CANCEL_SUB_AGV_POSITION,std::bind(&MsgCenter::response_cancelSubAgvPosition,this,std::placeholders::_1) },
    { MSG_TODO_SUB_AGV_STATSU,std::bind(&MsgCenter::response_subAgvStatus,this,std::placeholders::_1) },
    { MSG_TODO_CANCEL_SUB_AGV_STATSU,std::bind(&MsgCenter::response_cancelSubAgvStatus,this,std::placeholders::_1) },
    { MSG_TODO_SUB_LOG,std::bind(&MsgCenter::response_subUserLog,this,std::placeholders::_1) },
    { MSG_TODO_CANCEL_SUB_LOG,std::bind(&MsgCenter::response_cancelSubUserLog,this,std::placeholders::_1) },
    { MSG_TODO_SUB_TASK,std::bind(&MsgCenter::response_task_sub,this,std::placeholders::_1) },
    { MSG_TODO_CANCEL_SUB_TASK,std::bind(&MsgCenter::response_task_cancel_sub,this,std::placeholders::_1) },
    { MSG_TODO_TRAFFIC_CONTROL_STATION,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_TRAFFIC_CONTROL_LINE,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_TRAFFIC_RELEASE_STATION,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_TRAFFIC_RELEASE_LINE,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_PUB_AGV_POSITION,std::bind(&MsgCenter::pub_agv_position,this,std::placeholders::_1) },
    { MSG_TODO_PUB_AGV_STATUS,std::bind(&MsgCenter::response_user_login,this,std::placeholders::_1) },
    { MSG_TODO_PUB_LOG,std::bind(&MsgCenter::pub_agv_log,this,std::placeholders::_1) },
    { MSG_TODO_PUB_TASK,std::bind(&MsgCenter::pub_agv_task,this,std::placeholders::_1) },
};

    table[response["todo"].asInt()].f(response);

    return ;
}

void MsgCenter::response_user_login(const Json::Value &response)
{
    current_user_info.id = response["id"].asInt();
    current_user_info.role = response["role"].asInt();
    current_user_info.status = response["status"].asInt();
    current_user_info.username = QString::fromStdString( response["username"].asString() );
    current_user_info.password = QString::fromStdString( response["password"].asString() );

    emit loginSuccess(current_user_info.role);
}

void MsgCenter::response_user_logout(const Json::Value &response)
{
    current_user_info.id = 0;
    current_user_info.password = "";
    current_user_info.username = "";
    current_user_info.role = 0;
    current_user_info.status = 0;
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
        user_info.username = QString::fromStdString(oneUser["username"].asString());
        user_info.password = QString::fromStdString(oneUser["password"].asString());
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
    //TODO:
    qDebug()<<"get map json length = "<<response.toStyledString().length();
    g_onemap.clear();
    //地图信息
    for (int i = 0; i < response["points"].size(); ++i)
    {
        Json::Value station = response["points"][i];
        int id = station["id"].asInt();
        std::string name = station["name"].asString();
        int station_type = station["point_type"].asInt();
        int x = station["x"].asInt();
        int y = station["y"].asInt();
        int realX = station["realX"].asInt();
        int realY = station["realY"].asInt();
        int labelXoffset = station["labelXoffset"].asInt();
        int labelYoffset = station["labelYoffset"].asInt();
        bool mapchange = station["mapChange"].asBool();
        bool locked = station["locked"].asBool();
        std::string ip = station["ip"].asString();
        int port = station["port"].asInt();
        int agvType = station["agvType"].asInt();
        std::string lineId = station["lineId"].asString();

        MapPoint *p = new MapPoint(id,name,(MapPoint::Map_Point_Type)station_type,x,y,realX,realY,labelXoffset,labelYoffset,mapchange,locked,ip,port,agvType,lineId);
        g_onemap.addSpirit(p);
    }

    //2.解析线路
    for (int i = 0; i < response["paths"].size(); ++i)
    {
        Json::Value line = response["paths"][i];
        int id = line["id"].asInt();
        std::string name = line["name"].asString();
        int type = line["type"].asInt();
        int start = line["start"].asInt();
        int end = line["end"].asInt();
        int p1x = line["p1x"].asInt();
        int p1y = line["p1y"].asInt();
        int p2x = line["p2x"].asInt();
        int p2y = line["p2y"].asInt();
        int length = line["length"].asInt();
        bool locked = line["locked"].asBool();
        double speed = line["speed"].asDouble();
        MapPath *p = new MapPath(id,name,start,end,(MapPath::Map_Path_Type)type,length,p1x,p1y,p2x,p2y,locked,speed);
        g_onemap.addSpirit(p);
    }

    //4.解析背景图片
    for (int i = 0; i < response["bkgs"].size(); ++i)
    {
        Json::Value bkg = response["bkgs"][i];
        int id = bkg["id"].asInt();
        std::string name = bkg["name"].asString();
        std::string database64 = bkg["data"].asString();
        int lenlen = Base64decode_len(database64.c_str());
        char *data = new char[lenlen];
        Base64decode(data,database64.c_str());
        int imgdatalen = bkg["data_len"].asInt();
        int width = bkg["width"].asInt();
        int height = bkg["height"].asInt();
        int x = bkg["x"].asInt();
        int y = bkg["y"].asInt();
        std::string filename = bkg["filename"].asString();
        MapBackground *p = new MapBackground(id,name,data, lenlen,width,height,filename);
        p->setX(x);
        p->setY(y);
        g_onemap.addSpirit(p);
    }

    //3.解析楼层
    for (int i = 0; i < response["floors"].size(); ++i)
    {
        Json::Value floor = response["floors"][i];
        int id = floor["id"].asInt();
        std::string name = floor["name"].asString();
        Json::Value points = floor["points"];
        Json::Value paths = floor["paths"];
        int bkg = floor["bkg"].asInt();
        int originX = floor["originX"].asInt();
        int originY = floor["originY"].asInt();
        double rate = floor["rate"].asDouble();
        MapFloor *p = new MapFloor(id, name);
        p->setBkg(bkg);
        p->setOriginX(originX);
        p->setOriginY(originY);
        p->setRate(rate);
        for (int k = 0; k < points.size(); ++k) {
            Json::Value point = points[k];
            p->addPoint(point.asInt());
        }
        for (int k = 0; k < paths.size(); ++k) {
            Json::Value path = paths[k];
            p->addPath(path.asInt());
        }
        g_onemap.addSpirit(p);
    }

    //5.解析block
    for (int i = 0; i < response["blocks"].size(); ++i)
    {
        Json::Value block = response["blocks"][i];
        int id = block["id"].asInt();
        std::string name = block["name"].asString();
        Json::Value spirits = block["spirits"];
        MapBlock *p = new MapBlock(id,name);
        for(int k=0;k<spirits.size();++k){
            Json::Value spirit = spirits[k];
            p->addSpirit(spirit.asInt());
        }
        g_onemap.addSpirit(p);
    }

    //6.解析group
    for (int i = 0; i < response["groups"].size(); ++i)
    {
        Json::Value group = response["groups"][i];
        int id = group["id"].asInt();
        std::string name = group["name"].asString();
        Json::Value spirits = group["spirits"];
        MapGroup *p = new MapGroup(id,name);
        for(int k=0;k<spirits.size();++k){
            Json::Value spirit = spirits[k];
            p->addSpirit(spirit.asInt());
        }
        Json::Value agvs = group["agvs"];
        for(int k=0;k<agvs.size();++k){
            Json::Value agv = agvs[k];
            p->addAgv(agv.asInt());
        }
        g_onemap.addSpirit(p);
    }

    int max_id = response["maxId"].asInt();
    g_onemap.setMaxId(max_id);
    isMapLoaded = true;
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
        temp.ip = QString::fromStdString(agv["ip"].asString());
        temp.name = QString::fromStdString(agv["name"].asString());
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

void MsgCenter::pub_agv_position(const Json::Value &response)
{
    Json::Value json_agvs = response["agvs"];
    for(int i=0;i<json_agvs.size();++i){
        Json::Value json_one_agv = json_agvs[i];
        int id = json_one_agv["id"].asInt();
        QString name = QString::fromStdString(json_one_agv["name"].asString());
        double x = json_one_agv["x"].asDouble();
        double y = json_one_agv["y"].asDouble();
        double theta = json_one_agv["theta"].asDouble();
        emit sig_pub_agv_position(id,name,x,y,theta);
    }
}

void MsgCenter::response_task_add(const Json::Value &response)
{
    emit addTaskSuccess();
}

void MsgCenter::response_task_cancel(const Json::Value &response)
{
    emit cancelTaskSuccess();
}

void MsgCenter::response_task_sub(const Json::Value &response)
{
    emit subTaskSuccess();
}

void MsgCenter::response_task_cancel_sub(const Json::Value &response)
{
    emit cancelSubTaskSuccess();
}

void MsgCenter::pub_agv_task(const Json::Value &response)
{
    //TODO:
    //更新任务列表
    Json::Value json_tasks = response["tasks"];
    agvtaskinfos.clear();
    for(int i=0;i<json_tasks.size();++i){
        Json::Value json_one_task = json_tasks[i];
        TASK_INFO ti;
        ti.excuteAgv = json_one_task["agv"].asInt();
        if(ti.excuteAgv<=0)ti.excuteAgv = 0;
        ti.priority = json_one_task["priority"].asInt();
        ti.status = json_one_task["status"].asInt();
        ti.produceTime = QString::fromStdString(json_one_task["produceTime"].asString());
        ti.cancelTime = QString::fromStdString(json_one_task["cancelTime"].asString());
        ti.doTime = QString::fromStdString(json_one_task["doTime"].asString());
        ti.doneTime = QString::fromStdString(json_one_task["doneTime"].asString());
        ti.errorTime = QString::fromStdString(json_one_task["errorTime"].asString());
        ti.doingIndex = json_one_task["doingIndex"].asInt();
        ti.errorCode = json_one_task["errorCode"].asInt();
        ti.errorInfo = QString::fromStdString(json_one_task["errorInfo"].asString());
        ti.id = json_one_task["id"].asInt();
        ti.isCancel = json_one_task["isCancel"].asBool();

        Json::Value json_nodes = json_one_task["nodes"];
        for(int j=0;j<json_nodes.size();++j){
            Json::Value json_one_node = json_nodes[j];
            TaskNode node;
            node.stationid = json_one_node["station"].asInt();
            Json::Value json_things = json_one_node["things"];
            for(int k=0;k<json_things.size();++k){
                Json::Value json_one_thing = json_things[k];
                node.dowhat = json_one_thing["id"].asInt();
                //node.params =
            }
            ti.nodes.append(node);
        }
        agvtaskinfos.append(ti);
    }
    emit onSubTask();
}

void MsgCenter::response_subUserLog(const Json::Value &response)
{
    emit subUserLogSuccess();
}

void MsgCenter::response_cancelSubUserLog(const Json::Value &response)
{
    emit cancelSubUserLogSuccess();
}

void MsgCenter::response_subAgvPosition(const Json::Value &response)
{
    emit subAgvPositionSuccess();
}

void MsgCenter::response_subAgvStatus(const Json::Value &response)
{
    emit subAgvStatusSuccess();
}

void MsgCenter::response_subTask(const Json::Value &response)
{
    emit subTaskSuccess();
}

void MsgCenter::response_cancelSubAgvPosition(const Json::Value &response)
{
    emit cancelSubAgvPositionSuccess();
}

void MsgCenter::response_cancelSubAgvStatus(const Json::Value &response)
{
    emit cancelSubStatusSuccess();
}

void MsgCenter::response_cancelSubTask(const Json::Value &response)
{
    emit cancelSubTaskSuccess();
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


void MsgCenter::addagv(QString name,QString ip,int port,int station)
{
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_AGV_MANAGE_ADD;
    request["name"] = name.toStdString();
    request["ip"] = ip.toStdString();
    request["port"] = port;
    request["agv_type"] = "virtual";
    request["station"] = station;
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

void MsgCenter::subAgvPosition()
{
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_SUB_AGV_POSITION;
    requestWaitResponse(request);
}

void MsgCenter::cancelSubAgvPosition()
{
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_CANCEL_SUB_AGV_POSITION;
    requestWaitResponse(request);
}

void MsgCenter::subAgvStatus()
{
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_SUB_AGV_STATSU;
    requestWaitResponse(request);
}

void MsgCenter::cancelSubAgvStatus()
{
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_CANCEL_SUB_AGV_STATSU;
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

void MsgCenter::addTask(int priority, int agv, QMap<QString, QString> params,QList<TaskNode> nodes)
{
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_TASK_CREATE;
    request["priority"] = priority;
    request["agv"] = agv;
    Json::Value v_params;
    for(auto itr = params.begin();itr!=params.end();++itr){
        v_params[itr.key().toStdString()] = itr.value().toStdString();
    }

    if(!v_params.isNull()){
        request["extra_params"] = v_params;
    }

    Json::Value v_nodes;

    for(auto node:nodes){
        Json::Value v_node;
        v_node["station"] = node.stationid;
        v_node["dowhat"] = node.dowhat;
        std::stringstream ss;
        foreach (auto p, node.params) {
            ss<<p.toStdString()<<";";
        }
        v_node["params"] = ss.str();
        v_nodes.append(v_node);
    }

    request["nodes"] = v_nodes;
    requestWaitResponse(request);
}

void MsgCenter::cancelTask(int taskId)
{
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_TASK_CANCEL;
    request["id"] = taskId;
    requestWaitResponse(request);
}

void MsgCenter::subTask()
{
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_SUB_TASK;
    requestWaitResponse(request);
}

void MsgCenter::cancelSubTask()
{
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_CANCEL_SUB_TASK;
    requestWaitResponse(request);
}

QString ByteArrayToHexString(QByteArray data){
    QString ret(data.toHex().toUpper());
    int len = ret.length()/2;
    for(int i=1;i<len;i++)
    {
        ret.insert(2*i+i-1," ");
    }

    return ret;
}


void MsgCenter::mapSave(OneMap *onemap)
{
    //上传地图信息
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_MAP_SET_MAP;

    std::list<MapSpirit *> allspirit = onemap->getAllElement();

    Json::Value v_points;
    Json::Value v_paths;
    Json::Value v_floors;
    Json::Value v_bkgs;
    Json::Value v_blocks;
    Json::Value v_groups;
    for (auto spirit : allspirit)
    {
        if (spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Point) {
            MapPoint *p = static_cast<MapPoint *>(spirit);
            Json::Value pv;
            pv["id"] = p->getId();
            pv["name"] = p->getName();
            pv["type"] = p->getPointType();
            pv["x"] = p->getX();
            pv["y"] = p->getY();
            pv["realX"] = p->getRealX();
            pv["realY"] = p->getRealY();
            pv["labelXoffset"] = p->getLabelXoffset();
            pv["labelYoffset"] = p->getLabelYoffset();
            pv["mapChange"] = p->getMapChange();
            pv["locked"] = p->getLocked();
            pv["ip"] = p->getIp();
            pv["port"] = p->getPort();
            pv["agvType"] = p->getAgvType();
            pv["lineId"] = p->getLineId();
            v_points.append(pv);
        }
        else if (spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Path) {
            MapPath *p = static_cast<MapPath *>(spirit);
            Json::Value pv;
            pv["id"] = p->getId();
            pv["name"] = p->getName();
            pv["type"] = p->getPathType();
            pv["start"] = p->getStart();
            pv["end"] = p->getEnd();
            pv["p1x"] = p->getP1x();
            pv["p1y"] = p->getP1y();
            pv["p2x"] = p->getP2x();
            pv["p2y"] = p->getP2y();
            pv["length"] = p->getLength();
            pv["locked"] = p->getLocked();
            pv["speed"] = p->getSpeed();
            v_paths.append(pv);
        }
        else if (spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Background) {
            MapBackground *p = static_cast<MapBackground *>(spirit);
            Json::Value pv;
            pv["id"] = p->getId();
            pv["name"] = p->getName();
            int lenlen = Base64encode_len(p->getImgDataLen());
            char *ss = new char[lenlen];
            Base64encode(ss, p->getImgData(), p->getImgDataLen());
            pv["data"] = std::string(ss, lenlen);
            delete ss;
            pv["data_len"] = p->getImgDataLen();
            pv["width"] = p->getWidth();
            pv["height"] = p->getHeight();
            pv["x"] = p->getX();
            pv["y"] = p->getY();
            pv["filename"] = p->getFileName();
            v_bkgs.append(pv);
        }
        else if (spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Floor) {
            MapFloor *p = static_cast<MapFloor *>(spirit);
            Json::Value pv;
            pv["id"] = p->getId();
            pv["name"] = p->getName();
            pv["bkg"] = p->getBkg();
            pv["originX"] = p->getOriginX();
            pv["originY"] = p->getOriginY();
            pv["rate"] = p->getRate();

            Json::Value ppv;
            auto points = p->getPoints();
            int kk = 0;
            for (auto p : points) {
                ppv[kk++] = p;
            }
            if(!ppv.isNull()>0)
                pv["points"] = ppv;

            Json::Value ppv2;
            auto paths = p->getPaths();
            int kk2 = 0;
            for (auto p : paths) {
                ppv2[kk2++] = p;
            }
            if(!ppv2.isNull()>0)
                pv["paths"] = ppv2;

            v_floors.append(pv);
        }
        else if (spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Block) {
            MapBlock *p = static_cast<MapBlock *>(spirit);
            Json::Value pv;
            pv["id"] = p->getId();
            pv["name"] = p->getName();

            Json::Value ppv;
            auto ps = p->getSpirits();
            int kk = 0;
            for (auto p : ps) {
                ppv[kk++] = p;
            }
            if(!ppv.isNull()>0)
                pv["spirits"] = ppv;

            v_blocks.append(pv);
        }
        else if (spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Group) {
            MapGroup *p = static_cast<MapGroup *>(spirit);
            Json::Value pv;
            pv["id"] = p->getId();
            pv["name"] = p->getName();
            Json::Value ppv;
            auto ps = p->getSpirits();
            int kk = 0;
            for (auto p : ps) {
                ppv[kk++] = p;
            }
            if(!ppv.isNull()>0)
                pv["spirits"] = ppv;
            Json::Value ppv2;
            auto pps = p->getAgvs();
            kk = 0;
            for (auto p : pps) {
                ppv2[kk++] = p;
            }
            if(!ppv2.isNull()>0)
                pv["agvs"] = ppv2;
            v_groups.append(pv);
        }
    }

    if (v_points.size() > 0)
        request["points"] = v_points;
    if (v_paths.size() > 0)
        request["paths"] = v_paths;
    if (v_floors.size() > 0)
        request["floors"] = v_floors;
    if (v_bkgs.size() > 0)
        request["bkgs"] = v_bkgs;
    if (v_blocks.size() > 0)
        request["blocks"] = v_blocks;
    if (v_groups.size() > 0)
        request["groups"] = v_groups;
    request["maxId"] = onemap->getMaxId();

    requestWaitResponse(request);
}

void MsgCenter::mapLoad()
{
    Json::Value request;
    iniRequsttMsg(request);
    request["todo"] = MSG_TODO_MAP_GET_MAP;
    requestWaitResponse(request);
}
