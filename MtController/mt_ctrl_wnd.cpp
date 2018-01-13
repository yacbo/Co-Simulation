
#include <QtNetwork>
#include<QMessageBox>
#include <QFileDialog>
#include "mt_ctrl_wnd.h"
#include "ui_mt_ctrl_wnd.h"
#include "arch_param_settings.h"
#include "comm_type_def.h"
#include "comm_param_settings.h"
#include "xml_util.h"
#include <QPainter>
#include "log_util.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    commUI = nullptr;
    ui->setupUi(this);
    //this->setStyleSheet("background-image: url(:/bac.jpg)");
    this->setStyleSheet("background-color:#444240;");
    init();
}
void MainWindow::paintEvent(QPaintEvent* pEvent)
{
//    QPixmap _pBg;
//    _pBg.load(":/bac.jpg");
//    QPainter painter;
//    painter.begin(this);
//    QSize s =this->geometry().size();
//    painter.drawPixmap(0,0,s.width(),s.height(),_pBg);
//    painter.end();
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
   ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
   model = new QStandardItemModel();
   model->setColumnCount(2);  //设为两列.

   model->setHeaderData(0,Qt::Horizontal,"时间");
   model->setHeaderData(1,Qt::Horizontal,"信息");
   ui->tableView->setModel(model);
   ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
   //禁用注销按钮
   ui->pushButton_5->setEnabled(false);

   //初始化按钮
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);

   //默认事件同步.
   ui->radioButton->setChecked(true);

   //仿真控制端初值.
   ui->lineEdit->setText("1");
   ui->lineEdit_2->setText("100");
   ui->lineEdit_3->setText("5");

   ui->groupBox->setStyleSheet("color:white;border:1px solid black;");
   ui->groupBox_2->setStyleSheet("color:white;border:1px solid black;");
   ui->groupBox_3->setStyleSheet("color:white;border:1px solid black;");
   ui->groupBox_4->setStyleSheet("color:white;border:1px solid black;");
   ui->groupBox_5->setStyleSheet("color:white;border:1px solid black;");
   ui->groupBox_6->setStyleSheet("color:white;border:1px solid black;");

   ui->tableView->horizontalHeader()->setStyleSheet("color:black;");
   ui->tableView->verticalHeader()->setStyleSheet("color:black;");
   ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView {background-color:#444240 } QHeaderView::section {background-color:#444240;color:white;border:1px solid #5c5c5c;}");
   ui->tableView->verticalHeader()->setStyleSheet("QHeaderView {background-color:#444240 } QHeaderView::section {background-color:#444240;color:white;padding-left: 4px;border:1px solid #2c2c2c;}");

   ui->pushButton_4->setStyleSheet("QPushButton{border-radius:5px; background:#696969; color:white;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
   ui->pushButton_5->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);

   //ui->pushButton->setStyleSheet("QPushButton{border-radius:5px; background:#696969; color:white;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
   ui->pushButton->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
   ui->pushButton_2->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
   ui->pushButton_3->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);

   ui->label->setStyleSheet("border:0px");
   ui->label_2->setStyleSheet("border:0px");
   ui->label_3->setStyleSheet("border:0px");
   ui->label_4->setStyleSheet("border:0px");
   ui->label_5->setStyleSheet("border:0px");
   ui->label_6->setStyleSheet("border:0px");
   ui->label_7->setStyleSheet("border:0px");
   ui->label_9->setStyleSheet("border:0px");
   ui->label_10->setStyleSheet("border:0px");
   ui->radioButton->setStyleSheet("border:0px");
   ui->radioButton_2->setStyleSheet("border:0px");
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

    //初始化事件记录
    LogUtil::Instance()->Output(MACRO_LOCAL, "[Init_event]");

    //初始化按钮.
   ui->pushButton->setEnabled(false);
   ui->pushButton_2->setEnabled(true);
   ui->pushButton_3->setEnabled(false);

   ui->pushButton->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
   ui->pushButton_2->setStyleSheet("QPushButton{border-radius:5px; background:#696969; color:white;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
   ui->pushButton_3->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);

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
       usc_param.syn_type = eSynch_event; //事件同步
   }
   else if(ui->radioButton_2->isChecked())
   {
       usc_param.syn_type = eSynch_time ; //时间同步
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
    //开始仿真事件记录
    LogUtil::Instance()->Output(MACRO_LOCAL, "[start_simulation_event]");

    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(true);


    ui->pushButton->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:balck;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
    ui->pushButton_2->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:balck;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
    ui->pushButton_3->setStyleSheet("QPushButton{border-radius:5px; background:#696969; color:white;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
    _ms_handler->exec_sim_oper_cmd(eSimCmd_start_sim);
}

//停止仿真按钮事件.
void MainWindow::stop_simulation_event(){

    //停止仿真事件记录
    LogUtil::Instance()->Output(MACRO_LOCAL, "[stop_simulation_event]");

    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(false);

    ui->lineEdit->setEnabled(true);
    ui->lineEdit_2->setEnabled(true);
    ui->lineEdit_3->setEnabled(true);
    ui->comboBox->setEnabled(true);

    ui->pushButton->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
    ui->pushButton_2->setStyleSheet("QPushButton{border-radius:5px; background:#696969; color:white;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
    ui->pushButton_3->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
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
    progress_log_slots(LogUtil::Instance()->Output(MACRO_LOCAL, "starting register..."));

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
    progress_log_slots(LogUtil::Instance()->Output(MACRO_LOCAL, "eSimDev_sim_controller starting unregister..."));

    const IntMap& devIds = _ms_handler->get_dev_id_map();
    IntMap::const_iterator it =  devIds.find(eSimDev_sim_controller);
    if(it != devIds.cend()){
        int dev_id = it->second;
        bool ret = _ms_handler->unregister(DevNamesSet[eSimDev_sim_controller],dev_id, ui->lineEdit_8->text().toStdString().c_str());
        if(!ret){
            progress_log_slots(LogUtil::Instance()->Output(MACRO_LOCAL, "unregister fail."));
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
            progress_log_slots(LogUtil::Instance()->Output(MACRO_LOCAL, "[device online]"," dev_name:",dev_name.toStdString()," dev_ip:",dev_ip.toStdString()," dev_port:",dev_port));
            break;
          case eSimDev_communication:
            fm.setCommSoftOnLine();
            fm.setComboBoxCurrentText(dev_name);
            progress_log_slots(LogUtil::Instance()->Output(MACRO_LOCAL, "[device online]"," dev_name:",dev_name.toStdString()," dev_ip:",dev_ip.toStdString()," dev_port:",dev_port));
            break;
          case eSimDev_power_appl:
            fm.setElecAppSoftOnLine();
            fm.setComboBoxCurrentText(dev_name);
            progress_log_slots(LogUtil::Instance()->Output(MACRO_LOCAL, "[device online]"," dev_name:",dev_name.toStdString()," dev_ip:",dev_ip.toStdString()," dev_port:",dev_port));
            break;
          case eSimDev_sim_controller:
            if(ui->pushButton_4->isEnabled())//若控制端与SBS没连.
            {
                //禁用注册按钮s
                ui->pushButton_4->setEnabled(false);  //禁止注册按钮.
                ui->pushButton_5->setEnabled(true);  //启用注销按钮.
                ui->pushButton_5->setStyleSheet("QPushButton{border-radius:5px; background:#696969; color:white;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
                ui->pushButton_4->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);

                ui->lineEdit_8->setEnabled(false);  //SBS ip输入禁用.
                ui->lineEdit_9->setEnabled(false); //SBS port输入禁用.
                fm.setSbsSoftOnLine();  //SBS在线设置.

                //使能初始化，禁用开始仿真\停止仿真
                ui->pushButton->setEnabled(true);
                ui->pushButton_2->setEnabled(false);
                ui->pushButton_3->setEnabled(false);
                ui->pushButton->setStyleSheet("QPushButton{border-radius:5px; background:#696969; color:white;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
                ui->pushButton_2->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
                ui->pushButton_3->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);

                //
                progress_log_slots(LogUtil::Instance()->Output(MACRO_LOCAL, "MainWindow:login_singal_slots, sim_controller register successfully."));
                progress_log_slots(LogUtil::Instance()->Output(MACRO_LOCAL, "[sim_controller register success]"," dev_name:",dev_name.toStdString()," dev_ip:",dev_ip.toStdString()," dev_port:",dev_port));
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
            progress_log_slots(LogUtil::Instance()->Output(MACRO_LOCAL, "[device offline]"," dev_name:",dev_name.toStdString()," dev_ip:",dev_ip.toStdString()," dev_port:",dev_port));
            break;
        case eSimDev_communication:
            fm.setCommSoftOffLine();
            fm.setComboBoxCurrentTextClear(dev_name);
            progress_log_slots(LogUtil::Instance()->Output(MACRO_LOCAL, "[device offline]"," dev_name:",dev_name.toStdString()," dev_ip:",dev_ip.toStdString()," dev_port:",dev_port));
            break;
        case eSimDev_power_appl:
            fm.setElecAppSoftOffLine();
            fm.setComboBoxCurrentTextClear(dev_name);
            progress_log_slots(LogUtil::Instance()->Output(MACRO_LOCAL, "[device offline]"," dev_name:",dev_name.toStdString()," dev_ip:",dev_ip.toStdString()," dev_port:",dev_port));
            break;
        case eSimDev_sim_controller:
            if(!(ui->pushButton_4->isEnabled()))//若控制端与sbs相连.
            {
                //禁用注册按钮
                ui->pushButton_4->setEnabled(true);  //启用注册按钮.
                ui->pushButton_5->setEnabled(false);  //禁止注销按钮.
                ui->pushButton_4->setStyleSheet("QPushButton{border-radius:5px; background:#696969; color:white;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
                ui->pushButton_5->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
                ui->lineEdit_8->setEnabled(true);  //SBS ip输入禁用.
                ui->lineEdit_9->setEnabled(true); //SBS port输入禁用.
                fm.setSbsSoftOffLine();  //SBS离线设置.
                fm.setElecSoftOffLine();
                fm.setCommSoftOffLine();
                fm.setElecAppSoftOffLine();
                fm.setComboBoxClear();

                //使能初始化，禁用开始仿真\停止仿真
                ui->pushButton->setEnabled(false);
                ui->pushButton_2->setEnabled(false);
                ui->pushButton_3->setEnabled(false);
                ui->pushButton->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
                ui->pushButton_2->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
                ui->pushButton_3->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);

                //打印日志
                progress_log_slots(LogUtil::Instance()->Output(MACRO_LOCAL, "eSimDev_sim_controller unregister OK."));
                progress_log_slots(LogUtil::Instance()->Output(MACRO_LOCAL, "[sim_controller unregister success]"," dev_name:",dev_name.toStdString()," dev_ip:",dev_ip.toStdString()," dev_port:",dev_port));
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
