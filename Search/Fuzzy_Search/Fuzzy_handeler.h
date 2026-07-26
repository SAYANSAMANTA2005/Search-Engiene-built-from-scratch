#pragma once
#include<sqlite3.h>
#include<iostream>
#include<string>
#include<vector>
#include<utility>
using namespace std;

vector<pair<string,int>> fuzzy_search_handeler(sqlite3* db);
