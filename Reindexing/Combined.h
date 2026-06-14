#pragma once
#include<sqlite3.h>
#include<iostream>
#include<string.h>
#include<filesystem>
#include<chrono>


#include "../Database.h"


using namespace std;

void ReindexFile(sqlite3* db,std::string &Path);
bool Need_To_Change_OR_Create_indexing(sqlite3* db,std::string &Path);



