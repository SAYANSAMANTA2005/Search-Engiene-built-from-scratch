#pragma once
#include<sqlite3.h>
#include<iostream>
#include<string>
#include<vector>
#include<utility>
#include<vector>
using namespace std;


vector<pair<string,int>> Search_Single_Word_in_Database(sqlite3* db);
    