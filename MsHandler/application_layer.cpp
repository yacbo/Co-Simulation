
#include <QLibrary>
#include <QDateTime>
#include <QNetworkInterface>

#include "log_util.h"
#include "xml_util.h"
#include "decisionalth.h"
#include "client_proxy.h"
#include "application_layer.h"

application_layer::application_layer()
{
    _quit = false;
    start_rcv_thread();
}

application_layer::~application_layer()
{

}

bool application_layer::parse_event_dll(const char* dev_name, const char* dll_path)
{
    QLibrary parser(dll_path);
    if(!parser.load()){
        return false;
    }

    typedef DecisionAlth* (*Instance)();
    const char* instance_name = "GetInstance";
    Instance get_instance =  (Instance) parser.resolve(instance_name);
    if(!get_instance){
        return false;
    }

    DecisionAlth* decision_alth = get_instance();
    if(!decision_alth){
        return false;
    }

    return true;
}

bool application_layer::config_communication(const char* comm_ip, uint16_t comm_port, uint16_t bss_port, int proto_type, const char* host_ip, uint16_t host_port)
{
    if(!comm_ip){
        return false;
    }

    LogUtil::Instance()->Output(MACRO_LOCAL(application_layer), "ip:",comm_ip, "port:", comm_port, "protocol:", proto_type);

    _comm_tbl._dev_ip = comm_ip;
    _comm_tbl._dev_port = comm_port;
    _comm_tbl._proto_type = (EProtocolType)proto_type;
    _comm_tbl._business_port = bss_port;
    _comm_tbl._comm_host_ip = host_ip;
    _comm_tbl._comm_host_port = host_port;

    return true;
}

bool application_layer::config_sim_device(const char* dev_name, int ss_id,  const char* dll_path)
{
    if(!dev_name){
        return false;
    }

    LogUtil::Instance()->Output(MACRO_LOCAL(application_layer), "device name:",dev_name, "ss_id:", ss_id, "dll path:", dll_path);

    MsDeviceTable tbl;

    tbl._session_id = ss_id;
    tbl._device_state = eDeviceState_unregister;
    _dev_tbl[dev_name] = tbl;

    _dev_type = XmlUtil::query_sim_dev_type(dev_name);
    LogUtil::Instance()->SetFileName(dev_name);

    return parse_event_dll(dev_name, dll_path);
}

bool application_layer::config_sim_device(const char* dev_name, int ss_id,  const char* dev_ip, uint16_t dev_port, EProtocolType type)
{
    if(!dev_name){
        return false;
    }

    LogUtil::Instance()->Output(MACRO_LOCAL(application_layer), "device name:",dev_name, "ss_id:", ss_id, "device ip:", dev_ip, "device port:", dev_port, "protocol:", type);

    MsDeviceTable tbl;

    tbl._session_id = ss_id;
    tbl._device_state = eDeviceState_unregister;

    tbl._dev_ip = dev_ip;
    tbl._dev_port = dev_port;
    tbl._proto_type = type;

    if(dev_name){
       _dev_tbl[dev_name] = tbl;
       _dev_type = XmlUtil::query_sim_dev_type(dev_name);
       LogUtil::Instance()->SetFileName(dev_name);
    }

    return true;
}

bool application_layer::register_device(const char* dev_name,  int ss_id,  const char* sbs_ip, uint16_t sbs_port, EProtocolType type)
{
    if(!dev_name || !sbs_ip){
        return false;
    }

    LogUtil::Instance()->Output(MACRO_LOCAL(application_layer), "Register", "device name:",dev_name, "ss_id:", ss_id, "sbs ip:", sbs_ip, "sbs port:", sbs_port, "protocol:", type);

    //connect SBS
    _sbs_ip = sbs_ip;
    _sbs_port = sbs_port;

    QHostAddress host;
    const MsDeviceTable& tbl = _dev_tbl[dev_name];
    host.setAddress(tbl._dev_ip.c_str());

    emit snd_lower_register_signal(ss_id, _sbs_ip.c_str(), sbs_port, type, tbl._dev_port);
    QString info = LogUtil::Instance()->Output(MACRO_LOCAL(application_layer), "connect SBS:", "ss_id:", ss_id, "sbs ip:", sbs_ip, "sbs port:", sbs_port, "protocol:", type);
    emit progress_log_signal(info);
    QThread::msleep(500);

    //send login request xml
    QDomDocument* xml_reg= XmlUtil::generate_conn_xml(dev_name, host.toIPv4Address(), tbl._dev_port, ss_id, 10000, eSubProcedure_register, eMessage_request);
    emit snd_lower_signal(xml_reg, QString::fromStdString(_sbs_ip), sbs_port);

    info = LogUtil::Instance()->Output(MACRO_LOCAL(application_layer), "Login......:", "ss_id:", ss_id, "device name:",dev_name);
    emit progress_log_signal(info);

    return true;
}

bool application_layer::unregister(const char* dev_name, int ss_id, const char* sbs_ip)
{
    if(!dev_name){
        return false;
    }

    LogUtil::Instance()->Output(MACRO_LOCAL(application_layer), "Unregister", "device name:",dev_name, "ss_id:", ss_id, "sbs ip:", sbs_ip);

    //send logout request xml
    const MsDeviceTable& tbl = _dev_tbl[dev_name];
    QHostAddress host(tbl._dev_ip.c_str());

    QDomDocument* xml_unreg = XmlUtil::generate_conn_xml(dev_name, host.toIPv4Address(), tbl._dev_port, ss_id, 10000, eSubProcedure_unregister, eMessage_request);
    emit snd_lower_signal(xml_unreg, QString::fromStdString(_sbs_ip), _sbs_port);
    QThread::msleep(500);

    QString info = LogUtil::Instance()->Output(MACRO_LOCAL(application_layer), "Logout......:", "device id:", ss_id, "device name:",dev_name);
    emit progress_log_signal(info);

    return true;
}

//comm sim event config
bool application_layer::config_comm_sim_event()
{
    int ss_id = _dev_type_id_tbl[eSimDev_sim_controller];
    QString info = LogUtil::Instance()->Output(MACRO_LOCAL(application_layer), "send communication simulation events to SBS");
    emit progress_log_signal(info);

    QDomDocument* doc = XmlUtil::generate_CommSimEventConf_xml(ss_id, 10000);
    if(!doc){
        info = LogUtil::Instance()->Output(MACRO_LOCAL(application_layer), "generate_CommSimEventConf_xml failed");
        emit progress_log_signal(info);
        return false;
    }

    emit snd_lower_signal(doc, _sbs_ip.c_str(), _sbs_port);

    return true;
}

//comm parameter config
bool application_layer::config_comm_param(const PG_RTUI_Base* param)
{
    if(!param){
        return false;
    }

    int ss_id = _dev_type_id_tbl[eSimDev_sim_controller];
    int ps_id = _dev_type_id_tbl[eSimDev_communication];
    QString info = QString("application_layer:config_comm_param, PG_RTUI, type:%1, ss_id:%2, ps_id:%3").arg(param->type).arg(ss_id).arg(ps_id);
    notify_ui_msg(info);

    QDomDocument* doc = XmlUtil::create_PG_RTUI_xml(ss_id, ps_id, param);
    delete param;

    if(!doc){
        info = QString("application_layer:config_comm_param, create_PG_RTUI_xml failed.");
        notify_ui_msg(info);
        return false;
    }

    emit snd_lower_signal(doc, _sbs_ip.c_str(), _sbs_port);

    return true;
}

//union sim parameter config
bool application_layer::config_union_sim_param(const UnionSimConfParam* param)
{
    int ss_id = _dev_type_id_tbl[eSimDev_sim_controller];

    QString info = QString("application_layer:config_union_sim_param, UnionSimConfParam, ss_id:%1").arg(ss_id);
    qInfo(info.toStdString().c_str());
    emit progress_log_signal(info);

    QDomDocument* doc = XmlUtil::generate_UnionSimConfParam_xml(ss_id, 10000, param);
    emit snd_lower_signal(doc, _sbs_ip.c_str(), _sbs_port);

    return true;
}

//power sim parameter config
bool application_layer::config_power_sim_param(const PowerConfParam* power_cfg_data)
{
    QString info = QString("application_layer:config_power_sim_param.");
    notify_ui_msg(info);

    int ss_id = _dev_type_id_tbl[eSimDev_sim_controller];
    int ps_id = _dev_type_id_tbl[eSimDev_power];
    QDomDocument* doc = XmlUtil::generate_PowerSimConfParam_xml(ss_id, ps_id, power_cfg_data);
    emit snd_lower_signal(doc, _sbs_ip.c_str(), _sbs_port);

    return true;
}

//comm sim parameter config
bool application_layer::config_comm_sim_param(const CommConfParam* cfg_param)
{
    QString info = QString("application_layer:config_comm_sim_param.");
    notify_ui_msg(info);

    int ss_id = _dev_type_id_tbl[eSimDev_sim_controller];
    int ps_id = _dev_type_id_tbl[eSimDev_communication];
    QDomDocument* doc = XmlUtil::generate_CommSimConfParam_xml(ss_id, ps_id, cfg_param);
    emit snd_lower_signal(doc, _sbs_ip.c_str(), _sbs_port);

    return true;
}

//sim cmd
bool application_layer::exec_sim_oper_cmd(int cmd)
{
    QString info = QString("application_layer:exec_sim_oper_cmd, exec_sim_oper_cmd, cmd:%1").arg(cmd);
    notify_ui_msg(info);

    int ss_id = _dev_type_id_tbl[eSimDev_sim_controller];
    QDomDocument* doc = XmlUtil::generate_sim_cmd_xml(ss_id, 10000, cmd);
    emit snd_lower_signal(doc, _sbs_ip.c_str(), _sbs_port);

    return true;
}

void application_layer::quit()
{
    _quit = true;
    _snd_upper_que.set_completed_flag();
    _appl_msg_que.set_completed_flag();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

const IntMap& application_layer::get_dev_id_map()
{
    return _dev_type_id_tbl;
}

 void application_layer::register_lower_layer(const session_layer* session_layer_ptr, const network_layer* net_layer_ptr)
 {
     //向上层传
     connect(session_layer_ptr, &session_layer::snd_upper_signal, this, &application_layer::rcv_lower_slots, Qt::UniqueConnection);

     connect(session_layer_ptr, &session_layer::snd_upper_type_id_signal, this, &application_layer::rcv_lower_type_id_slots, Qt::UniqueConnection);

     //注册
     connect(this, &application_layer::snd_lower_register_signal, session_layer_ptr, &session_layer::rcv_upper_register_slots, Qt::UniqueConnection);

     //注销
     connect(this, &application_layer::snd_lower_unregister_signal, session_layer_ptr, &session_layer::rcv_upper_unregister_slots, Qt::UniqueConnection);

     //向下层发
     connect(this, &application_layer::snd_lower_signal, net_layer_ptr, &network_layer::rcv_upper_slots, Qt::UniqueConnection);

     _net_layer_ptr = const_cast<network_layer*>(net_layer_ptr);
 }

 void application_layer::rcv_lower_slots(ApplMessage* msg)
 {
    _snd_upper_que.push(msg);
 }

 void application_layer::rcv_lower_type_id_slots(IntMap* type_ids)
 {
     _dev_type_id_tbl = *type_ids;
     if(_dev_type == eSimDev_sim_controller){
         IntMap::const_iterator it = _dev_type_id_tbl.cbegin();
         for(; it != _dev_type_id_tbl.cend(); ++it){
             emit login_signal(DevNamesSet[it->first], "", 0, true);
         }
     }
 }

 void application_layer::handle_login(ApplMessage* msg)
 {
    if(msg->_proc_msg->_msg_type == eMessage_notify){
        VariableMsgDataBody* var1 = (VariableMsgDataBody*)msg->_proc_msg->_data_vector[0];
        VariableMsgDataBody* var2 = (VariableMsgDataBody*)msg->_proc_msg->_data_vector[1];

        QString dev_name = var1->_var_value.c_str();
        int login_type = std::stoi(var2->_var_value);
        emit login_signal(dev_name, "", 0, login_type == eSubProcedure_register);
        return;
    }

     if(msg->_proc_msg->_data_vector.empty()){
         delete msg;
         return;
     }

     QString info;
     NetAddrMsgDataBody* net = (NetAddrMsgDataBody*)msg->_proc_msg->_data_vector[0];
     MsDeviceMap::iterator it =  _dev_tbl.find(net->_device_name) ;
     if(it == _dev_tbl.end()){
         info = QString("application_layer::handle_login, but not found device: %1").arg(net->_device_name.c_str());
         qInfo(info.toStdString().c_str());
         delete msg;
         return;
     }

     MsDeviceTable& tbl = it.value();
     tbl._session_id = msg->_u2i;
     tbl._device_state = eDeviceState_ready;

     ESimDevType type = XmlUtil::query_sim_dev_type(net->_device_name.c_str());
     if(type == eSimDev_communication){
         client_proxy* proxy = manager_proxy(msg->_i2u, msg->_u2i);
         bool ret = proxy && proxy->start_sock_service(_comm_tbl._dev_ip.c_str(), _comm_tbl._dev_port, _comm_tbl._proto_type, tbl._dev_port, _comm_tbl._business_port, _comm_tbl._comm_host_ip.c_str(), _comm_tbl._comm_host_port);
         info = QString("application_layer:handle_login, connect communication ") + (ret ? "successfully" : "failed");
         notify_ui_msg(info);
     }

     info = QString("application_layer:handle login, device id: %1, device name: %2, login successfully. ").arg(msg->_u2i).arg(net->_device_name.c_str());
     notify_ui_msg(info);

     emit login_signal(net->_device_name.c_str(), "", net->_device_port, true);

     delete msg;
 }

 void application_layer::handle_logout(ApplMessage* msg)
 {
     if(msg->_proc_msg->_msg_type == eMessage_notify){
         VariableMsgDataBody* var1 = (VariableMsgDataBody*)msg->_proc_msg->_data_vector[0];
         VariableMsgDataBody* var2 = (VariableMsgDataBody*)msg->_proc_msg->_data_vector[1];

         QString dev_name = var1->_var_value.c_str();
         int login_type = std::stoi(var2->_var_value);
         emit login_signal(dev_name, "", 0, false);
         return;
     }

     manager_proxy(msg->_i2u, msg->_u2i, false);
     if(msg->_proc_msg->_data_vector.empty()){
         delete msg;
         return;
     }

     QString info;
     NetAddrMsgDataBody* net = (NetAddrMsgDataBody*)msg->_proc_msg->_data_vector[0];
     MsDeviceMap::iterator it =  _dev_tbl.find(net->_device_name) ;
     if(it == _dev_tbl.end()){
         info = QString("application_layer:handle logout, but not found device: %1").arg(net->_device_name.c_str());
         qInfo(info.toStdString().c_str());
         delete msg;
         return;
     }

     _dev_tbl.remove(net->_device_name);
     emit login_signal(net->_device_name.c_str(), "", net->_device_port, false);

     info = QString("application_layer:handle logout, device id: %1, device name: %2, logout successfully.").arg(msg->_u2i).arg(net->_device_name.c_str());
     notify_ui_msg(info);

     //disconnect SBS
     emit snd_lower_unregister_signal(0, _sbs_ip.c_str(), _sbs_port);

     delete msg;
 }

void application_layer::handle_interoper(ApplMessage* msg)
{
    long msg_type = msg->_proc_msg->_msg_type;
    long proc_type = msg->_proc_msg->_proc_type;

    QString info = QString("application_layer:handle_interoper, proc type: %1, msg name: %2").arg(proc_type).arg(msg->_proc_msg->_msg_name.c_str());
    notify_ui_msg(info);

//    if(msg_type == eMessage_request && proc_type == eSubProcedure_appl_request){
//        ApplProcedureOper* oper = nullptr;
//        if(oper = _appl_proc_tbl.FindProcedureTemplate(msg->_proc_msg->_appl_msg_body->_appl_name.c_str())){
//            QDomDocument* doc = XmlUtil::generate_appl_req_xml(msg->_u2i, msg->_i2u,
//                                                               msg->_proc_msg->_appl_msg_body->_appl_name.c_str(),
//                                                               msg->_proc_msg->_appl_msg_body->_msg_step->_step_vec,
//                                                               eMessage_confirm);

//            emit snd_lower_signal(doc, QString::fromStdString(_sbs_ip), _sbs_port);

//            //创建实例
//            string dev_name = DevNamesSet[_dev_type];
//            oper = _appl_proc_tbl.CreateProcInstance(dev_name.c_str(),
//                                                     msg->_proc_msg->_appl_msg_body->_appl_name,
//                                                     _dev_method_tbl, false);
//            if(oper){
//                client_proxy* proxy = manager_proxy(msg->_i2u, msg->_u2i);
//                proxy->set_appl_proc_oper(oper);
//            }
//        }
//        else{        //mismatch
//            QDomDocument* doc = XmlUtil::generate_appl_req_xml(msg->_u2i, msg->_i2u,
//                                                               msg->_proc_msg->_appl_msg_body->_appl_name.c_str(),
//                                                               msg->_proc_msg->_appl_msg_body->_msg_step->_step_vec,
//                                                               eMessage_error, "mismatch");

//            emit snd_lower_signal(doc, QString::fromStdString(_sbs_ip), _sbs_port);
//        }
//    }
//    else{
        client_proxy* proxy = manager_proxy(msg->_i2u, msg->_u2i);
        emit proxy->ready_rcv_signal(msg);
//    }
}

void application_layer::handle_union_sim_param(ApplMessage* msg)
{
    long proc_type = msg->_proc_msg->_proc_type;
    Q_ASSERT(proc_type == eSubProcedure_cfg_sim_param_data);

    QString info = QString("application_layer:handle_union_sim_param, proc type: %1, msg name: %2").arg(proc_type).arg(msg->_proc_msg->_msg_name.c_str());
    notify_ui_msg(info);

    long len = 0;
    char param[128] = {0};
    if(len = msg->_proc_msg->FetchPayloadData(param) > 0){
        memcpy(&_union_sim_conf_param, param, sizeof(UnionSimConfParam));
    }
}

void application_layer::handle_ack_comm_cfg_param(ApplMessage* msg)
{
    long proc_type = msg->_proc_msg->_proc_type;
    Q_ASSERT(proc_type == eSubProcedure_cfg_communication_data);

    QString info = QString("application_layer:handle_ack_comm_cfg_param, proc type: %1, msg name: %2").arg(proc_type).arg(msg->_proc_msg->_msg_name.c_str());
    notify_ui_msg(info);

    PG_RTUI_Base* data = XmlUtil::parse_PG_RTUI_xml((EPGRTUIType)msg->_pg_rtui_type, msg->_proc_msg->_data_vector);
    if(!data){
        QString info = QString("application_layer:handle_ack_comm_cfg_param, parse_PG_RTUI_xml failed.");
        notify_ui_msg(info);
        return;
    }

    emit ack_comm_cfg_param_signal(data);
}

 client_proxy* application_layer::manager_proxy(int i2u, int u2i, bool bcreate)
 {
     bool b_find = false;
     client_proxy* proxy = nullptr;

     ClientProxyIdMap::iterator it = _cli_proxy_tbl.begin();
     for(; it != _cli_proxy_tbl.end() && !b_find; ++it){
         if(it.value().second == u2i){
             b_find = true;
             break;
         }
     }

     QString tips = !bcreate ? "delete client_proxy" : (b_find ? "invoke client_proxy" : "create client_proxy");
     QString info = QString("application_layer: manager_proxy, : %1").arg(tips);
     emit progress_log_signal(info);

     if(b_find){
         proxy = it.key();
         if(!bcreate){
             _cli_proxy_tbl.erase(it);
             proxy->quit();
             delete proxy;
             proxy = nullptr;
         }
     }
     else if(bcreate){
        proxy = init_proxy(i2u, u2i);
     }

     return proxy;
 }

 client_proxy* application_layer::init_proxy(int i2u, int u2i)
 {
     client_proxy* proxy = new client_proxy(this, _sbs_ip.c_str(),  _sbs_port, _dev_type);
     if(!proxy){
         QString info = QString("application_layer: init_proxy, fail, dst_id(u2i):%1").arg(u2i);
         qInfo(info.toStdString().c_str());
         return nullptr;
     }

     proxy->register_lower_layer(_net_layer_ptr);
     connect(proxy, &client_proxy::ready_rcv_signal, proxy, &client_proxy::rcv_lower_slots, Qt::UniqueConnection);
     connect(proxy, &client_proxy::progress_log_signal, this, &application_layer::progress_log_signal, Qt::UniqueConnection);

     _cli_proxy_tbl[proxy] = std::make_pair(i2u, u2i);

     return proxy;
 }

void application_layer::notify_ui_msg(const QString& info)
{
    emit progress_log_signal(info);
    qInfo(info.toStdString().c_str());
    //LogUtil::Instance()->Output(QtInfoMsg, info);
}

void application_layer::start_rcv_thread()
{
     _rcv_th = std::thread(std::bind(&application_layer::rcv_lower_thread, this));
     _rcv_th.detach();
}

void application_layer::rcv_lower_thread()
{
    ApplMessage* msg = nullptr;
    while(!_quit){
        if(msg = _snd_upper_que.pop(!_quit)){
            handle_msg(msg);
        }
    }
}

void application_layer::handle_msg(ApplMessage* msg)
{
    long proc_type = msg->_proc_msg->_proc_type;
    long msg_type = msg->_proc_msg->_msg_type;

    QString info = QString("application_layer::handle_msg, msg_name:%1, src_id:%2, dst_id:%3, proc_type:%4, msg_type:%5").arg(msg->_proc_msg->_msg_name.c_str()).arg(msg->_i2u).arg(msg->_u2i).arg(proc_type).arg(msg_type);
    notify_ui_msg(info);

    client_proxy* proxy = nullptr;
    if(proc_type != eSubProcedure_register && _dev_type != eSimDev_sim_controller){
        proxy = manager_proxy(msg->_i2u, msg->_u2i);
    }

    switch(proc_type){
    case eSubProcedure_register: handle_login(msg); break;
    case eSubProcedure_unregister: handle_logout(msg); break;
    case eSubProcedure_session_begin:
    case eSubProcedure_session_end:
    case eSubProcedure_appl_request:
    case eSubProcedure_data_send:
    case eSubProcedure_invoke: handle_interoper(msg); break;
    case eSubProcedure_sim_cmd: emit proxy->ready_rcv_signal(msg); break;
    case eSubProcedure_cfg_sim_param_data: handle_union_sim_param(msg); break;
    case eSubProcedure_cfg_power_data:  {
        if(_dev_type == eSimDev_power || _dev_type == eSimDev_power_appl) {
            emit proxy->ready_rcv_signal(msg);
        }
        break;
    }
    case eSubProcedure_cfg_communication_data:{
        if(_dev_type == eSimDev_communication) emit proxy->ready_rcv_signal(msg);
        else if(_dev_type == eSimDev_sim_controller) handle_ack_comm_cfg_param(msg);
        break;
    }
    default: break;
    }
}
