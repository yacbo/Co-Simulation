 #ifndef COMM_PARAM_SETTINGS_H
#define COMM_PARAM_SETTINGS_H

#include <QWidget>
#include <QStringListModel>
#include "ms_handler.h"
#include <QStandardItemModel>
namespace Ui {
class comm_param_settings;
}

class comm_param_settings : public QWidget
{
    Q_OBJECT

public:
    explicit comm_param_settings(unsigned int ip, MsHandler* msHandler,QWidget *parent = 0);
    ~comm_param_settings();
public:
    void init();
private:
    Ui::comm_param_settings *ui;

//业务配置.
private slots:
    void business_config_event();
    void business_config_cancel_event();

//业务通道配置.
private slots:
    void business_channel_config_event();
    void business_channel_confirm_event();

//通信节点参数管理.
private slots:
    void comm_note_para_read_event();
    void comm_note_para_config_event();
    void comm_note_para_cancel_event();

//通信链路参数管理.
private slots:
    void comm_link_para_read_event();
    void comm_link_para_config_event();
    void comm_link_para_cancel_event();

//节点故障管理.
private slots:
    void note_breakdown_confirm_event();
    void note_breakdown_cancel_event();
    void note_breakdown_recovery_event();

//链路故障管理.
    private slots:
        void link_breakdown_confirm_event();
        void link_breakdown_cancel_event();
        void link_breakdown_recovery_event();

//恶意路由攻击.
private slots:
        void rout_attack_confirm_event();
        void rout_attack_cancel_event();
        void rout_attack_recovery_event();

//数据篡改攻击.
private slots:
    void data_tamper_confirm_event();
    void data_tamper_cancel_event();
    void data_tamper_recovery_event();

//checkBox单选事件.
private slots:
    void comm_note_para_checkBox_event();

    void comm_link_para_checkBox_event();

    void node_radio_button_event();

    void node_radio_button2_event();

    void link_radio_button3_event();

    void link_radio_button4_event();

private :
    //listView 存放数据
    QStringList current_cloud_id_list;
    QStringListModel *model=new QStringListModel(this);
private:
    unsigned int _ip;
    MsHandler* _msHandler;

private:
    QString _path;   //读取的文本路径

private:
    comm_param_settings* pCommSetUI;

public:
    comm_param_settings* instance();

public:
    void set_ack_comm_cfg_param(PG_RTUI_Base* data);

private slots:
    void combox_select_event(int a);

//事件列表添加事件
private slots:
    void com_node_para_button18_event();
    void com_link_para_button25_event();
    void node_breakdown_button26_event();
    void link_breakdown_button27_event();
    void rout_attack_button28_event();
    void data_tamper_button29_event();

public:
    //列表序号
    int n =0;
    QStandardItemModel  *model_t;
};

#endif // COMM_PARAM_SETTINGS_H
