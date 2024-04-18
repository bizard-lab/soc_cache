#ifndef cacheItem_hpp
#define cacheItem_hpp
#include "all_header.h"
#include "queryItem.hpp"
using namespace std;
class cacheItem
{
private:
    bound_type upper;
    bound_type lowers;
    measure_type measure;
    data_cell_num_type sz;
    ca_type ca;
    layer_type low_min_layer;
    friend class SOC;

public:
    cacheItem() = delete;
    cacheItem(queryItem self_queryItem);
    inline data_cell_num_type getSz() const;
    inline ca_type getCa() const;
    inline layer_type getLow_min_layer() const;
    void update_state();
    void update_eqClass_sz();
    void update_ca();
    inline bound_type getUp() const;
    inline bound_type getLow() const;
    inline measure_type getMeasure() const;
    inline bool sameMeasure(const queryItem &cur_queryItem) const;
    bool addToCacheItem(const dims_type &queryItem_dims, int &state);
    void merge_up(const cacheItem &merged_cacheItem);
    void merge_low(const cacheItem &merged_cacheItem);
    void display() const;
    static bool cover(const dims_type &left_queryItem_dims, const dims_type &right_queryItem_dims);
    static const dims_type conjunction(const dims_type &left, const dims_type &right);
    static ca_type std_ca(const pair<dims_type, dims_type> &region);
};

inline data_cell_num_type cacheItem::getSz() const
{
    return upper.size() + lowers.size();
}

inline ca_type cacheItem::getCa() const
{
    return ca;
}

inline layer_type cacheItem::getLow_min_layer() const
{
    return low_min_layer;
}

inline bound_type cacheItem::getUp() const
{
    return upper;
}

inline bound_type cacheItem::getLow() const
{
    return lowers;
}

inline measure_type cacheItem::getMeasure() const
{
    return measure;
}

inline bool cacheItem::sameMeasure(const queryItem &cur_queryItem) const
{
    return this->measure == cur_queryItem.getMeasure();
}

#endif /* equivalentClass_hpp */
