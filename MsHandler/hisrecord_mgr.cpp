
#include <io.h>
#include<iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include "hisrecord_mgr.h"
#include "power_data_def.h"

HisRecordMgr::HisRecordMgr()
    : _rec_name_prefix("./sim-rec/his-rec-")
    , _rec_name_postfix(".rec")
{
    load_file_list(_file_list_vec);
}

HisRecordMgr::~HisRecordMgr()
{

}

bool  HisRecordMgr::load_file_list(StrVec& vec)
{
    struct _finddata_t file;
    const char* folder = "./sim-rec";
    const char* local_dir = ".";
    const char* parent_dir = "..";

    long hndl = _findfirst( folder, &file);
    if ( hndl < 0 ){
        return false;
    }

    int  ret  = 1;
    while ( ret >= 0 ){
        if(file.attrib != _A_SUBDIR && strcmp(file.name, local_dir) != 0 && strcmp(file.name, parent_dir) != 0){
            _file_list_vec.push_back(file.name);
        }
        ret = _findnext( hndl, &file);
    }

    _findclose(hndl);

    return true;
}

bool HisRecordMgr::load_recodes(UnionSimDatVec& vec, int64_t sim_time)
{
    string file_name;
    if(sim_time > 0){
        file_name = std::to_string(sim_time) + _rec_name_postfix;;
    }
    else if(!_file_list_vec.empty()){
        file_name = _file_list_vec.back();
    }
    else{
        return false;
    }

    string rec_path = _rec_name_prefix + file_name;
    std::ifstream in(rec_path);
    if(in.fail()){
        return false;
    }

    StrVec dat_vec;
    string line_data, tmp_dat;
    getline(in, line_data);        //skip header

    while(getline(in, line_data)){
        std::istringstream istr(line_data);
        while(istr >> tmp_dat){
            dat_vec.push_back(tmp_dat);
        }

        UnionSimData dat;
        PowerBusInforData pbid;
        dat.sim_time = std::stod(dat_vec[0]);
        pbid.bus_id = std::stoul(dat_vec[1]);
        dat.comm_dat.src_id = std::stoul(dat_vec[2]);
        dat.comm_dat.dst_id = std::stoul(dat_vec[3]);
        dat.comm_dat.err_type  = (EErrorType)std::stoi(dat_vec[4]);
        dat.comm_dat.trans_delay = std::stol(dat_vec[5]);
        pbid.bus_volt = std::stod(dat_vec[6]);
        pbid.bus_angle = std::stod(dat_vec[7]);

        memcpy(dat.power_dat, &pbid, sizeof(PowerBusInforData));

        vec.push_back(dat);
    }

    return true;
}

bool HisRecordMgr::write_recordes(int64_t sim_time, const UnionSimDatVec& vec)
{
    if(vec.size() <= 0){
        return false;
    }

    string file_name = std::to_string(sim_time) + _rec_name_postfix;;
    string rec_path = _rec_name_prefix + file_name;
    std::ofstream out(rec_path);
    if(out.fail()){
        return false;
    }

    out << std::setw(16) << "sim time"
          << std::setw(10) << "power bus id"
          << std::setw(10) << "comm src id"
          << std::setw(10) << "ocmm dst id"
          << std::setw(10) << "comm err type"
          << std::setw(10) << "comm tr delay"
          << std::setw(10) << "power voltage"
          << std::setw(10) << "power angle";

    for(int i=0; i<vec.size(); ++i){
        const UnionSimData& dat = vec[i];
        const PowerBusInforData* pbid = (const PowerBusInforData*)dat.power_dat ;
        out << dat.sim_time
              << pbid->bus_id
              << dat.comm_dat.src_id
              << dat.comm_dat.dst_id
              << dat.comm_dat.err_type
              << dat.comm_dat.trans_delay
              << pbid->bus_volt
              << pbid->bus_angle;
    }

    _file_list_vec.push_back(file_name);

    out.close();
}
