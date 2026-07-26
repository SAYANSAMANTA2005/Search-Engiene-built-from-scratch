#include "Explorer.h"


#include "../config/config.h"
#include "../ReadFile/Readfile.h"
#include "../Database/Database.h"

#include "../Search/Single_Word/Search.h"
#include "../Reindexing/FileChangeDetector.h"
#include "../Reindexing/ReindexFile.h"
#include "Directory_Explorer/Directory_Explorer.h"
#include <chrono>
#include<sqlite3.h>
#include <iostream>
#include <string>
#include <filesystem>



using namespace std;
using namespace filesystem;


extern long long Processed_files_count;
extern chrono::system_clock::time_point start_time;







void Disk_Explorer(string root, sqlite3* db){
    
   // fs::path diskPath = "F:\\"; 
    // Normalize root path trailing slashes for Windows safety
    fs::path diskPath(root);

    cout<<"HGhfhfhfh"<<endl;
      
       try {
        if (fs::exists(diskPath) && fs::is_directory(diskPath)) {
            std::cout << "Iterating top-level of: " << diskPath << "\n\n";
            
            for (const auto& entry : fs::directory_iterator(diskPath)) {
                // entry.path() gives the full path
                string Path=entry.path().string();
                // it recursively goes in all the subdirectories of the " {diskPath} :" drive
                Directory_Explorer(Path,db);
                
            }
        } else {
            std::cerr << "Path does not exist or is not a directory.\n";
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    
}

/*
    // Replace with your target disk/path
    fs::path diskPath = "F:\\"; 
    cout<<"HGhfhfhfh"<<endl;
      
       try {
        if (fs::exists(diskPath) && fs::is_directory(diskPath)) {
            std::cout << "Iterating top-level of: " << diskPath << "\n\n";
            
            for (const auto& entry : fs::directory_iterator(diskPath)) {
                // entry.path() gives the full path
                std::cout << entry.path().string() << "\n";
            }
        } else {
            std::cerr << "Path does not exist or is not a directory.\n";
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }



*/