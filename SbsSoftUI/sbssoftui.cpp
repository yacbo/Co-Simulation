#include "sbssoftui.h"
#include "ui_sbssoftui.h"
#include <QPainter>
#include <QHeaderView>
#include "log_util.h"

SbsSoftUI::SbsSoftUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SbsSoftUI)
{
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowMinimizeButtonHint;
    flags |=Qt::WindowMaximizeButtonHint;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    //this->setStyleSheet("background-color:#444240;");
    ui->setupUi(this);

    init();
}
void SbsSoftUI::paintEvent(QPaintEvent* pEvent)
{
     QPixmap _pBg;
     _pBg.load(":/bac.jpg");
     QPainter painter;
     painter.begin(this);
     QSize s =this->geometry().size();
     painter.drawPixmap(0,0,s.width(),s.height(),_pBg);
     painter.end();
}
SbsSoftUI::~SbsSoftUI()
{
    if(_sbs_handler){
        delete _sbs_handler;
    }

    delete ui;
}

void SbsSoftUI::init()
{
    _sbs_handler = new SbSHandler();
    connect(_sbs_handler, &SbSHandler::ui_login_signal, this, &SbsSoftUI::rcv_reg_slot, Qt::UniqueConnection);
    connect(_sbs_handler, &SbSHandler::progress_log_signal, this, &SbsSoftUI::rcv_progress_log_slot, Qt::UniqueConnection);

    //启动服务按钮对应事件.
    connect(ui->pushButton, &QPushButton::clicked, this, &SbsSoftUI::start_service, Qt::UniqueConnection);
    //停止服务按钮对应事件.
    connect(ui->pushButton_2, &QPushButton::clicked, this, &SbsSoftUI::exit_service, Qt::UniqueConnection);

    ui->tableView->horizontalHeader()->setStretchLastSection(true); //就是这个地方
    QPalette pll_1 = ui->tableView->palette();
    pll_1.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));
    ui->tableView->setPalette(pll_1);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    _login_model = new QStandardItemModel();
    _login_model->setColumnCount(4);  //设置4列.
    _login_model->setHeaderData(0,Qt::Horizontal,"器件ID");
    _login_model->setHeaderData(1,Qt::Horizontal,"器件名称");
    _login_model->setHeaderData(2,Qt::Horizontal,"器件IP");
    _login_model->setHeaderData(3,Qt::Horizontal,"器件端口");
    ui->tableView->setModel(_login_model);

    //ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    _login_num = 0;

    ui->tableView_2->horizontalHeader()->setStretchLastSection(true); //就是这个地方

    QPalette pll = ui->tableView_2->palette();
    pll.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));
    ui->tableView_2->setPalette(pll);

    //ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->tableView_2->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    _log_model = new QStandardItemModel();
    _log_model->setColumnCount(2);  //设置3列.
    _log_model->setHeaderData(0,Qt::Horizontal,"时间");
    _log_model->setHeaderData(1,Qt::Horizontal,"信息");
    ui->tableView_2->setModel(_log_model);
    //ui->tableView_2->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    _log_num = 0;

    //给监听端口设置初值.
    ui->lineEdit->setText("9999");
    //设置本机ip值
    ui->lineEdit_2->setText(_sbs_handler->get_local_ip());
    ui->lineEdit_2->setEnabled(false);
    //禁用"停止服务"按钮
    ui->pushButton_2->setEnabled(false);

    ui->groupBox_3->setStyleSheet("color:black;border:1px solid black");//border-color:black
    //QGroupBox{border:1px solid black;margin-top: 0.5ex;}
    //ui->groupBox_2->setStyleSheet("color:black;");
   // ui->groupBox->setStyleSheet("color:black;")
    ui->groupBox_2->setStyleSheet("color:black;border:1px solid black;");
    ui->groupBox->setStyleSheet("color:black;border:1px solid black;");
    ui->tableView->setStyleSheet("color:black;border:1px solid black;");
    //ui->tableView->horizontalHeader()->setStyleSheet("color:black;");
    //ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:#444240;color:black;border:1px solid #5c5c5c;}");
    //ui->tableView->verticalHeader()->hide();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableView_2->setStyleSheet("color:black;border:1px solid black;");
    //ui->tableView_2->horizontalHeader()->setStyleSheet("color:black;");
    //ui->tableView_2->horizontalHeader()->setStyleSheet("QHeaderView {background-color:#444240 } QHeaderView::section {background-color:#444240;color:black;border:1px solid #5c5c5c;}");
    //ui->tableView_2->verticalHeader()->setStyleSheet("QHeaderView {background-color:#444240 } QHeaderView::section {background-color:#444240;color:black;padding-left: 4px;border:1px solid #2c2c2c;}");
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->label->setStyleSheet("border:0px;");
    ui->label_2->setStyleSheet("border:0px;");
    ui->label_3->setStyleSheet("border:0px;");
    //ui->pushButton->setStyleSheet("QPushButton{border-radius:5px; background:#696969; color:black;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
    //ui->pushButton_2->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);

}

void SbsSoftUI::start_service()
{

     //禁用lineEdit，port
    ui->lineEdit->setEnabled(false);
    //禁用combox
    ui->comboBox->setEnabled(false);
    //禁用开始服务button
    ui->pushButton->setEnabled(false);
    //开启停止服务
    ui->pushButton_2->setEnabled(true);
    //ui->pushButton_2->setStyleSheet("QPushButton{border-radius:5px; background:#696969; color:black;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
    //ui->pushButton->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);


    int port = 9999;
    if(!ui->lineEdit->text().isEmpty() && !ui->lineEdit->text().isNull())
    {
        port = ui->lineEdit->text().toInt();
    }
    //参数:监听端口.通信协议.同步方式

     if(_sbs_handler->start_sbs_service(port,E_ProtocolType(ui->comboBox->currentIndex()) , eSynch_undef)) //0 tcp,1,udp
     {

     }

     //操作日志
      LogUtil::Instance()->Output(MACRO_LOCAL, "[start_service]"," localhost:",ui->lineEdit_2->text().toStdString()," Port:",ui->lineEdit->text().toStdString()," ProtocolType:",ui->comboBox->currentText().toStdString());
}

//停止服务事件
void SbsSoftUI::exit_service()
{
  if(!ui->pushButton->isEnabled())
  {
      if(_sbs_handler->exit_service())
      {

      }
      ui->pushButton->setEnabled(true);
      ui->pushButton_2->setEnabled(false);
      //ui->pushButton->setStyleSheet("QPushButton{border-radius:5px; background:#696969; color:black;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);
      //ui->pushButton_2->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;border:1px solid black;min-height:20;}" MACRO_CSS_BUTTON_HOVER MACRO_CSS_BUTTON_PRESS);

      ui->lineEdit->setEnabled(true);
      ui->comboBox->setEnabled(true);
      //停止服务的时候，清除"连接的设备"的日志.
      _login_num = 0;
      _login_model->removeRows(0,_login_model->rowCount());

      //操作日志
       LogUtil::Instance()->Output(MACRO_LOCAL, "[exit_service]"," localhost:",ui->lineEdit_2->text().toStdString()," Port:",ui->lineEdit->text().toStdString()," ProtocolType:",ui->comboBox->currentText().toStdString());

  }
}

void SbsSoftUI::rcv_reg_slot(QString dev_name, QString dev_ip, int dev_port, int dev_id, bool login)
{
    QString s_dev_id= QString::number(dev_id, 10);

    //查找匹配字符串text的项目，并返回查找结果
    QList<QStandardItem*> tList = _login_model->findItems(s_dev_id);

    if(login)
    {
        //器件存在则不打印信息到界面
        if(tList.size()>0)
        {
            return;
        }

        _login_model->setItem(_login_num,0,new QStandardItem(s_dev_id));
        _login_model->item(_login_num, 0)->setTextAlignment(Qt::AlignCenter);
        _login_model->setItem(_login_num,1,new QStandardItem(dev_name));
        _login_model->item(_login_num, 1)->setTextAlignment(Qt::AlignCenter);
        _login_model->setItem(_login_num,2,new QStandardItem(dev_ip));
        _login_model->item(_login_num, 2)->setTextAlignment(Qt::AlignCenter);
        QString s_port = QString::number(dev_port, 10);
        _login_model->setItem(_login_num,3,new QStandardItem(s_port));
        _login_model->item(_login_num, 3)->setTextAlignment(Qt::AlignCenter);
        ++_login_num;

        //操作日志 记录连接的器件
         QString log = LogUtil::Instance()->Output(MACRO_LOCAL, "[online device]"," dev_id:",s_dev_id.toStdString()," dev_name:",dev_name.toStdString()," dev_ip:",dev_ip.toStdString()," dev_port:",dev_port);
         rcv_progress_log_slot(log);//打印到界面
    }
    else
    {
        //器件不存在
        if(tList.size() == 0)
        {
            return;
        }
        //操作日志 记录删除连接的器件
         QString log = LogUtil::Instance()->Output(MACRO_LOCAL, "[offline device]"," dev_id:",s_dev_id.toStdString()," dev_name:",dev_name.toStdString()," dev_ip:",dev_ip.toStdString()," dev_port:",dev_port);
         rcv_progress_log_slot(log);//打印到界面
         //定位“设备id”列
        QStandardItem* tItem = tList.at(0);
        _login_model->removeRow(tItem->row());//移除
        --_login_num;
    }
}

void SbsSoftUI::rcv_progress_log_slot(QString log)
{
    QDateTime current_date = QDateTime::currentDateTime();
    QString date = current_date.toString("yyyy-MM-dd hh:mm:ss");
    _log_model->setItem(_log_num,0,new QStandardItem(date));
    _log_model->setItem(_log_num,1,new QStandardItem(log));
    ++_log_num;
}
