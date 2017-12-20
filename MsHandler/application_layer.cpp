
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

   DeviceMethodMap method;

    //appl_req
    EvTypeFuncMap appl_req_ev_func_map;
    appl_req_ev_func_map[eEvent_proc_req_snding] = std::bind(&DecisionAlth::ApplReq_ProcedureRequestSendBefore, decision_alth, std::placeholders::_1, std::placeholders::_2);
    appl_req_ev_func_map[eEvent_proc_req_snded] = std::bind(&DecisionAlth::ApplReq_ProcedureRequestSendAfter, decision_alth, std::placeholders::_1, std::placeholders::_2);
    appl_req_ev_func_map[eEvent_proc_req_rcved] = std::bind(&DecisionAlth::ApplReq_ProcedureRequestSendReceived, decision_alth, std::placeholders::_1, std::placeholders::_2);
    appl_req_ev_func_map[eEvent_proc_rep_snded] = std::bind(&DecisionAlth::ApplReq_ProcedureResponseSendAfter, decision_alth, std::placeholders::_1, std::placeholders::_2);
    appl_req_ev_func_map[eEvent_proc_cnf_rcved] = std::bind(&DecisionAlth::ApplReq_ProcedureResponseConfirmReceived, decision_alth, std::placeholders::_1, std::placeholders::_2);
    appl_req_ev_func_map[eEvent_proc_err_rcved] = std::bind(&DecisionAlth::ApplReq_ProcedureResponseErrorReceived, decision_alth, std::placeholders::_1, std::placeholders::_2);
    method[dev_name][eSubProcedure_appl_request] = appl_req_ev_func_map;

    //data send
    EvTypeFuncMap data_snd_ev_func_map;
    data_snd_ev_func_map[eEvent_proc_req_snding] = std::bind(&DecisionAlth::DataSnd_ProcedureRequestSendBefore, decision_alth, std::placeholders::_1, std::placeholders::_2);
    data_snd_ev_func_map[eEvent_proc_req_snded] = std::bind(&DecisionAlth::DataSnd_ProcedureRequestSendAfter, decision_alth, std::placeholders::_1, std::placeholders::_2);
    data_snd_ev_func_map[eEvent_proc_req_rcved] = std::bind(&DecisionAlth::DataSnd_ProcedureRequestSendReceived, decision_alth, std::placeholders::_1, std::placeholders::_2);
    data_snd_ev_func_map[eEvent_proc_rep_snded] = std::bind(&DecisionAlth::DataSnd_ProcedureResponseSendAfter, decision_alth, std::placeholders::_1, std::placeholders::_2);
    data_snd_ev_func_map[eEvent_proc_cnf_rcved] = std::bind(&DecisionAlth::DataSnd_ProcedureResponseConfirmReceived, decision_alth, std::placeholders::_1, std::placeholders::_2);
    data_snd_ev_func_map[eEvent_proc_err_rcved] = std::bind(&DecisionAlth::DataSnd_ProcedureResponseErrorReceived, decision_alth, std::placeholders::_1, std::placeholders::_2);
    method[dev_name][eSubProcedure_data_send] = data_snd_ev_func_map;

    EvTypeFuncMap invoke_ev_func_map;
    invoke_ev_func_map[eEvent_proc_req_snding] = std::bind(&DecisionAlth::Invoke_ProcedureRequestSendBefore, decision_alth, std::placeholders::_1, std::placeholders::_2);
    invoke_ev_func_map[eEvent_proc_req_snded] = std::bind(&DecisionAlth::Invoke_ProcedureRequestSendAfter, decision_alth, std::placeholders::_1, std::placeholders::_2);
    invoke_ev_func_map[eEvent_proc_req_rcved] = std::bind(&DecisionAlth::Invoke_ProcedureRequestSendReceived, decision_alth, std::placeholders::_1, std::placeholders::_2);
    invoke_ev_func_map[eEvent_proc_rep_snded] = std::bind(&DecisionAlth::Invoke_ProcedureResponseSendAfter, decision_alth, std::placeholders::_1, std::placeholders::_2);
    invoke_ev_func_map[eEvent_proc_cnf_rcved] = std::bind(&DecisionAlth::Invoke_ProcedureResponseConfirmReceived, decision_alth, std::placeholders::_1, std::placeholders::_2);
    invoke_ev_func_map[eEvent_proc_err_rcved] = std::bind(&DecisionAlth::Invoke_ProcedureResponseErrorReceived, decision_alth, std::placeholders::_1, std::placeholders::_2);
    method[dev_name][eSubProcedure_invoke] = invoke_ev_func_map;

    _dev_method_tbl.SetDeviceMethodMap(method);

    ProcEventParam param;
    param._in_out_info = "34.983002	1.049125	-6.944657	1.049050	-6.635524	1.028482	-10.326038	1.002328	-10.913132	1.004129	-9.400167	1.006524	-8.716241	0.996019	-10.664953	0.995074	-11.041618	1.027805	-8.713837	1.015317	-6.489169	1.011137	-7.253084	0.998449	-7.329469	1.012411	-7.300055	1.009908	-9.216595	1.013579	-10.483818	1.029744	-9.437223	1.031243	-10.003648	1.028598	-10.619849	1.048117	-5.021838	0.988396	-6.748405	1.028577	-7.287324	1.047898	-2.799310	1.042238	-3.066859	1.035798	-9.488572	1.054605	-5.549691	1.048489	-7.457448	1.034551	-9.782227	1.047588	-3.672972	1.047932	-0.825515	1.048771	-3.681040	0.981474	-0.522239	0.980573	1.751638	0.996967	0.342189	1.012135	-1.530680	1.049923	2.297490	1.062385	5.021262	1.028639	1.429075	1.026046	6.420662	1.030339	-7.092982";
    double dvg_ret[39] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

    ProcedureHandler handler = appl_req_ev_func_map[eEvent_proc_req_snding];
    handler(&param, dvg_ret);

    return true;
}

bool application_layer::config_communication(const char* comm_ip, uint16_t comm_port, uint16_t bss_port, int proto_type, const char* host_ip, uint16_t host_port)
{
    if(!comm_ip){
        return false;
    }

    QString info = QString("application_layer:config_communication, comm_ip:%1, comm_port:%2, proto_type:%3").arg(comm_ip).arg(comm_port).arg(proto_type);
    qInfo(info.toStdString().c_str());

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
    QString info = QString("application_layer:config_sim_device, dev_name:%1, ss_id:%2, dll_path:%3").arg(dev_name).arg(ss_id).arg(dll_path);
    qInfo(info.toStdString().c_str());

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
    QString info = QString("application_layer:config_sim_device, dev_name:%1, ss_id:%2, dev_ip:%3, dev_port:%4, proto_type:%5").arg(dev_name).arg(ss_id).arg(dev_ip).arg(dev_port).arg(type);
    qInfo(info.toStdString().c_str());

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
    QString info = QString("application_layer:register_device, dev_name:%1, ss_id:%2, sbs_ip:%3, sbs_port:%4, proto_type:%5").arg(dev_name).arg(ss_id).arg(sbs_ip).arg(sbs_port).arg(type);
    qInfo(info.toStdString().c_str());

    //connect SBS
    _sbs_ip = sbs_ip;
    _sbs_port = sbs_port;

    QHostAddress host;
    const MsDeviceTable& tbl = _dev_tbl[dev_name];
    host.setAddress(tbl._dev_ip.c_str());

    emit snd_lower_register_signal(ss_id, _sbs_ip.c_str(), sbs_port, type, tbl._dev_port);
    info = QString("application_layer:register_device, connect SBS, ss_id:%1, sbs_ip:%2, sbs_port:%3, proto_type:%4").arg(ss_id).arg(sbs_ip).arg(sbs_port).arg(type);
    qInfo(info.toStdString().c_str());
   // QThread::msleep(500);

    //send login request xml
    QDomDocument* xml_reg= XmlUtil::generate_conn_xml(dev_name, host.toIPv4Address(), tbl._dev_port, ss_id, 10000, eSubProcedure_register, eMessage_request);
    emit snd_lower_signal(xml_reg, QString::fromStdString(_sbs_ip), sbs_port);

    info = QString("application_layer:register_device, device id: %1, device name: %2, Logging in...").arg(ss_id).arg(dev_name);
    notify_ui_msg(info);

    return true;
}

bool application_layer::unregister(const char* dev_name, int ss_id, const char* sbs_ip)
{
    QString info = QString("application_layer:unregister, dev_name:%1, ss_id:%2, sbs_ip:%3").arg(dev_name).arg(ss_id).arg(sbs_ip);
    qInfo(info.toStdString().c_str());

    //send logout request xml
    const MsDeviceTable& tbl = _dev_tbl[dev_name];
    QHostAddress host(tbl._dev_ip.c_str());

    QDomDocument* xml_unreg = XmlUtil::generate_conn_xml(dev_name, host.toIPv4Address(), tbl._dev_port, ss_id, 10000, eSubProcedure_unregister, eMessage_request);
    emit snd_lower_signal(xml_unreg, QString::fromStdString(_sbs_ip), _sbs_port);

    info = QString("application_layer:unregister, device id:%1, device name: %2, Aborting...").arg(ss_id).arg(dev_name);
    notify_ui_msg(info);

    return true;
}

//comm sim event config
bool application_layer::config_comm_sim_event()
{
    int ss_id = _dev_type_id_tbl[eSimDev_sim_controller];
    QString info = QString("application_layer:config_comm_sim_event, snd comm sim event to SBS.");
    notify_ui_msg(info);

    QDomDocument* doc = XmlUtil::generate_CommSimEventConf_xml(ss_id, 10000);
    if(!doc){
        info = QString("application_layer:config_comm_sim_event, generate_CommSimEventConf_xml failed.");
        notify_ui_msg(info);
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
     qInfo(info.toStdString().c_str());
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
    case eSubProcedure_cfg_power_data:  if(_dev_type == eSimDev_power) emit proxy->ready_rcv_signal(msg); break;
    case eSubProcedure_cfg_communication_data:{
        if(_dev_type == eSimDev_communication) emit proxy->ready_rcv_signal(msg);
        else if(_dev_type == eSimDev_sim_controller) handle_ack_comm_cfg_param(msg);
        break;
    }
    default: break;
    }
}
