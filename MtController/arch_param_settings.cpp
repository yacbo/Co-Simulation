#include "arch_param_settings.h"
#include "ui_arch_param_settings.h"
#include "comm_type_def.h"
#include <QPainter>
arch_param_settings::arch_param_settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::arch_param_settings)
{
    //this->setStyleSheet("background-image: url(:/bac.jpg)");
    this->setStyleSheet("background-color:#444240;");
    ui->setupUi(this);
    setSbsSoftOffLine();  //SBS离线设置.
    setElecSoftOffLine();  //电力离线设置.
    setCommSoftOffLine();   //通信离线设置.
    setElecAppSoftOffLine(); //电力应用离线设置.
    init();
}

arch_param_settings::~arch_param_settings()
{
    delete ui;
}
void arch_param_settings::paintEvent(QPaintEvent* pEvent)
{
//    QPixmap _pBg;
//    _pBg.load(":/bac.jpg");
//    QPainter painter;
//    painter.begin(this);
//    QSize s =this->geometry().size();
//    painter.drawPixmap(0,0,s.width(),s.height(),_pBg);
//    painter.end();
}
void arch_param_settings::init()
{
    ui->comboBox->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    //ui->lineEdit_8->setStyleSheet("border:1px;");
    ui->label_3->setStyleSheet("border:0px;");
    ui->label_4->setStyleSheet("border:0px;");
    ui->label_11->setStyleSheet("border:0px;");
    ui->label_15->setStyleSheet("border:0px;");
    ui->label_16->setStyleSheet("border:0px;");
    ui->label_17->setStyleSheet("border:0px;");
    ui->label_18->setStyleSheet("border:0px;");
}
void arch_param_settings::setComboBoxClear()
{
    ui->comboBox->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
}
void arch_param_settings::setComboBoxCurrentText(QString devName)
{
    QStringList listName;
    listName.push_back(devName);
    if(devName.toStdString() == DevNamesSet[0])
    {
        ui->comboBox->clear();
        ui->comboBox->addItems(listName);
        ui->comboBox->setCurrentIndex(0);
    }
    else if(devName.toStdString() == DevNamesSet[1])
    {
        ui->comboBox_2->clear();
        ui->comboBox_2->addItems(listName);
        ui->comboBox_2->setCurrentIndex(0);
    }
    else if(devName.toStdString() == DevNamesSet[2])
    {
        ui->comboBox_3->clear();
        ui->comboBox_3->addItems(listName);
        ui->comboBox_3->setCurrentIndex(0);
    }
}

void arch_param_settings::setComboBoxCurrentTextClear(QString devName)  //comboBox设值为空
{
    if(devName.toStdString() == DevNamesSet[0])
    {
        ui->comboBox->clear();
    }
    else if(devName.toStdString() == DevNamesSet[1])
    {
        ui->comboBox_2->clear();
    }
    else if(devName.toStdString() == DevNamesSet[2])
    {
        ui->comboBox_3->clear();
    }
}

void arch_param_settings::setSbsSoftOnLine()  //电力在线设置.
{
    QImage *label_img_15 = new QImage(":/onLine.png");
    ui->label_15->setPixmap(QPixmap::fromImage(label_img_15->scaled(19,19,Qt::KeepAspectRatio)));
    delete label_img_15;
}
void arch_param_settings::setSbsSoftOffLine()  //电力离线设置.
{
    QImage *label_img_15 = new QImage(":/offLine.png");
    ui->label_15->setPixmap(QPixmap::fromImage(label_img_15->scaled(19,19,Qt::KeepAspectRatio)));
    delete label_img_15;
}

void arch_param_settings::setElecSoftOnLine()  //电力在线设置.
{
    QImage *label_img_16 = new QImage(":/onLine.png");
    ui->label_16->setPixmap(QPixmap::fromImage(label_img_16->scaled(19,19,Qt::KeepAspectRatio)));
    delete label_img_16;
}
void arch_param_settings::setElecSoftOffLine()  //电力离线设置.
{
    QImage *label_img_16 = new QImage(":/offLine.png");
    ui->label_16->setPixmap(QPixmap::fromImage(label_img_16->scaled(19,19,Qt::KeepAspectRatio)));
    delete label_img_16;
}

void arch_param_settings::setCommSoftOnLine()   //通信在线设置.
{
    QImage *label_img_17 = new QImage(":/onLine.png");
    ui->label_17->setPixmap(QPixmap::fromImage(label_img_17->scaled(19,19,Qt::KeepAspectRatio)));
    delete label_img_17;
}
void arch_param_settings::setCommSoftOffLine()   //通信离线设置.
{
    QImage *label_img_17 = new QImage(":/offLine.png");
    ui->label_17->setPixmap(QPixmap::fromImage(label_img_17->scaled(19,19,Qt::KeepAspectRatio)));
    delete label_img_17;
}

void arch_param_settings::setElecAppSoftOnLine()  //电力应用在线设置.
{
    QImage *label_img_18 = new QImage(":/onLine.png");
    ui->label_18->setPixmap(QPixmap::fromImage(label_img_18->scaled(19,19,Qt::KeepAspectRatio)));
    delete label_img_18;
}
void arch_param_settings::setElecAppSoftOffLine()  //电力应用离线设置.
{
    QImage *label_img_18 = new QImage(":/offLine.png");
    ui->label_18->setPixmap(QPixmap::fromImage(label_img_18->scaled(19,19,Qt::KeepAspectRatio)));
    delete label_img_18;
}
