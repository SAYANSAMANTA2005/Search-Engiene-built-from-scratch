#pragma once
#include<string.h>
#include<sqlite3.h>
#include<iostream>
#include<vector>
#include<utility>
using namespace std;

vector<pair<string,int>> search_single_word(sqlite3* db,string &word);
