#include <iostream>
#include<bits/stdc++.h>
#include <filesystem>
//#include "tokenize/tokeniser.h"
#include "config/config.h"
#include "ReadFile/Readfile.h"
#include "Database/Database.h"
#include "Database/SearchWord/searchword.h"
#include "Search/Single_Word/Search.h"
#include<sqlite3.h>

using namespace std;
using namespace filesystem;






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
             

                //Read_file2 --> sends to tokenize by buffer of a fixed size
              Read_File2(Path,db,file_id);

            if(file_id%1000==0)
            cout<<" PROCESSED Relevant files count : "<<file_id<<endl;
            }
        }
        
       
        
   
    }
}


int main()
{
    //sqlite setup
    sqlite3* db=OpenDB();
    CreateTables(db);
   

  // searching
    Search_in_Database(db);



    //user input directory 
    string root;
    cout<<"Enter The Directory U Want to Traverse: ";
    getline(cin,root);
    Directory_Explorer(root, db);   
    

   //closing db
    CloseDB(db);


    cout<<endl;
    cout<<" TOTAL Relevant Files count: "<<file_id<<endl;
    return 0;
}