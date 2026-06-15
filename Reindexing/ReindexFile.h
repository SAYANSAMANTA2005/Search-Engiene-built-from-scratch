#pragma once
#include<sqlite3.h>
#include<iostream>
#include<string.h>

using namespace std;

void ReindexFile(sqlite3* db,std::string &Path,int Processed_files_count);
