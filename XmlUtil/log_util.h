#ifndef LOG_UTIL_H
#define LOG_UTIL_H

#include <string>
#include <sstream>
#include <QString>
#include <QMutex>
#include "xmlutil_global.h"

#define MACRO_FIL {\
    char info[1024] = {0};\
    sprintf_s(info, 1024, "func: %s line: %d", __FUNCTION__, __LINE__);  \
    return std::string(info);}

class  XMLUTILSHARED_EXPORT LogUtil
{
private:
    LogUtil();

public:
    static LogUtil* Instance();

public:
    void SetFileName(const QString& name);
    void Output(QtMsgType type, const QString& msg);

public:
    template<typename T, typename...Args>
    QString Output(const T &t, const Args&...rest)
    {
        std::stringstream ss;
        to_string(ss,t,rest...);

        QString msg = ss.str().c_str();
        OutputMsg(QtInfoMsg, msg);

        return msg;
    }

private:
    template <typename T>
    bool to_string(std::stringstream& ss, const T &t)
    {
        ss << t << " ";
        return true;
    }

    template <typename T,typename...Args>
    bool to_string(std::stringstream& ss,const T &t,const Args&...rest)
    {
        ss <<  t  << " " ;
        return to_string(ss,  rest...);
    }

private:
    void OutputMsg(QtMsgType type, const QString& msg);

private:
    QMutex _mutex;
    QString _file_name;
};

#endif // LOG_UTIL_H
