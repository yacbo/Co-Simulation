#ifndef SBS_HANDLER_H
#define SBS_HANDLER_H

#include <QObject>
#include "sbshandler_global.h"
#include "xml_message.h"

class network_layer;
class session_layer;
class application_layer;
class SBSHANDLERSHARED_EXPORT SbSHandler : public QObject
{
    Q_OBJECT
public:
    SbSHandler();
    virtual ~SbSHandler();

signals:
    void ui_login_signal(QString dev_name, QString dev_ip, int dev_port, int dev_id, bool login);
    void progress_log_signal(QString log);

public:
    bool start_sbs_service(uint16_t listen_port, EProtocolType proto_type, ESynchronizeType syn_type);
    bool exit_service();

public:
    QString get_local_ip();

private:
    network_layer* _net_layer_ptr;
    session_layer* _sess_layer_ptr;
    application_layer* _appl_layer_ptr;
};

#endif // SBS_HANDLER_H
