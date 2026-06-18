#include "searchword.h"
#include "../../config/config.h"
#include <iostream>
#include<sqlite3.h>
#include<string.h>
#include <chrono>
#include<vector>

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
int cnt=0;


vector<pair<string,int>>SearchWordResult;



static int callback(
    void* data,
    int argc,
    char** argv,
    char** colNames)
{
   // if(cnt<MAX_NO_OF_FILES_TO_SHOW_IN_SEARCH)
   // cout << argv[0] << " (" << argv[1] << " matches)"<< '\n';
    if( cnt<MAX_LIMIT_OF_STORING_SEARCH_RESULT){
        SearchWordResult.push_back(make_pair(string(argv[0]), stoi(string(argv[1]))));//it stores the sorted search result of a word
    }
    
    cnt++;
    return 0;
}

vector<pair<string,int>> search_single_word(sqlite3* db,string &word){

   SearchWordResult.clear();




start_time_db = std::chrono::steady_clock::now();
 std::string sql=
        "SELECT file_path, frequency "
        "FROM files "
        "JOIN inverted_index "
        "ON files.file_id = inverted_index.file_id "
        "WHERE word = '" + word + "' "
        "ORDER BY frequency DESC, files.file_id ASC;";


     char* err=nullptr;
     int check=sqlite3_exec(db,sql.c_str(),callback,nullptr,&err);
     if(check != SQLITE_OK){
        cerr<<" Error: "<<err<<endl;
        cerr << "SQL = " << sql << '\n';
        sqlite3_free(err);
     }
    
    //clearing       

 
           
              //printing res
            auto now = std::chrono::steady_clock::now();

            auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>( now - start_time_db);
            cout<<endl;

            cout<< "   Word : ["<< word << "  ]    Found in "<< cnt <<" files\n";
            cnt=0;//reseting cnt for next call

            cout<< "     Searched Word  : ["<< word << "  ]   Time Taken:" << elapsed.count()<< " ms"<< '\n';

            
            start_time_db = now;



       return   SearchWordResult;     

}
