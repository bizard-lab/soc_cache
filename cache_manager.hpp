#ifndef cache_manager_hpp
#define cache_manager_hpp
#include "all_header.h"
#include "queryItem.hpp"
#include "cacheItem.hpp"
class cache_manager
{
public:
    virtual equivalence_class_num_type get_the_number_of_equivalence_classes() const = 0;
    virtual data_cell_num_type get_the_number_of_data_units() const = 0;
    virtual bool query_in_cache(queryItem &queryItem) = 0;
    virtual void refresh_cache(const queryItem &queryItem) = 0;
    virtual void cache_replacement_policy() = 0;
    virtual void dispaly_experimental_result_to_file(data_cell_num_type queryItemSet_size, int hit, time_t all_time) const = 0;
    virtual vector<queryItem> load_query_file() = 0;
    virtual void displasy_cachaItem() const = 0;
    virtual void temp() = 0;
    virtual vector<queryItem> loadQueryFile() = 0;
};
#endif /* cache_manager_hpp */
