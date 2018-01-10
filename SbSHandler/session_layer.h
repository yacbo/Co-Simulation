#ifndef SESSION_LAYER_H
#define SESSION_LAYER_H

#include <QMap>
#include <QObject>
#include <QTimer>
#include <QMutex>
#include "data_queue.h"
#include "xml_message.h"
#include "network_layer.h"
#include "pg_rtui_def.h"

class session_layer : public QObject
{
    Q_OBJECT
public:
    session_layer();
    virtual ~session_layer();

signals:
    void snd_upper_signal(ApplMessage* msg);     //向上层发送数据
    void snd_upper_type_id_signal(int type, int id);
    void snd_lower_signal(QDomDocument* doc, QString dst_ip, quint16 port);
    void progress_log_signal(QString log);

public slots:
    void rcv_lower_slots(QDomElement* elem);      //接收下层数据

public:
    void quit();
    void start_rcv_thread();
    void rcv_lower_thread();
    void register_lower_layer(const network_layer* net_layer_ptr);

private:
    bool check_login(SessionMessageBody* sess_msg);
    bool relay_handle(SessionMessageBody* sess_msg);
    void handle_msg(SessionMessageBody* sess_msg);
    bool forward_power_cfg_param_to_power_appl(const PowerConfParam& param);
    bool forward_power_cfg_param_to_communication(const PowerConfParam& param);

private:
    QDomDocument* handle_cfg_comm_param(SessionMessageBody* sess_msg);
    QDomDocument* handle_cfg_power_param(SessionMessageBody* sess_msg);

private:
    bool _quit;
    std::thread _rcv_th;
    SessionMessageBody* _sess_msg_ptr ;

private:
    StrSet _upper_cond_set;
    bool deliver_to_upper(SessionMessageBody* sess_msg);

private:
    typedef data_queue<QDomElement*> SessionMsgQue;
    SessionMsgQue _snd_upper_que;

private:
    int32_t _local_id;
    IntStrMap _session_id_ip_tbl;

private:
    /*系统全部器件id集合，器件id由SBS分配管理*/
    int _cur_id;
    IntMap _session_type_id_tbl;
};

#endif // SESSION_LAYER_H
