#include "algorithm_util.h"

#include<string>
#include<vector>
#include<sstream>

typedef vector<std::string> StrVec;
typedef vector<StrVec> Mat;

AlgorithmUtil::AlgorithmUtil()
{

}

AlgorithmUtil::~AlgorithmUtil()
{

}

bool AlgorithmUtil::Alth_Calculate_dVg(const char* sim_data,  int bus_num, double* dvg_ret)
{
    std::string tmp;
    StrVec data_vec;
    std::istringstream istr(sim_data);
    while(istr >> tmp){
        data_vec.push_back(tmp);
    }

    BusInfor* businfo = new BusInfor[bus_num];
    for (int m = 0; m < bus_num; ++m){
        businfo[m].busno = m + 1;
        businfo[m].Volt = (double)atof(data_vec[2 * m + 1].c_str());
        businfo[m].V_ang = (double)atof(data_vec[2 * m + 2].c_str());
    }

    bool ret = CalculateDVG(&businfo[0], bus_num, dvg_ret);
    delete[] businfo;

    return ret;
}

bool AlgorithmUtil::Alth_Calculate_CtrlOrder(const char* sim_data, char* order_ret)
{
    std::string tmp;
    StrVec data_vec;
    std::istringstream istr(sim_data);
    while(istr >> tmp){
        data_vec.push_back(tmp);
    }

    const int size_int = sizeof(int);
    const int size_2int = size_int << 1;
    double lne1_power = atof(data_vec[1].c_str());
    double lne2_power = atof(data_vec[2].c_str());

    int flag = 0;
    int exe_sta_bus_id = 40861;
    char gname[32] = {"sym_40861_1"};

    if(lne1_power < 0.001 && lne2_power < 0.001){
        memcpy(order_ret, &flag, size_int);
        memcpy(order_ret + size_int, &exe_sta_bus_id, size_int);
        memcpy(order_ret + size_2int,  gname, sizeof(gname));
    }

    return true;
}

bool AlgorithmUtil::Alth_Calculate_Iterator(const char* sim_data, char* order_ret)
{
    std::string tmp;
    StrVec data_vec;
    std::istringstream istr(sim_data);
    while(istr >> tmp){
        data_vec.push_back(tmp);
    }

    int bus_id = 0;
    double sim_time = std::stod(data_vec[0]);
    if(data_vec[1] == "freq"){
        bus_id = std::stoi(data_vec[2]);
        double freq = std::stod(data_vec[3]);
        //
    }
    else{
        //data[1]: id, data[2]: dp

    }

    return true;
}
