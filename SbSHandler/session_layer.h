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
    void check_sim_time_event_slots();                  //定时检查通信事件
    void rcv_lower_slots(QDomElement* elem);      //接收下层数据

public:
    void quit();
    void start_rcv_thread();
    void register_lower_layer(const network_layer* net_layer_ptr);

private:
    void rcv_lower_thread();
    bool check_login(SessionMessageBody* sess_msg);
    bool relay_handle(SessionMessageBody* sess_msg);
    void handle_msg(SessionMessageBody* sess_msg);

private:
    QDomDocument* handle_cfg_comm_param(SessionMessageBody* sess_msg);
    QDomDocument* handle_cfg_power_param(SessionMessageBody* sess_msg);

private:
    bool _quit;
    std::thread _rcv_th;
    SessionMessageBody* _sess_msg_ptr ;

private:
    StrSet _unack_set;                 //未应答集合
    bool check_ack_status(SessionMessageBody* sess_msg);
    StrSet _upper_cond_set;
    bool deliver_to_upper(SessionMessageBody* sess_msg);

    double _cur_sim_time;
    DblVec _event_time_vec;
    ByteArrVec _event_data_vec;
    void insert_event_data(const DblVec& time, const ByteArrVec& data);

    QMutex _mtx;
    QTimer* _event_timer;
    void trans_event_data(QByteArray& data);

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
