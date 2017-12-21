
#include <QtNetwork>
#include<QMessageBox>
#include <QFileDialog>
#include "mt_ctrl_wnd.h"
#include "ui_mt_ctrl_wnd.h"
#include "arch_param_settings.h"
#include "comm_type_def.h"
#include "comm_param_settings.h"
#include "xml_util.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    commUI = nullptr;
    ui->setupUi(this);
    this->setStyleSheet("background-image: url(:/bac.jpg)");
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(!commUI)
    {
        commUI->close();
       delete commUI;
    }
}
void MainWindow::init()
{
    _ms_handler = new MsHandler();
    if(!_ms_handler){
        return;
    }

//    将子ui加载到主界面中
    ui->horizontalLayout_2->addWidget(&fm);

    QString ip = _ms_handler->get_local_ip();
    QHostAddress host(ip);
    commUI = new comm_param_settings(host.toIPv4Address(),_ms_handler);
//    "初始化"按钮事件.
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::config_Init_event, Qt::UniqueConnection);
//    "开始仿真"按钮事件.
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::start_simulation_event, Qt::UniqueConnection);
//    "停止仿真"按钮事件.
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::stop_simulation_event, Qt::UniqueConnection);
//    注册按钮事件.
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::register_device, Qt::UniqueConnection);
//    注销按钮事件.
    connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::unregister_device, Qt::UniqueConnection);

//    配置工具事件.
    connect(ui->toolButton, &QPushButton::clicked, this, &MainWindow::elec_info_event, Qt::UniqueConnection);
    connect(ui->toolButton_3, &QPushButton::clicked, this, &MainWindow::comm_net_note_event, Qt::UniqueConnection);
    connect(ui->toolButton_4, &QPushButton::clicked, this, &MainWindow::elec_app_event, Qt::UniqueConnection);
//    菜单选项.
    connect(ui->action_3,&QAction::triggered,this,&MainWindow::menu_action_event, Qt::UniqueConnection);

    connect(_ms_handler, &MsHandler::login_signal, this, &MainWindow::login_singal_slots, Qt::UniqueConnection);

    connect(_ms_handler, &MsHandler::progress_log_signal, this, &MainWindow::progress_log_slots, Qt::UniqueConnection);

    connect(_ms_handler,&MsHandler::ack_comm_cfg_param_signal,this,&MainWindow::ack_comm_cfg_param_slots, Qt::UniqueConnection);

    //初始化显示本地ip
   ui->lineEdit_8->setText(ip);
   ui->lineEdit_9->setText("9999");

   //初始化tableView
   ui->tableView->horizontalHeader()->setStretchLastSection(true); //就是这个地方
   ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
   model = new QStandardItemModel();
   model->setColumnCount(2);  //设为两列.

   model->setHeaderData(0,Qt::Horizontal,"时间");
   model->setHeaderData(1,Qt::Horizontal,"信息");
   ui->tableView->setModel(model);

   //禁用注销按钮
   ui->pushButton_5->setEnabled(false);

   //默认事件同步.
   ui->radioButton->setChecked(true);

   //仿真控制端初值.
   ui->lineEdit->setText("1");
   ui->lineEdit_2->setText("100");
   ui->lineEdit_3->setText("5");
}

//初始化按钮事件.
void MainWindow::config_Init_event()
{
    if(ui->lineEdit_4->text().isEmpty() ||
            ui->lineEdit_3->text().isEmpty() ||
            ui->lineEdit_2->text().isEmpty() ||
            ui->lineEdit->text().isEmpty())
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }
    //初始化按钮.
   ui->pushButton->setEnabled(false);
   ui->lineEdit->setEnabled(false);
   ui->lineEdit_2->setEnabled(false);
   ui->lineEdit_3->setEnabled(false);
   ui->comboBox->setEnabled(false);

   UnionSimConfParam usc_param;
   if(!ui->lineEdit_3->text().isEmpty())
   {
       usc_param.power_ctrl_prd = ui->lineEdit_3->text().toDouble();
   }

   if(!ui->lineEdit_2->text().isEmpty())
   {
       usc_param.fed_sim_time = ui->lineEdit_2->text().toDouble();
   }

   if(!ui->lineEdit->text().isEmpty())
   {
       usc_param.time_prop_step = ui->lineEdit->text().toDouble();
   }

   if(ui->radioButton->isChecked())
   {
       usc_param.syn_type = (ESynchronizeType) 0 ; //事件同步
   }
   if(ui->radioButton_2->isChecked())
   {
       usc_param.syn_type = (ESynchronizeType) 1 ; //时间同步
   }
   usc_param.proto_type =(EProtocolType)ui->comboBox->currentIndex();

   PowerConfParam power_cfg_data;
   QString power_cfg_file = ui->lineEdit_4->text();
   if(XmlUtil::load_PowerSimConfParam_cfg(power_cfg_file.toStdString().c_str(), power_cfg_data)){
       //发送数据power sim parameter config
       _ms_handler->config_power_sim_param(&power_cfg_data);
   }

   CommConfParam comm_cfg_data;
   QString communication_cfg_file = ui->lineEdit_6->text();
   if(XmlUtil::load_CommConfParam_cfg(communication_cfg_file.toStdString().c_str(), comm_cfg_data)){
       //发送数据CommConfParamr config
       _ms_handler->config_comm_sim_param(&comm_cfg_data);
   }

   //发送数据union sim parameter config
   _ms_handler->config_union_sim_param(&usc_param);
}

//开始仿真按钮事件.
void MainWindow::start_simulation_event()
{ 
    ui->pushButton_2->setEnabled(false);
    _ms_handler->exec_sim_oper_cmd(eSimCmd_start_sim);
}

//停止仿真按钮事件.
void MainWindow::stop_simulation_event(){
    ui->pushButton_2->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->lineEdit->setEnabled(true);
    ui->lineEdit_2->setEnabled(true);
    ui->lineEdit_3->setEnabled(true);
    ui->comboBox->setEnabled(true);
    //_ms_handler->exec_sim_oper_cmd(eSimCmd_stop_sim);
}

//注册按钮事件.
void MainWindow::register_device()
{
    if(ui->lineEdit_8->text().isEmpty() ||ui->lineEdit_9->text().isEmpty() )
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查连接配置参数!"));
        return;
    }
    //打印开始注册.
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    model->setItem(n,0,new QStandardItem(current_date));
    model->setItem(n,1,new QStandardItem("starting register..."));
    n++;

    string SBSipStr = ui->lineEdit_8->text().toStdString();
    int SBSport = ui->lineEdit_9->text().toInt();
    string  dev_name = DevNamesSet[eSimDev_sim_controller];
    string ipStr = _ms_handler->get_local_ip().toStdString();  //本机ip.

    //注册
    E_ProtocolType proroltype = E_ProtocolType(ui->comboBox->currentIndex());
    bool ret = _ms_handler->config_device(dev_name.c_str(), 0, ipStr.c_str(), 12321, proroltype);
    if(!ret){
        return;
    }
    ret = _ms_handler->register_device(dev_name.c_str(), 0,  SBSipStr.c_str(), SBSport, proroltype);
    if(!ret){
        return;
    }
}

//注销按钮事件.
void MainWindow::unregister_device()
{
    //打印开始注销.
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    model->setItem(n,0,new QStandardItem(current_date));
    model->setItem(n,1,new QStandardItem("eSimDev_sim_controller starting unregister..."));
    n++;
    const IntMap& devIds = _ms_handler->get_dev_id_map();
    IntMap::const_iterator it =  devIds.find(eSimDev_sim_controller);
    if(it != devIds.cend()){
        int dev_id = it->second;
        bool ret = _ms_handler->unregister(DevNamesSet[eSimDev_sim_controller],dev_id, ui->lineEdit_8->text().toStdString().c_str());
        if(!ret){
            QDateTime current_date = QDateTime::currentDateTime();
            QString date = current_date.toString("yyyy-MM-dd hh:mm:ss");
            model->setItem(n,0,new QStandardItem(date));
            model->setItem(n,1,new QStandardItem("unregister fail."));
            n++;
            string info = "unregister fail";
            qInfo(info.c_str());
        }
    }
}

//电力仿真配置文件.
void MainWindow::elec_info_event()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open CFG File"), "", "*.cfg", 0);
    ui->lineEdit_4->setText(path);
}

//通信网络配置文件.
void MainWindow::comm_net_note_event()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open Xml File"), "", "*.cfg", 0);
    ui->lineEdit_6->setText(path);
}

//电力应用配置文件.
void MainWindow::elec_app_event()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open Xml File"), "", "*.xml", 0);
    ui->lineEdit_7->setText(path);
}

// 菜单栏选项.
void MainWindow::menu_action_event()
{
    commUI->show();
}

void MainWindow::login_singal_slots(QString dev_name, QString dev_ip, uint16_t dev_port, bool login)
{
    //根据sbs传过来的信号,显示在线设备
    ESimDevType  devType = XmlUtil::query_sim_dev_type(dev_name.toStdString().c_str());
    //login为true，登录成功.
    if(login)
    {
        switch(devType)
        {
          case eSimDev_power:
            fm.setElecSoftOnLine();
            fm.setComboBoxCurrentText(dev_name);
            break;
          case eSimDev_communication:
            fm.setCommSoftOnLine();
            fm.setComboBoxCurrentText(dev_name);
            break;
          case eSimDev_power_appl:
            fm.setElecAppSoftOnLine();
            fm.setComboBoxCurrentText(dev_name);
            break;
          case eSimDev_sim_controller:
            if(ui->pushButton_4->isEnabled())//若控制端与SBS没连.
            {
                //禁用注册按钮
                ui->pushButton_4->setEnabled(false);  //禁止注册按钮.
                ui->pushButton_5->setEnabled(true);  //启用注销按钮.
                ui->lineEdit_8->setEnabled(false);  //SBS ip输入禁用.
                ui->lineEdit_9->setEnabled(false); //SBS port输入禁用.
                fm.setSbsSoftOnLine();  //SBS在线设置.
                //
                QDateTime current_date = QDateTime::currentDateTime();
                QString date = current_date.toString("yyyy-MM-dd hh:mm:ss");
                model->setItem(n,0,new QStandardItem(date));
                model->setItem(n,1,new QStandardItem("MainWindow:login_singal_slots, sim_controller register successfully."));
                n++;
            }
          break;
          deafult: break;
        }
    }
    else if(!login)  //login为false，注销成功.
    {
        switch(devType)
        {
        case eSimDev_power:
            fm.setElecSoftOffLine();
            fm.setComboBoxCurrentTextClear(dev_name);
            break;
        case eSimDev_communication:
            fm.setCommSoftOffLine();
            fm.setComboBoxCurrentTextClear(dev_name);
            break;
        case eSimDev_power_appl:
            fm.setElecAppSoftOffLine();
            fm.setComboBoxCurrentTextClear(dev_name);
            break;
        case eSimDev_sim_controller:
            if(!(ui->pushButton_4->isEnabled()))//若控制端与sbs相连.
            {
                //禁用注册按钮
                ui->pushButton_4->setEnabled(true);  //启用注册按钮.
                ui->pushButton_5->setEnabled(false);  //禁止注销按钮.
                ui->lineEdit_8->setEnabled(true);  //SBS ip输入禁用.
                ui->lineEdit_9->setEnabled(true); //SBS port输入禁用.
                fm.setSbsSoftOffLine();  //SBS离线设置.
                fm.setElecSoftOffLine();
                fm.setCommSoftOffLine();
                fm.setElecAppSoftOffLine();
                fm.setComboBoxClear();
                //
                QDateTime current_date = QDateTime::currentDateTime();
                QString date = current_date.toString("yyyy-MM-dd hh:mm:ss");
                model->setItem(n,0,new QStandardItem(date));
                model->setItem(n,1,new QStandardItem("eSimDev_sim_controller unregister OK."));
                n++;
            }
            break;
        }
    }

}


void MainWindow::progress_log_slots(QString log)
{
    QDateTime current_date = QDateTime::currentDateTime();
    QString date = current_date.toString("yyyy-MM-dd hh:mm:ss");
    model->setItem(n,0,new QStandardItem(date));
    model->setItem(n,1,new QStandardItem(log));
    n++;
}

void MainWindow::ack_comm_cfg_param_slots(PG_RTUI_Base* data)
{
    commUI->set_ack_comm_cfg_param(data);
}
