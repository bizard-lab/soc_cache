#ifndef SOC_hpp
#define SOC_hpp
#include "all_header.h"
#include "queryItem.hpp"
#include "cacheItem.hpp"
#include "cache_manager.hpp"
#include <stdio.h>
class SOC : public cache_manager
{
private:
    struct by_layer
    {
    };
    struct by_measure
    {
    };
    typedef boost::multi_index_container<
        cacheItem,
        indexed_by<
            ordered_non_unique<tag<by_layer>, BOOST_MULTI_INDEX_MEMBER(cacheItem, layer_type, low_min_layer)>,
            hashed_non_unique<tag<by_measure>, BOOST_MULTI_INDEX_MEMBER(cacheItem, measure_type, measure)>>>
        cacheItem_Index;
    cacheItem_Index SOC_Container;

public:
    virtual vector<queryItem> loadQueryFile();
    virtual equivalence_class_num_type get_the_number_of_equivalence_classes() const;
    virtual data_cell_num_type get_the_number_of_data_units() const;
    virtual bool query_in_cache(queryItem &queryItem);
    virtual void refresh_cache(const queryItem &queryItem);
    virtual void cache_replacement_policy();
    virtual void dispaly_experimental_result_to_file(data_cell_num_type queryItemSet_size, int hit, time_t all_time) const; // 打印结果到文件中，有4种
    virtual vector<queryItem> load_query_file();
    virtual void displasy_cachaItem() const;
    virtual void get_ca_frequency() const;
    virtual void temp()
    {
        int ret1 = 0;
        for (auto a : SOC_Container)
        {
            ret1 += a.getUp().size() + a.getLow().size();
        }
        cout << "the number of data cells:" << ret1 << endl;
    }
};
#endif /* SOC_hpp */
