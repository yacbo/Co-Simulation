#include "comm_param_settings.h"
#include "ui_comm_param_settings.h"
#include "pg_rtui_def.h"
#include <QMessageBox>
#include"Qtextstream"
#include <QFile>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include<xml_util.h>


bool flagSendGetInterface =false;

comm_param_settings::comm_param_settings(unsigned int ip , MsHandler* msHandler,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::comm_param_settings)
{
    _ip = ip;
    _msHandler = msHandler;
    this->setStyleSheet("background-image: url(:/bac.jpg)");
    ui->setupUi(this);
    init();
}

comm_param_settings::~comm_param_settings()
{
    delete ui;
}

void comm_param_settings::init()
{
    //0-65535的正则表达.
    QRegExp acIntRE("([0-9])|([1-9][0-9])|([1-9][0-9]{2,3})|([1-5][0-9]{4})|(6[0-4][0-9]{3})|(654[0-9]{2})|(655[0-2][0-9])|(6553[0-5])");
    //long
    QRegExp acLongRE("[0-9]|[1-9][0-9]{1,12}");
    //12H23M23S
    QRegExp acTimeRE("((([1-9])|1([1-9])|(2[0-3]))H(([1-9])|([1-5][0-9]))M(([1-9])|([1-5][0-9]))S)|(([1-9][0-9]{0,2})S)|(([1-9][0-9]{0,2})M([1-9]|([1-5][0-9]))S)");
    //浮点校验
    QRegExp acFloatRE("[0][.][0-9]{1,8}|[1-9][0-9]{0,7}[.][0-9]{1,8}");
    //ip校验
    QRegExp acIPRE("([0-9]{0,3})[.]([0-9]{0,3})[.]([0-9]{0,3})[.][0-9]{0,3}");
    //bool校验
    QRegExp acBoolRE("[0]|[1]");

    //业务配置参数正则.
    ui->lineEdit_35->setValidator(new QRegExpValidator(acIntRE, this));
    ui->lineEdit_36->setValidator(new QRegExpValidator(acIntRE, this));
    ui->lineEdit_37->setValidator(new QRegExpValidator(acLongRE, this));
    ui->lineEdit_42->setValidator(new QRegExpValidator(acLongRE, this));
    ui->lineEdit_41->setValidator(new QRegExpValidator(acTimeRE, this));
    //ui->lineEdit_43->setValidator(new QRegExpValidator(acTimeRE, this));
    ui->lineEdit_44->setValidator(new QRegExpValidator(acTimeRE, this));

    //通信节点参数管理正则.
    ui->lineEdit->setValidator(new QRegExpValidator(acIntRE, this));
    ui->lineEdit_2->setValidator(new QRegExpValidator(acLongRE, this));
    ui->lineEdit_3->setValidator(new QRegExpValidator(acFloatRE, this));
    ui->lineEdit_4->setValidator(new QRegExpValidator(acFloatRE, this));
    ui->lineEdit_21->setValidator(new QRegExpValidator(acBoolRE, this));

    //通信链路参数管理正则.
    ui->lineEdit_5->setValidator(new QRegExpValidator(acIntRE, this));
    ui->lineEdit_6->setValidator(new QRegExpValidator(acIntRE, this));
    ui->lineEdit_7->setValidator(new QRegExpValidator(acLongRE, this));
    ui->lineEdit_8->setValidator(new QRegExpValidator(acLongRE, this));
    ui->lineEdit_9->setValidator(new QRegExpValidator(acFloatRE, this));
    ui->lineEdit_10->setValidator(new QRegExpValidator(acFloatRE, this));
    ui->lineEdit_11->setValidator(new QRegExpValidator(acFloatRE, this));

    //节点故障管理正则.
    ui->lineEdit_13->setValidator(new QRegExpValidator(acIntRE, this));
    ui->lineEdit_14->setValidator(new QRegExpValidator(acFloatRE, this));
    ui->lineEdit_15->setValidator(new QRegExpValidator(acFloatRE, this));

    //链路故障管理正则.
    ui->lineEdit_16->setValidator(new QRegExpValidator(acIntRE, this));
    ui->lineEdit_17->setValidator(new QRegExpValidator(acIntRE, this));
    ui->lineEdit_18->setValidator(new QRegExpValidator(acFloatRE, this));
    ui->lineEdit_19->setValidator(new QRegExpValidator(acFloatRE, this));

    //数据篡改管理正则.
    ui->lineEdit_45->setValidator(new QRegExpValidator(acIntRE, this));
    ui->lineEdit_46->setValidator(new QRegExpValidator(acFloatRE, this));
    ui->lineEdit_47->setValidator(new QRegExpValidator(acFloatRE, this));
    ui->lineEdit_20->setValidator(new QRegExpValidator(acIntRE, this));
    ui->lineEdit_22->setValidator(new QRegExpValidator(acIntRE, this));

   //恶意路由正则.
    ui->lineEdit_48->setValidator(new QRegExpValidator(acIntRE, this));
    ui->lineEdit_49->setValidator(new QRegExpValidator(acFloatRE, this));
    //ui->lineEdit_50->setValidator(new QRegExpValidator(acIPRE, this));
    ui->lineEdit_51->setValidator(new QRegExpValidator(acIPRE, this));
    ui->lineEdit_52->setValidator(new QRegExpValidator(acIPRE, this));
    ui->lineEdit_53->setValidator(new QRegExpValidator(acLongRE, this));
/*********************************************************************************/
    //通信链路参数管理中误码率编辑框禁用.
    ui->lineEdit_12->setEnabled(false);
    //节点故障和链路故障初始化半实物设置.
    ui->radioButton_2->setChecked(true);
    ui->lineEdit_14->setEnabled(false);
    ui->lineEdit_15->setEnabled(false);

    ui->radioButton_4->setChecked(true);
    ui->lineEdit_18->setEnabled(false);
    ui->lineEdit_19->setEnabled(false);
 /********************************************************************************/
    //业务配置.
    connect(ui->pushButton_15, &QPushButton::clicked, this, &comm_param_settings::business_config_event);
    connect(ui->pushButton_16, &QPushButton::clicked, this, &comm_param_settings::business_config_cancel_event);

    //业务通道配置.
    connect(ui->pushButton_13, &QPushButton::clicked, this, &comm_param_settings::business_channel_config_event);
    connect(ui->pushButton_14, &QPushButton::clicked, this, &comm_param_settings::business_channel_confirm_event);

    //通信节点参数管理.
    connect(ui->pushButton, &QPushButton::clicked, this, &comm_param_settings::comm_note_para_read_event);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &comm_param_settings::comm_note_para_config_event);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &comm_param_settings::comm_note_para_cancel_event);

    //通信链路参数管理.
    connect(ui->pushButton_4, &QPushButton::clicked, this, &comm_param_settings::comm_link_para_read_event);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &comm_param_settings::comm_link_para_config_event);
    connect(ui->pushButton_6, &QPushButton::clicked, this, &comm_param_settings::comm_link_para_cancel_event);

    //节点故障管理.
    connect(ui->pushButton_7, &QPushButton::clicked, this, &comm_param_settings::note_breakdown_confirm_event);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &comm_param_settings::note_breakdown_cancel_event);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &comm_param_settings::note_breakdown_recovery_event);

    //链路故障管理.
    connect(ui->pushButton_10, &QPushButton::clicked, this, &comm_param_settings::link_breakdown_confirm_event);
    connect(ui->pushButton_11, &QPushButton::clicked, this, &comm_param_settings::link_breakdown_cancel_event);
    connect(ui->pushButton_12, &QPushButton::clicked, this, &comm_param_settings::link_breakdown_recovery_event);

    //恶意路由攻击.
    connect(ui->pushButton_22, &QPushButton::clicked, this, &comm_param_settings::rout_attack_confirm_event);
    connect(ui->pushButton_23, &QPushButton::clicked, this, &comm_param_settings::rout_attack_cancel_event);
    connect(ui->pushButton_24, &QPushButton::clicked, this, &comm_param_settings::rout_attack_recovery_event);

    //数据篡改攻击.
    connect(ui->pushButton_19, &QPushButton::clicked, this, &comm_param_settings::data_tamper_confirm_event);
    connect(ui->pushButton_20, &QPushButton::clicked, this, &comm_param_settings::data_tamper_cancel_event);
    connect(ui->pushButton_21, &QPushButton::clicked, this, &comm_param_settings::data_tamper_recovery_event);

    //checkBox事件.
    connect(ui->checkBox, &QCheckBox::stateChanged, this, &comm_param_settings::comm_note_para_checkBox_event);
    connect(ui->checkBox_2, &QCheckBox::stateChanged, this, &comm_param_settings::comm_link_para_checkBox_event);

    connect(ui->radioButton,&QRadioButton::clicked,this,&comm_param_settings::node_radio_button_event);
    connect(ui->radioButton_2,&QRadioButton::clicked,this,&comm_param_settings::node_radio_button2_event);
    connect(ui->radioButton_3,&QRadioButton::clicked,this,&comm_param_settings::link_radio_button3_event);
    connect(ui->radioButton_4,&QRadioButton::clicked,this,&comm_param_settings::link_radio_button4_event);


    //combox选择事件
   connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(combox_select_event(int)));


   //事件列表添加事件
   connect(ui->pushButton_18,&QRadioButton::clicked,this,&comm_param_settings::com_node_para_button18_event);
   connect(ui->pushButton_25,&QRadioButton::clicked,this,&comm_param_settings::com_link_para_button25_event);
   connect(ui->pushButton_26,&QRadioButton::clicked,this,&comm_param_settings::node_breakdown_button26_event);
   connect(ui->pushButton_27,&QRadioButton::clicked,this,&comm_param_settings::link_breakdown_button27_event);
   connect(ui->pushButton_28,&QRadioButton::clicked,this,&comm_param_settings::rout_attack_button28_event);
   connect(ui->pushButton_29,&QRadioButton::clicked,this,&comm_param_settings::data_tamper_button29_event);

   connect(ui->pushButton_17,&QRadioButton::clicked,this,&comm_param_settings::send_data_button17_event);
   connect(ui->pushButton_30,&QRadioButton::clicked,this,&comm_param_settings::delete_data_button30_event);
   //table事件列表
   ui->tableView->horizontalHeader()->setStretchLastSection(true); //就是这个地方
   ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
   ui->tableView->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
   ui->tableView->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
   model_t = new QStandardItemModel();
   model_t->setColumnCount(2);  //设为两列.
   model_t->setHeaderData(0,Qt::Horizontal,"事件时间(S)");
   model_t->setHeaderData(1,Qt::Horizontal,"事件类型");
   ui->tableView->setModel(model_t);

}

/************业务配置.***********************/
void comm_param_settings::business_config_event()
{
    if(ui->lineEdit_35->text().isEmpty() || ui->lineEdit_36->text().isEmpty()
            || ui->lineEdit_37->text().isEmpty()|| ui->lineEdit_41->text().isEmpty()
            || ui->lineEdit_42->text().isEmpty()|| ui->lineEdit_43->text().isEmpty()
            || ui->lineEdit_44->text().isEmpty())
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }
    PG_RTUI_AddCBR data;
    data.clientAddr.ip_addr =_ip;//ip uInt32_t型
    data.clientAddr.port = 65500;//port  uInt32_t型
    data.type = ePG_RTUI_add_cbr; //type
    data.length = sizeof(PG_RTUI_AddCBR);//lengrh大小

    data.srcId = ui->lineEdit_35->text().toLong();
    data.dstId = ui->lineEdit_36->text().toLong();
    data.itemsToSend = ui->lineEdit_37->text().toLong();
    data.itemSize = ui->lineEdit_42->text().toLong();

    //char startTime[20]
    std::string lineEdit44Str = ui->lineEdit_44->text().toStdString();
    strcpy(data.startTime,lineEdit44Str.data());
    //char interval[20]
    std::string lineEdit43Str = ui->lineEdit_43->text().toStdString();
    strcpy(data.interval,lineEdit43Str.data());
    //char endTimeStr[20]
    std::string lineEdit41Str = ui->lineEdit_41->text().toStdString();
    strcpy(data.endTime,lineEdit41Str.data());
    //发送数据给qualnet通信软件
    _msHandler->config_comm_param(&data);
}
void comm_param_settings::business_config_cancel_event(){
    ui->lineEdit_35->setText("");
    ui->lineEdit_36->setText("");
    ui->lineEdit_37->setText("");
    ui->lineEdit_41->setText("");
    ui->lineEdit_42->setText("");
    ui->lineEdit_43->setText("");
    ui->lineEdit_44->setText("");
}

/***********************************业务通道配置.*****************/
void comm_param_settings::business_channel_config_event()
{
    //1.读取文本
    _path = QFileDialog::getOpenFileName(this, tr("Open Xml File"), "", "*.txt", 0);
}

void comm_param_settings::business_channel_confirm_event()
{
    char buffer[256];
    std::ifstream in(_path.toStdString());
    if (! in.is_open())
    {
        std::cout << "Error opening file";
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("文件读取失败!"));
        return; }
    int lineNum = 0;  //行号.
    while (!in.eof() )
    {
        in.getline (buffer,256);
        lineNum++;
        //读取一行的数据
        std::string  strline = buffer;
        if( lineNum >= 1 )
        {
            if(strline[strline.length()-1]=='\n')
            {
                strline.erase(strline.length()-1,1);
            }
            //将分割后的字符串写到字符串数组
            std::vector<std::string> stu;  //存放一行数据的数组
            std::stringstream sstr(strline);
            std::string token;
            while(getline(sstr, token, ' '))
            {
                stu.push_back(token);
            }
            //根据数组中数据给创建的结构体赋值
            PG_RTUI_StaticRoute data;
            data.clientAddr.ip_addr =_ip;//ip uInt32_t型
            data.clientAddr.port = 65500;//port  uInt32_t型
            data.type = ePG_RTUI_static_route; //type
            data.length = sizeof(PG_RTUI_StaticRoute);//lengrh大小

            data.pdsi_delay_time = 0;
            data.nodeId = std::stoi(stu[0]);
            strcpy(data.dst_ip,stu[1].data());
            strcpy(data.nxt_hop,stu[2].data());
            strcpy(data.out_itf_ip,stu[3].data());
            data.cost = std::stoi(stu[4]);
            //发送数据给qualnet通信软件
            _msHandler->config_comm_param(&data);
        }
        //2.将文本数据显示在list缓存
        current_cloud_id_list.push_back(QString::fromStdString(strline));
    }
    //current_cloud_id_list.push_back(QString::fromStdString("strline"));//再次往list中添加数据
    model->setStringList(current_cloud_id_list);//往listView中添加数据
    ui->listView->setModel(model);
}

/**************通信节点参数管理.*******************/
void comm_param_settings::comm_note_para_read_event()
{
    //只读情况.
    if(ui->checkBox->isChecked())
    {
        if(ui->lineEdit->text().isEmpty())
        {
            QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
            return;
        }

        PG_RTUI_Msg_GetInterfaceNum data1;
        data1.clientAddr.ip_addr =_ip;//ip uInt32_t型
        data1.clientAddr.port = 65500;//port  uint16_t型
        data1.type = ePG_RTUI_msg_getinterfacenum; //uint16_t
        data1.length = sizeof(PG_RTUI_Msg_GetInterfaceNum);//uint16_t length大小

        data1.nodeId = ui->lineEdit->text().toInt();  //uint16_t

        //发送数据给qualnet通信软件
        _msHandler->config_comm_param(&data1);
    }
}

void comm_param_settings::comm_note_para_config_event()
{
    //非只读情况
    if(!ui->checkBox->isChecked())
    {
        if(ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty()
                || ui->lineEdit_3->text().isEmpty() || ui->lineEdit_4->text().isEmpty()
                || ui->lineEdit_21->text().isEmpty())
        {
            QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
            return;
        }
        PG_RTUI_Msg_SetInterface data;

        data.clientAddr.ip_addr =_ip;//ip uInt32_t型
        data.clientAddr.port = 65500;//port  uInt32_t型
        data.type = ePG_RTUI_msg_setinterface; //type
        data.length = sizeof(PG_RTUI_Msg_SetInterface);//length大小

        data.nodeId = ui->lineEdit->text().toInt();
        data.interfaceIndex =ui->comboBox->currentText().toInt();
        data.bandwidth = ui->lineEdit_2->text().toLongLong();
        data.txPower = ui->lineEdit_3->text().toDouble();
        data.rxSensitivity = ui->lineEdit_4->text().toDouble();
        data.mode = ui->lineEdit_21->text().toStdString()=="1"?true:false;
        //发送数据给qualnet通信软件
        _msHandler->config_comm_param(&data);
    }
}

void comm_param_settings::comm_note_para_cancel_event(){
    flagSendGetInterface = false;
    ui->comboBox->clear();
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");
    ui->lineEdit_21->setText("");
}

/*******************通信链路参数管理.**********************************/
void comm_param_settings::comm_link_para_read_event()
{
    //只读情况.
    if(ui->checkBox_2->isChecked())
    {
        if(ui->lineEdit_5->text().isEmpty() || ui->lineEdit_6->text().isEmpty())
        {
            QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
            return;
        }
        PG_RTUI_Msg_GetLink data1;

        data1.clientAddr.ip_addr =_ip;//ip uInt32_t型
        data1.clientAddr.port = 65500;//port  uInt32_t型
        data1.type = ePG_RTUI_msg_getlink; //type
        data1.length = sizeof(PG_RTUI_Msg_GetLink);//length大小
        data1.nodeSrcId = ui->lineEdit_5->text().toInt();
        data1.nodeDstId = ui->lineEdit_6->text().toInt();
        //发送数据给qualnet通信软件
        _msHandler->config_comm_param(&data1);
    }

}

void comm_param_settings::comm_link_para_config_event()
{
    //非只读情况
    if(!ui->checkBox_2->isChecked())
    {
        if(ui->lineEdit_5->text().isEmpty() || ui->lineEdit_6->text().isEmpty()
                ||ui->lineEdit_7->text().isEmpty() ||ui->lineEdit_8->text().isEmpty()
                ||ui->lineEdit_9->text().isEmpty() ||ui->lineEdit_10->text().isEmpty()
                ||ui->lineEdit_11->text().isEmpty())
        {
            QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
            return;
        }

        PG_RTUI_Msg_SetLink data;

        data.clientAddr.ip_addr =_ip;//ip uInt32_t型
        data.clientAddr.port = 65500;//port  uInt32_t型
        data.type = ePG_RTUI_msg_setlink; //type
        data.length = sizeof(PG_RTUI_Msg_SetLink);//length大小

        data.nodeSrcId = ui->lineEdit_5->text().toInt();//uint16_t
        data.nodeDstId = ui->lineEdit_6->text().toInt();//uint16_t
        data.switchNum = ui->lineEdit_7->text().toInt();// int32_t
        data.switchLoss = ui->lineEdit_8->text().toDouble();//double
        data.connectorNum = ui->lineEdit_9->text().toInt();//int32_t
        data.connectorLoss = ui->lineEdit_10->text().toDouble();// double
        data.lossCoefficient = ui->lineEdit_11->text().toDouble();//double
        //data->ber = ui->lineEdit_12->text().toDouble();//double
        //data->time = 10;//uint64_t
        //发送数据给qualnet通信软件
        _msHandler->config_comm_param(&data);
    }

}

void comm_param_settings::comm_link_para_cancel_event(){
    ui->lineEdit_5->setText("");
    ui->lineEdit_6->setText("");
    ui->lineEdit_7->setText("");
    ui->lineEdit_8->setText("");
    ui->lineEdit_9->setText("");
    ui->lineEdit_10->setText("");
    ui->lineEdit_11->setText("");
    ui->lineEdit_12->setText("");
}

/********************节点故障管理.********************************/
void comm_param_settings::note_breakdown_confirm_event()
{
    if(ui->radioButton_2->isChecked()&&ui->lineEdit_13->text().isEmpty())
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }
    if(ui->radioButton->isChecked()&&(ui->lineEdit_13->text().isEmpty() ||ui->lineEdit_14->text().isEmpty() ||ui->lineEdit_15->text().isEmpty()))
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }
    PG_RTUI_ChangeNodeStatus data;

    data.clientAddr.ip_addr =_ip;//ip uInt32_t型
    data.clientAddr.port = 65500;//port  uInt16_t型
    if(ui->radioButton_2->isChecked())   //非半实物
    {
        data.type = ePG_RTUI_change_node_status; //type 23
    }
    if(ui->radioButton->isChecked())   //半实物
    {
        data.type = ePG_RTUI_hr_change_node_status; //type 5
    }
    data.length = sizeof(PG_RTUI_ChangeNodeStatus);//length大小

    data.nodeId = ui->lineEdit_13->text().toInt();
    data.pdsi_delay_time = ui->lineEdit_14->text().toFloat();
    data.change_delay_time = ui->lineEdit_15->text().toFloat();
    //发送数据给qualnet通信软件
    _msHandler->config_comm_param(&data);

}
void comm_param_settings::note_breakdown_cancel_event()
{  
    ui->lineEdit_13->setText("");
    ui->lineEdit_14->setText("");
    ui->lineEdit_15->setText("");
}

void comm_param_settings::note_breakdown_recovery_event()
{
    if(ui->radioButton_2->isChecked()&&ui->lineEdit_13->text().isEmpty())
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }
    if(ui->radioButton->isChecked()&&(ui->lineEdit_13->text().isEmpty() ||ui->lineEdit_14->text().isEmpty() ||ui->lineEdit_15->text().isEmpty()))
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }
    PG_RTUI_ChangeNodeStatus data;

    data.clientAddr.ip_addr =_ip;//ip uInt32_t型
    data.clientAddr.port = 65500;//port  uInt32_t型
    if(ui->radioButton_2->isChecked())   //非半实物
    {
        data.type = ePG_RTUI_recover_node_status; //type 24
    }
    if(ui->radioButton->isChecked())   //半实物
    {
        data.type = ePG_RTUI_hr_recover_node_status; //type 6
    }
    data.length = sizeof(PG_RTUI_ChangeNodeStatus);//length大小

    data.nodeId = ui->lineEdit_13->text().toInt();
    data.pdsi_delay_time = ui->lineEdit_14->text().toFloat();
    data.change_delay_time = ui->lineEdit_15->text().toFloat();
    //发送数据给qualnet通信软件
    _msHandler->config_comm_param(&data);
}

/**********************链路故障管理.**********************/
void comm_param_settings::link_breakdown_confirm_event()
{
    if(ui->radioButton_4->isChecked()&&(ui->lineEdit_16->text().isEmpty() || ui->lineEdit_17->text().isEmpty()))
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }
    if(ui->radioButton_3->isChecked()&&(ui->lineEdit_16->text().isEmpty() || ui->lineEdit_17->text().isEmpty() || ui->lineEdit_18->text().isEmpty() || ui->lineEdit_19->text().isEmpty()))
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }
    PG_RTUI_ChangePortStatus data;
    data.clientAddr.ip_addr =_ip;//ip uInt32_t型
    data.clientAddr.port = 65500;//port  uInt32_t型
    if(ui->radioButton_4->isChecked())   //非半实物
    {
        data.type = ePG_RTUI_change_port_status; //type
    }
    if(ui->radioButton_3->isChecked())   //半实物
    {
        data.type = ePG_RTUI_hr_change_port_status; //type
    }
    data.length = sizeof(PG_RTUI_ChangePortStatus);//length大小

    data.nodeId1 = ui->lineEdit_16->text().toInt();
    data.nodeId2 = ui->lineEdit_17->text().toInt();
    data.pdsi_delay_time = ui->lineEdit_18->text().toFloat();
    data.change_delay_time = ui->lineEdit_19->text().toFloat();
    //发送数据给qualnet通信软件
    _msHandler->config_comm_param(&data);
}
void comm_param_settings::link_breakdown_cancel_event()
{
    ui->lineEdit_16->setText("");
    ui->lineEdit_17->setText("");
    ui->lineEdit_18->setText("");
    ui->lineEdit_19->setText("");
}
void comm_param_settings::link_breakdown_recovery_event()
{
    if(ui->radioButton_4->isChecked()&&(ui->lineEdit_16->text().isEmpty() || ui->lineEdit_17->text().isEmpty()))
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }
    if(ui->radioButton_3->isChecked()&&(ui->lineEdit_16->text().isEmpty() || ui->lineEdit_17->text().isEmpty() || ui->lineEdit_18->text().isEmpty() || ui->lineEdit_19->text().isEmpty()))
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }
    PG_RTUI_ChangePortStatus data;
    data.clientAddr.ip_addr =_ip;//ip uInt32_t型
    data.clientAddr.port = 65500;//port  uInt32_t型
    if(ui->radioButton_4->isChecked())   //非半实物
    {
        data.type = ePG_RTUI_recover_port_status; //type
    }
    if(ui->radioButton_3->isChecked())   //半实物
    {
        data.type = ePG_RTUI_hr_recover_port_status; //type
    }
    data.length = sizeof(PG_RTUI_ChangePortStatus);//length大小

    data.nodeId1 = ui->lineEdit_16->text().toInt();
    data.nodeId2 = ui->lineEdit_17->text().toInt();
    data.pdsi_delay_time = ui->lineEdit_18->text().toFloat();
    data.change_delay_time = ui->lineEdit_19->text().toFloat();
    //发送数据给qualnet通信软件
    _msHandler->config_comm_param(&data);
}


/******************恶意路由攻击.******************/
void comm_param_settings::rout_attack_confirm_event()
{
    if(ui->lineEdit_48->text().isEmpty() || ui->lineEdit_49->text().isEmpty()
            || ui->lineEdit_50->text().isEmpty() || ui->lineEdit_51->text().isEmpty()
            || ui->lineEdit_52->text().isEmpty() || ui->lineEdit_53->text().isEmpty())
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }
    PG_RTUI_StaticRoute data;
    data.clientAddr.ip_addr =_ip;//ip uInt32_t型
    data.clientAddr.port = 65500;//port  uInt32_t型
    data.type = ePG_RTUI_add_staticroute; //type
    data.length = sizeof(PG_RTUI_StaticRoute);//length大小
    data.nodeId=ui->lineEdit_48->text().toInt();
    data.pdsi_delay_time = ui->lineEdit_49->text().toFloat();
    data.cost =ui->lineEdit_53->text().toInt();
    string stu1 =ui->lineEdit_50->text().toStdString();
    string stu2 =ui->lineEdit_51->text().toStdString();
    string stu3 =ui->lineEdit_52->text().toStdString();
    strcpy(data.dst_ip,stu1.data());
    strcpy(data.nxt_hop,stu2.data());
    strcpy(data.out_itf_ip,stu3.data());
    //发送数据给qualnet通信软件
    _msHandler->config_comm_param(&data);
}
void comm_param_settings::rout_attack_cancel_event(){
    ui->lineEdit_48->setText("");
    ui->lineEdit_49->setText("");
    ui->lineEdit_50->setText("");
    ui->lineEdit_51->setText("");
    ui->lineEdit_52->setText("");
    ui->lineEdit_53->setText("");
}
void comm_param_settings::rout_attack_recovery_event(){
    if(ui->lineEdit_48->text().isEmpty() || ui->lineEdit_49->text().isEmpty()
            || ui->lineEdit_50->text().isEmpty() || ui->lineEdit_51->text().isEmpty()
            || ui->lineEdit_52->text().isEmpty() || ui->lineEdit_53->text().isEmpty())
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }
    PG_RTUI_StaticRoute data;
    data.clientAddr.ip_addr =_ip;//ip uInt32_t型
    data.clientAddr.port = 65500;//port  uInt32_t型
    data.type = ePG_RTUI_remove_staticroute; //type
    data.length = sizeof(PG_RTUI_StaticRoute);//length大小
    data.nodeId=ui->lineEdit_48->text().toInt();
    data.pdsi_delay_time = ui->lineEdit_49->text().toFloat();
    data.cost =ui->lineEdit_53->text().toInt();
    string stu1 =ui->lineEdit_50->text().toStdString();
    string stu2 =ui->lineEdit_51->text().toStdString();
    string stu3 =ui->lineEdit_52->text().toStdString();
    strcpy(data.dst_ip,stu1.data());
    strcpy(data.nxt_hop,stu2.data());
    strcpy(data.out_itf_ip,stu3.data());
    //发送数据给qualnet通信软件
    _msHandler->config_comm_param(&data);


}

/***************************数据篡改攻击.**************************/
void comm_param_settings::data_tamper_confirm_event()
{
    if(ui->lineEdit_20->text().isEmpty()|| ui->lineEdit_47->text().isEmpty()
            || ui->lineEdit_22->text().isEmpty() || ui->lineEdit_23->text().isEmpty()
            || ui->lineEdit_45->text().isEmpty() || ui->lineEdit_46->text().isEmpty())
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }
    //第一个结构体
    PG_RTUI_SetDataTamperSimTime data;
    data.clientAddr.ip_addr =_ip;//ip uInt32_t型
    data.clientAddr.port = 65500;//port  uInt32_t型
    data.type = ePG_RTUI_set_data_tamper_sim_time; //type
    data.length = sizeof(PG_RTUI_SetDataTamperSimTime);//length大小

    data.nodeId = ui->lineEdit_45->text().toInt();
    data.powernodeId =ui->lineEdit_20->text().toInt();
    data.datatype =ui->comboBox_4->currentIndex()+1;
    data.dataPlace =ui->lineEdit_22->text().toInt();
    string stu1 = ui->lineEdit_23->text().toStdString();
    strcpy(data.dataDev,stu1.data());
    data.preset_time = ui->lineEdit_46->text().toFloat();
    data.tamper_time= ui->lineEdit_47->text().toFloat();
    //发送数据给qualnet通信软件
    _msHandler->config_comm_param(&data);

    //第二个结构体
    PG_RTUI_SetDataTamperLastTime data1;
    data1.clientAddr.ip_addr =_ip;//ip uInt32_t型
    data1.clientAddr.port = 65500;//port  uInt32_t型
    data1.type = ePG_RTUI_set_data_tamper_last_time; //type
    data1.length = sizeof(PG_RTUI_SetDataTamperLastTime);//length大小

    data1.nodeId = ui->lineEdit_45->text().toInt();
    data1.powernodeId =ui->lineEdit_20->text().toInt();
    data1.datatype =ui->comboBox_4->currentIndex()+1;
    data1.dataPlace =ui->lineEdit_22->text().toInt();
    string stu2 = ui->lineEdit_23->text().toStdString();
    strcpy(data1.dataDev,stu2.data());
    data1.preset_time = ui->lineEdit_46->text().toFloat();
    data1.tamper_time= ui->lineEdit_47->text().toFloat();
    //发送数据给qualnet通信软件
    _msHandler->config_comm_param(&data1);

}
void comm_param_settings::data_tamper_cancel_event()
{
    ui->lineEdit_45->setText("");
    ui->lineEdit_46->setText("");
    ui->lineEdit_47->setText("");
    ui->lineEdit_20->setText("");
    ui->lineEdit_22->setText("");
    ui->lineEdit_23->setText("");
}
void comm_param_settings::data_tamper_recovery_event()
{
    PG_RTUI_StopDataTamper data;
    data.clientAddr.ip_addr =_ip;//ip uInt32_t型
    data.clientAddr.port = 65500;//port  uInt32_t型
    data.type = ePG_RTUI_stop_data_tamper; //type
    data.length = sizeof(PG_RTUI_StopDataTamper);//length大小
    //发送数据给qualnet通信软件
    _msHandler->config_comm_param(&data);
}

//checkBox单选事件.
void comm_param_settings::comm_note_para_checkBox_event()
{
    if(ui->checkBox->isChecked())
    {
        ui->lineEdit_2->setEnabled(false);
        ui->lineEdit_3->setEnabled(false);
        ui->lineEdit_4->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->lineEdit_21->setEnabled(false);
    }
    if(!ui->checkBox->isChecked())
    {
        ui->lineEdit_2->setEnabled(true);
        ui->lineEdit_3->setEnabled(true);
        ui->lineEdit_4->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->lineEdit_21->setEnabled(true);
    }
}
void comm_param_settings::comm_link_para_checkBox_event()
{
    if(ui->checkBox_2->isChecked())
    {
        ui->lineEdit_7->setEnabled(false);
        ui->lineEdit_9->setEnabled(false);
        ui->lineEdit_11->setEnabled(false);
        ui->lineEdit_8->setEnabled(false);
        ui->lineEdit_10->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_6->setEnabled(false);
    }
    if(!ui->checkBox_2->isChecked())
    {
        ui->lineEdit_7->setEnabled(true);
        ui->lineEdit_9->setEnabled(true);
        ui->lineEdit_11->setEnabled(true);
        ui->lineEdit_8->setEnabled(true);
        ui->lineEdit_10->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_6->setEnabled(true);
    }
}
//radiobutton事件
void comm_param_settings::node_radio_button_event()
{
    ui->lineEdit_14->setEnabled(true);
    ui->lineEdit_15->setEnabled(true);
}

void comm_param_settings::node_radio_button2_event()
{
    ui->lineEdit_14->setEnabled(false);
    ui->lineEdit_15->setEnabled(false);
}

void comm_param_settings::link_radio_button3_event()
{
    ui->lineEdit_18->setEnabled(true);
    ui->lineEdit_19->setEnabled(true);
}

void comm_param_settings::link_radio_button4_event()
{
    ui->lineEdit_18->setEnabled(false);
    ui->lineEdit_19->setEnabled(false);
}

//接受通信软件返回回来的数据.
void comm_param_settings::set_ack_comm_cfg_param(PG_RTUI_Base* data)
{
    switch(data->type)
    {
    case ePG_RTUI_ack_getinterfacenum:  //通信节点参数管理返回得到的结构.
    {
        flagSendGetInterface = false;
        PG_RTUI_Ack_GetInterfaceNum *paraInfo = (PG_RTUI_Ack_GetInterfaceNum*)(data);
        ui->comboBox->clear();
        //向comboBox添加数据.
        int num = paraInfo->interfaceNum;
        QStringList test;
        for(int i=0;i<num;i++)
        {
            test.push_back(QString::number(i, 10));
        }
        flagSendGetInterface = true;
        ui->comboBox->addItems(test);
        ui->comboBox->setCurrentIndex(0);//设置当前的索引.
        delete paraInfo;
        break;
    }

    case ePG_RTUI_ack_getlink:   //通信链路参数管理返回得到的结构.
    {
        PG_RTUI_Ack_GetLink *linkInfo =(PG_RTUI_Ack_GetLink *)(data);
        ui->lineEdit_5->setText(QString::number(linkInfo->nodeSrcId, 10));
        ui->lineEdit_6->setText(QString::number(linkInfo->nodeDstId, 10));
        ui->lineEdit_7->setText(QString::number(linkInfo->switchNum, 10));
        ui->lineEdit_8->setText(QString::number(linkInfo->connectorNum, 10));

        ui->lineEdit_9->setText(QString::number(linkInfo->switchLoss));
        ui->lineEdit_10->setText(QString::number(linkInfo->connectorLoss));
        ui->lineEdit_11->setText(QString::number(linkInfo->lossCoefficient));
        double ber = linkInfo->ber;
        if(-0.0000001<ber && ber<0.0000001)
            ber=0;
        ui->lineEdit_12->setText(QString::number(ber));
        delete linkInfo;
        break;
    }

    case ePG_RTUI_ack_getinterface:   //通信参数管理,端口选择后返回.
    {
        PG_RTUI_Ack_GetInterface *paraInfo = (PG_RTUI_Ack_GetInterface*)(data);
        ui->lineEdit->setText(QString::number(paraInfo->nodeId,10));
        ui->lineEdit_2->setText(QString::number(paraInfo->bandwidth,10));
        double txPower = paraInfo->txPower;
        if(-0.0000001<txPower && txPower<0.0000001)
            txPower=0;
        ui->lineEdit_3->setText(QString::number(txPower));
        double rxSensitivity = paraInfo->rxSensitivity;
        if(-0.0000001<rxSensitivity && rxSensitivity<0.0000001)
            rxSensitivity=0;
        ui->lineEdit_4->setText(QString::number(rxSensitivity));
        ui->lineEdit_21->setText(paraInfo->mode?"1":"0");
        delete paraInfo;
        break;
    }

    }
}

//通信节点参数管路通信端数目选择.
void comm_param_settings::combox_select_event(int a)
{
    if(!ui->checkBox->isChecked() || !flagSendGetInterface)
        return;
    PG_RTUI_Msg_GetInterface *data1 = new PG_RTUI_Msg_GetInterface();
    data1->clientAddr.ip_addr =_ip;//ip uInt32_t型
    data1->clientAddr.port = 65500;//port  uInt32_t型
    data1->type = ePG_RTUI_msg_getinterface; //type
    data1->length = sizeof(PG_RTUI_Msg_GetInterface);//length大小
    data1->nodeId = ui->lineEdit->text().toInt();
    data1->interfaceIndex = ui->comboBox->currentText().toInt();
    //发送数据给qualnet通信软件
    _msHandler->config_comm_param(data1);
}

//事件列表添加事件 节点参数
void comm_param_settings::com_node_para_button18_event()
{
    if(ui->checkBox->isChecked())
        return;
    if(ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty()
            || ui->lineEdit_3->text().isEmpty() || ui->lineEdit_4->text().isEmpty()
            || ui->lineEdit_21->text().isEmpty())
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }
    PG_RTUI_Msg_SetInterface data;
    data.clientAddr.ip_addr =_ip;//ip uInt32_t型
    data.clientAddr.port = 65500;//port  uInt32_t型
    data.type = ePG_RTUI_msg_setinterface; //type
    data.length = sizeof(PG_RTUI_Msg_SetInterface);//length大小
    data.nodeId = ui->lineEdit->text().toInt();
    data.interfaceIndex =ui->comboBox->currentText().toInt();
    data.bandwidth = ui->lineEdit_2->text().toLongLong();
    data.txPower = ui->lineEdit_3->text().toDouble();
    data.rxSensitivity = ui->lineEdit_4->text().toDouble();
    data.mode = ui->lineEdit_21->text().toStdString()=="1"?true:false;
    double time = ui->lineEdit_24->text().toDouble();
    const int length = sizeof(PG_RTUI_Msg_SetInterface)+sizeof(double);
    char d[length]={0};
    memcpy(d,&data,data.length);
    memcpy(d+data.length,&time,sizeof(double));
    XmlUtil::add_CommSimEventConf_data(d,length);
    model_t->setItem(n,0,new QStandardItem(QString::number(time)));
    model_t->setItem(n,1,new QStandardItem("节点参数事件"));
    n++;
    vec.push_back(QByteArray::fromRawData(d,length));
}
//事件列表添加事件 链路参数
void comm_param_settings::com_link_para_button25_event()
{
    if(ui->checkBox_2->isChecked())
        return;
    if(ui->lineEdit_5->text().isEmpty() || ui->lineEdit_6->text().isEmpty()
            ||ui->lineEdit_7->text().isEmpty() ||ui->lineEdit_8->text().isEmpty()
            ||ui->lineEdit_9->text().isEmpty() ||ui->lineEdit_10->text().isEmpty()
            ||ui->lineEdit_11->text().isEmpty())
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }

    PG_RTUI_Msg_SetLink data;

    data.clientAddr.ip_addr =_ip;//ip uInt32_t型
    data.clientAddr.port = 65500;//port  uInt32_t型
    data.type = ePG_RTUI_msg_setlink; //type
    data.length = sizeof(PG_RTUI_Msg_SetLink);//length大小
    data.nodeSrcId = ui->lineEdit_5->text().toInt();//uint16_t
    data.nodeDstId = ui->lineEdit_6->text().toInt();//uint16_t
    data.switchNum = ui->lineEdit_7->text().toInt();// int32_t
    data.switchLoss = ui->lineEdit_8->text().toDouble();//double
    data.connectorNum = ui->lineEdit_9->text().toInt();//int32_t
    data.connectorLoss = ui->lineEdit_10->text().toDouble();// double
    data.lossCoefficient = ui->lineEdit_11->text().toDouble();//double
    double time = ui->lineEdit_25->text().toDouble();
    const int length = sizeof(PG_RTUI_Msg_SetLink)+sizeof(double);
    char d[length]={0};
    memcpy(d,&data,data.length);
    memcpy(d+data.length,&time,sizeof(double));
    XmlUtil::add_CommSimEventConf_data(d,length);
    model_t->setItem(n,0,new QStandardItem(QString::number(time)));
    model_t->setItem(n,1,new QStandardItem("链路参数事件"));
    n++;
    vec.push_back(QByteArray::fromRawData(d,length));
}
//事件列表添加事件 节点故障
void comm_param_settings::node_breakdown_button26_event()
{
    if(ui->radioButton_2->isChecked()&&ui->lineEdit_13->text().isEmpty())
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }
    if(ui->radioButton->isChecked()&&(ui->lineEdit_13->text().isEmpty() ||ui->lineEdit_14->text().isEmpty() ||ui->lineEdit_15->text().isEmpty()))
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }
    PG_RTUI_ChangeNodeStatus data;

    data.clientAddr.ip_addr =_ip;//ip uInt32_t型
    data.clientAddr.port = 65500;//port  uInt16_t型
    if(ui->radioButton_2->isChecked())   //非半实物
    {
        data.type = ePG_RTUI_change_node_status; //type 23
    }
    if(ui->radioButton->isChecked())   //半实物
    {
        data.type = ePG_RTUI_hr_change_node_status; //type 5
    }
    data.length = sizeof(PG_RTUI_ChangeNodeStatus);//length大小

    data.nodeId = ui->lineEdit_13->text().toInt();
    data.pdsi_delay_time = ui->lineEdit_14->text().toFloat();
    data.change_delay_time = ui->lineEdit_15->text().toFloat();
    double time = ui->lineEdit_26->text().toDouble();
    const int length = sizeof(PG_RTUI_ChangeNodeStatus)+sizeof(double);
    char d[length]={0};
    memcpy(d,&data,data.length);
    memcpy(d+data.length,&time,sizeof(double));
    XmlUtil::add_CommSimEventConf_data(d,length);
    model_t->setItem(n,0,new QStandardItem(QString::number(time)));
    model_t->setItem(n,1,new QStandardItem("节点故障事件"));
    n++;
    vec.push_back(QByteArray::fromRawData(d,length));
}
//事件列表添加事件  链路故障
void comm_param_settings::link_breakdown_button27_event()
{
    if(ui->radioButton_4->isChecked()&&(ui->lineEdit_16->text().isEmpty() || ui->lineEdit_17->text().isEmpty()))
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }
    if(ui->radioButton_3->isChecked()&&(ui->lineEdit_16->text().isEmpty() || ui->lineEdit_17->text().isEmpty() || ui->lineEdit_18->text().isEmpty() || ui->lineEdit_19->text().isEmpty()))
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }
    PG_RTUI_ChangePortStatus data;
    data.clientAddr.ip_addr =_ip;//ip uInt32_t型
    data.clientAddr.port = 65500;//port  uInt32_t型
    if(ui->radioButton_4->isChecked())   //非半实物
    {
        data.type = ePG_RTUI_change_port_status; //type
    }
    if(ui->radioButton_3->isChecked())   //半实物
    {
        data.type = ePG_RTUI_hr_change_port_status; //type
    }
    data.length = sizeof(PG_RTUI_ChangePortStatus);//length大小

    data.nodeId1 = ui->lineEdit_16->text().toInt();
    data.nodeId2 = ui->lineEdit_17->text().toInt();
    data.pdsi_delay_time = ui->lineEdit_18->text().toFloat();
    data.change_delay_time = ui->lineEdit_19->text().toFloat();

    double time = ui->lineEdit_27->text().toDouble();
    const int length = sizeof(PG_RTUI_ChangePortStatus)+sizeof(double);
    char d[length]={0};
    memcpy(d,&data,data.length);
    memcpy(d+data.length,&time,sizeof(double));
    XmlUtil::add_CommSimEventConf_data(d,length);
    model_t->setItem(n,0,new QStandardItem(QString::number(time)));
    model_t->setItem(n,1,new QStandardItem("链路故障事件"));
    n++;
    vec.push_back(QByteArray::fromRawData(d,length));
}
//事件列表添加事件 路由攻击
void comm_param_settings::rout_attack_button28_event()
{
    if(ui->lineEdit_48->text().isEmpty() || ui->lineEdit_49->text().isEmpty()
            || ui->lineEdit_50->text().isEmpty() || ui->lineEdit_51->text().isEmpty()
            || ui->lineEdit_52->text().isEmpty() || ui->lineEdit_53->text().isEmpty())
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }
    PG_RTUI_StaticRoute data;
    data.clientAddr.ip_addr =_ip;//ip uInt32_t型
    data.clientAddr.port = 65500;//port  uInt32_t型
    data.type = ePG_RTUI_add_staticroute; //type
    data.length = sizeof(PG_RTUI_StaticRoute);//length大小
    data.nodeId=ui->lineEdit_48->text().toInt();
    data.pdsi_delay_time = ui->lineEdit_49->text().toFloat();
    data.cost =ui->lineEdit_53->text().toInt();
    string stu1 =ui->lineEdit_50->text().toStdString();
    string stu2 =ui->lineEdit_51->text().toStdString();
    string stu3 =ui->lineEdit_52->text().toStdString();
    strcpy(data.dst_ip,stu1.data());
    strcpy(data.nxt_hop,stu2.data());
    strcpy(data.out_itf_ip,stu3.data());
    double time = ui->lineEdit_28->text().toDouble();
    const int length = sizeof(PG_RTUI_StaticRoute)+sizeof(double);
    char d[length]={0};
    memcpy(d,&data,data.length);
    memcpy(d+data.length,&time,sizeof(double));
    XmlUtil::add_CommSimEventConf_data(d,length);
    model_t->setItem(n,0,new QStandardItem(QString::number(time)));
    model_t->setItem(n,1,new QStandardItem("路由攻击事件"));
    n++;
    vec.push_back(QByteArray::fromRawData(d,length));
}
//事件列表添加事件 数据篡改
void comm_param_settings::data_tamper_button29_event()
{
    if(ui->lineEdit_20->text().isEmpty()|| ui->lineEdit_47->text().isEmpty()
            || ui->lineEdit_22->text().isEmpty() || ui->lineEdit_23->text().isEmpty()
            || ui->lineEdit_45->text().isEmpty() || ui->lineEdit_46->text().isEmpty())
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }

    PG_RTUI_SetDataTamperSimTime data;
    data.clientAddr.ip_addr =_ip;//ip uInt32_t型
    data.clientAddr.port = 65500;//port  uInt32_t型
    data.type = ePG_RTUI_set_data_tamper_sim_time; //type
    data.length = sizeof(PG_RTUI_SetDataTamperSimTime);//length大小

    data.nodeId = ui->lineEdit_45->text().toInt();
    data.powernodeId =ui->lineEdit_20->text().toInt();
    data.datatype =ui->comboBox_4->currentIndex()+1;
    data.dataPlace =ui->lineEdit_22->text().toInt();
    string stu1 = ui->lineEdit_23->text().toStdString();
    strcpy(data.dataDev,stu1.data());
    data.preset_time = ui->lineEdit_46->text().toFloat();
    data.tamper_time= ui->lineEdit_47->text().toFloat();

    double time = ui->lineEdit_29->text().toDouble();
    const int length = sizeof(PG_RTUI_SetDataTamperSimTime)+sizeof(double);
    char d[length]={0};
    memcpy(d,&data,data.length);
    memcpy(d+data.length,&time,sizeof(double));
    XmlUtil::add_CommSimEventConf_data(d,length);
    model_t->setItem(n,0,new QStandardItem(QString::number(time)));
    model_t->setItem(n,1,new QStandardItem("数据篡改事件"));
    n++;
    vec.push_back(QByteArray::fromRawData(d,length));
}

//发送数据
void comm_param_settings::send_data_button17_event()
{
    //发送数据
    //config_comm_sim_event();
    //清除tableView数据
    auto iter = vec.begin();
    for ( ;iter!=vec.end();)
    {
        iter = vec.erase(iter);
    }
    //清空tablevie列表
    while(n>=0)
    {
        model_t->removeRow(n);
        n--;
    }
    n=0;
}

//删除数据
void comm_param_settings::delete_data_button30_event()
{
    QModelIndex i = ui->tableView->selectionModel()->currentIndex();
    if(i.row()<0)
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("没有行被选中!"));
        return;
    }
    model_t->removeRow(i.row());
    n--;
    //调用删除数据接口
    XmlUtil::delete_CommSimEventConf_data(vec.at(i.row()).data(),vec.at(i.row()).count());
    //删除集合中数据
    vec.erase(vec.begin() + i.row());
}
