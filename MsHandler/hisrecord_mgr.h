#ifndef HISRECORD_MGR_H
#define HISRECORD_MGR_H

#include "comm_type_def.h"
#include "power_data_def.h"

class HisRecordMgr
{
private:
    HisRecordMgr();
    virtual ~HisRecordMgr();

public:
    static HisRecordMgr* instance();

public:
    bool load_record(int power_data_type, UnionSimDatVec& vec, int64_t sim_time = -1);
    bool write_record(int64_t sim_time, int power_data_type, const UnionSimDatVec& vec);
    bool fill_record(int power_data_type, const UnionSimDatVec& his, UnionSimDatVec& record);

private:
    bool load_businfor_record(UnionSimDatVec& vec, int64_t sim_time);
    bool write_businfor_record(int64_t sim_time, const UnionSimDatVec& vec);
    bool fill_businfor_record(const UnionSimDatVec& his, UnionSimDatVec& record);

private:
    bool load_file_list(StrVec& vec);

private:
    const string _rec_name_prefix;
    const string _rec_name_postfix;

private:
    StrVec _file_list_vec;
};

#endif // HISRECORD_MGR_H
