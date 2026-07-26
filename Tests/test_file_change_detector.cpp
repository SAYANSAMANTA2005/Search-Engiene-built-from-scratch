#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <cstdlib>
#include <sqlite3.h>
#include "../Reindexing/FileChangeDetector.h"
#include "../Database/Database.h"
#include "../Database/ModifiedTime/ModifiedTime.h"
#include "check.h"

namespace fs = std::filesystem;

void test_file_change_detector() {
    std::cout << "\n--- Testing File Change Detector ---\n";

    sqlite3* db = nullptr;
    sqlite3_open(":memory:", &db);
    CreateTables(db);

    std::string path = "F:\\JU SUPER FOLDER\\CP_JUDGE\\SEARCH_ENGINE\\repo\\Tests";//a original path in my laptop
    std::ofstream f(path);
    f << "test data\n";
    f.close();

    // Scenario 1: file is not in the database yet -> must need indexing
    bool needs_index = Need_To_Change_OR_Create_indexing(db, path);
    check(needs_index == true, "unindexed file needs indexing");

    auto ftime = fs::last_write_time(path);
    long long actual_mtime = std::abs(
        (long long)std::chrono::duration_cast<std::chrono::seconds>(ftime.time_since_epoch()).count()
    );

    // Scenario 2: DB already has the exact current mtime -> should NOT need reindexing
    modify_DB_indexing_time(db, path, actual_mtime);
    needs_index = Need_To_Change_OR_Create_indexing(db, path);
    check(needs_index == false, "file with matching mtime in DB does not need reindexing");

    // Scenario 3: DB has an older mtime (simulating the file was edited) -> SHOULD need reindexing
    // NOTE: this currently FAILS. Need_To_Change_OR_Create_indexing() compares
    // `llabs(modified_time) < DB_Modified_time`, which only returns true when the
    // file's current mtime is OLDER than the DB's stored value. A real edit makes
    // the file's mtime NEWER than what's stored, so this condition never fires for
    // genuinely edited files -- the comparison operator needs to be `>`, not `<`,
    // in Reindexing/FileChangeDetector.cpp. Left asserting the correct expected
    // behavior on purpose, so this test keeps failing (and reminding you) until
    // that's fixed, instead of being quietly rewritten to match the buggy result.
    modify_DB_indexing_time(db, path, actual_mtime - 1000);
    needs_index = Need_To_Change_OR_Create_indexing(db, path);
    check(needs_index == true, "file edited since last index needs reindexing");

    fs::remove(path);
    sqlite3_close(db);
}
