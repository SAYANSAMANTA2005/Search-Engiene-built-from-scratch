#include <iostream>
#include<bits/stdc++.h>
#include <filesystem>
//#include "tokenize/tokeniser.h"
#include "config/config.h"
#include "ReadFile/Readfile.h"
#include "Database/Database.h"

#include "Search/Specify_Search_Handeler.h"
#include "Reindexing/FileChangeDetector.h"
#include "Reindexing/ReindexFile.h"
#include "Reindexing/Combined.h"
#include "tokenize/tokeniser.h"

#include<iostream>
#include <chrono>
#include<utility>
#include<vector>
#include<string>

#include<sqlite3.h>
/*

#include "Explorer/Directory_Explorer.cpp"
#include "Explorer/Disk_Explorer.cpp"


*/


using namespace std;
using namespace filesystem;
namespace fs = std::filesystem;

auto start_time = std::chrono::steady_clock::now();

int Processed_files_count = 0;







/*

Directory_Explorer(root, db)
│
├── Start recursive traversal from root
│
├── For every filesystem entry
│
├── Is it a directory?
│      │
│      ├── YES
│      │      │
│      │      ├── Get directory name
│      │      │
│      │      ├── Check SKIP_DIRECTORIES
│      │      │
│      │      ├── If present
│      │      │      │
│      │      │      ├── disable_recursion_pending()
│      │      │      ├── Print "Skipping..."
│      │      │      └── Ignore entire subtree
│      │      │
│      │      └── Else continue recursion
│      │
│      └── NO
│
├── Is it a regular file?
│      │
│      ├── NO
│      │      └── Ignore
│      │
│      └── YES
│
├── Get file extension
│
├── Check text_extensions
│      │
│      ├── Not supported
│      │      └── Ignore file
│      │
│      └── Supported
              │

              
              ├── GetOrCreateFileId()
              │
              ├── DeleteFilePostings()
              │
              ├── Read_File2()
              │
              ├── Read file chunk-by-chunk
              ├── Tokenize
              ├── Build frequency map
              └── BulkInsert into DB




*/


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
                cout << "Skipping: " << entry.path() << '\n';
                it.disable_recursion_pending();
                 
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
    
                
                  ReindexFile(db,Path,Processed_files_count);

                Processed_files_count++;

                if(Processed_files_count%INTERVAL_OF_PROCESSED_FILE_SHOW==0){
                cout<<" File Id: "<<file_id <<" /Path : " <<Path<<endl;
                cout<<" PROCESSED Relevant files count : "<<Processed_files_count<<endl;
                file_id=0;//reseting file_id for next chunk of 10000 files



            auto now = std::chrono::steady_clock::now();

            auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>( now - start_time);

            cout<< " Time elapsed for  "<<" processing [ "<<INTERVAL_OF_PROCESSED_FILE_SHOW<< " ]  files / "  << elapsed.count()<< " ms"<< '\n';

            start_time = now;   // restart timer for next 10000 files
            }


             }
            
          
            }
        }
   
    }

   
}






int main()
{

    
    //sqlite setup
    sqlite3* db=OpenDB();
    CreateTables(db);
   

//user input for searching or not
   while(true){

    cout<<"Do U want to Search In The Database? (Y/N): ";
    string want;
    getline(cin,want);
    cin.clear();


    if(want.size() &&(want[0]=='y'||want[0]=='Y'))
    {Specify_Search_Handeler(db);}

    else break;
  // searching --> we only search in the database if user wants to search

    
   }

   



    //user input directory 
    while(true){
        cout<<" Do U Want To Traverse a  Directory  ? (Y/N): ";
        string want;getline(cin,want);
        cin.clear();
        if(want.size() && (want[0]=='y'||want[0]=='Y')){
    string root;
    cout<<"Enter The Directory  U Want to Traverse: ";

    getline(cin,root);
    cin.clear();

    start_time = std::chrono::steady_clock::now();

    // wrapping the 
    //Directory_Explorer(root, db);   
    try { Directory_Explorer(root, db);}
    catch(const std::exception& e){cerr << "Exception: "<< e.what()<< '\n';}
    
    cout<<endl;
    cout<<" TOTAL Relevant Files count: "<<Processed_files_count<<endl;
    Processed_files_count=0;//reseting Processed_files_count after finishing one traversal


  
        }
        else break;
    }

     //closing db
    CloseDB(db);


    return 0;
}


/*

Directory_Explorer
        │
        ▼
Get current modified_time
        │
        ▼
Get DB modified_time
        │
        ▼
      Same?
        │
    ┌───┴────┐
    │        │
   YES      NO
    │        │
    ▼        ▼

 Skip    Re-index File



*/
