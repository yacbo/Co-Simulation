
#include <QNetworkInterface>
#include "session_layer.h"
#include "xml_util.h"
#include "log_util.h"
#include "pg_rtui_def.h"

#define EPS 1e-7

session_layer::session_layer()
{
    _quit = false;
    _cur_id = 1;
    _local_id = 10000;
    _cur_sim_time = 0.0;
    _sess_msg_ptr = new SessionMessageBody();

    _event_timer = new QTimer();
    connect(_event_timer, &QTimer::timeout, this, &session_layer::check_sim_time_event_slots);
    _event_timer->setInterval(1000);
    _event_timer->start();

    _upper_cond_set.insert("login");
    _upper_cond_set.insert("logout");

    start_rcv_thread();
}

session_layer::~session_layer()
{
    if(_event_timer){
        _event_timer->stop();
        delete _event_timer;
    }

    if(_sess_msg_ptr){
        delete _sess_msg_ptr;
    }
}

void session_layer::quit()
{
    _quit = true;
    _snd_upper_que.set_completed_flag();

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void session_layer::register_lower_layer(const network_layer* net_layer_ptr)
{
    //向上层传
    connect(net_layer_ptr, &network_layer::snd_upper_signal, this, &session_layer::rcv_lower_slots, Qt::UniqueConnection);

    //向下层发
    connect(this, &session_layer::snd_lower_signal, net_layer_ptr, &network_layer::rcv_upper_slots, Qt::UniqueConnection);
}

void session_layer::rcv_lower_slots(QDomElement* elem)
{
   _snd_upper_que.push(elem);
}

void session_layer::start_rcv_thread()
{
    _rcv_th = std::thread(std::bind(&session_layer::rcv_lower_thread, this));
    _rcv_th.detach();
}

void session_layer::rcv_lower_thread()
{
    QDomElement* elem = nullptr;
    while(!_quit){
        if(elem = _snd_upper_que.pop(!_quit)){
            _sess_msg_ptr->XmlElement2Attr(*elem);
            delete elem;

            handle_msg(_sess_msg_ptr);

            QString info = QString("session_layer:rcv_lower_thread,msg_name: %1, ss_id: %2, ps_id: %3, proc_type: %4, msg_type: %5").arg(_sess_msg_ptr->_procedure_msg_body->_msg_name.c_str()).arg(_sess_msg_ptr->_id_i2u).arg(_sess_msg_ptr->_id_u2i).arg(_sess_msg_ptr->_procedure_msg_body->_proc_type).arg(_sess_msg_ptr->_procedure_msg_body->_msg_type);
            LogUtil::Instance()->Output(QtInfoMsg, info);
        }
    }
}

void session_layer::handle_msg(SessionMessageBody* sess_msg)
{
    QString info = QString("session_layer: handle_msg, session msg name: %1").arg(sess_msg->_procedure_msg_body->_msg_name.c_str());
    qInfo(info.toStdString().c_str());
    emit progress_log_signal(info);

    if(!deliver_to_upper(_sess_msg_ptr)){
        return;
    }

    ApplMessage* appl_msg = new ApplMessage();
    appl_msg->_i2u = _sess_msg_ptr->_id_i2u;
    appl_msg->_u2i = _sess_msg_ptr->_id_u2i;
    appl_msg->_proc_msg = _sess_msg_ptr->_procedure_msg_body;

    emit snd_upper_signal(appl_msg);
}

bool session_layer::deliver_to_upper(SessionMessageBody* sess_msg)
{
    //bool ack_flag =  check_ack_status(sess_msg);

    bool ret = true;
    long type = sess_msg->_procedure_msg_body->_proc_type;
    if((type & eProcedure_session) == eProcedure_session || (type & eProcedure_interoper) == eProcedure_interoper){
        if(type == eSubProcedure_register || type == eSubProcedure_unregister){
            check_login(sess_msg);
        }
    }
    else if((type & eProcedure_custom) == eProcedure_custom &&
                type != eSubProcedure_cfg_sim_param_data &&
                type != eSubProcedure_sim_cmd){
        relay_handle(sess_msg);
        ret = false;
    }

    return ret;
}

bool session_layer::check_login(SessionMessageBody* sess_msg)
{
    if(!sess_msg){
        return false;
    }

    if(sess_msg->_procedure_msg_body->_proc_type == eSubProcedure_register){
        if(_session_id_ip_tbl.find(sess_msg->_id_i2u) != _session_id_ip_tbl.end()){
            return false;
        }

        NetAddrMsgDataBody* net =  (NetAddrMsgDataBody*)sess_msg->_procedure_msg_body->_data_vector[0];
        QString id = QString("%1:%2").arg(net->_device_ip).arg(net->_device_port);
        _session_id_ip_tbl[_cur_id] = id.toStdString();
        sess_msg->_id_i2u = _cur_id;                                                      //设置ID

        ESimDevType dev_type = XmlUtil::query_sim_dev_type(net->_device_name.c_str());
        _session_type_id_tbl[dev_type] = _cur_id;
        emit snd_upper_type_id_signal(dev_type, _cur_id);              //发送到应用层，以便通过目标器件类型查找其id
        ++_cur_id;

        QString info = QString("session_layer: check_login, set device id = %1 for %2").arg(_cur_id - 1).arg(net->_device_name.c_str());
        qInfo(info.toStdString().c_str());
        emit progress_log_signal(info);
    }
    else if(sess_msg->_procedure_msg_body->_proc_type == eSubProcedure_unregister){
        IntStrMap::iterator it = _session_id_ip_tbl.find(sess_msg->_id_i2u);
        if(it == _session_id_ip_tbl.end()){
            return false;
        }

        _session_id_ip_tbl.erase(it);

        NetAddrMsgDataBody* net =  (NetAddrMsgDataBody*)sess_msg->_procedure_msg_body->_data_vector[0];
        ESimDevType dev_type = XmlUtil::query_sim_dev_type(net->_device_name.c_str());
        IntMap::iterator it_t = _session_type_id_tbl.find(dev_type);
        if(it_t != _session_type_id_tbl.end()){
            _session_type_id_tbl.erase(it_t);
        }

        QString info = QString("session_layer: check_login, remove device: %1").arg(net->_device_name.c_str());
        qInfo(info.toStdString().c_str());
        emit progress_log_signal(info);
    }

    return true;
}

bool session_layer::relay_handle(SessionMessageBody* sess_msg)
{
    IntStrMap::const_iterator it = _session_id_ip_tbl.find(sess_msg->_id_u2i);
    if(it == _session_id_ip_tbl.cend()){
        return false;
    }

    QDomDocument* doc = nullptr;
    if(sess_msg->_procedure_msg_body->_proc_type == eSubProcedure_cfg_power_data){
        doc = handle_cfg_power_param(sess_msg);
    }
    else if(sess_msg->_procedure_msg_body->_proc_type == eSubProcedure_cfg_communication_data){
        doc = handle_cfg_comm_param(sess_msg);
    }
    else{
        RootMessageBody root;
        root._session_msg_body = sess_msg;
        root._session_msg_body->_procedure_msg_body->_msg_type = eMessage_confirm;
        doc = root.Attr2Document();
    }

    if(doc){
        QStringList id = QString(it->second.c_str()).split(':');
        QString ip = id.at(0);
        QString port = id.at(1);
        QHostAddress dst(ip.toULong());
        emit snd_lower_signal(doc, dst.toString(), port.toUShort());
    }

    return true;
}

QDomDocument* session_layer::handle_cfg_power_param(SessionMessageBody* sess_msg)
{
    int ss_id = sess_msg->_id_i2u;
    int ps_id = sess_msg->_id_u2i;

    QString info = QString("session_layer: handle_cfg_power_param, transmmit power config data, ss_id: %1, ps_id: %2, data type: %3").arg(ss_id).arg(ps_id).arg(sess_msg->_msg_type);
    qInfo(info.toStdString().c_str());
    emit progress_log_signal(info);

    QDomDocument* doc = nullptr;
    PowerConfParam power_cfg_data;
    if(XmlUtil::parse_PowerSimConfParam_xml(sess_msg->_procedure_msg_body->_data_vector, power_cfg_data)){
        doc = XmlUtil::generate_PowerSimConfParam_xml(ss_id, ps_id, &power_cfg_data);
    }

    return doc;
}

QDomDocument* session_layer::handle_cfg_comm_param(SessionMessageBody* sess_msg)
{
    int ss_id = sess_msg->_id_i2u;
    int ps_id = sess_msg->_id_u2i;

    QString info = QString("session_layer: handle_cfg_comm_param, transmmit communication config data, ss_id: %1, ps_id: %2, data type: %3").arg(ss_id).arg(ps_id).arg(sess_msg->_msg_type);
    qInfo(info.toStdString().c_str());
    emit progress_log_signal(info);

    if(sess_msg->_msg_type ==  ePG_comm_sim_event_data){
        DblVec time; ByteArrVec data;
        if(XmlUtil::parse_CommSimEventConf_xml(sess_msg->_procedure_msg_body->_data_vector, data, time)){
            insert_event_data(time, data);
        }
        return nullptr;
    }
    else if(sess_msg->_procedure_msg_body->_proc_type == eSubProcedure_sim_time_notify_data){
        VariableMsgDataBody* var = (VariableMsgDataBody*)sess_msg->_procedure_msg_body->_data_vector[0];
        _cur_sim_time = std::stod(var->_var_value);
        return nullptr;
    }

    RootMessageBody root;
    root._session_msg_body = sess_msg;
    QDomDocument* doc = root.Attr2Document();
    return doc;
}

void session_layer::insert_event_data(const DblVec& time, const ByteArrVec& data)
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

void session_layer::check_sim_time_event_slots()
{
    int index = -1;
    for(int i=0; i<_event_time_vec.size(); ++i){
        double diff = _event_time_vec[i] - _cur_sim_time;
        if(diff > -EPS && diff < 1.0){
            index = i;
            break;
        }
    }

    _mtx.lock();
    for(int i=0; i<index; ++i){

    }
    _mtx.unlock();
}

void session_layer::trans_event_data(QByteArray& data)
{
    if(data.length() == 0){
        return;
    }

    char param[1024] = {0};
    memcpy(param, data.data(), data.length());
    int ss_id = 10000;
    int ps_id = _session_type_id_tbl[eSimDev_communication];

    const int offset_type = sizeof(LocalAddr);
    uint16_t type = data.mid(offset_type, sizeof(uint16_t)).toUShort();

    QDomDocument* doc = nullptr;
    switch(type){
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

    //return doc;
}

bool session_layer::check_ack_status(SessionMessageBody* sess_msg)
{
    bool ack_flag = true;
    char tmp[128] = {0};
    const char* fmt = "%d:%d:%d";

    switch(sess_msg->_procedure_msg_body->_msg_type){
    case eMessage_request:{
        sprintf_s(tmp, fmt, sess_msg->_id_u2i, sess_msg->_id_i2u, sess_msg->_procedure_msg_body->_proc_type);
        _unack_set.insert(tmp);
        break;
    }
    case eMessage_confirm:
    case eMessage_error:{
        sprintf_s(tmp, fmt, sess_msg->_id_i2u, sess_msg->_id_u2i, sess_msg->_procedure_msg_body->_proc_type);
        StrSet::iterator it = _unack_set.find(tmp);
        if(it != _unack_set.end()){
            _unack_set.erase(it);
        }
        else{
            ack_flag = false;
        }
        break;
    }
    default: break;
    }

    return ack_flag;
}



