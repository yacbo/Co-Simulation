#ifndef NETWORK_LAYER_H
#define NETWORK_LAYER_H

#include <QObject>
#include "sock_util.h"
#include "data_queue.h"
#include "xml_message.h"

class network_layer : public QObject
{
    Q_OBJECT
public:
    network_layer();
    virtual ~network_layer();

signals:
    void snd_upper_signal(QDomElement* elem);            //向上层发送数据
    void progress_log_signal(QString log);

public slots:
    void rcv_upper_slots(QDomDocument* doc, QString dst_ip, uint16_t port);               //接收上层数据
    void rcv_upper_register_slots(QString sbs_ip, int sbs_port,  int proto_type, int dev_port);
    void rcv_upper_unregister_slots(QString sbs_ip, int port);

public:
    void start_snd_rcv_thread();

public:
    void quit();
    void register_lower_layer(SockUtil* sock_layer_ptr);

private:
    void rcv_lower_thread();
    void snd_lower_thread();

public:
    void rcv_lower_info_callback(const char* data, int len);
    bool test_send_xml(const QString& dst_ip, int port, const char* packet, int len);

private:
    typedef  data_queue<QDomDocument*> DocQue;
    DocQue _rcv_upper_doc_que;
    typedef data_queue<const char*> StrQue;
    StrQue _snd_upper_que;
    typedef data_queue<QString> QStrQue;
    QStrQue _rcv_upper_ip_que;

private:
    bool _quit;
    std::thread _snd_th;
    std::thread _rcv_th;
    EProtocolType _protocol_type;

    SockUtil* _sock_layer_ptr;
    XmlFrameUtil* _xml_frm_util_ptr;
};

#endif // NETWORK_LAYER_H
