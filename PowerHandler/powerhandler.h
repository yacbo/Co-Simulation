#ifndef POWERHANDLER_H
#define POWERHANDLER_H

#include "Api.hpp"
#include "Utils.hpp"
#include "power_data_def.h"
#include "powerhandler_global.h"

struct BusInfor
{
    int busno;
    double Volt;//电压幅值
    double V_ang;
    double sim_time;
};
struct DGInfor
{
    int busno;
    double dV;
    double timedifference;
};

class POWERHANDLERSHARED_EXPORT PowerHandler
{
public:
    PowerHandler();
    virtual ~PowerHandler();

public:
    bool InitHandler(const char* prj_name, const char* case_name, double sim_time, double sim_period);
    int Execute(int dg_num, EPowerDataType dg_type, PowerSimInputData *dg_infor, int bus_num, EPowerDataType bus_type, PowerSimResultData *bus_infor);
    double QueryCurSimTime(bool realtime = false);

private:
    DataObject* GetChild(DataObject* object, const char* child_name);
    DataObject* GetProject(DataObject* user, const char* prj_name);
    DataObject* GetCaseObject(DataObject* project, const char* case_name);
    DataObject* GetAvrObject(DataObject* prj,  const char *avrname, const char *powerplantname);
    int CalculateTransient(Application* app, double simutime);
    int ExportCalculateResult(Application* app, const char *filename);

    int SetEvents(Application* app, int dg_num, EPowerDataType dg_type, PowerSimInputData* dgInfor, double simtime);

private:
    bool ReadSimRet(const char* filename, SStrVec& mat);

private:
    ApiFixture* _api_fixture;

private:
    Api* _instance;

    int _sim_count;
    double _sim_time;
    double _sim_period;
    double _cur_time;
};

#endif // POWERHANDLER_H
