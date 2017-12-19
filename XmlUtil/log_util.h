#ifndef LOG_UTIL_H
#define LOG_UTIL_H

#include <QMutex>
#include "xmlutil_global.h"

class  XMLUTILSHARED_EXPORT LogUtil
{
private:
    LogUtil();

public:
    static LogUtil* Instance();
    static void SetFileName(const QString& name);
    static void Output(QtMsgType type, const QString& msg);

private:
    static void OutputMsg(QtMsgType type, const QString& msg);

private:
    static QMutex _mutex;
    static QString _file_name;
};

#endif // LOG_UTIL_H
