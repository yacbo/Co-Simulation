#ifndef LOG_UTIL_H
#define LOG_UTIL_H

#include <string>
#include <sstream>
#include <QString>
#include <QMutex>
#include "xmlutil_global.h"

#define MACRO_CLASS(x) #x
#define MACRO_LINE __LINE__
#define MACRO_FUNCTION __FUNCTION__

#define MACRO_SUCFAIL(x) (x ? "successfully" : "failed")

#define MACRO_IN1_LOCAL(c, f, l) ("<"##c##"::"f ":"###l##">")
#define MACRO_IN2_LOCAL(c, f, l) MACRO_IN1_LOCAL(c, f, l)
#define MACRO_LOCAL(c)  MACRO_IN2_LOCAL(MACRO_CLASS(c),  MACRO_FUNCTION,  MACRO_LINE)

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
    void InitTypeStrMap();
    void OutputMsg(QtMsgType type, const QString& msg);

private:
    QMutex _mutex;
    QString _file_name;
};

XMLUTILSHARED_EXPORT const char* parse_type(int type);

#endif // LOG_UTIL_H
