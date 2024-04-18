#ifndef MVMB_hpp
#define MVMB_hpp
#include "all_header.h"
#include "queryItem.hpp"
#include "cacheItem.hpp"
#include "cache_manager.hpp"
#include "MV.hpp"
#include <stdio.h>
class mvmb : public cache_manager
{
private:
    list<queryItem> MVMB_Container;
    map<string, int> MVMB_map;
    vector<MV> mv;
    map<string, int> mv_map;

public:
    void setMV(vector<MV> p_mv);
    vector<MV> getMV();
    vector<int> sort_MV(vector<MV> p_mv);
    vector<int> load_MV();
    vector<int> sort_MV2(vector<MV> p_mv);
    void match_best_MV();
    void write_mv_to_file(vector<int> mv_flag);
    void powSet(int i, vector<char> chr, vector<string> &tempstr_vector);
    virtual equivalence_class_num_type get_the_number_of_equivalence_classes() const;
    virtual data_cell_num_type get_the_number_of_data_units() const;
    virtual bool query_in_cache(queryItem &queryItem);
    virtual void refresh_cache(const queryItem &queryItem);
    virtual void cache_replacement_policy();
    virtual void dispaly_experimental_result_to_file(data_cell_num_type queryItemSet_size, int hit, time_t all_time) const;
    virtual vector<queryItem> load_query_file();
    virtual void displasy_cachaItem() const;
    virtual void temp()
    {
        cout << "ssss:" << MVMB_Container.size() << endl;
    }
    virtual vector<queryItem> loadQueryFile();
};
#endif
