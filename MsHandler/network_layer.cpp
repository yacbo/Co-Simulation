
#include "client_proxy.h"
#include "application_layer.h"
#include "session_layer.h"
#include "network_layer.h"
#include "xml_util.h"
#include "log_util.h"

network_layer::network_layer()
{
    _quit = false;
    _sock_layer_ptr = new SockUtil();
    register_lower_layer(_sock_layer_ptr);

    _xml_frm_util_ptr = new XmlFrameUtil();
    start_snd_rcv_thread();
}

network_layer::~network_layer()
{
    quit();

    if(_sock_layer_ptr){
        delete _sock_layer_ptr;
        _sock_layer_ptr = nullptr;
    }

    if(_xml_frm_util_ptr){
        delete _xml_frm_util_ptr;
        _xml_frm_util_ptr = nullptr;
    }
}

void network_layer::quit()
{
    _quit = true;
    _snd_upper_que.set_completed_flag();
    _rcv_upper_doc_que.set_completed_flag();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void network_layer::register_lower_layer(SockUtil* sock_layer_ptr)
{
    sock_layer_ptr->register_rcv_callback(std::bind(&network_layer::rcv_lower_info_callback, this, std::placeholders::_1, std::placeholders::_2));
}

void network_layer::rcv_upper_register_slots(QString sbs_ip, int sbs_port,  int proto_type, int dev_port)
{
    _protocol_type = (EProtocolType)proto_type;
    bool ret =_sock_layer_ptr->start_trans_service(sbs_ip, sbs_port, _protocol_type, dev_port);

    QString info = LogUtil::Instance()->Output(MACRO_LOCAL(network_layer), "connect sbs", MACRO_SUCFAIL(ret));
    emit progress_log_signal(info);
}

void network_layer::rcv_upper_unregister_slots(QString sbs_ip, int port)
{
    bool ret = _sock_layer_ptr->stop_trans_service(sbs_ip, port);

    QString info = LogUtil::Instance()->Output(MACRO_LOCAL(network_layer), "disconnect sbs", MACRO_SUCFAIL(ret));
    emit progress_log_signal(info);
}

void network_layer::rcv_lower_info_callback(const char* data, int len)
{
    QString info;
    if(!_xml_frm_util_ptr->VerifyCheckSum(data)){
        info = LogUtil::Instance()->Output(MACRO_LOCAL(network_layer), "VerifyCheckSum failed");
        emit progress_log_signal(info);
        return;
    }

    if(len <= 0){
        len = _xml_frm_util_ptr->GetMsgLength();
    }

    char* dst_data = new char[len];
    memcpy(dst_data, data, len);

    _snd_upper_que.push(dst_data);

    info = LogUtil::Instance()->Output(MACRO_LOCAL(network_layer), "rcv data, len:", len);
    emit progress_log_signal(info);
}

void network_layer::rcv_upper_slots(QDomDocument* doc, QString dst_ip, uint16_t port)
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
        const char* packet = _snd_upper_que.pop(true);
        if(!packet){
            continue;
        }

        QDomDocument* doc = _xml_frm_util_ptr->DataFrame2Xml(packet, eEncode_utf8, 0);
        delete[] packet;

        QDomNodeList node_list = doc->elementsByTagName("session");
        delete doc;

        if(node_list.isEmpty()){
            continue;
        }

        QDomElement* root_elem = new QDomElement(node_list.at(0).toElement());
        emit snd_upper_signal(root_elem);
    }
}

void network_layer::snd_lower_thread()
{
    while(!_quit){
        QDomDocument* doc = _rcv_upper_doc_que.pop(true);
        if(!doc){
            continue;
        }

        QString id = _rcv_upper_ip_que.pop();
        const char* packet = _xml_frm_util_ptr->Xml2DataFrame(doc, eEncode_utf8, _protocol_type);
        delete doc;

        if(!packet){
            continue;
        }

        QStringList ids = id.split(':');
        QString ip = ids.at(0);
        QString port = ids.at(1);
        int pkt_len = _xml_frm_util_ptr->GetMsgLength();
        bool ret = _sock_layer_ptr->send_data(ip, port.toUShort(), packet, pkt_len);
        delete[] packet;

        QString info = LogUtil::Instance()->Output(MACRO_LOCAL(network_layer), "send data", MACRO_SUCFAIL(ret), "len:", pkt_len);
        emit progress_log_signal(info);
    }
}

bool network_layer::test_send_xml(const QString& dst_ip, int port, const char* packet, int len)
{
    return _sock_layer_ptr->send_data(dst_ip, port, packet, len);
}

