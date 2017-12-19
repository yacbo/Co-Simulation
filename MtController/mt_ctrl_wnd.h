#ifndef MT_CTRL_WND_H
#define MT_CTRL_WND_H

#include <QMainWindow>
#include "ms_handler.h"
#include "comm_param_settings.h"
#include "arch_param_settings.h"
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    void init();
private:
    Ui::MainWindow *ui;

//初始化、开始、停止.
private slots:
    void config_Init_event();
    void start_simulation_event();
    void stop_simulation_event();

//注册、注销.
private slots:
    void register_device();
    void unregister_device();

//工具配置.
private slots:
       void elec_info_event();
       void comm_net_note_event();
       void elec_app_event();
// 菜单栏选项.
 private slots:
       void menu_action_event();

private:
       MsHandler* _ms_handler;

private:
    arch_param_settings fm; //架构显示图.
    comm_param_settings* commUI;  //通信控制界面.

 private slots:
    void login_singal_slots(QString dev_name, QString dev_ip, uint16_t dev_port, bool login);
    void progress_log_slots(QString log);
    void ack_comm_cfg_param_slots(PG_RTUI_Base* data);

public:
    //列表序号
    int n =0;
    QStandardItemModel  *model;



};

#endif // MT_CTRL_WND_H
