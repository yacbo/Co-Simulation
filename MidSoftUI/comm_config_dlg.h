#ifndef COMM_CONFIG_DLG_H
#define COMM_CONFIG_DLG_H

#include <QDialog>
#include <inttypes.h>
namespace Ui {
class comm_config_dlg;
}

class comm_config_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit comm_config_dlg(QWidget *parent = 0);
    ~comm_config_dlg();

private:
    Ui::comm_config_dlg *ui;

private slots:
    void choose_dll_event();
    void button_box_ok();

public :
    void init();
    void get_net_info(QString& ip, uint16_t& port, int& proto_type, uint16_t& bss_port,QString& host_ip,uint16_t &host_port);
    void paintEvent(QPaintEvent* pEvent);

private:
    QString _ip;
    uint16_t _port;
    uint16_t _type;              //协议, tcp/udp.
    uint16_t _port_business;    //业务端口.
    QString _host_ip;
    uint16_t _host_port;

};

#endif // COMM_CONFIG_DLG_H
