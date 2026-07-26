#include <iostream>
#include <string>
#include <unordered_map>
#include <sqlite3.h>
#include "../Database/Database.h"
#include "../Database/Chunked_Bulk_Insertion/BulkInsert.h"
#include "../Database/Chunked_Bulk_Insertion/DeleteFilePostings.h"

using namespace std;

void test_database() {
    cout << "\n--- Testing Database Operations ---\n";
    cout << "Opening an isolated in-memory database...\n";
    
    sqlite3* db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        cout << "Failed to open in-memory database.\n";
        return;
    }
    
    cout << "Creating tables...\n";
    CreateTables(db);
    
    cout << "Simulating crawling a file '/dummy/path.txt' with modified time 12345...\n";
    long long file_id = GetOrCreateFileId(db, "/dummy/path.txt", 12345);
    cout << "Assigned File ID in DB: " << file_id << "\n";
    
    cout << "Bulk inserting words for this file (hello: 3, world: 2)...\n";
    unordered_map<string, int> freqs = {{"hello", 3}, {"world", 2}};
    BulkInsert(db, file_id, freqs);
    
    cout << "Querying DB to verify 'hello' frequency...\n";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT frequency FROM inverted_index WHERE word='hello'", -1, &stmt, nullptr);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "Found 'hello' with frequency: " << sqlite3_column_int(stmt, 0) << "\n";
    }
    sqlite3_finalize(stmt);
    
    cout << "Deleting postings for File ID " << file_id << " (simulating re-indexing)...\n";
    DeleteFilePostings(db, file_id);
    
    sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM inverted_index", -1, &stmt, nullptr);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "Remaining postings in DB: " << sqlite3_column_int(stmt, 0) << "\n";
    }
    sqlite3_finalize(stmt);
    
    sqlite3_close(db);
    cout << "Database closed safely.\n";
}
