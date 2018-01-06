
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
    _sess_msg_ptr = new SessionMessageBody();

    _upper_cond_set.insert("login");
    _upper_cond_set.insert("logout");

    start_rcv_thread();
}

session_layer::~session_layer()
{
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
        if(!(elem = _snd_upper_que.pop(!_quit))){
            continue;
        }

        _sess_msg_ptr->XmlElement2Attr(*elem);
        delete elem;

        handle_msg(_sess_msg_ptr);
    }
}

void session_layer::handle_msg(SessionMessageBody* sess_msg)
{
    long msg_type = sess_msg->_procedure_msg_body->_msg_type;
    long proc_type =  sess_msg->_procedure_msg_body->_proc_type;

    LogUtil::Instance()->Output(MACRO_LOCAL, "Handle Message",
                                "msg_name:", sess_msg->_procedure_msg_body->_msg_name,
                                "ss_id:", sess_msg->_id_i2u,
                                "ps_id:", sess_msg->_id_u2i,
                                "proc_type:", parse_type(proc_type),
                                "msg_type:", parse_type(msg_type));

    if(!deliver_to_upper(_sess_msg_ptr)){
        return;
    }

    ApplMessage* appl_msg = new ApplMessage();
    appl_msg->_i2u = _sess_msg_ptr->_id_i2u;
    appl_msg->_u2i = _sess_msg_ptr->_id_u2i;
    appl_msg->_pg_rtui_type = _sess_msg_ptr->_msg_type;
    appl_msg->_proc_msg = _sess_msg_ptr->_procedure_msg_body;

    emit snd_upper_signal(appl_msg);
}

bool session_layer::deliver_to_upper(SessionMessageBody* sess_msg)
{
    bool ret = true;
    long proc_type = sess_msg->_procedure_msg_body->_proc_type;
    if(proc_type == eSubProcedure_register || proc_type == eSubProcedure_unregister){
        ret = check_login(sess_msg);
    }
    else if(proc_type != eSubProcedure_cfg_sim_param_data && proc_type != eSubProcedure_sim_cmd &&
               proc_type != eSubProcedure_sim_time_notify_data){
        ret = !relay_handle(sess_msg);
    }

    return ret;
}

bool session_layer::check_login(SessionMessageBody* sess_msg)
{
    if(!sess_msg){
        LogUtil::Instance()->Output(MACRO_LOCAL, "null pointer");
        return false;
    }

    LogUtil::Instance()->Output(MACRO_LOCAL);

    long proc_type = sess_msg->_procedure_msg_body->_proc_type;
    if(proc_type == eSubProcedure_register){
        NetAddrMsgDataBody* net =  (NetAddrMsgDataBody*)sess_msg->_procedure_msg_body->_data_vector[0];
        QString id = QString("%1:%2").arg(net->_device_ip).arg(net->_device_port);

        IntStrMap::iterator it = _session_id_ip_tbl.begin();
        for(; it != _session_id_ip_tbl.end(); ++it){
            if(it->second == id.toStdString()){
                return false;
            }
        }

        _session_id_ip_tbl[_cur_id] = id.toStdString();
        sess_msg->_id_i2u = _cur_id;                                                      //设置ID

        ESimDevType dev_type = XmlUtil::query_sim_dev_type(net->_device_name.c_str());
        _session_type_id_tbl[dev_type] = _cur_id;
        emit snd_upper_type_id_signal(dev_type, _cur_id);              //发送到应用层，以便通过目标器件类型查找其id

        QString info = LogUtil::Instance()->Output(MACRO_LOCAL, "Set [", net->_device_name, "] device id:", _cur_id++);
        emit progress_log_signal(info);
    }
    else if(proc_type == eSubProcedure_unregister){
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

        QString info = LogUtil::Instance()->Output(MACRO_LOCAL, "Remove device [", net->_device_name, "]");
        emit progress_log_signal(info);
    }

    return true;
}

bool session_layer::relay_handle(SessionMessageBody* sess_msg)
{
    LogUtil::Instance()->Output(MACRO_LOCAL);

    IntStrMap::const_iterator it = _session_id_ip_tbl.find(sess_msg->_id_u2i);
    if(it == _session_id_ip_tbl.cend()){
        LogUtil::Instance()->Output(MACRO_LOCAL, "not found device id:", sess_msg->_id_u2i);
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
        doc = root.Attr2Document();
    }

    if(doc){
        QStringList id = QString(it->second.c_str()).split(':');
        QString ip = id.at(0);
        QString port = id.at(1);
        QHostAddress dst(ip.toULong());
        emit snd_lower_signal(doc, dst.toString(), port.toUShort());
    }
    else{
        LogUtil::Instance()->Output(MACRO_LOCAL, "generate QDomDocument failed, current proc_type:", parse_type(sess_msg->_procedure_msg_body->_proc_type));
    }

    return doc != nullptr;
}

QDomDocument* session_layer::handle_cfg_power_param(SessionMessageBody* sess_msg)
{
    int ss_id = sess_msg->_id_i2u;
    int ps_id = sess_msg->_id_u2i;
    int proc_type = sess_msg->_procedure_msg_body->_proc_type;

    QString info = LogUtil::Instance()->Output(MACRO_LOCAL, "forward power config data", "ss_id", ss_id, "ps_id", ps_id, "proc_type:", proc_type);
    emit progress_log_signal(info);

    QDomDocument* doc = nullptr;
    PowerConfParam power_cfg_data;
    if(XmlUtil::parse_PowerSimConfParam_xml(sess_msg->_procedure_msg_body->_data_vector, power_cfg_data)){
        doc = XmlUtil::generate_PowerSimConfParam_xml(ss_id, ps_id, &power_cfg_data);
        forward_power_cfg_param_to_power_appl(power_cfg_data);            //forward to power appl
    }
    else{
        LogUtil::Instance()->Output(MACRO_LOCAL, "parse power config data failed");
    }

    return doc;
}

bool session_layer::forward_power_cfg_param_to_power_appl(const PowerConfParam& param)
{
    LogUtil::Instance()->Output(MACRO_LOCAL);

    IntMap::const_iterator it_id = _session_type_id_tbl.find(eSimDev_power_appl);
    if(it_id == _session_type_id_tbl.cend()){
        LogUtil::Instance()->Output(MACRO_LOCAL, "not found power application");
        return false;
    }

    IntStrMap::const_iterator it_ip = _session_id_ip_tbl.find(it_id->second);
    if(it_ip == _session_id_ip_tbl.cend()){
        LogUtil::Instance()->Output(MACRO_LOCAL, "not found valid ip of power");
        return false;
    }

    int ss_id = 10000;
    int ps_id = it_id->second;
    QDomDocument* doc = XmlUtil::generate_PowerSimConfParam_xml(ss_id, ps_id, &param);
    if(!doc){
        LogUtil::Instance()->Output(MACRO_LOCAL, "generate power sim config parameter xml failed");
        return false;
    }

    QStringList id = QString(it_ip->second.c_str()).split(':');
    QString ip = id.at(0);
    QString port = id.at(1);
    QHostAddress dst(ip.toULong());
    emit snd_lower_signal(doc, dst.toString(), port.toUShort());

    QString info = LogUtil::Instance()->Output(MACRO_LOCAL, "forward power sim config parameter to power application",
                                               "ss_id", ss_id, "ps_id", ps_id, "proc_type", parse_type(eSubProcedure_cfg_power_data));
    emit progress_log_signal(info);
}

QDomDocument* session_layer::handle_cfg_comm_param(SessionMessageBody* sess_msg)
{
    int ss_id = sess_msg->_id_i2u;
    int ps_id = sess_msg->_id_u2i;
    int proc_type = sess_msg->_procedure_msg_body->_proc_type;

    if(sess_msg->_msg_type ==  ePG_comm_sim_event_data){
        LogUtil::Instance()->Output(MACRO_LOCAL, "communication sim event data");
        return nullptr;
    }

    QString info = LogUtil::Instance()->Output(MACRO_LOCAL, "forward communication config parameter",
                                               "ss_id", ss_id, "ps_id", ps_id, "proc_type", parse_type(proc_type));
    emit progress_log_signal(info);

    RootMessageBody root;
    root._session_msg_body = sess_msg;
    QDomDocument* doc = root.Attr2Document();
    return doc;
}
