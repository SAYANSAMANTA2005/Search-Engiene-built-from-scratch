#include "Explorer.h"


#include "../config/config.h"
#include "../ReadFile/Readfile.h"
#include "../Database/Database.h"

#include "../Search/Single_Word/Search.h"
#include "../Reindexing/FileChangeDetector.h"
#include "../Reindexing/ReindexFile.h"
#include <chrono>
#include<sqlite3.h>
#include <iostream>
#include <string>
#include <filesystem>

using namespace std;
using namespace filesystem;


extern long long Processed_files_count;
extern chrono::system_clock::time_point start_time;


void Disk_Explorer(string root, sqlite3* db);
void Directory_Explorer(string root, sqlite3* db);
