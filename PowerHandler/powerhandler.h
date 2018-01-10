#ifndef POWERHANDLER_H
#define POWERHANDLER_H

#include <string>
#include "Api.hpp"
#include "Utils.hpp"
#include "power_data_def.h"
#include "powerhandler_global.h"

class POWERHANDLERSHARED_EXPORT PowerHandler
{
public:
    PowerHandler();
    virtual ~PowerHandler();

public:
    bool InitHandler(EPowerPrjType prj_type, const char* prj_name, const char* case_name, double sim_time, double sim_period);
    int Execute(int sd_num, EPowerDataType sd_type, const PowerSDDataVec& sd_data, int su_num, EPowerDataType su_type, const PowerSUDataVec& su_data);
    double QueryCurSimTime(bool realtime = false);
    void ExitHandler();

private:
    DataObject* GetChild(DataObject* object, const char* child_name);
    DataObject* GetProject(DataObject* user, const char* prj_name);
    DataObject* GetCaseObject(DataObject* project, const char* case_name);
    DataObject* GetAvrObject(DataObject* prj,  const char *avrname, const char *powerplantname);
    DataObject* GetGenerateObject(DataObject* prj, const char *gname, const char *gridname);
    int CalculateTransient(Application* app, double simutime);
    int ExportCalculateResult(Application* app, const char *filename);

private:
    int SetAVREvents(Application* app, int sd_num, EPowerDataType sd_type, const PowerSDDataVec& sd_data, double simtime);
    int SetPsseJSEvents(Application* app, int sd_num, EPowerDataType sd_type, const PowerSDDataVec& sd_data, double simtime);

private:
    bool ReadSimRet(const char* filename, SStrVec& mat);

private:
    Api* _instance;
    ApiFixture* _api_fixture;

private:
    int _sim_count;
    double _sim_time;
    double _sim_period;
    double _cur_time;

private:
    std::string _prj_case;
    std::string _prj_name;
    EPowerPrjType _prj_type;
};

#endif // POWERHANDLER_H
