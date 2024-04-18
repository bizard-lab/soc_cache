#include "sqliteOP.hpp"
#include "sqlite3.h"
vector<string> sqliteOP::table_ziduan;
static int get_table_ziduan(void *data, int argc, char **argv, char **azColName)
{
    sqliteOP::table_ziduan.push_back(argv[1]);
    return 0;
}
static int get_table_ziduan_new()
{
    sqliteOP::table_ziduan.push_back("dim1");
    sqliteOP::table_ziduan.push_back("dim2");
    sqliteOP::table_ziduan.push_back("dim3");
    sqliteOP::table_ziduan.push_back("dim4");
    sqliteOP::table_ziduan.push_back("dim5");
    sqliteOP::table_ziduan.push_back("dim6");
    sqliteOP::table_ziduan.push_back("dim7");
    sqliteOP::table_ziduan.push_back("dim8");
    sqliteOP::table_ziduan.push_back("dim9");
    sqliteOP::table_ziduan.push_back("dim10");
    sqliteOP::table_ziduan.push_back("msr");
    return 0;
}
static double measure = 0.0;
static vector<double> mv_measure;
static vector<queryItem> mv_queryItems;
static int get_measure(void *data, int argc, char **argv, char **azColName)
{
    measure = atof(argv[0]);
    return 0;
}
static int get_mv_measure(void *data, int argc, char **argv, char **azColName)
{
    dims_type temp_dims = {0};
    measure_type temp_measure = 0.0;
    vector<string> temp_strs;
    for (int i = 0; i < argc - 1; ++i)
    {
        string temp_str = azColName[i];
        temp_strs.push_back(temp_str.erase(0, 3));
    }
    int j = 0;
    for (int i = 0; i < dimsNum; ++i)
    {
        if (to_string(i + 1) == temp_strs[j])
        {
            string temp_str = argv[j];
            temp_dims[i] = atoi(temp_str.c_str());
            j++;
        }
        else
        {
            temp_dims[i] = 0;
        }
    }
    string temp_str = azColName[argc - 1];
    temp_measure = atof(argv[argc - 1]);
    queryItem temp_queryItem(temp_dims, temp_measure);
    mv_queryItems.push_back(temp_queryItem);
    return 0;
}

void sqliteOP::connctDatabase()
{
    int rc = sqlite3_open(DB_DBNAME.c_str(), &db);
    if (rc)
    {
        fprintf(stderr, "sqlite connect failure:%s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else
        cout << "sqlite connect successfully" << endl;
    string sql = "PRAGMA table_info('" + DB_TABLENAME + "')";
    rc = sqlite3_exec(db, sql.c_str(), get_table_ziduan, NULL, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error:%s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    get_table_ziduan_new();
    display_tables_ziduan();
    prefix_sql = "select ";
    for (int i = dimsNum; i < totalNum; ++i)
    {
        prefix_sql += "sum(" + sqliteOP::table_ziduan[i] + "), ";
    }
    prefix_sql.pop_back();
    prefix_sql.pop_back();
    prefix_sql += " from " + string(DB_TABLENAME);
    cout << prefix_sql << endl;
}

void sqliteOP::display_tables_ziduan()
{
    for (auto a : sqliteOP::table_ziduan)
    {
        cout << a << endl;
    }
}

vector<MV> sqliteOP::getMV()
{
    return mv;
}

void sqliteOP::queryDatabase(queryItem &queryItem)
{
    dims_type query = queryItem.getDims();
    string where;
    for (int i = 0; i < dimsNum; ++i)
    {
        if (query[i] != 0)
        {
            where += " " + sqliteOP::table_ziduan[i] + " = " + to_string(query[i]) + " and";
        }
    }
    where.erase(where.end() - 3, where.end());
    string queryStr = prefix_sql + " where " + where;
    int rc = sqlite3_exec(db, queryStr.c_str(), get_measure, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "fail to query:%s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    queryItem.setMeasure(measure);
}

vector<queryItem> sqliteOP::queryDatabase_to_build_mv(queryItem &queryItem)
{
    dims_type query = queryItem.getDims();
    string where;
    where += "select ";
    for (int i = 0; i < dimsNum; ++i)
    {
        if (query[i] != 0)
        {
            where += sqliteOP::table_ziduan[i] + ",";
        }
    }
    where += "sum(msr) from " + DB_TABLENAME + " group by ";
    for (int i = 0; i < dimsNum; ++i)
    {
        if (query[i] != 0)
        {
            where += sqliteOP::table_ziduan[i] + ",";
        }
    }
    where.erase(where.end() - 1, where.end());
    string queryStr = where + ";";
    cout << "sql:" << queryStr << endl;
    int rc = sqlite3_exec(db, queryStr.c_str(), get_mv_measure, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "fail to query:%s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    queryItem.setMeasure(measure);
    return mv_queryItems;
}

void sqliteOP::queryDatabaseForMV(queryItem &queryItem)
{
    dims_type query = queryItem.getDims();
    string where;
    int dim_number = 0;
    where += "select ";

    for (int i = 0; i < dimsNum; ++i)
    {
        if (query[i] != 0)
        {
            where += sqliteOP::table_ziduan[i] + ",";
            dim_number++;
        }
    }
    where += "sum(msr) from " + DB_TABLENAME + " group by ";
    vector<char> chr;
    for (int i = 0; i < dimsNum; ++i)
    {
        if (query[i] != 0)
        {
            where += sqliteOP::table_ziduan[i] + ",";
            chr.push_back((char)('0' + i));
        }
    }
    where.erase(where.end() - 1, where.end());
    string perCountStr = "select count(*) from (";
    string endCountStr = ") as t;";
    string queryStr = perCountStr + where + endCountStr;
    cout << "sql:" << queryStr << endl;
    int rc = sqlite3_exec(db, queryStr.c_str(), get_measure, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "fail to query:%s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    MV temp_mv;
    temp_mv.setCost(measure);
    temp_mv.setDims(dim_number);
    temp_mv.setCount(measure);
    temp_mv.setSqlstr(where);
    temp_mv.setChr(chr);
    mv.push_back(temp_mv);
}
void sqliteOP::free_connct()
{
    sqlite3_close(db);
}
