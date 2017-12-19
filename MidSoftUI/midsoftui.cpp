#include "midsoftui.h"
#include "ui_midsoftui.h"
#include <QStandardItemModel>
#include <QDateTime>
#include "comm_config_dlg.h"
#include "xml_util.h"

MidSoftUI::MidSoftUI(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MidSoftUI)
{
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowMinimizeButtonHint;
    flags |=Qt::WindowMaximizeButtonHint;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    this->setStyleSheet("background-image: url(:/bac.jpg)");
    ui->setupUi(this);
    init();
}

MidSoftUI::~MidSoftUI()
{
    if(_ms_handler){
        delete _ms_handler;
    }

    delete ui;
    delete model;
}

void MidSoftUI::init()
{
    this->setWindowTitle(ui->comboBox_2->currentText());//标题显示.
    _ms_handler = new MsHandler();
    if(!_ms_handler){
        return;
    }

    QString local_ip = _ms_handler->get_local_ip();

    ui->lineEdit->setText(local_ip);
    ui->lineEdit_2->setText("9999");

    ui->lineEdit_3->setText(local_ip);
    ui->lineEdit_4->setText("9998");

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    model = new QStandardItemModel();
    model->setColumnCount(2);  //设为两列.

    model->setHeaderData(0,Qt::Horizontal,"时间");
    model->setHeaderData(1,Qt::Horizontal,"信息");
    ui->tableView->setModel(model);
//    ui->tableView->setColumnWidth(0,90);  //设置列宽
//    ui->tableView->setColumnWidth(1,160);

    //注册按钮事件.
    connect(ui->registerButton, &QPushButton::clicked, this, &MidSoftUI::register_device, Qt::UniqueConnection);
    //注销按钮事件.
    connect(ui->unregisterButton, &QPushButton::clicked, this, &MidSoftUI::unregister_device, Qt::UniqueConnection);

     //combox选择事件
    connect(ui->comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(combox_select_event(int)));

    //
    connect(_ms_handler, &MsHandler::login_signal, this, &MidSoftUI::login_singal_slots, Qt::UniqueConnection);

    connect(_ms_handler, &MsHandler::progress_log_signal, this, &MidSoftUI::progress_log_slots, Qt::UniqueConnection);

    //禁用注销按钮
    ui->unregisterButton->setEnabled(false);
}

void MidSoftUI::register_device()
{
    _sbs_ip = ui->lineEdit->text();
    _sbs_port = ui->lineEdit_2->text().toInt();
    _protocol_type = ui->comboBox->currentIndex();
    _dev_ip = ui->lineEdit_3->text();
    _listen_port = ui->lineEdit_4->text().toInt();

    //打印开始注册.
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    model->setItem(n,0,new QStandardItem(current_date));
    model->setItem(n,1,new QStandardItem("starting register..."));
    n++;

    _dev_name = DevNamesSet[ui->comboBox_2->currentIndex()];//设备名称.
    _dev_type = XmlUtil::query_sim_dev_type(_dev_name.toStdString().c_str());

    if(_dev_type == eSimDev_communication){
        QString comm_ip; uint16_t comm_port, bss_port; int proto_type;
        QString host_ip; uint16_t host_port;
        comm_dlg.get_net_info(comm_ip, comm_port, proto_type, bss_port,host_ip,host_port);
        _ms_handler->config_communication(comm_ip.toStdString().c_str(), comm_port, bss_port, proto_type, host_ip.toStdString().c_str(), host_port);
    }

    bool ret = _ms_handler->config_device(_dev_name.toStdString().c_str(), 0, _dev_ip. toStdString().c_str(), _listen_port, _protocol_type);
    if(!ret){
        return;
    }

    ret = _ms_handler->register_device(_dev_name.toStdString().c_str(), 0,  _sbs_ip.toStdString().c_str(), _sbs_port, _protocol_type);
    if(!ret){
        return;
    }
}

void MidSoftUI::unregister_device()
{
    //打印开始注销.
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    model->setItem(n,0,new QStandardItem(current_date));
    model->setItem(n,1,new QStandardItem("starting unregister..."));
    n++;
    const IntMap& devIds = _ms_handler->get_dev_id_map();
    IntMap::const_iterator it =  devIds.find(_dev_type);
    if(it != devIds.cend()){
        _dev_id = it->second;
        bool ret = _ms_handler->unregister(_dev_name.toStdString().c_str(),_dev_id, _sbs_ip.toStdString().c_str());
        if(!ret){
            QDateTime current_date = QDateTime::currentDateTime();
            QString date = current_date.toString("yyyy-MM-dd hh:mm:ss");
            model->setItem(n,0,new QStandardItem(date));
            model->setItem(n,1,new QStandardItem("unregister fail"));
            n++;
            string info = "unregister fail";
            qInfo(info.c_str());
        }
    }

}

void MidSoftUI::combox_select_event(int a)
{
    this->setWindowTitle(ui->comboBox_2->currentText());
    if(ui->comboBox_2->currentIndex() == 1 ){
        comm_dlg.show();

    }
}

void MidSoftUI::login_singal_slots(QString dev_name, QString dev_ip, uint16_t dev_port, bool login)
{
    ESimDevType  devType = XmlUtil::query_sim_dev_type(dev_name.toStdString().c_str());
    if(devType==_dev_type)
    {
        QDateTime current_date = QDateTime::currentDateTime();
        QString date = current_date.toString("yyyy-MM-dd hh:mm:ss");
        model->setItem(n,0,new QStandardItem(date));

        QString tips = "MidSoftUI:login_singal_slots, ";
        QString info = tips + dev_name + (login ? " register successfully." : " unregister successfully.");
        model->setItem(n,1,new QStandardItem(info));
        n++;
        //若注册成功，注册按钮禁用,启用注销按钮.
        if(ui->registerButton->isEnabled() && login)
        {
           ui->registerButton->setEnabled(false);  //注册按钮禁用.
           ui->unregisterButton->setEnabled(true);//注销按钮启用.
           ui->lineEdit->setEnabled(false);  //sbs ip输入禁用.
           ui->lineEdit_2->setEnabled(false);//sbs port输入禁用.
           ui->lineEdit_3->setEnabled(false);
           ui->lineEdit_4->setEnabled(false);
           ui->comboBox->setEnabled(false);
           ui->comboBox_2->setEnabled(false);
        }

        //若注销成功，注册按钮禁用,启用注销按钮.
        if(ui->unregisterButton->isEnabled() && !login)
        {
           ui->registerButton->setEnabled(true);  //注册按钮启用.
           ui->unregisterButton->setEnabled(false);//注销按钮禁用.
           ui->lineEdit->setEnabled(true);  //sbs ip输入启用.
           ui->lineEdit_2->setEnabled(true);//sbs port输入启用.
           ui->lineEdit_3->setEnabled(true);
           ui->lineEdit_4->setEnabled(true);
           ui->comboBox->setEnabled(true);
           ui->comboBox_2->setEnabled(true);
        }
    }
}


void MidSoftUI::progress_log_slots(QString log)
{
    QDateTime current_date = QDateTime::currentDateTime();
    QString date = current_date.toString("yyyy-MM-dd hh:mm:ss");
    model->setItem(n,0,new QStandardItem(date));
    model->setItem(n,1,new QStandardItem(log));
    n++;
}
