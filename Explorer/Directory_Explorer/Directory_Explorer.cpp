#include "../Explorer.h"


#include "../../config/config.h"
#include "../../ReadFile/Readfile.h"
#include "../../Database/Database.h"

#include "../../Search/Single_Word/Search.h"
#include "../../Reindexing/FileChangeDetector.h"
#include "../../Reindexing/ReindexFile.h"
#include <chrono>
#include<sqlite3.h>
#include <iostream>
#include <string>
#include <filesystem>

using namespace std;
using namespace filesystem;


extern long long Processed_files_count;
extern chrono::system_clock::time_point start_time;



void Directory_Explorer(string root, sqlite3* db){
    // Directory_Explorer() --> expects root to be valide directory path,
    // if not it will throw exception
    // so handel it in main() or Disk_Explorer()

    for(auto it=recursive_directory_iterator(root);
    it!=recursive_directory_iterator();it++){


       const auto & entry=*it;
        string Path= entry.path().string();

        if(entry.is_directory()){
        // goes to subdirectory
        string dirname = entry.path().filename().string();
       //cout<<" Directory:"<<Path<<endl;
            if(SKIP_DIRECTORIES.count(dirname)){
                 it.disable_recursion_pending();
                 cout << "Skipping: " << entry.path() << '\n';
                 continue;
             }
 
        }
  

        if(entry.is_regular_file()){ 
       
            // Reads valide type file 
            string extension=entry.path().extension().string();


               if(text_extensions.count(extension)){
              //updating maps & only stores valide & uuser's predifined types files
           
              //possible issue is when a Path is not present in DB may create issues

             if(Need_To_Change_OR_Create_indexing(db,Path)){
    
                
                  ReindexFile(db,Path);

                Processed_files_count++;

                if(Processed_files_count%INTERVAL_OF_PROCESSED_FILE_SHOW==0){
                cout<<"Current File ID : "<<file_id<<" /  File: "<<Path <<endl;
                cout<<" PROCESSED Relevant files count : "<<Processed_files_count<<endl;
                file_id=0;//reseting file_id for next chunk of 10000 files



            auto now = std::chrono::steady_clock::now();

            auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>( now - start_time);

            cout<< "Processed "<< file_id<< " files | Time = " << elapsed.count()<< " ms"<< '\n';

            start_time = now;   // restart timer for next 10000 files
            }


             }
            
          
            }
        }
   
    }

   
}