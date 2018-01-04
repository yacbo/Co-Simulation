
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include "log_util.h"

LogUtil::LogUtil()
{
    _file_name = "log.txt";
}

LogUtil* LogUtil::Instance()
{
    static LogUtil log;
    return &log;
}

void LogUtil::SetFileName(const QString& name)
{
    _file_name = name + ".log";
}

void LogUtil::Output(QtMsgType type, const QString& msg)
{
    OutputMsg(type, msg);
}

void LogUtil::OutputMsg(QtMsgType type, const QString& msg)
{
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString message = QString("[%1] %2").arg(current_date_time).arg(msg);

    _mutex.lock();
    QFile log(_file_name);
    if(log.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        QTextStream log_stream(&log);
        log_stream << message << "\r\n";
        log.flush();
        log.close();
    }

    _mutex.unlock();
}
