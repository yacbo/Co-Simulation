#include "sbssoftui.h"
#include "ui_sbssoftui.h"
#include <QPainter>
SbsSoftUI::SbsSoftUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SbsSoftUI)
{
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowMinimizeButtonHint;
    flags |=Qt::WindowMaximizeButtonHint;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    this->setStyleSheet("background-color:#444240;");
    ui->setupUi(this);

    init();
}
void SbsSoftUI::paintEvent(QPaintEvent* pEvent)
{
//    QPixmap _pBg;
//    _pBg.load(":/bac.jpg");
//    QPainter painter;
//    painter.begin(this);
//    QSize s =this->geometry().size();
//    painter.drawPixmap(0,0,s.width(),s.height(),_pBg);
//    painter.end();
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
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    _login_model = new QStandardItemModel();
    _login_model->setColumnCount(4);  //设置4列.
    _login_model->setHeaderData(1,Qt::Horizontal,"器件名称");
    _login_model->setHeaderData(2,Qt::Horizontal,"器件IP");
    _login_model->setHeaderData(3,Qt::Horizontal,"器件端口");
    _login_model->setHeaderData(0,Qt::Horizontal,"器件ID");
    ui->tableView->setModel(_login_model);
    //ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    _login_num = 0;

    ui->tableView_2->horizontalHeader()->setStretchLastSection(true); //就是这个地方
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView_2->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
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

    ui->groupBox_3->setStyleSheet("color:white;border:1px solid black");//border-color:black
    //QGroupBox{border:1px solid black;margin-top: 0.5ex;}
    //ui->groupBox_2->setStyleSheet("color:white;");
   // ui->groupBox->setStyleSheet("color:white;")
    ui->groupBox_2->setStyleSheet("color:white;border:1px solid black;");
    ui->groupBox->setStyleSheet("color:white;border:1px solid black;");
    ui->tableView->setStyleSheet("color:white;border:1px solid black;");
    ui->tableView->horizontalHeader()->setStyleSheet("color:black;");
    ui->tableView->verticalHeader()->setStyleSheet("color:black;");
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableView_2->setStyleSheet("color:white;border:1px solid black;");
    ui->tableView_2->horizontalHeader()->setStyleSheet("color:black;");
    ui->tableView_2->verticalHeader()->setStyleSheet("color:black;");
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->pushButton->setFlat(true);

    ui->label->setStyleSheet("border:0px;");
    ui->label_2->setStyleSheet("border:0px;");
    ui->label_3->setStyleSheet("border:0px;");
    ui->pushButton->setStyleSheet("QPushButton{border-radius:5px; background:#696969; color:white;border:1px solid black;min-height:20;}");
    ui->pushButton_2->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;border:1px solid black;min-height:20;}");

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
    ui->pushButton_2->setStyleSheet("QPushButton{border-radius:5px; background:#696969; color:white;border:1px solid black;min-height:20;}");
    ui->pushButton->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;border:1px solid black;min-height:20;}");
    ui->pushButton_2->setEnabled(true);


    int port = 9999;
    if(!ui->lineEdit->text().isEmpty() && !ui->lineEdit->text().isNull())
    {
        port = ui->lineEdit->text().toInt();
    }
    //参数:监听端口.通信协议.同步方式

     if(_sbs_handler->start_sbs_service(port,E_ProtocolType(ui->comboBox->currentIndex()) , eSynch_undef)) //0 tcp,1,udp
     {

     }
}

//停止服务事件
void SbsSoftUI::exit_service()
{
  if(!ui->pushButton->isEnabled())
  {
      if(_sbs_handler->exit_service())
      {

      }

      ui->pushButton->setStyleSheet("QPushButton{border-radius:5px; background:#696969; color:white;border:1px solid black;min-height:20;}");
      ui->pushButton_2->setStyleSheet("QPushButton{border-radius:5px; background:#444240; color:black;border:1px solid black;min-height:20;}");
      ui->pushButton->setEnabled(true);
      ui->pushButton_2->setEnabled(false);
      ui->lineEdit->setEnabled(true);
      ui->comboBox->setEnabled(true);
      //停止服务的时候，清除"连接的设备"的日志.
      _login_num = 0;
      _login_model->removeRows(0,_login_model->rowCount());

  }
}

void SbsSoftUI::rcv_reg_slot(QString dev_name, QString dev_ip, int dev_port, int dev_id, bool login)
{
    if(login)
    {
        //器件存在则不在打印信息到界面
        QList<QStandardItem*> tList = _login_model->findItems(dev_name);
        if(tList.size()>0) return;
        QString s_dev_id= QString::number(dev_id, 10);
        _login_model->setItem(_login_num,0,new QStandardItem(s_dev_id));
        _login_model->setItem(_login_num,1,new QStandardItem(dev_name));
        _login_model->setItem(_login_num,2,new QStandardItem(dev_ip));
        QString s_port = QString::number(dev_port, 10);
        _login_model->setItem(_login_num,3,new QStandardItem(s_port));
        ++_login_num;
    }
    else
    {
        //查找匹配字符串text的项目，并返回查找结果
        QList<QStandardItem*> tList = _login_model->findItems(dev_name);
        //按照第一列的值查找
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
