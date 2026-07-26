#pragma once
#include<string.h>
#include<sqlite3.h>
#include<iostream>
#include<vector>
#include<utility>
using namespace std;

vector<pair<string,int>> search_single_word(sqlite3* db,string &word);
vector<tuple<int,int,string>> search_single_word_give_result_by_file_id(sqlite3* db,string &word);

