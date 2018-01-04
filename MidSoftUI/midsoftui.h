#ifndef MIDSOFTUI_H
#define MIDSOFTUI_H

#include <QDialog>
#include <QString>
#include "ms_handler.h"
#include "comm_config_dlg.h"
#include <QStandardItemModel>

namespace Ui {
class MidSoftUI;
}

class MidSoftUI : public QDialog
{
    Q_OBJECT

public:
    explicit MidSoftUI(QWidget *parent = 0);
    ~MidSoftUI();

private:
    void init();

private:
    Ui::MidSoftUI *ui;
    MsHandler* _ms_handler;

private:
    int _dev_id;
    QString _dev_ip;
    QString _dev_name;
    ESimDevType _dev_type;

    QString _sbs_ip;
    uint16_t _sbs_port;
    uint16_t _listen_port;

    int _protocol_type;

    comm_config_dlg comm_dlg;

private slots:
    void register_device();
    void unregister_device();
private slots:
    void combox_select_event(int a);

private slots:
    void login_singal_slots(QString dev_name, QString dev_ip, uint16_t dev_port, bool login);
    void progress_log_slots(QString log);
    void paintEvent(QPaintEvent* pEvent);

public:
    //列表序号
    int n =0;
    QStandardItemModel  *model;
};

#endif // MIDSOFTUI_H
