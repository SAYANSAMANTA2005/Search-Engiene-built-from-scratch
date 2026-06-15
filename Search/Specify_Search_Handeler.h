#pragma once
#include<sqlite3.h>
#include<iostream>
#include<string>
#include<vector>
#include<utility>
#include "Single_Word/Search.h"
#include "Multi_Word/Multi_Search.h"

using namespace std;

//Specify_Search_Handeler.cpp 's function
void Specify_Search_Handeler(sqlite3* db);//
//Single_Word/Search.h 's function
vector<pair<string,int>> Search_Single_Word_in_Database(sqlite3* db);

//Multi_Word/Multi_Search.h 's function
vector<pair<string,int>> Multi_Search(sqlite3* db);