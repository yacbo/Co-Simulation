
#include <io.h>
#include<iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <QDir>
#include <QCoreApplication>
#include "hisrecord_mgr.h"
#include "power_data_def.h"
#include "log_util.h"

HisRecordMgr::HisRecordMgr()
    : _rec_base_dir(QCoreApplication::applicationDirPath().toStdString())
    , _rec_name_prefix("/sim-rec/his-rec-")
    , _rec_name_postfix(".rec")
{
    load_file_list(_file_list_vec);
}

HisRecordMgr::~HisRecordMgr()
{

}

HisRecordMgr* HisRecordMgr::instance()
{
    static HisRecordMgr mgr;
    return &mgr;
}

bool HisRecordMgr::check_folder(const string& folder)
{
    QString path(folder.c_str());
    QDir dir(path);
    if(dir.exists()){
      return true;
    }

    LogUtil::Instance()->Output(MACRO_LOCAL, "create history record folder:", folder);

    //创建多级目录
    return dir.mkpath(path);
}

bool HisRecordMgr::load_file_list(StrVec& vec)
{
    string folder = _rec_base_dir + "/sim-rec";
    check_folder(folder);

    const char* local_dir = ".";
    const char* parent_dir = "..";

    struct _finddata_t file;
    long hndl = _findfirst( folder.c_str(), &file);
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

bool HisRecordMgr::load_record(int power_data_type, UnionSimDatVec& vec, int64_t sim_time)
{
    bool ret = false;
    switch(power_data_type){
    case ePowerData_businfor: ret = load_businfor_record(vec, sim_time); break;
    case ePowerData_freqinfor: ret = load_commctrl_record(vec, sim_time); break;
    default: break;
    }

    return ret;
}

bool HisRecordMgr::write_record(int64_t sim_time, int power_data_type, const UnionSimDatVec& vec)
{
    if(vec.size() <= 0){
        return false;
    }

    bool ret = false;
    switch(power_data_type){
    case ePowerData_businfor: ret = write_businfor_record(sim_time, vec); break;
    case ePowerData_freqinfor: ret = write_commctrl_record(sim_time, vec); break;
    default: break;
    }

    return ret;
}

bool HisRecordMgr::fill_record(int power_data_type, const UnionSimDatVec& his, UnionSimDatVec& record)
{
    bool ret = false;
    switch(power_data_type){
    case ePowerData_businfor: ret = fill_businfor_record(his, record); break;
    case ePowerData_freqinfor: ret = fill_commctrl_record(his, record); break;
    default: break;
    }

    return ret;
}

bool HisRecordMgr::load_businfor_record(UnionSimDatVec& vec, int64_t sim_time)
{
    string file_name;
    if(sim_time > 0){
        file_name = std::to_string(sim_time) + _rec_name_postfix;;
    }
    else if(!_file_list_vec.empty()){
        file_name = _file_list_vec.back();
    }
    else{
        LogUtil::Instance()->Output(MACRO_LOCAL, "not exist history record and current simulation time is:", sim_time);
        return false;
    }

    string rec_path = file_name;
    std::ifstream in(rec_path);
    if(in.fail()){
        LogUtil::Instance()->Output(MACRO_LOCAL, "open file:", rec_path, "failed");
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
        dat_vec.clear();
    }

    in.close();

    return true;
}

bool HisRecordMgr::load_commctrl_record(UnionSimDatVec& vec, int64_t sim_time)
{
    string file_name;
    if(sim_time > 0){
        file_name = std::to_string(sim_time) + _rec_name_postfix;;
    }
    else if(!_file_list_vec.empty()){
        file_name = _file_list_vec.back();
    }
    else{
        LogUtil::Instance()->Output(MACRO_LOCAL, "not exist history record and current simulation time is:", sim_time);
        return false;
    }

    string rec_path = file_name;
    std::ifstream in(rec_path);
    if(in.fail()){
        LogUtil::Instance()->Output(MACRO_LOCAL, "open file:", rec_path, "failed");
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
        dat.sim_time = std::stod(dat_vec[0]);
        dat.comm_dat.src_id = std::stoul(dat_vec[1]);
        dat.comm_dat.dst_id = std::stoul(dat_vec[2]);
        dat.comm_dat.err_type  = (EErrorType)std::stoi(dat_vec[3]);
        dat.comm_dat.trans_delay = std::stol(dat_vec[4]);

        QByteArray d_base64(dat_vec[5].c_str(), dat_vec[5].length());
        QByteArray d = QByteArray::fromBase64(d_base64);
        memcpy(dat.power_dat, d.data(), sizeof(dat.power_dat));

        vec.push_back(dat);
        dat_vec.clear();
    }

    in.close();

    return true;
}

bool HisRecordMgr::write_businfor_record(int64_t sim_time, const UnionSimDatVec& vec)
{
    string file_name = std::to_string(sim_time) + _rec_name_postfix;;
    string rec_path = _rec_base_dir + _rec_name_prefix + file_name;
    std::ofstream out(rec_path);
    if(out.fail()){
        LogUtil::Instance()->Output(MACRO_LOCAL, "open file:", rec_path, "failed");
        return false;
    }

    out << "sim time"
          << std::setw(20) << "power bus id"
          << std::setw(20) << "comm src id"
          << std::setw(20) << "comm dst id"
          << std::setw(20) << "comm err type"
          << std::setw(20) << "comm tr delay"
          << std::setw(20) << "power voltage"
          << std::setw(20) << "power angle"
          << std::endl;

    for(int i=0; i<vec.size(); ++i){
        const UnionSimData& dat = vec[i];
        const PowerBusInforData* pbid = (const PowerBusInforData*)dat.power_dat ;
        out << dat.sim_time
              << std::setw(20) << pbid->bus_id
              << std::setw(20) << dat.comm_dat.src_id
              << std::setw(20) << dat.comm_dat.dst_id
              << std::setw(20) << dat.comm_dat.err_type
              << std::setw(20) << dat.comm_dat.trans_delay
              << std::setw(20) << pbid->bus_volt
              << std::setw(20) << pbid->bus_angle
              << std::endl;
    }

    out.close();

    _file_list_vec.push_back(rec_path);

    return true;
}

bool HisRecordMgr::write_commctrl_record(int64_t sim_time, const UnionSimDatVec& vec)
{
        string file_name = std::to_string(sim_time) + _rec_name_postfix;;
        string rec_path = _rec_base_dir + _rec_name_prefix + file_name;
        std::ofstream out(rec_path, std::ios::trunc);
        if(out.fail()){
            LogUtil::Instance()->Output(MACRO_LOCAL, "open file:", rec_path, "failed");
            return false;
        }

        out << "sim time"
              << std::setw(20) << "comm src id"
              << std::setw(20) << "comm dst id"
              << std::setw(20) << "comm err type"
              << std::setw(20) << "comm tr delay"
              << std::setw(320) << "comm ctrl data"
              << std::endl;

        for(int i=0; i<vec.size(); ++i){
            const UnionSimData& dat = vec[i];
            QByteArray tmp(dat.power_dat, sizeof(dat.power_dat));
            QByteArray d_base64 = tmp.toBase64();

            out << dat.sim_time
                  << std::setw(20) << dat.comm_dat.src_id
                  << std::setw(20) << dat.comm_dat.dst_id
                  << std::setw(20) << dat.comm_dat.err_type
                  << std::setw(20) << dat.comm_dat.trans_delay
                  << std::setw(320) << d_base64.toStdString()
                  << std::endl;
        }

        out.close();

        _file_list_vec.push_back(rec_path);

        return true;
}

bool HisRecordMgr::fill_businfor_record(const UnionSimDatVec& his, UnionSimDatVec& record)
{
    IntMap his_map;
    SimpleIntSet his_set, rec_set;

    //记录历史数据母线集
    for(int i=0; i<his.size(); ++i){
        const UnionSimData& dat = his[i];
        const PowerBusInforData* pbid = (const PowerBusInforData*)dat.power_dat ;
        his_map[pbid->bus_id] = i;
        his_set.insert(pbid->bus_id);
    }

    //记录当前数据母线集
    string cur_bus_id = "current record bus id: ";
    UnionSimDatVec tmp_vec(his.size());
    for(int i=0; i<record.size(); ++i){
        const UnionSimData& dat = record[i];
        const PowerBusInforData* pbid = (const PowerBusInforData*)dat.power_dat ;
        memcpy(&tmp_vec[pbid->bus_id - 1],  &dat, sizeof(UnionSimData));
        cur_bus_id += std::to_string(pbid->bus_id) + " ";
        rec_set.insert(pbid->bus_id);
    }

    //与历史母线集对比查询当前母线集缺少的母线编号
    IntVec diff_vec(his.size() > record.size() ? his.size() : record.size());
    IntVec::iterator diff_end = std::set_difference(his_set.begin(), his_set.end(), rec_set.begin(), rec_set.end(), diff_vec.begin());
    int diff_num = diff_end - diff_vec.begin();

    //借助历史数据补全当前数据集
    string need_bus_id = "insert history record bus id: ";
    for(int i=0; i<diff_num; ++i){
        need_bus_id += std::to_string(diff_vec[i]) + " ";
        int index = his_map[diff_vec[i]];
        memcpy(&tmp_vec[index], &his[index], sizeof(UnionSimData));
        PowerBusInforData* pbid = (PowerBusInforData*)tmp_vec[index].power_dat;
        pbid->bus_angle = 0.0; pbid->bus_volt = 0.0;
    }

    LogUtil::Instance()->Output(MACRO_LOCAL, cur_bus_id, need_bus_id);

    //设置当前数据
    record.clear();
    record.resize(tmp_vec.size());
    for(int i=0; i<tmp_vec.size(); ++i){
        memcpy(&record[i], &tmp_vec[i], sizeof(UnionSimData));
    }

    return true;
}

bool HisRecordMgr::fill_commctrl_record(const UnionSimDatVec& his, UnionSimDatVec& record)
{
    StrIntMap his_map;
    SimpleStrSet his_set, rec_set;

    //记录历史数据编号集
    for(int i=0; i<his.size(); ++i){
        char ids[32] = {0};
        const UnionSimData& dat = his[i];
        sprintf_s(ids, "%u:%u", dat.comm_dat.src_id, dat.comm_dat.dst_id);
        his_map[ids] = i;
        his_set.insert(ids);
    }

    //记录当前数据编号集
    string cur_comm_id = "current record comm id: ";
    UnionSimDatVec tmp_vec(his.size());
    for(int i=0; i<record.size(); ++i){
        char ids[32] = {0};
        const UnionSimData& dat = record[i];
        sprintf_s(ids, "%u:%u", dat.comm_dat.src_id, dat.comm_dat.dst_id);
        memcpy(&tmp_vec[i], &record[i], sizeof(UnionSimData));
        cur_comm_id += std::string(ids) + " ";
        rec_set.insert(ids);
    }

    //与历史母线集对比查询当前母线集缺少的母线编号
    StrVec diff_vec(his.size() > record.size() ? his.size() : record.size());
    StrVec::iterator diff_end = std::set_difference(his_set.begin(), his_set.end(), rec_set.begin(), rec_set.end(), diff_vec.begin());
    int diff_num = diff_end - diff_vec.begin();

    //借助历史数据补全当前数据集
    string need_comm_id = "insert history record bus id: ";
    for(int i=0, j=record.size(); i<diff_num; ++i, ++j){
        need_comm_id += diff_vec[i] + " ";
        int index = his_map[diff_vec[i]];
        memcpy(&tmp_vec[j], &his[index], sizeof(UnionSimData));
        tmp_vec[j].comm_dat.err_type = -1;
        tmp_vec[j].comm_dat.trans_delay = -1;
    }

    LogUtil::Instance()->Output(MACRO_LOCAL, cur_comm_id, need_comm_id);

    //设置当前数据
    record.clear();
    record.resize(tmp_vec.size());
    for(int i=0; i<tmp_vec.size(); ++i){
        memcpy(&record[i], &tmp_vec[i], sizeof(UnionSimData));
    }

    return true;
}
