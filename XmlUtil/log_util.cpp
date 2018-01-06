
#include <QFile>
#include <QMap>
#include <QDateTime>
#include <QTextStream>

#include "log_util.h"
#include "comm_type_def.h"

typedef QMap<int, std::string> StrMap;
static StrMap _type_mean_map;

LogUtil::LogUtil()
{
    _file_name = "log.txt";
    InitTypeStrMap();
}

LogUtil::~LogUtil()
{
    if(_log_file.isOpen()){
        _log_file.close();
    }
}

LogUtil* LogUtil::Instance()
{
    static LogUtil log;
    return &log;
}

void LogUtil::SetFileName(const QString& name)
{
    _file_name = name + ".log";
    _log_file.setFileName(_file_name);
    if(!_log_file.open(QIODevice::WriteOnly | QIODevice::Append)){

    }
}

void LogUtil::Output(QtMsgType type, const QString& msg)
{
    OutputMsg(type, msg);
}

void LogUtil::OutputMsg(QtMsgType type, const QString& msg)
{
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString message = QString("[%1] %2").arg(current_date_time).arg(msg);

    if(!_log_file.isOpen()){
        _log_file.open(QIODevice::WriteOnly | QIODevice::Append);
    }

    _mutex.lock();
    QTextStream log_stream(&_log_file);
    log_stream << message << "\r\n";
    _log_file.flush();
    _mutex.unlock();
}

void LogUtil::InitTypeStrMap()
{
    _type_mean_map[eMessage_request] = "[msg: request]";
    _type_mean_map[eMessage_confirm] = "[msg: confirm]";
    _type_mean_map[eMessage_notify] = "[msg: notify]";
    _type_mean_map[eMessage_error] = "[msg: error]";

    _type_mean_map[eSubProcedure_register] = "[subprogress: register]";
    _type_mean_map[eSubProcedure_unregister] = "[subprogress: unregister]";
    _type_mean_map[eSubProcedure_session_begin] = "[subprogress: begin session]";
    _type_mean_map[eSubProcedure_session_end] = "[subprogress: end session]";
    _type_mean_map[eSubProcedure_appl_request] = "[subprogress: application progress request]";
    _type_mean_map[eSubProcedure_data_send] = "[subprogress: send data]";
    _type_mean_map[eSubProcedure_invoke] = "[subprogress: invoke]";
    _type_mean_map[eSubProcedure_note_wait] = "[subprogress: wait notification]";
    _type_mean_map[eSubProcedure_test] = "[subprogress: test]";

    _type_mean_map[eSubProcedure_dev_type_id_data] = "[data: device type&id]";
    _type_mean_map[eSubProcedure_cfg_communication_data] = "[data: communication config]";
    _type_mean_map[eSubProcedure_cfg_power_data] = "[data: power config]";
    _type_mean_map[eSubProcedure_cfg_power_appl_data] = "[data: power application config]";
    _type_mean_map[eSubProcedure_cfg_sim_param_data] = "[data: simulation parameter config]";
    _type_mean_map[eSubProcedure_sim_time_notify_data] = "[data: simulation time notification]";
    _type_mean_map[eSubProcedure_sim_cmd] = "[data: simulation command]";

    _type_mean_map[eSimCmd_start_sim] = "[cmd: start simulation]";
    _type_mean_map[eSimCmd_pause_sim] = "[cmd: pause simulation]";
    _type_mean_map[eSimCmd_keep_sim] = "[cmd: keep simulation]";
    _type_mean_map[eSimCmd_stop_sim] = "[cmd: stop simulation]";

    _type_mean_map[eCommCmd_sim_delay_cfg] = "[data: communication sim max handle delay config]";
    _type_mean_map[eCommCmd_start_send_data] = "[cmd: start send data]";
    _type_mean_map[eCommCmd_stop_send_data] = "[cmd: stop send data]";

    _type_mean_map[eTransData_net_addr] = "[data: network address]";
    _type_mean_map[eTransData_file] = "[data: file]";
    _type_mean_map[eTransData_table] = "[data: table]";
    _type_mean_map[eTransData_variable] = "[data: variable]";
}

const char* parse_type(int type)
{
    StrMap::const_iterator it = _type_mean_map.find(type);
    if(it == _type_mean_map.cend()){
        return "";
    }

    return it.value().c_str();
}
