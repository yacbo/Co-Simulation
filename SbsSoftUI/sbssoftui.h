#ifndef SBSSOFTUI_H
#define SBSSOFTUI_H

#include <QStandardItemModel>
#include <QDialog>
#include "sbs_handler.h"

namespace Ui {
class SbsSoftUI;
}

class SbsSoftUI : public QDialog
{
    Q_OBJECT

public:
    explicit SbsSoftUI(QWidget *parent = 0);
    ~SbsSoftUI();

private slots:
    void start_service();
    void exit_service();

private slots:
    void rcv_reg_slot(QString dev_name, QString dev_ip, int dev_port, int dev_id, bool login);
    void rcv_progress_log_slot(QString log);

private:
    void init();

private:
    Ui::SbsSoftUI *ui;

private:
    SbSHandler* _sbs_handler;

public:
    //列表序号
    int _login_num;
    QStandardItemModel  *_login_model;

    int _log_num;
    QStandardItemModel  *_log_model;

private:
    void paintEvent(QPaintEvent* pEvent);
};

#endif // SBSSOFTUI_H
