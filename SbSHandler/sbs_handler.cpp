#include "sbs_handler.h"
#include "network_layer.h"
#include "session_layer.h"
#include "application_layer.h"
#include "sock_util.h"
#include "log_util.h"

SbSHandler::SbSHandler()
{
    LogUtil::Instance()->SetFileName("SoftBusServer");

    _net_layer_ptr = new network_layer();
    _sess_layer_ptr = new session_layer();
    _appl_layer_ptr = new application_layer();

    _appl_layer_ptr->register_lower_layer(_sess_layer_ptr, _net_layer_ptr);
    _sess_layer_ptr->register_lower_layer(_net_layer_ptr);

    connect(_appl_layer_ptr, &application_layer::ui_login_signal, this, &SbSHandler::ui_login_signal, Qt::UniqueConnection);
    connect(_appl_layer_ptr, &application_layer::progress_log_signal, this, &SbSHandler::progress_log_signal, Qt::UniqueConnection);
    connect(_sess_layer_ptr, &session_layer::progress_log_signal, this, &SbSHandler::progress_log_signal, Qt::UniqueConnection);
    connect(_net_layer_ptr, &network_layer::progress_log_signal, this, &SbSHandler::progress_log_signal, Qt::UniqueConnection);
}

SbSHandler::~SbSHandler()
{
    if(_net_layer_ptr){
        delete _net_layer_ptr;
    }

    if(_sess_layer_ptr){
        delete _sess_layer_ptr;
    }

    if(_appl_layer_ptr){
        delete _appl_layer_ptr;
    }
}

bool SbSHandler::start_sbs_service(uint16_t listen_port, EProtocolType proto_type, ESynchronizeType syn_type)
{
    _appl_layer_ptr->set_synchronize_type(syn_type);

    string local_ip = SockUtil::query_local_ip();

    return _net_layer_ptr->start_sbs_rt_service(local_ip.c_str(), listen_port, proto_type);
}

bool SbSHandler::exit_service()
{
    return _net_layer_ptr->stop_sbs_service();
}

QString SbSHandler::get_local_ip()
{
    string ip = SockUtil::query_local_ip();
    return QString(ip.c_str());
}

