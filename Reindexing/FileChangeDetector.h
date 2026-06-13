#pragma once
#include<filesystem>
#include<chrono>
#include<iostream>
#include<sqlite3.h>
#include<string.h>


using namespace std;


bool Need_To_Change_OR_Create_indexing(sqlite3* db,std::string &Path);