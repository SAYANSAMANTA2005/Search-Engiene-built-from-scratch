#pragma once

#include <sqlite3.h>
#include <string>
#include <unordered_map>

using namespace std;

// Flush frequency map to database and clear memory
void Freeing_map(
    sqlite3* db,
    long long file_id,
    unordered_map<string,int>& freq
);

// Increment word frequency and flush if threshold reached
void Incrementing_Word_freq(
    string& word,
    unordered_map<string,int>& freq,
    long long file_id,
    sqlite3* db
);

// Read file in chunks, tokenize, count frequencies,
// and write postings to the database
void Read_File2(
    string path,
    sqlite3* db,
    long long file_id
);