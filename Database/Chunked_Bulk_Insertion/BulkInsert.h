#pragma once
#include "../Database.h"
#include "../SearchWord/searchword.h"

#include <iostream>
#include <unordered_map>


void BulkInsert(
    sqlite3* db,
    long long file_id,
    const std::unordered_map<std::string,int>& freq
);