#include "searchword.h"
#include <iostream>
#include<sqlite3.h>
#include<string.h>
using namespace std;


static int callback(
    void* data,
    int argc,
    char** argv,
    char** colNames)
{
    cout << argv[0] << '\n';
    return 0;
}

void search_word(sqlite3* db,string &word){

 std::string sql="SELECT file_path "
     " FROM files "
     " JOIN inverted_index ON files.file_id=inverted_index.file_id "
     " WHERE word= '"+word+"';";


     char* err=nullptr;
     int check=sqlite3_exec(db,sql.c_str(),callback,nullptr,&err);
     if(check != SQLITE_OK){
        cerr<<" Error: "<<err<<endl;
        cerr << "SQL = " << sql << '\n';
        sqlite3_free(err);
     }
}