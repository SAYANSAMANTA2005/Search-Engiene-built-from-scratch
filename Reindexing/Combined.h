#pragma once
#include<sqlite3.h>
#include<iostream>
#include<string.h>
#include<filesystem>
#include<chrono>


#include "../Database/Database.h"


using namespace std;

void ReindexFile(sqlite3* db,std::string &Path,int Processed_files_count);
bool Need_To_Change_OR_Create_indexing(sqlite3* db,std::string &Path);



