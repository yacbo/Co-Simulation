
#include "server_proxy.h"
#include "application_layer.h"
#include "session_layer.h"
#include "network_layer.h"
#include "log_util.h"

network_layer::network_layer()
{
    _quit = false;
    _sock_layer_ptr = new SockUtil();
    _xml_frm_util_ptr = new XmlFrameUtil();

    register_lower_layer(_sock_layer_ptr);

    start_snd_rcv_thread();
}

network_layer::~network_layer()
{
    if(_sock_layer_ptr){
        delete _sock_layer_ptr;
    }

    if(_xml_frm_util_ptr){
        delete _xml_frm_util_ptr;
    }
}

void network_layer::quit()
{
    _quit = true;

    _rcv_upper_doc_que.set_completed_flag();
    _snd_upper_que.set_completed_flag();
    _rcv_upper_ip_que.set_completed_flag();

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

bool network_layer::start_sbs_rt_service(const char* local_ip,  uint16_t listen_port, EProtocolType type)
{
    _protocol_type = type;
    bool ret = _sock_layer_ptr->start_trans_service(local_ip, listen_port, type, 0, false);

    QString info = LogUtil::Instance()->Output(MACRO_LOCAL, "start SBS service", MACRO_SUCFAIL(ret));
    emit progress_log_signal(info);

    return ret;
}

bool network_layer::stop_sbs_service()
{
    bool ret = _sock_layer_ptr->stop_sbs_service();

    QString info = LogUtil::Instance()->Output(MACRO_LOCAL, "stop SBS service", MACRO_SUCFAIL(ret));
    emit progress_log_signal(info);

    return ret;
}

void network_layer::register_lower_layer(SockUtil* sock_layer_ptr)
{
    sock_layer_ptr->register_rcv_callback(std::bind(&network_layer::rcv_lower_info_callback, this, std::placeholders::_1, std::placeholders::_2));
}

void network_layer::rcv_lower_info_callback(const char* data, int len)
{
    QString info;
    if(!_xml_frm_util_ptr->VerifyCheckSum(data)){
        info = LogUtil::Instance()->Output(MACRO_LOCAL, "verify CheckSum failed, data length:", len);
        emit progress_log_signal(info);
        return;
    }

    if(len <= 0){
        len = _xml_frm_util_ptr->GetMsgLength();
    }

    char* dst_data = new char[len];
    memcpy(dst_data, data, len);
    _snd_upper_que.push(dst_data);

    LogUtil::Instance()->Output(MACRO_LOCAL, "receive data, length:", len);
}

void network_layer::rcv_upper_slots(QDomDocument* doc, QString dst_ip, quint16 port)
{
    QString id = QString("%1:%2").arg(dst_ip).arg(port);
    _rcv_upper_ip_que.push(id);
    _rcv_upper_doc_que.push(doc);
}

void network_layer::start_snd_rcv_thread()
{
    _snd_th = std::thread(std::bind(&network_layer::snd_lower_thread, this));
    _snd_th.detach();

    _rcv_th = std::thread(std::bind(&network_layer::rcv_lower_thread, this));
    _rcv_th.detach();
}

void network_layer::rcv_lower_thread()
{
    while(!_quit){
        const char* packet = _snd_upper_que.pop(!_quit);
        if(!packet){
            LogUtil::Instance()->Output(MACRO_LOCAL, "null packet");
            continue;
        }

        QDomDocument* doc = _xml_frm_util_ptr->DataFrame2Xml(packet, eEncode_utf8, 0);
        delete[] packet;

        if(!doc){
            LogUtil::Instance()->Output(MACRO_LOCAL, "data frame to xml failed");
            continue;
        }

        QDomNodeList node_list = doc->elementsByTagName("session");
        delete doc;
        if(node_list.isEmpty()){
            LogUtil::Instance()->Output(MACRO_LOCAL, "not found xml session node");
            continue;
        }

        QDomElement* root_elem = new QDomElement(node_list.at(0).toElement());
        emit snd_upper_signal(root_elem);
    }
}

void network_layer::snd_lower_thread()
{
    while(!_quit){
        QDomDocument* doc = _rcv_upper_doc_que.pop(!_quit);
        if(!doc){
            LogUtil::Instance()->Output(MACRO_LOCAL, "invalid QDomDocument");
            continue;
        }

        const QString& id = _rcv_upper_ip_que.pop();
        const char* packet = _xml_frm_util_ptr->Xml2DataFrame(doc, eEncode_utf8, _protocol_type);
        delete doc;

        int pkt_len = _xml_frm_util_ptr->GetMsgLength();
        if(!pkt_len){
            LogUtil::Instance()->Output(MACRO_LOCAL, "msg length:", pkt_len);
            continue;
        }

        QStringList ids = id.split(':');
        QString ip = ids.at(0);
        QString port = ids.at(1);
        bool ret = _sock_layer_ptr->send_data(ip, port.toUShort(), packet, pkt_len);
        delete[] packet;

        LogUtil::Instance()->Output(MACRO_LOCAL, "send data", MACRO_SUCFAIL(ret), "length:", pkt_len, "dst ip:", ip.toStdString(), "dst port:", port.toUShort());
    }
}

