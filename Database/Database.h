#pragma once

#include <sqlite3.h>
#include <string>
#include "SearchWord/searchword.h"

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

void CloseDB(sqlite3* db);