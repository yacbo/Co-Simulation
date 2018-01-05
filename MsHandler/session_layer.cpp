
#include "log_util.h"
#include "xml_util.h"
#include "session_layer.h"

session_layer::session_layer()
{
    _quit = false;
    _local_id = 0;
    _sess_msg_ptr = new SessionMessageBody();

    start_rcv_thread();
}

session_layer::~session_layer()
{
    quit();

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

    //注册
    connect(this, &session_layer::snd_lower_register_signal, net_layer_ptr, &network_layer::rcv_upper_register_slots, Qt::UniqueConnection);

    //注销
    connect(this, &session_layer::snd_lower_unregister_signal, net_layer_ptr, &network_layer::rcv_upper_unregister_slots, Qt::UniqueConnection);
}

void session_layer::rcv_lower_slots(QDomElement* elem)
{
   _snd_upper_que.push(elem);
}

void session_layer::rcv_upper_register_slots(int ss_id, QString sbs_ip, int sbs_port,  int proto_type, int dev_port)
{
    _local_id = ss_id;

     //连接SBS服务器
     emit snd_lower_register_signal(sbs_ip, sbs_port, proto_type, dev_port);
}

void session_layer::rcv_upper_unregister_slots(int ss_id, QString sbs_ip, uint16_t port)
{
    _local_id = 0;
    emit snd_lower_unregister_signal(sbs_ip, port);
}

void session_layer::start_rcv_thread()
{
    _rcv_th = std::thread(std::bind(&session_layer::rcv_lower_thread, this));
    _rcv_th.detach();
}

void session_layer::handle_msg(SessionMessageBody* msg)
{
    long msg_type = msg->_procedure_msg_body->_msg_type;
    long proc_type =  msg->_procedure_msg_body->_proc_type;

    bool ret = false;
    if(proc_type == eSubProcedure_dev_type_id_data){
        if(ret = XmlUtil::parse_device_id_type_data(msg->_procedure_msg_body->_data_vector, _session_type_id_tbl)){
            emit snd_upper_type_id_signal(&_session_type_id_tbl);
        }

        QString info = LogUtil::Instance()->Output(MACRO_LOCAL(session_layer), "rcv device type&id_data", MACRO_SUCFAIL(ret));
        emit progress_log_signal(info);

        return;
    }
    else if(proc_type == eSubProcedure_register && msg_type == eMessage_confirm){
        _local_id = msg->_id_u2i;
        QString info = LogUtil::Instance()->Output(MACRO_LOCAL(session_layer), "rcv register confirm msg", parse_type(proc_type), "local id:", _local_id);
        emit progress_log_signal(info);
    }

    ApplMessage* appl_msg = new ApplMessage();
    appl_msg->_i2u = msg->_id_i2u;
    appl_msg->_u2i = msg->_id_u2i;
    appl_msg->_pg_rtui_type = msg->_msg_type;
    appl_msg->_proc_msg = msg->_procedure_msg_body;

    emit snd_upper_signal(appl_msg);
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

        LogUtil::Instance()->Output(MACRO_LOCAL(session_layer), "Ready Handle Message",
                                    "msg_name:", _sess_msg_ptr->_procedure_msg_body->_msg_name,
                                    "ss_id:", _sess_msg_ptr->_id_i2u,
                                    "ps_id:", _sess_msg_ptr->_id_u2i,
                                    "proc_type:", parse_type(_sess_msg_ptr->_procedure_msg_body->_proc_type),
                                    "msg_type:", parse_type((_sess_msg_ptr->_procedure_msg_body->_msg_type)));

        handle_msg(_sess_msg_ptr);
    }
}

