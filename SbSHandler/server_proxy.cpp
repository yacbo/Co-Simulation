
#include <functional>
#include "server_proxy.h"
#include "log_util.h"

server_proxy::server_proxy()
{
    _quit = false;
    _proxy_status = eProxyState_none_ready;
    start_rcv_thread();
}

server_proxy::~server_proxy()
{

}

void server_proxy::quit()
{
    _quit = true;
    _proxy_msg_que.set_completed_flag();

    std::this_thread::sleep_for(std::chrono::milliseconds(250));
}

void server_proxy::register_lower_layer(const network_layer* net_layer_ptr)
{
    //向下层发
    connect(this, &server_proxy::snd_lower_signal, net_layer_ptr, &network_layer::rcv_upper_slots, Qt::UniqueConnection);
}

EProxyState server_proxy::query_state()
{
    return _proxy_status;
}

void server_proxy::rcv_lower_slots(ApplMessage* msg)
{
    _proxy_msg_que.push(msg);
}

void server_proxy::start_rcv_thread()
{
    _rcv_th = std::thread(std::bind(&server_proxy::rcv_lower_thread, this));
    _rcv_th.detach();
}

void server_proxy::handle_msg(ApplMessage* msg)
{
    int proc_type = msg->_proc_msg->_proc_type;
    LogUtil::Instance()->Output(MACRO_LOCAL, parse_type(proc_type));

    Q_ASSERT(msg->_proc_msg->_proc_type == eSubProcedure_session_begin);

    if(msg->_proc_msg->_msg_type == eMessage_request && _proxy_status == eProxyState_none_ready){
        _proxy_status = eProxyState_active_ready;
    }
    else if(msg->_proc_msg->_msg_type == eMessage_confirm && _proxy_status == eProxyState_active_ready){
        _proxy_status = eProxyState_both_ready;
    }
    else if(_proxy_status == eProxyState_both_ready){

    }
}

void server_proxy::rcv_lower_thread()
{
    ApplMessage* msg = nullptr;
    while(!_quit){
       if(msg =  _proxy_msg_que.pop(!_quit)){
           handle_msg(msg);
       }
    }
}
