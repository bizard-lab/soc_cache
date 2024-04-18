

#ifndef LRU_hpp
#define LRU_hpp

#include "all_header.h"
#include "queryItem.hpp"
#include "cacheItem.hpp"
#include "cache_manager.hpp"

#include <stdio.h>
class LRU:public cache_manager{
private:
    
    list<queryItem> LRU_Container;
    
public:

    
    virtual equivalence_class_num_type get_the_number_of_equivalence_classes()const;                          

    virtual vector<queryItem> loadQueryFile();
    virtual data_cell_num_type get_the_number_of_data_units()const;                                
    
    virtual bool query_in_cache(queryItem& queryItem);           
    
    virtual void refresh_cache(const queryItem& queryItem);              
    
    virtual void cache_replacement_policy();                                   
    
    virtual void dispaly_experimental_result_to_file(data_cell_num_type queryItemSet_size,int hit,time_t all_time)const;            
    
    
    virtual vector<queryItem>  load_query_file();                          
    virtual void displasy_cachaItem()const;                                 
    
  
    
    virtual void temp() {
        cout<<"ssss : "<<LRU_Container.size()<<endl;
    }
    
};


#endif /* LRU_hpp */
