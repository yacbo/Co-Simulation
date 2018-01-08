#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include "powerhandler.h"

PowerHandler::PowerHandler()
{
    _sim_count = 1;
    _instance = nullptr;

    _api_fixture = new ApiFixture();
}

PowerHandler::~PowerHandler()
{
    if(_api_fixture){
        delete _api_fixture;
        _api_fixture = nullptr;
    }
}

bool PowerHandler::InitHandler(const char* prj_name, const char* case_name, double sim_time, double sim_period)
{
    _instance = _api_fixture->GetInstance();      //ApiFixture is responsible for creation/destruction api instance
    if (!_instance) {
        //std::cout << "InitHandler, error:ApiFixture::GetInstance()" << std::endl;
        return false;
    }

    int ret = 0;
    Application* app = _instance->GetApplication();
    DataObject* user = app->GetCurrentUser();
    DataObject* prj = GetProject(user, prj_name);
    DataObject* e_case = GetCaseObject(prj, case_name);

    Value v_prj(prj);
    Value v_case(e_case);
    if (prj != app->GetActiveProject()){
        app->Execute("Activate", &v_prj, &ret);                     //激活项目
        if (ret > 0){
            //std::cout << "InitHandler, error: Execute v_prj" << std::endl;
            return false;
        }
    }

    if (e_case != app->GetActiveStudyCase()){
        app->Execute("Activate", &v_case, &ret);//激活项目
        if (ret > 0){
           // std::cout << "InitHandler, error: Execute v_case" << std::endl;
            return false;
        }
    }

    //delete the last EvtParam for no influencing the next simulation
    Value v_ev("IntEvt");
    ValueGuard int_ev(app->Execute("GetCaseObject", &v_ev));
    DataObject* events = int_ev->GetDataObject();
    DataObjVec folders = Utils::GetChildren(events, "EvtParam");

    DataObjVec::iterator it = folders.begin();
    for (; it != folders.end(); ++it) {
        DataObject* folder = *it;
        folder->DeleteObject();
    }

    _sim_time = sim_time;
    _sim_period = sim_period;

    //std::cout << "InitHandler, success." << std::endl;

    return true;
}

int PowerHandler::Execute(int dg_num, EPowerDataType dg_type, const PowerSDDataVec& dg_infor, int bus_num, EPowerDataType bus_type, const PowerSUDataVec& bus_infor)
{
    int err = 0;
    Application* app = _instance->GetApplication();

    if(!app){
        //std::cout << "Execute, error: _instance->GetApplication()" << std::endl;
        return -1;
    }

    _cur_time = _sim_period * _sim_count++;
    if(_cur_time >= _sim_time){  //exit simulation
        return -2;
    }

    double simutime = _cur_time;
    err = SetEvents(app, dg_num, dg_type, dg_infor, simutime);
    if(err > 0){
        //std::cout << "Execute, error: SetEvents" << std::endl;
        return -3;
    }

    err = CalculateTransient(app, simutime);         //to perform a transient calculation
    if (err > 0){                                                       //Error: Transient calculation failed
        //std::cout << "Execute, error: CalculateTransient" << std::endl;
        return -4;
    }

    const char *filename = "calc_data.txt";
    err = ExportCalculateResult(app, filename);              //Export the calculation result.
    if (err > 0){                                                                //Error: Result Export failed
        //std::cout << "Execute, error: ExportCalculateResult" << std::endl;
        return -5;
    }

    SStrVec mat;
    if(!ReadSimRet(filename, mat)){
        //std::cout << "Execute, error: ReadSimRet" << std::endl;
        return -6;
    }

    if(bus_type == ePowerData_businfor){
        int current = mat.size();
        for (int i = 0; i < bus_num; i++){
            PowerBusInforData* info = (PowerBusInforData*)bus_infor[i];
            info->bus_id = i + 1;
            info->cur_sim_time = _cur_time;
            info->bus_volt = (double)atof(mat[current - 1][2 * i + 1].c_str());
            info->bus_angle = (double)atof(mat[current - 1][2 * i + 2].c_str());
        }
    }

    //std::cout << "Execute, success: Execute" << std::endl;

    return 0;
}

double PowerHandler::QueryCurSimTime(bool realtime)
{
    if(!realtime){
        return _cur_time;
    }

    return 0.0;
}

void PowerHandler::ExitHandler()
{
    if(_api_fixture){
        delete _api_fixture;
        _api_fixture = nullptr;
    }
}

DataObject* PowerHandler::GetChild(DataObject* object, const char* child_name)
{
    DataObject* d_chi = nullptr;
    const Value* children = object->GetChildren(false);

    for (int i = 0; i < children->VecGetSize(); ++i){
        DataObject* child = children->VecGetDataObject(i);
        if (strcmp(child->GetName()->GetString(), child_name) == 0){
            d_chi = child;
            break;
        }
    }

    return d_chi;
}

DataObject* PowerHandler::GetProject(DataObject* user, const char* prj_name)
{
    DataObject* d_prj = nullptr;
    DataObjVec folders = Utils::GetChildren(user, "IntPrj");

    DataObjVec::iterator it =  folders.begin();
    for (; it != folders.end(); ++it) {
        DataObject* project = *it;
        ValueGuard name(project->GetName());
        if (strstr(name->GetString(), prj_name)) {
            d_prj = project;
            break;
        }
    }

    return d_prj;
}

DataObject* PowerHandler::GetCaseObject(DataObject* project, const char* case_name)
{
    DataObject* d_case = nullptr;
    DataObjVec folders = Utils::GetChildren(project, "IntPrjfolder");

    DataObjVec::iterator it =  folders.begin();
    for (; it != folders.end(); ++it) {
        DataObject* studyCases = *it;
        ValueGuard name(studyCases->GetName());
        if (!strstr(name->GetString(), "Study Cases")) {
            continue;
        }

        ValueGuard children(studyCases->GetChildren(false));
        for (int i = 0, count = children->VecGetSize(); i < count; ++i) {
             DataObject* child = children->VecGetDataObject(i);
             if (strstr(child->GetName()->GetString(), case_name)) {
                  d_case = child;
                  break;
             }
         }
    }

    return d_case;
}

DataObject* PowerHandler::GetAvrObject(DataObject* prj,  const char *avrname, const char *powerplantname)
{
    DataObject* d_avr = nullptr;
    DataObject* networkmodel = GetChild(prj, "Network Model");
    DataObject* networkdata = GetChild(networkmodel, "Network Data");
    DataObject* mygrid = GetChild(networkdata, "Grid");

    api::DataObject* powerplant = GetChild(mygrid, powerplantname);
    d_avr = GetChild(powerplant, avrname);

    return d_avr;
}

//--------------------------------------------------------------------------------
/// Performs a transient calculation for active project.
///
/// @return     0 if calculation was successful
///                   1 on error
//--------------------------------------------------------------------------------
int PowerHandler::CalculateTransient(Application* app, double simutime)
{
    DataObject* prj = app->GetActiveProject();
    if (!prj) {
        return 1;
    }
    //run a Inc
    Value com_inc("ComInc");
    ValueGuard comInc(app->Execute("GetCaseObject", &com_inc));
    comInc->GetDataObject()->Execute("Execute", nullptr);

    //run a transient calculation
    Value com_sim("ComSim");
    ValueGuard comSim(app->Execute("GetCaseObject", &com_sim));
    app->DefineTransferAttributes("ComSim", "tstop");
    Value args(Value::VECTOR);
    args.VecInsertDouble(simutime);

    int ret = 0;
    comSim->GetDataObject()->SetAttributes(&args, &ret);
    if (ret > 0) {
        return 1;
    }

    comSim->GetDataObject()->Execute("Execute", nullptr);

    if (ValueGuard(app->Execute("IsSimValid", nullptr))->GetInteger() == 0) {
        return 1;
    }

    return 0;
}

//--------------------------------------------------------------------------------
/// Performs a result export for active project.
///
/// @return     0 if calculation was successful
///                   1 on error
//--------------------------------------------------------------------------------
int PowerHandler::ExportCalculateResult(Application* app, const char *filename)
{
    if (!app || !filename) {
        return 1;
    }
    //run a Result Export
    Value com_res("ComRes");
    ValueGuard comRes(app->Execute("GetCaseObject", &com_res));
    app->DefineTransferAttributes("ComRes", "iopt_exp,f_name");

    Value atts(Value::VECTOR);
    atts.VecInsertInteger(4);
    atts.VecInsertString(filename);

    int ret = 0;
    comRes->GetDataObject()->SetAttributes(&atts, &ret);
    if (ret > 0) {
        return 1;
    }

    comRes->GetDataObject()->Execute("Execute", nullptr, &ret);

    return ret;
}

//--------------------------------------------------------------------------------
/// Set events  for active project.
///
/// @return     0 if calculation was successful
///                   1 on error
//--------------------------------------------------------------------------------
int PowerHandler::SetEvents(Application* app, int dg_num, EPowerDataType dg_type, const PowerSDDataVec& dgInfor, double simtime)
{
    DataObject* prj = app->GetActiveProject();
    if (!prj) {
        return 1;
    }

    //run a Result Export
    Value int_evt("IntEvt");
    ValueGuard intEvt(app->Execute("GetCaseObject", &int_evt));
    DataObject* events = intEvt->GetDataObject();

    const int obj_num = dg_num;
    DataObject* paramevent[10];
    DataObject* dslavr[10];

    char avr[32] = {0}, pp[32] = {0};
    for(int i=0; i<dg_num - 1; ++i){
        sprintf_s(avr, "AVR 0%d", i + 1);
        sprintf_s(pp, "Power Plant 0%d", i + 1);
        dslavr[i] = GetAvrObject(prj, avr, pp);
    }
    dslavr[dg_num - 1] = GetAvrObject(prj, "AVR 10", "Power Plant 10");

    int error = 0;
    char avrparam[32] = {0};
    for(int i=0; i<dg_num; ++i){
        sprintf_s(avrparam, "avrparam%d", i + 1);
        paramevent[i] = events->CreateObject("EvtParam", avrparam);
        paramevent[i]->SetAttributeObject("p_target", dslavr[i], &error);
        paramevent[i]->SetAttributeString("variable", "Vbias2", &error);
    }

    double etime = simtime;
    for (int i = 0; i < dg_num; ++i){
        paramevent[i]->SetAttributeDouble("hrtime", 0, &error);                //这边时间为当前仿真时刻+通信时延
        paramevent[i]->SetAttributeDouble("mtime", 0, &error);                 //这边时间为当前仿真时刻+通信时延
        paramevent[i]->SetAttributeDouble("time", etime, &error);              //这边时间为当前仿真时刻+通信时延

        if(dg_type == ePowerData_dginfor){
            const PowerDGInforData* dg_infor = (const PowerDGInforData*)dgInfor[i];
            paramevent[i]->SetAttributeDouble("value", dg_infor->dv, &error); //这边值为发电机机端电压调整量
        }
    }

    return error;
}

bool PowerHandler::ReadSimRet(const char* filename, SStrVec& mat)
{
    std::ifstream in(filename);
    if(!in.is_open()){
        return false;
    }

    mat.clear();
    std::string line, data;
    while (getline(in, line)){
        StrVec var;
        std::istringstream istr(line);
        while (istr >> data){
            var.push_back(data);
        }
        mat.push_back(var);
    }

    return true;
}

