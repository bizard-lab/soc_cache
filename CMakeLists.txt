cmake_minimum_required(VERSION 3.8)
project(soc)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/include)

FIND_LIBRARY(RUNTIME_LIB rt /usr/lib  /usr/local /usr/local/lib NO_DEFAULT_PATH)

link_libraries(sqlite3)

add_executable(soc
        sqlite3.h
        all_header.h
        cache_manager.cpp
        cache_manager.hpp
        cacheItem.cpp
        cacheItem.hpp
        database_operation.cpp
        database_operation.hpp
        LFU.cpp
        LFU.hpp
        LRU.cpp
        LRU.hpp
        MVMB.cpp
        MVMB.hpp
        MV.cpp
        MV.hpp
        main.cpp
        queryItem.cpp
        queryItem.hpp
        SOC.cpp
        SOC.hpp
        sqliteOP.cpp
        sqliteOP.hpp
        support_fun.cpp
        support_fun.hpp)
