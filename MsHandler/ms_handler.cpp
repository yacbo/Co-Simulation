
#include "ms_handler.h"
#include "network_layer.h"
#include "session_layer.h"
#include "application_layer.h"
#include "sock_util.h"

MsHandler::MsHandler()
{
    _net_layer_ptr = new network_layer();
    _sess_layer_ptr = new session_layer();
    _appl_layer_ptr = new application_layer();

    _appl_layer_ptr->register_lower_layer(_sess_layer_ptr, _net_layer_ptr);
    _sess_layer_ptr->register_lower_layer(_net_layer_ptr);

    connect(_appl_layer_ptr, &application_layer::login_signal, this, &MsHandler::login_signal, Qt::UniqueConnection);
    connect(_appl_layer_ptr, &application_layer::progress_log_signal, this, &MsHandler::progress_log_signal, Qt::UniqueConnection);
    connect(_appl_layer_ptr, &application_layer::ack_comm_cfg_param_signal, this, &MsHandler::ack_comm_cfg_param_signal, Qt::UniqueConnection);
    connect(_sess_layer_ptr, &session_layer::progress_log_signal, this, &MsHandler::progress_log_signal, Qt::UniqueConnection);
    connect(_net_layer_ptr, &network_layer::progress_log_signal, this, &MsHandler::progress_log_signal, Qt::UniqueConnection);
}

MsHandler::~MsHandler()
{
//    if(_net_layer_ptr){
//        delete _net_layer_ptr;
//    }

//    if(_sess_layer_ptr){
//        delete _sess_layer_ptr;
//    }

    if(_appl_layer_ptr){
        delete _appl_layer_ptr;
    }
}

bool MsHandler::config_communication(const char* comm_ip, uint16_t comm_port, uint16_t bss_port, int protp_type, const char* host_ip, uint16_t host_port)
{
    return _appl_layer_ptr->config_communication(comm_ip, comm_port, bss_port, protp_type, host_ip, host_port);
}

bool MsHandler::config_device(const char* dev_name, uint32_t ss_id,  const char* dll_path)
{
    return _appl_layer_ptr->config_sim_device(dev_name, ss_id, dll_path);
}

bool MsHandler::config_device(const char* dev_name, uint32_t ss_id,  const char* dev_ip, uint16_t dev_port, int type)
{
    return _appl_layer_ptr->config_sim_device(dev_name, ss_id, dev_ip, dev_port, (EProtocolType)type);
}

bool MsHandler::register_device(const char* dev_name, uint32_t ss_id, const char* sbs_ip, uint16_t sbs_port, int type)
{
    return _appl_layer_ptr->register_device(dev_name, ss_id, sbs_ip, sbs_port, (EProtocolType)type);
}

bool MsHandler::unregister(const char* dev_name, uint32_t ss_id, const char* sbs_ip)
{
    return _appl_layer_ptr->unregister(dev_name, ss_id, sbs_ip);
}

QString MsHandler::get_local_ip()
{
    string ip = SockUtil::query_local_ip();
    return QString(ip.c_str());
}

const IntMap& MsHandler::get_dev_id_map()
{
    return _appl_layer_ptr->get_dev_id_map();
}

bool MsHandler::config_comm_sim_event()
{
    return _appl_layer_ptr->config_comm_sim_event();
}

//comm parameter config PG_RTUI
bool MsHandler::config_comm_param(const PG_RTUI_Base* param)
{
    return _appl_layer_ptr->config_comm_param(param);
}

//union sim parameter config
bool MsHandler::config_union_sim_param(const UnionSimConfParam* param)
{
    return _appl_layer_ptr->config_union_sim_param(param);
}

//power sim parameter config
bool MsHandler::config_power_sim_param(const PowerConfParam* power_cfg_data)
{
    return _appl_layer_ptr->config_power_sim_param(power_cfg_data);
}

//comm sim parameter config
bool MsHandler::config_comm_sim_param(const CommConfParam* cfg_param)
{
    return _appl_layer_ptr->config_comm_sim_param(cfg_param);
}

//sim cmd
bool MsHandler::exec_sim_oper_cmd(int cmd)
{
    return _appl_layer_ptr->exec_sim_oper_cmd(cmd);
}

bool MsHandler::test_send_xml(const QString& dst_ip, int port, const char* packet, int len)
{
    return _net_layer_ptr->test_send_xml(dst_ip, port, packet, len);
}


