#include<sqlite3.h>
#include<iostream>
#include<filesystem>
#include<string.h>
#include "../config/config.h"
#include "../ReadFile/Readfile.h"
#include "../Database/Database.h"

using namespace std;
/*
ReindexFile --> 
- Deletes the old entries of that file in the inverted index
- Inserts the new entries of that file in the inverted index
- Modify the indexing time of the file in DB
*/
void ReindexFile(sqlite3* db,std::string &Path,int Processed_files_count){
        auto ftime = filesystem::last_write_time(Path);
        auto modified_time =
        chrono::duration_cast< chrono::seconds>(ftime.time_since_epoch()).count();


        file_id = GetOrCreateFileId(db,Path,llabs(modified_time));

        DeleteFilePostings(db,file_id);
     //Read_file2 --> sends to tokenize by buffer of a fixed size
        Read_File2(Path,db,file_id);

        //after completing indexing of file -->
        // u update the last_modified_time_of_a_file_in_DB
        modify_DB_indexing_time(db,Path,llabs(modified_time));
        // using llabs bcz on some system time is negative

}