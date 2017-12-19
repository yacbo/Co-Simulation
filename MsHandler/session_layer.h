#ifndef SESSION_LAYER_H
#define SESSION_LAYER_H

#include <QMap>
#include <QObject>
#include "data_queue.h"
#include "xml_message.h"
#include "network_layer.h"

class session_layer : public QObject
{
    Q_OBJECT
public:
    session_layer();
    virtual ~session_layer();

signals:
    void snd_upper_signal(ApplMessage* msg);     //向上层发送数据
    void snd_upper_type_id_signal(IntMap* type_ids);
    void snd_lower_signal(QDomDocument* doc, QString dst_ip, quint16 dst_port);
    void snd_lower_register_signal(QString sbs_ip, int sbs_port,  int proto_type, int dev_port);
    void snd_lower_unregister_signal(QString sbs_ip, int sbs_port);
    void progress_log_signal(QString log);

public slots:
    void rcv_lower_slots(QDomElement* elem);      //接收下层数据
    void rcv_upper_register_slots(int ss_id, QString sbs_ip, int port,  int proto_type, int dev_port);
    void rcv_upper_unregister_slots(int ss_id, QString sbs_ip, uint16_t port);

public:
    void quit();
    void start_rcv_thread();
    void register_lower_layer(const network_layer* net_layer_ptr);

private:
    void rcv_lower_thread();
    void handle_msg(SessionMessageBody* msg);

private:
    int32_t _local_id;

private:
    bool _quit;
    std::thread _rcv_th;
    SessionMessageBody* _sess_msg_ptr ;

private:
    typedef IntMap TypeIdMap;
    TypeIdMap _session_type_id_tbl;

    typedef data_queue<QDomElement*> SessionMsgQue;
    SessionMsgQue _snd_upper_que;
};

#endif // SESSION_LAYER_H
