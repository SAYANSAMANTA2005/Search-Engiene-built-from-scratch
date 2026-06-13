#include "searchword.h"
#include <iostream>
#include<sqlite3.h>
#include<string.h>
#include <chrono>
using namespace std;

auto start_time_db = std::chrono::steady_clock::now();

/*
Single-word search complexity:

O(log N + M log F + M log M)

where:
N = total postings
F = total indexed files
M = files containing the queried word

In practice, sorting the matching documents
(O(M log M)) dominates the query time.
*/
/*

User Input
    │
    ▼
Crawler.cpp
    │
    ▼
search_word(db, word)
    │
    ▼
Database/SearchWord/searchword.h
    │
    ▼
Database/SearchWord/searchword.cpp
    │
    ▼
sqlite3_exec()
    │
    ▼
search_engine.db
    │
    ▼
files
(file_id, file_path)

        +
        │
        ▼

inverted_index
(word, file_id, frequency)
    │
    ▼
SQL JOIN
    │
    ▼
Ranked Result Set
(ORDER BY frequency DESC)
    │
    ▼
callback()
(searchword.cpp)
    │
    ▼
Console Output


*/
static int callback(
    void* data,
    int argc,
    char** argv,
    char** colNames)
{
    cout << argv[0] << " (" << argv[1] << " matches)"<< '\n';
    return 0;
}

void search_word(sqlite3* db,string &word){

start_time_db = std::chrono::steady_clock::now();
 std::string sql=
        "SELECT file_path, frequency "
        "FROM files "
        "JOIN inverted_index "
        "ON files.file_id = inverted_index.file_id "
        "WHERE word = '" + word + "' "
        "ORDER BY frequency DESC;";


     char* err=nullptr;
     int check=sqlite3_exec(db,sql.c_str(),callback,nullptr,&err);
     if(check != SQLITE_OK){
        cerr<<" Error: "<<err<<endl;
        cerr << "SQL = " << sql << '\n';
        sqlite3_free(err);
     }

            auto now = std::chrono::steady_clock::now();

            auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>( now - start_time_db);

            cout<< "Searched Word in  "<< 
            " Time Taken:" << elapsed.count()<< " ms"<< '\n';

            start_time_db = now;
            

}