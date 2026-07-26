#include "searchword.h"
#include "../../config/config.h"
#include <iostream>
#include<sqlite3.h>
#include<string.h>
#include <chrono>
#include<vector>
#include <tuple>

using namespace std;

auto start_time_db2 = std::chrono::steady_clock::now();
int cnt2=0;

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
 


vector<tuple<int,int,string>>SearchWordResult_by_File_Id;



static int callback(
    void* data,
    int argc,
    char** argv,
    char** colNames)
{
   // if(cnt<MAX_NO_OF_FILES_TO_SHOW_IN_SEARCH)
   // cout << argv[0] << " (" << argv[1] << " matches)"<< '\n';
    if( cnt2<MAX_LIMIT_OF_STORING_MULTI_WORD_SEARCH_RESULT){
        string path = argv[2] ? argv[2] : "";
        SearchWordResult_by_File_Id.push_back(make_tuple(stoi(string(argv[0])), stoi(string(argv[1])),path));
        //it stores the sorted search result of a word
    }
    
    cnt2++;
    return 0;
}

vector<tuple<int,int,string>> search_single_word_give_result_by_file_id(sqlite3* db,string &word){

   SearchWordResult_by_File_Id.clear();
   cnt2=0;//setting 0 




start_time_db2 = std::chrono::steady_clock::now();
std::string sql =
    "SELECT inverted_index.file_id, "
    "inverted_index.frequency, "
    "files.file_path "
    "FROM inverted_index "
    "JOIN files "
    "ON inverted_index.file_id = files.file_id "
    "WHERE word = '" + word + "' "
    "ORDER BY inverted_index.file_id ASC;";


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
            std::chrono::duration_cast<std::chrono::milliseconds>( now - start_time_db2);
            cout<<endl;

            cout<< "   Word : ["<< word << "  ]    Found in "<< cnt2 <<" files\n";
            cnt2=0;//reseting cnt for next call

            cout<< "     Searched Word  : ["<< word << "  ]   Time Taken:" << elapsed.count()<< " ms"<< '\n';

            
            auto start_time_db2= now;



       return   SearchWordResult_by_File_Id;     

}