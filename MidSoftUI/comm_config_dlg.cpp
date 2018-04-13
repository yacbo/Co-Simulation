#include "comm_config_dlg.h"
#include "ui_comm_config_dlg.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
comm_config_dlg::comm_config_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::comm_config_dlg)
{
    //this->setStyleSheet("background-image: url(:/bac.jpg)");
     this->setFixedSize( 360,325);
    //this->setStyleSheet("background-color:#444240;color:white");
    ui->setupUi(this);
    init();
}

comm_config_dlg::~comm_config_dlg()
{
    delete ui;
}
void comm_config_dlg::paintEvent(QPaintEvent* pEvent)
{
     QPixmap _pBg;
     _pBg.load(":/bac.jpg");
     QPainter painter;
     painter.begin(this);
     QSize s =this->geometry().size();
     painter.drawPixmap(0,0,s.width(),s.height(),_pBg);
     painter.end();
}
void comm_config_dlg::init()
{
    //0-65535的正则表达.
    QRegExp acIntRE("([0-9])|([1-9][0-9])|([1-9][0-9]{2,3})|([1-5][0-9]{4})|(6[0-4][0-9]{3})|(654[0-9]{2})|(655[0-2][0-9])|(6553[0-5])");
    //ip校验
    QRegExp acIPRE("([0-9]{0,3})[.]([0-9]{0,3})[.]([0-9]{0,3})[.][0-9]{0,3}");

    //配置参数正则.
    ui->lineEdit->setValidator(new QRegExpValidator(acIPRE, this));
    ui->lineEdit_2->setValidator(new QRegExpValidator(acIntRE, this));
    ui->lineEdit_3->setValidator(new QRegExpValidator(acIntRE, this));

    //注册按钮选择文件事件.
    connect(ui->toolButton, &QToolButton::clicked, this, &comm_config_dlg::choose_dll_event);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &comm_config_dlg::button_box_ok);
    ui->toolButton->setVisible(false);
    ui->lineEdit->setText("192.168.31.35"); //设定ip初值.
    ui->lineEdit_2->setText("5132");//设定port初值.
    ui->lineEdit_3->setText("6666");//业务端口设初值.

    ui->lineEdit_4->setText("192.168.31.35");
    ui->lineEdit_5->setText("12321");

   // ui->groupBox->setStyleSheet("color:white;border:1px solid black;");
    //ui->groupBox_2->setStyleSheet("color:white;border:1px solid black;");
//QDalogButtonBox{border-radius:5px; color:white;border:1px solid black;min-height:20;}
    //ui->buttonBox->setStyleSheet("color:white;border:1px solid black;");
}

void comm_config_dlg::get_net_info(QString& ip, uint16_t& port, int& proto_type,uint16_t&port_business,QString& host_ip,uint16_t &host_port)
{
    ip = _ip;
    port = _port;
    proto_type = _type;
    port_business = _port_business;
    host_ip =_host_ip;
    host_port = _host_port;
}

void comm_config_dlg::button_box_ok()
{
    if(ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty() ||ui->lineEdit_3->text().isEmpty())
    {
        QMessageBox::information(this, QString::fromStdString("警告"),QString::fromStdString("请检查输入的参数!"));
        return;
    }
    _ip = ui->lineEdit->text();
    _port = ui->lineEdit_2->text().toUShort();
    _type = ui->comboBox->currentIndex();
    _port_business = ui->lineEdit_3->text().toUShort();
    _host_ip =ui->lineEdit_4->text();
    _host_port= ui->lineEdit_5->text().toUShort();
}

void comm_config_dlg::choose_dll_event()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open Xml File"), "", "*.dll", 0);
}
