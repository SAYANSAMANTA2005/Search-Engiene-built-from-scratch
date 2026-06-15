#pragma once

#include <sqlite3.h>
#include<iostream>
#include <string>
#include<utility>
#include<vector>
#include <unordered_map>
/*
#include "SearchWord/searchword.h"
#include "Chunked_Bulk_Insertion/BulkInsert.h"
#include "Chunked_Bulk_Insertion/DeleteFilePostings.h"
#include "ModifiedTime/ModifiedTime.h"

*/


using namespace std;


sqlite3* OpenDB();

void CreateTables(sqlite3* db);

long long GetOrCreateFileId(
    sqlite3* db,
    const std::string& path,
    long long modified_time
);

void InsertWord(
    sqlite3* db,
    const std::string& word,
    long long file_id
);






void CloseDB(sqlite3* db);

//returns function of "SearchWord/searchword.cpp" file to run 

vector<pair<string,int>> search_single_word(sqlite3* db,string &word);


// returns functions of "Chunked_Bulk_Insertion/DeleteFilePostings.cpp" file to run

void DeleteFilePostings(
    sqlite3* db,
    long long file_id
);

// returns functions of "Chunked_Bulk_Insertion/BulkInsert.cpp" file to run
void BulkInsert(
    sqlite3* db,
    long long file_id,
    const std::unordered_map<std::string,int>& freq
);

// returns functions of "ModifiedTime/ModifiedTime.cpp" file to run


void modify_DB_indexing_time(
    sqlite3* db,
    const std::string& path,
    long long modified_time
);

long long get_db_last_modified(
    sqlite3* db,
    const std::string& path
);