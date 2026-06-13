#pragma once

#include<string.h>
#include<sqlite3.h>
#include<iostream>
using namespace std;


void search_word(sqlite3* db,std::string &word);