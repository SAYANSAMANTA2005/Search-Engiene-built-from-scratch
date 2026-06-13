#include <iostream>
#include<bits/stdc++.h>
#include <filesystem>
//#include "tokenize/tokeniser.h"
#include "config/config.h"
#include "ReadFile/Readfile.h"
#include "Database/Database.h"
#include "Database/SearchWord/searchword.h"
#include "Search/Single_Word/Search.h"
#include <chrono>
#include<sqlite3.h>

using namespace std;
using namespace filesystem;

auto start_time = std::chrono::steady_clock::now();



void Directory_Explorer(string root, sqlite3* db){
    
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
            if(file_id%10000==0)
           cout<<"File :"<<Path<<endl;
            // Reads valide type file 
            string extension=entry.path().extension().string();


               if(text_extensions.count(extension)){
              //updating maps & only stores valide & uuser's predifined types files
                file_id = GetOrCreateFileId(db,Path);
            //file_id is defined in cofig/config.h
            // path_to_file_id[Path] = file_id;
         // file_id_to_path[file_id] = Path;
             
            
            DeleteFilePostings(db,file_id);
                //Read_file2 --> sends to tokenize by buffer of a fixed size
              Read_File2(Path,db,file_id);

            if(file_id%100==0){
                 cout<<" PROCESSED Relevant files count : "<<file_id<<endl;

            auto now = std::chrono::steady_clock::now();

            auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>( now - start_time);

            cout<< "Processed "<< file_id<< " files | Time = " << elapsed.count()<< " ms"<< '\n';

            start_time = now;   // restart timer for next 100 files
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
      bool UserWant_To_Search=false;
    cout<<"Do U want to Search In The Database? (Y/N): ";
    string want;
    getline(cin,want);
    if(want.size() &&(want[0]=='y'||want[0]=='Y'))UserWant_To_Search=true;
    else break;
  // searching
    Search_in_Database(db,UserWant_To_Search);
    
   }

   



    //user input directory 
    while(true){
        cout<<" Do U Want To Traverse a Directory ? (Y/N): ";
        string want;getline(cin,want);
        if(want.size() && (want[0]=='y'||want[0]=='Y')){
    string root;
    cout<<"Enter The Directory U Want to Traverse: ";

    getline(cin,root);

    start_time = std::chrono::steady_clock::now();

    // wrapping the 
    //Directory_Explorer(root, db);   
    try { Directory_Explorer(root, db);}
    catch(const std::exception& e){cerr << "Exception: "<< e.what()<< '\n';}
    

   //closing db
    CloseDB(db);
        }
        else break;
    }

    cout<<endl;
    cout<<" TOTAL Relevant Files count: "<<file_id<<endl;
    return 0;
}