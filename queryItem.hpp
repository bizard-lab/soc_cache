#ifndef querySet_hpp
#define querySet_hpp
#include "all_header.h"
class queryItem
{
private:
    dims_type dims;
    measure_type measure;
    layer_type layer;
    int LFU_count;

public:
    queryItem() = delete;
    queryItem(dims_type _dims, measure_type _measure);
    inline void setMeasure(measure_type _measure);
    inline measure_type getMeasure() const;
    inline dims_type getDims() const;
    inline layer_type getLayer() const;
    string get_Dims();
    void display() const;
    void add_LFU_count();
    int get_LFU_count() const;
};

inline void queryItem::setMeasure(measure_type _measure)
{
    measure = _measure;
}

inline measure_type queryItem::getMeasure() const
{
    return measure;
}

inline dims_type queryItem::getDims() const
{
    return dims;
}

inline layer_type queryItem::getLayer() const
{
    return layer;
}

#endif /* querySet_hpp */
