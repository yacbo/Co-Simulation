
#include <QLibrary>
#include <QNetworkInterface>
#include "server_proxy.h"
#include "application_layer.h"
#include "xml_util.h"
#include "log_util.h"

#define EPS 1e-7

application_layer::application_layer()
{
    _quit = false;
    LogUtil::Instance()->SetFileName("SbS");

    _cur_sim_time = 0.0;
    _event_timer = new QTimer();
    _event_timer->setInterval(1000);
    connect(_event_timer, &QTimer::timeout, this, &application_layer::check_sim_time_event_slots);
    _event_timer->start();

    start_rcv_thread();
}

application_layer::~application_layer()
{
    if(_event_timer){
        _event_timer->stop();
        delete _event_timer;
    }
}

void application_layer::quit()
{
    _quit = true;
    _snd_upper_que.set_completed_flag();
    _appl_msg_que.set_completed_flag();
    _event_timer->stop();

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

 void application_layer::register_lower_layer(const session_layer* session_layer_ptr, const network_layer* net_layer_ptr)
 {
     //向上层传
     connect(session_layer_ptr, &session_layer::snd_upper_signal, this, &application_layer::rcv_lower_slots, Qt::UniqueConnection);

     connect(session_layer_ptr, &session_layer::snd_upper_type_id_signal, this, &application_layer::rcv_lower_type_id_slots, Qt::UniqueConnection);

     //向下层发
     connect(this, &application_layer::snd_lower_signal, net_layer_ptr, &network_layer::rcv_upper_slots, Qt::UniqueConnection);

     _net_layer_ptr = const_cast<network_layer*>(net_layer_ptr);
 }

 void application_layer::rcv_lower_slots(ApplMessage* msg)
 {
    _snd_upper_que.push(msg);
 }

 void application_layer::rcv_lower_type_id_slots(int type, int id)
 {
      _dev_type_id_tbl[type] = id;
 }

 void application_layer::handle_login(ApplMessage* msg)
 {
     if(!msg){
         return;
     }

     NetAddrMsgDataBody* net = (NetAddrMsgDataBody*)msg->_proc_msg->_data_vector[0];

     QString info = QString("application_layer: handle_login, dev_name: %1, dev_ip: %2, dev_id: %3").arg(net->_device_name.c_str()).arg(net->_device_ip).arg(msg->_i2u);
     emit progress_log_signal(info);

     //confirm
     QDomDocument* xml_confirm = XmlUtil::generate_conn_xml(net->_device_name.c_str(), net->_device_ip, net->_device_port, 10000, msg->_i2u, eSubProcedure_register, eMessage_confirm);
     if(!xml_confirm){
         return;
     }

     SbsDeviceTable tbl;
     tbl._dev_id = msg->_i2u;
     tbl._dev_port = net->_device_port;
     tbl._dev_ip = QHostAddress(net->_device_ip).toString().toStdString();
     _dev_tbl[net->_device_name] = tbl;

     quint16 dst_port = _dev_tbl[net->_device_name]._dev_port;
     const char* dst_ip = _dev_tbl[net->_device_name]._dev_ip.c_str();

     //发送登录确认信息
     emit snd_lower_signal(xml_confirm, dst_ip, dst_port);
     info = QString("application_layer: handle_login, send login confirm info to device: %1").arg(net->_device_name.c_str());
     emit progress_log_signal(info);

     //向UI发送登录信息
     emit ui_login_signal(net->_device_name.c_str(), dst_ip, tbl._dev_port, true);

     //向全部中间件通知设备类型和ID
     IntMap::const_iterator it = _dev_type_id_tbl.cbegin();
     for(; it != _dev_type_id_tbl.cend(); ++it){
         const char* dev_name = DevNamesSet[it->first];
         SbsDeviceMap::const_iterator it_t = _dev_tbl.find(dev_name);
         if(it_t != _dev_tbl.cend()){
             QDomDocument* notify_xml = XmlUtil::generate_notify_dev_id_xml(10000, it->second, _dev_type_id_tbl);
             emit snd_lower_signal(notify_xml, it_t->_dev_ip.c_str(), it_t->_dev_port);

             info = QString("application_layer: handle_login, notify register device type and id to device: %1").arg(dev_name);
             emit progress_log_signal(info);
         }
     }
 }

 void application_layer::handle_logout(ApplMessage* msg)
 {
     if(!msg){
         return;
     }

     NetAddrMsgDataBody* net = (NetAddrMsgDataBody*)msg->_proc_msg->_data_vector[0];
     SbsDeviceMap::iterator it = _dev_tbl.find(net->_device_name.c_str());
     if(it == _dev_tbl.end()){
         QString info = QString("application_layer: handle_logout, not found device: %1").arg(net->_device_name.c_str());
         emit progress_log_signal(info);
         return;
     }

     SbsDeviceTable& tbl = it.value();

     //confirm
     QDomDocument* xml_confirm = XmlUtil::generate_conn_xml(net->_device_name.c_str(), net->_device_ip, net->_device_port, 10000, tbl._dev_id, eSubProcedure_unregister, eMessage_confirm);
     if(!xml_confirm){
         return;
     }

     //发送登出确认信息
     emit snd_lower_signal(xml_confirm, tbl._dev_ip.c_str(), tbl._dev_port);

     //向controller转发登出信息
     const char* ctrl_name = DevNamesSet[eSimDev_sim_controller];
     if(net->_device_name != ctrl_name){
         SbsDeviceMap::iterator it = _dev_tbl.find(ctrl_name);
         if(it != _dev_tbl.end()){
             QDomDocument* notify_xml = XmlUtil::generate_reg_xml(10000, it->_dev_id, net->_device_name.c_str(), eSubProcedure_unregister);
             emit snd_lower_signal(notify_xml, it->_dev_ip.c_str(), it->_dev_port);

             QString info = QString("application_layer: handle_logout, transmmit login msg to controller");
             emit progress_log_signal(info);
         }
     }

     //向UI发送登出信息
     emit ui_login_signal(net->_device_name.c_str(), tbl._dev_ip.c_str(), tbl._dev_port, false);
     _dev_tbl.remove(net->_device_name.c_str());

     ESimDevType dev_type = XmlUtil::query_sim_dev_type(net->_device_name.c_str());
     IntMap::iterator it_i = _dev_type_id_tbl.find(dev_type);
     if(it_i !=  _dev_type_id_tbl.end()){
          _dev_type_id_tbl.erase(it_i);
     }
 }

void application_layer::handle_session(ApplMessage* msg)
{
    long msg_type = msg->_proc_msg->_msg_type;
    long proc_type = msg->_proc_msg->_proc_type;
    string msg_name = msg->_proc_msg->_msg_name;

    ESimDevType dev_type = query_dev_type(msg->_u2i);
    string dev_name = DevNamesSet[dev_type];

    QString info = QString("application_layer: handle_session, dev_name: %1, msg_name:%2, proc_type: %3, msg_type: %4").arg(dev_name.c_str()).arg(msg_name.c_str()).arg(proc_type).arg(msg_type);
    emit progress_log_signal(info);

    SbsDeviceMap::iterator it = _dev_tbl.find(dev_name);
    if(it == _dev_tbl.end()){
        delete msg;
        return;
    }

    QString tips = "session: ";
    const char* err_msg = nullptr;
    if(msg_type == eMessage_error){
        VariableMsgDataBody* var = (VariableMsgDataBody*)msg->_proc_msg->_data_vector[0];
        err_msg = var->_var_value.c_str();
    }

    QDomDocument* doc = nullptr;
    switch(proc_type){
    case eSubProcedure_session_begin:{
        server_proxy* proxy = nullptr;
        if(proxy = manager_proxy(msg->_i2u, msg->_u2i)){
            EProxyState state = proxy->query_state() ;
            emit proxy->ready_rcv_signal(msg);
            if(msg_type == eMessage_confirm){
                it->_device_state = eDeviceState_busy;
            }
        }

        const char* dst_dev = "";
        VariableMsgDataBody* var  = nullptr;
        if(!err_msg){
            var = (VariableMsgDataBody*)msg->_proc_msg->_data_vector[0];
            dst_dev = var->_var_value.c_str();
        }

        tips += QString("session begin, ") + (msg_type == eMessage_request ? "request" : "confirm");
        doc = XmlUtil::generate_session_xml(msg->_i2u, msg->_u2i, dst_dev, proc_type, msg_type, err_msg);
        break;
    }
    case eSubProcedure_session_end:{
        manager_proxy(msg->_i2u, msg->_u2i, false);

        const char* dst_dev = "";
        VariableMsgDataBody* var  = nullptr;
        if(!err_msg){
            var = (VariableMsgDataBody*)msg->_proc_msg->_data_vector[0];
            dst_dev = var->_var_value.c_str();
        }

        tips += QString("session end, ") + (msg_type == eMessage_request ? "request" : "confirm");
        doc = XmlUtil::generate_session_xml(msg->_i2u, msg->_u2i, dst_dev, proc_type, msg_type, err_msg);
        break;
    }
    case eSubProcedure_appl_request: {
        tips += QString("application request, ") + (msg_type == eMessage_request ? "request" : "confirm");
        doc = XmlUtil::generate_appl_req_xml(msg->_i2u, msg->_u2i,
                                                                      msg->_proc_msg->_appl_msg_body->_appl_name.c_str(),
                                                                      msg->_proc_msg->_appl_msg_body->_msg_step->_step_vec,
                                                                      msg->_proc_msg->_msg_type,
                                                                      err_msg
                                                                      );
        break;
    }
    case eSubProcedure_data_send:{
        tips += QString("data send, ") + (msg_type == eMessage_request ? "request" : "confirm");
        doc = XmlUtil::generate_snd_data_xml(msg->_i2u, msg->_u2i,
                                                                      msg->_proc_msg->_appl_msg_body->_appl_name.c_str(),
                                                                      msg->_proc_msg->_appl_msg_body->_appl_step,
                                                                      msg->_proc_msg->_data_vector,
                                                                      msg->_proc_msg->_msg_type
                                                                      );
        break;
    }
    case eSubProcedure_invoke:{
        tips += QString("invoke, ") + (msg_type == eMessage_request ? "request" : "confirm");
        doc = XmlUtil::generate_invoke_xml(msg->_i2u, msg->_u2i,
                                                                  msg->_proc_msg->_func_invoke_body->_name.c_str(),
                                                                  msg->_proc_msg->_appl_msg_body->_appl_name.c_str(),
                                                                  msg->_proc_msg->_appl_msg_body->_appl_step,
                                                                  msg->_proc_msg->_func_invoke_body->_data,
                                                                  msg->_proc_msg->_msg_type,
                                                                  err_msg
                                                                  );
        break;
    }
    default: break;
    }

    if(doc){
       emit snd_lower_signal(doc, it->_dev_ip.c_str(), it->_dev_port);

       info = QString("application_layer: handle_session, %1").arg(tips);
       emit progress_log_signal(info);
    }
}

void application_layer::handle_sim_cmd(ApplMessage* msg)
{
    long msg_type = msg->_proc_msg->_msg_type;

    QString info = QString("application_layer: handle_sim_cmd, cmd: %1").arg(msg_type);
    emit progress_log_signal(info);

    IntMap::const_iterator it = _dev_type_id_tbl.cbegin();
    for(; it != _dev_type_id_tbl.cend(); ++it){
        if(it->first == eSimDev_sim_controller){
            continue;
        }

        const char* dev_name = DevNamesSet[it->first];
        SbsDeviceMap::const_iterator it_t = _dev_tbl.find(dev_name);
        if(it_t != _dev_tbl.cend()){
            QDomDocument* cmd = XmlUtil::generate_sim_cmd_xml(10000, it->second, msg_type);
            emit snd_lower_signal(cmd, it_t->_dev_ip.c_str(), it_t->_dev_port);
        }
    }
}

void application_layer::handle_cfg_sim_param(ApplMessage* msg)
{
    QString info = QString("application_layer: handle_cfg_sim_param");
    emit progress_log_signal(info);

    XmlUtil::parse_UnionSimConfParam_xml(msg->_proc_msg->_data_vector, _sim_conf_param);

    //向各中间件转发联合仿真配置参数
    IntMap::const_iterator it = _dev_type_id_tbl.cbegin();
    for(; it != _dev_type_id_tbl.cend(); ++it){
        if(it->first == eSimDev_sim_controller){
            continue;
        }

        const char* dev_name = DevNamesSet[it->first];
        SbsDeviceMap::const_iterator it_t = _dev_tbl.find(dev_name);
        if(it_t != _dev_tbl.cend()){
            QDomDocument* doc = XmlUtil::generate_UnionSimConfParam_xml(10000, it->second, &_sim_conf_param);
            emit snd_lower_signal(doc, it_t->_dev_ip.c_str(), it_t->_dev_port);
        }
    }
}

void application_layer::set_synchronize_type(ESynchronizeType type)
{
    _synch_type = type;
}

server_proxy* application_layer::init_proxy(int i2u, int u2i)
{
    server_proxy* proxy = new server_proxy();
    if(!proxy){
        QString info = QString("application_layer: init_proxy, fail, dst_id(u2i):%1").arg(u2i);
        emit progress_log_signal(info);
        return nullptr;
    }

    QString info = QString("application_layer: init_proxy, i2u: %1, u2i: %2").arg(i2u).arg(u2i);
    emit progress_log_signal(info);

    proxy->register_lower_layer(_net_layer_ptr);
    connect(proxy, &server_proxy::ready_rcv_signal, proxy, &server_proxy::rcv_lower_slots, Qt::UniqueConnection);
    connect(proxy, &server_proxy::progress_log_signal, this, &application_layer::progress_log_signal, Qt::UniqueConnection);

    _server_proxy_tbl[proxy] = std::make_pair(i2u, u2i);

    return proxy;
}

 server_proxy* application_layer::manager_proxy(int i2u, int u2i, bool bcreate)
 {
     server_proxy* proxy = nullptr;

     bool b_find = false;
     ServerProxyIdMap::const_iterator it = _server_proxy_tbl.cbegin();
     for(; it != _server_proxy_tbl.cend(); ++it){
         if(it.value().second == u2i){
             b_find = true;
             break;
         }
     }

     QString tips = !bcreate ? "delete server_proxy" : (b_find ? "invoke server_proxy" : "create server_proxy");
     QString info = QString("application_layer: manager_proxy, : %1").arg(tips);
     emit progress_log_signal(info);

     if(b_find){
         proxy = it.key();
         if(!bcreate){
             _server_proxy_tbl.remove(proxy);
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

            QString info = QString("client_proxy:rcv_lower_thread, msg_name: %1, ss_id: %2, ps_id: %3, proc_type: %4, msg_type: %5").arg(msg->_proc_msg->_msg_name.c_str()).arg(msg->_i2u).arg(msg->_u2i).arg(msg->_proc_msg->_proc_type).arg(msg->_proc_msg->_msg_type);
            LogUtil::Instance()->Output(QtInfoMsg, info);
        }
    }
}

void application_layer::handle_msg(ApplMessage* msg)
{
    if(!msg){
        return;
    }

    QString info = QString("application_layer: handle_msg, proc_type: %1").arg(msg->_proc_msg->_proc_type);
    emit progress_log_signal(info);

    switch(msg->_proc_msg->_proc_type){
    case eSubProcedure_sim_cmd: handle_sim_cmd(msg); break;
    case eSubProcedure_cfg_sim_param_data: handle_cfg_sim_param(msg); break;
    case eSubProcedure_cfg_communication_data: handle_comm_sim_event(msg); break;
    case eSubProcedure_session_begin:
    case eSubProcedure_appl_request:
    case eSubProcedure_data_send:
    case eSubProcedure_invoke:
    case eSubProcedure_session_end: handle_session(msg); break;
    case eSubProcedure_register: handle_login(msg); break;
    case eSubProcedure_unregister: handle_logout(msg); break;
    default: break;
    }
}

ESimDevType application_layer::query_dev_type(int dev_id)
{
    ESimDevType  dev_type = eSimDev_undef;
    IntMap::const_iterator it = _dev_type_id_tbl.cbegin();
    for(; it != _dev_type_id_tbl.cend(); ++it){
        if(it->second == dev_id){
            dev_type = (ESimDevType)it->first;
            break;
        }
    }

    return dev_type;
}

void application_layer::handle_comm_sim_event(ApplMessage* msg)
{
    int ss_id = msg->_i2u;
    int ps_id = msg->_u2i;

    QString info = QString("application_layer: handle_comm_sim_event, ss_id: %1, ps_id: %2, data type: %3").arg(ss_id).arg(ps_id).arg(msg->_pg_rtui_type);
    emit progress_log_signal(info);

    if(msg->_pg_rtui_type ==  ePG_comm_sim_event_data){
        DblVec time; ByteArrVec data;
        if(XmlUtil::parse_CommSimEventConf_xml(msg->_proc_msg->_data_vector, data, time)){
            insert_event_data(time, data);
        }
    }
    else if(msg->_proc_msg->_proc_type == eSubProcedure_sim_time_notify_data){
        VariableMsgDataBody* var = (VariableMsgDataBody*)msg->_proc_msg->_data_vector[0];
        _cur_sim_time = std::stod(var->_var_value);
    }
}

void application_layer::insert_event_data(const DblVec& time, const ByteArrVec& data)
{
    _event_time_vec.insert(_event_time_vec.end(), time.begin(), time.end());
    _event_data_vec.insert(_event_data_vec.end(), data.begin(), data.end());

    //对事件按时间升序排列
    _mtx.lock();

    int nsize = _event_time_vec.size();
    for(int i=0; i<nsize - 1; ++i){
        for(int j=1; j<nsize; ++j){
            if(_event_time_vec[j] - _event_time_vec[i] > EPS){
                std::swap(_event_time_vec[j], _event_time_vec[i]);
                std::swap(_event_data_vec[j], _event_data_vec[i]);
            }
        }
    }

    _mtx.unlock();
}

void application_layer::check_sim_time_event_slots()
{
    int index = -1;
    for(int i=0; i<_event_time_vec.size(); ++i){
        double diff = _event_time_vec[i] - _cur_sim_time;
        if(diff > -EPS && diff < 1.0){
            index = i;
            break;
        }
    }

    for(int i=0; i<index; ++i){
        forward_event_data(_event_data_vec[i]);
    }

    _mtx.lock();

    DblVec::iterator it_dbl = _event_time_vec.begin();
    DblVec::iterator it_dbl_end = it_dbl + index + 1;
    while(it_dbl != it_dbl_end){
        it_dbl = _event_time_vec.erase(it_dbl);
    }

    ByteArrVec::iterator it_byte = _event_data_vec.begin();
    ByteArrVec::iterator it_byte_end = it_byte + index + 1;
    while(it_byte != it_byte_end){
        it_byte = _event_data_vec.erase(it_byte);
    }

    _mtx.unlock();
}

void application_layer::forward_event_data(QByteArray& data)
{
    if(data.length() == 0){
        return;
    }

    int ss_id = 10000;
    int ps_id = _dev_type_id_tbl[eSimDev_communication];

    char param[1024] = {0};
    memcpy(param, data.data(), data.length());

    const int offset_type = sizeof(LocalAddr);
    uint16_t event_type = data.mid(offset_type, sizeof(uint16_t)).toUShort();

    QDomDocument* doc = nullptr;
    switch(event_type){
    case ePG_RTUI_msg_getinterfacenum:{
        const PG_RTUI_Msg_GetInterfaceNum* pg = (PG_RTUI_Msg_GetInterfaceNum*)param;
        doc = XmlUtil::generate_PG_RTUI_Msg_GetInterfaceNum_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_ack_getinterfacenum:{
        const PG_RTUI_Ack_GetInterfaceNum* pg = (PG_RTUI_Ack_GetInterfaceNum*)param;
        doc = XmlUtil::generate_PG_RTUI_Ack_GetInterfaceNum_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_msg_getinterface:{
        const PG_RTUI_Msg_GetInterface* pg = (PG_RTUI_Msg_GetInterface*)param;
        doc = XmlUtil::generate_PG_RTUI_Msg_GetInterface_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_ack_getinterface:{
        const PG_RTUI_Ack_GetInterface* pg = (PG_RTUI_Ack_GetInterface*)param;
        doc = XmlUtil::generate_PG_RTUI_Ack_GetInterface_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_msg_setinterface:{
        const PG_RTUI_Msg_SetInterface* pg = (PG_RTUI_Msg_SetInterface*)param;
        doc = XmlUtil::generate_PG_RTUI_Msg_SetInterface_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_msg_getlink:{
        const PG_RTUI_Msg_GetLink* pg = (PG_RTUI_Msg_GetLink*)param;
        doc = XmlUtil::generate_PG_RTUI_GetLink_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_ack_getlink:{
        const PG_RTUI_Ack_GetLink* pg = (PG_RTUI_Ack_GetLink*)param;
        doc = XmlUtil::generate_PG_RTUI_Ack_GetLink_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_msg_setlink:{
        const PG_RTUI_Msg_SetLink* pg = (PG_RTUI_Msg_SetLink*)param;
        doc = XmlUtil::generate_PG_RTUI_Msg_SetLink_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_recover_node_status:
    case ePG_RTUI_change_node_status:{
        const PG_RTUI_ChangeNodeStatus* pg = (PG_RTUI_ChangeNodeStatus*)param;
        doc = XmlUtil::generate_PG_RTUI_ChangeNodeStatus_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_recover_port_status:
    case ePG_RTUI_change_port_status:{
        const PG_RTUI_ChangePortStatus* pg = (PG_RTUI_ChangePortStatus*)param;
        doc = XmlUtil::generate_PG_RTUI_ChangePortStatus_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_static_route:{
        const PG_RTUI_StaticRoute* pg = (PG_RTUI_StaticRoute*)param;
        doc = XmlUtil::generate_PG_RTUI_StaticRoute_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_add_cbr:{
        const PG_RTUI_AddCBR* pg = (PG_RTUI_AddCBR*)param;
        doc = XmlUtil::generate_PG_RTUI_AddCBR_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_set_data_tamper_sim_time:{
        const PG_RTUI_SetDataTamperSimTime* pg = (PG_RTUI_SetDataTamperSimTime*)param;
        doc = XmlUtil::generate_PG_RTUI_SetDataTamperSimulationTime_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_set_data_tamper_last_time:{
        const PG_RTUI_SetDataTamperLastTime* pg = (PG_RTUI_SetDataTamperLastTime*)param;
        doc = XmlUtil::generate_PG_RTUI_SetDataTamperLastTime_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_stop_data_tamper:{
        const PG_RTUI_StopDataTamper* pg = (PG_RTUI_StopDataTamper*)param;
        doc = XmlUtil::generate_PG_RTUI_StopDataTamper_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_add_staticroute:
    case ePG_RTUI_remove_staticroute:{
            const PG_RTUI_StaticRoute* pg = (PG_RTUI_StaticRoute*)param;
            doc = XmlUtil::generate_PG_RTUI_StaticRoute_xml(ss_id, ps_id, pg);
            break;
        }
    default: break;
    }

    if(doc){
        const char* dev_name = DevNamesSet[eSimDev_communication];
        SbsDeviceMap::const_iterator it = _dev_tbl.find(dev_name);
        if(it == _dev_tbl.cend()){
            delete doc;
            return;
        }

        emit snd_lower_signal(doc, it->_dev_ip.c_str(), it->_dev_port);

        QString info = QString("application_layer: forward_event_data, event type: %1").arg(event_type);
        emit progress_log_signal(info);
    }
}

