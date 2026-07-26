#include <iostream>
#include <string>
#include <unordered_map>
#include <sqlite3.h>
#include "../Database/Database.h"
#include "../Database/Chunked_Bulk_Insertion/BulkInsert.h"
#include "../Database/Chunked_Bulk_Insertion/DeleteFilePostings.h"
#include "check.h"

void test_database() {
    std::cout << "\n--- Testing Database Operations ---\n";

    sqlite3* db = nullptr;
    bool opened = (sqlite3_open(":memory:", &db) == SQLITE_OK);
    check(opened, "opened isolated in-memory database");
    if (!opened) return;

    CreateTables(db);

    long long file_id = GetOrCreateFileId(db, "/dummy/path.txt", 12345);
    check(file_id > 0, "GetOrCreateFileId assigns a positive file id");

    std::unordered_map<std::string, int> freqs = {{"hello", 3}, {"world", 2}};
    BulkInsert(db, file_id, freqs);

    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(db, "SELECT frequency FROM inverted_index WHERE word='hello'", -1, &stmt, nullptr);
    bool found_hello = (sqlite3_step(stmt) == SQLITE_ROW);
    int hello_freq = found_hello ? sqlite3_column_int(stmt, 0) : -1;
    sqlite3_finalize(stmt);

    check(found_hello, "'hello' posting exists after BulkInsert");
    check_eq<int>(hello_freq, 3, "'hello' frequency is 3 after BulkInsert");

    DeleteFilePostings(db, file_id);

    sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM inverted_index", -1, &stmt, nullptr);
    int remaining = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        remaining = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    check_eq<int>(remaining, 0, "no postings remain after DeleteFilePostings");

    sqlite3_close(db);
}
