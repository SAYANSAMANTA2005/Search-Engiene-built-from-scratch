#include<sqlite3.h>
#include<iostream>
#include<string>

using namespace std;

void modify_DB_indexing_time(
    sqlite3* db,
    const std::string& path,
    long long modified_time
);

long long get_db_last_modified(
    sqlite3* db,
    const std::string& path
);