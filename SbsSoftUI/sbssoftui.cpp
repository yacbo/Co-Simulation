#include "sbssoftui.h"
#include "ui_sbssoftui.h"

SbsSoftUI::SbsSoftUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SbsSoftUI)
{
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowMinimizeButtonHint;
    flags |=Qt::WindowMaximizeButtonHint;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    this->setStyleSheet("background-image: url(:/bac.jpg)");
//    QPixmap pixmap = QPixmap(":/bac.jpg").scaled(this->size());
//    QPalette palette(this->palette());
//    palette.setBrush(QPalette::Background, QBrush(pixmap));
//    this->setPalette(palette);
    ui->setupUi(this);

    init();
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

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    _login_model = new QStandardItemModel();
    _login_model->setColumnCount(3);  //设置3列.
    _login_model->setHeaderData(0,Qt::Horizontal,"器件名称");
    _login_model->setHeaderData(1,Qt::Horizontal,"器件地址");
    _login_model->setHeaderData(2,Qt::Horizontal,"器件端口");
    ui->tableView->setModel(_login_model);
    //ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    _login_num = 0;

    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
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

      ui->pushButton->setEnabled(true);
      ui->pushButton_2->setEnabled(false);
      ui->lineEdit->setEnabled(true);
      ui->comboBox->setEnabled(true);

      //停止服务的时候，清除"连接的设备"的日志.
      _login_num = 0;
      _login_model->removeRows(0,_login_model->rowCount());

  }
}

void SbsSoftUI::rcv_reg_slot(QString dev_name, QString dev_ip, int dev_port, bool login)
{
    if(login)
    {
        //器件存在则不在打印信息到界面
        QList<QStandardItem*> tList = _login_model->findItems(dev_name);
        if(tList.size()>0) return;

        _login_model->setItem(_login_num,0,new QStandardItem(dev_name));
        _login_model->setItem(_login_num,1,new QStandardItem(dev_ip));
        QString sport = QString::number(dev_port, 10);
        _login_model->setItem(_login_num,2,new QStandardItem(sport));
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
