#pragma once

#include <sqlite3.h>
#include <string>
#include <unordered_map>
#include "SearchWord/searchword.h"
#include "Chunked_Bulk_Insertion/BulkInsert.h"
#include "Chunked_Bulk_Insertion/DeleteFilePostings.h"

using namespace std;


sqlite3* OpenDB();

void CreateTables(sqlite3* db);

long long GetOrCreateFileId(
    sqlite3* db,
    const std::string& path
);

void InsertWord(
    sqlite3* db,
    const std::string& word,
    long long file_id
);




void DeleteFilePostings(
    sqlite3* db,
    long long file_id
);


void BulkInsert(
    sqlite3* db,
    long long file_id,
    const std::unordered_map<std::string,int>& freq
);

void CloseDB(sqlite3* db);

//returns function of "SearchWord/searchword.cpp" file to run 

void search_word(sqlite3* db,std::string &word);