#include<filesystem>
#include<chrono>
#include<iostream>
#include<sqlite3.h>
#include<string.h>
#include "../Database/ModifiedTime/ModifiedTime.h"

using namespace std;

/*
Need_To_Change_OR_Create_indexing -->
 checks if the file need to be indexed or not
 
 ** if file is not in db --> u need to create (first time) --> returns true
 ** if file is in db --> chcek files_modified_time & db_modified_time_of_That_file
                        return true if modified else return false


*/

bool Need_To_Change_OR_Create_indexing(sqlite3* db,std::string &Path){
      

          
          auto ftime =filesystem::last_write_time(Path);
          auto modified_time = chrono::duration_cast<chrono::seconds >(ftime.time_since_epoch()).count();            
          auto DB_Modified_time=get_db_last_modified(db,Path);
          //DB_Modified_time =-1 --> means first time we visitting this file



          if(((DB_Modified_time== -1)||(llabs(modified_time)<DB_Modified_time))){
        //   cout<<"Do modifiy(true)"<<endl;
             return true;// u need to index this file
          }
          
          //cout<<"dont Modify(false)"<<endl;
          return false;// u dont need to index this file( already index file 
          // with notihng chnaged since previous traversal of this file)


}