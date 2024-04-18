#ifndef database_operation_hpp
#define database_operation_hpp
#include "queryItem.hpp"
class database_operation{
public:
    virtual void connctDatabase() = 0;
    virtual void display_tables_ziduan() = 0;
    virtual void queryDatabase(queryItem & queryItem) = 0;
    virtual void queryDatabaseForMV(queryItem & queryItem) = 0;
    virtual void free_connct() = 0;
};
#endif /* database_operation_hpp */
