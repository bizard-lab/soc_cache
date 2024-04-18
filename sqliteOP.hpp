#ifndef sqliteOP_hpp
#define sqliteOP_hpp
#include "all_header.h"
#include "database_operation.hpp"
#include "MV.hpp"
#include "sqlite3.h"
class sqliteOP : public database_operation
{
private:
    sqlite3 *db;
    char *zErrMsg = 0;
    string prefix_sql;
    vector<MV> mv;

public:
    static vector<string> table_ziduan;
    vector<MV> getMV();
    vector<queryItem> queryDatabase_to_build_mv(queryItem &queryItem);
    virtual void connctDatabase();
    virtual void display_tables_ziduan();
    virtual void queryDatabase(queryItem &queryItem);
    virtual void queryDatabaseForMV(queryItem &queryItem);
    virtual void free_connct();
};
#endif /* sqliteOP_hpp */
