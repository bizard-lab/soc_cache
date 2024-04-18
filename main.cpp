#include "all_header.h"
#include "cacheItem.hpp"
#include "queryItem.hpp"
#include "cache_manager.hpp"
#include "SOC.hpp"
#include "LRU.hpp"
#include "LFU.hpp"
#include "MVMB.hpp"
#include "database_operation.hpp"
#include "sqliteOP.hpp"
using namespace std;
void soc()
{
    // 创建sqlite数据库对象
    database_operation *database_op = new sqliteOP();
    // 连接数据库
    database_op->connctDatabase();
    //  创建缓存管理器SOC
    cache_manager *manager = new SOC();
    // 加载查询文件
    vector<queryItem> queryItemSet = manager->load_query_file();
    cout << "start query" << endl;
    // 缓存命中条数
    int hit = 0;
    time_t start, end;
    start = time(NULL);
    int i = 1;
    for (auto &querydata : queryItemSet)
    {
        cout << "th" << i++ << "tiao" << endl;
        querydata.display();
        // 查询的数据是否在缓存中命中
        bool flag = manager->query_in_cache(querydata);
        if (flag)
        {
            cout << "the query has been found in cache=====================================" << endl;
            hit++;
            querydata.display();
        }
        else
        {
            // 在数据库中进行查询
            database_op->queryDatabase(querydata);
            // 刷新缓存
            manager->refresh_cache(querydata);
        }
    }
    end = time(NULL);
    manager->displasy_cachaItem();
    cout << "SOC" << endl;
    cout << "time(s):" << end - start << endl;
    cout << "hit ratio:" << (hit / (float)queryItemSet.size()) << endl;
    // 将实验结果保存到文件中
    manager->dispaly_experimental_result_to_file(queryItemSet.size(), hit, end - start);
    // 断开数据库连接
    database_op->free_connct();
}

void lru()
{

    database_operation *database_op = new sqliteOP();
    database_op->connctDatabase();
    cache_manager *manager = new LRU();
    vector<queryItem> queryItemSet = manager->load_query_file();
    cout << "start query" << endl;
    int hit = 0;
    time_t start, end;
    start = time(NULL);
    int i = 1;
    for (auto &querydata : queryItemSet)
    {
        cout << "di" << i++ << "tiao" << endl;
        querydata.display();
        bool flag = manager->query_in_cache(querydata);
        if (flag)
        {
            cout << "the query has been found in cache=====================================" << endl;
            hit++;
            querydata.display();
        }
        else
        {
            database_op->queryDatabase(querydata);
            manager->refresh_cache(querydata);
        }
    }
    end = time(NULL);
    manager->displasy_cachaItem();
    cout << "LRU" << endl;
    cout << "time(s):" << end - start << endl;
    cout << "hit ratio:" << (hit / (float)queryItemSet.size()) << endl;
    manager->dispaly_experimental_result_to_file(queryItemSet.size(), hit, end - start);
    database_op->free_connct();
}

void lfu()
{
    database_operation *database_op = new sqliteOP();
    database_op->connctDatabase();
    cache_manager *manager = new LFU();
    vector<queryItem> queryItemSet = manager->load_query_file();
    cout << "start query" << endl;
    int hit = 0;
    time_t start, end;
    start = time(NULL);
    int i = 1;
    for (auto &querydata : queryItemSet)
    {
        cout << "di" << i++ << "tiao" << endl;
        querydata.display();
        bool flag = manager->query_in_cache(querydata);
        if (flag)
        {
            cout << "the query has been found in cache=====================================" << endl;
            hit++;
            querydata.display();
        }
        else
        {
            database_op->queryDatabase(querydata);

            manager->refresh_cache(querydata);
        }
    }
    end = time(NULL);
    manager->displasy_cachaItem();
    cout << "LFU" << endl;
    cout << "time(s):" << end - start << endl;
    cout << "hit ratio:" << (hit / (float)queryItemSet.size()) << endl;
    manager->dispaly_experimental_result_to_file(queryItemSet.size(), hit, end - start);
    database_op->free_connct();
}

void MVMB()
{
    sqliteOP *database_op = new sqliteOP();
    database_op->connctDatabase();
    time_t start, end;
    mvmb *manager = new mvmb();
    int i = 1;
    int hit = 0;
    vector<queryItem> queryItemSet_mv = manager->loadQueryFile();
    vector<int> mv_flag;
    vector<int> mv_flags;
    cout << "calculating the benefit of MV" << endl;
    // vector<queryItem>  queryItemSet = manager->load_query_file();
    for (auto &querydata : queryItemSet_mv)
    {
        // cout<<"di"<<i++<<"tiao"<<endl;
        // querydata.display();
        database_op->queryDatabaseForMV(querydata);
        /*bool flag = manager->query_in_cache(querydata);
        if(flag){
            cout<<"the query has been found in cache====================================="<<endl;
            hit++;
            querydata.display();
        }else{
            //database_op->queryDatabase(querydata);
            manager->refresh_cache(querydata);
        }*/
    }
    vector<MV> mv = database_op->getMV();
    // mv_flag = manager->sort_MV(mv);
    cout << "selecting mv" << endl;
    mv_flag = manager->sort_MV2(mv);
    mv_flags = mv_flag;
    manager->write_mv_to_file(mv_flag);
    vector<queryItem> mv_querys;
    cout << "puting the mvs into cache" << endl;
    for (int i = 0; i < mv_flag.size(); ++i)
    {
        vector<queryItem> temp_mv_querys;

        temp_mv_querys = database_op->queryDatabase_to_build_mv(queryItemSet_mv[mv_flag[i]]);
        temp_mv_querys.pop_back();
        mv_querys.insert(mv_querys.end(), temp_mv_querys.begin(), temp_mv_querys.end());
    }
    for (auto &querydata : mv_querys)
    {
        manager->refresh_cache(querydata);
    }

    start = time(NULL);
    vector<queryItem> queryItemSet = manager->load_query_file();

    for (auto &querydata : queryItemSet)
    {
        cout << "di" << i++ << "tiao" << endl;
        querydata.display();
        // database_op->load_query_file(querydata);
        bool flag = manager->query_in_cache(querydata);

        if (flag)
        {
            cout << "the query has been found in cache=====================================" << endl;
            hit++;
            querydata.display();
        }
        else
        {

            database_op->queryDatabase(querydata);
        }
    }

    end = time(NULL);
    cout << "mv size " << mv_querys.size() << endl;
    cout << "cache siez" << manager->get_the_number_of_data_units() << endl;

    // manager->displasy_cachaItem();
    manager->dispaly_experimental_result_to_file(queryItemSet.size(), hit, end - start);
    cout << "MVMB" << endl;
    cout << "time(s)" << end - start << endl;
    cout << "hit ratio:" << (hit / (float)queryItemSet.size()) << endl;
    database_op->free_connct();
    // cout << "mvmb" << endl;
}

void MVMB_static()
{
    sqliteOP *database_op = new sqliteOP();
    database_op->connctDatabase();
    time_t start, end;
    mvmb *manager = new mvmb();
    int i = 1;
    int hit = 0;
    vector<queryItem> queryItemSet_mv = manager->loadQueryFile();
    vector<int> mv_flag;
    cout << "selecting mv" << endl;
    mv_flag = manager->load_MV();
    vector<queryItem> mv_querys;
    cout << "puting the mvs into cache" << endl;
    cout << mv_flag.size() << endl;
    for (int i = 0; i < mv_flag.size(); ++i)
    {
        vector<queryItem> temp_mv_querys;

        temp_mv_querys = database_op->queryDatabase_to_build_mv(queryItemSet_mv[mv_flag[i]]);

        mv_querys.insert(mv_querys.end(), temp_mv_querys.begin(), temp_mv_querys.end());
    }
    for (auto &querydata : mv_querys)
    {
        manager->refresh_cache(querydata);
    }
    cout << "mv size " << mv_querys.size() << endl;
    cout << "cache siez " << manager->get_the_number_of_data_units() << endl;
    start = time(NULL);
    vector<queryItem> queryItemSet = manager->load_query_file();
    for (auto &querydata : queryItemSet)
    {
        cout << "di" << i++ << "tiao" << endl;
        querydata.display();
        bool flag = manager->query_in_cache(querydata);
        if (flag)
        {
            cout << "the query has been found in cache=====================================" << endl;
            hit++;
            querydata.display();
        }
        else
        {
            database_op->queryDatabase(querydata);
        }
    }
    end = time(NULL);
    cout << "mv size " << mv_querys.size() << endl;
    cout << "cache siez" << manager->get_the_number_of_data_units() << endl;
    manager->dispaly_experimental_result_to_file(queryItemSet.size(), hit, end - start);
    cout << "MVMB" << endl;
    cout << "time(s)" << end - start << endl;
    cout << "hit ratio:" << (hit / (float)queryItemSet.size()) << endl;
    database_op->free_connct();
}

int main(void)
{
    // 执行soc算法
    soc();
    return 0;
}
