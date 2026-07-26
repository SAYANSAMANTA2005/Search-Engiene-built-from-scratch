#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <sqlite3.h>
#include "../Reindexing/FileChangeDetector.h"
#include "../Database/Database.h"
#include "../Database/ModifiedTime/ModifiedTime.h"

using namespace std;
namespace fs = std::filesystem;

void test_file_change_detector() {
    cout << "\n--- Testing File Change Detector ---\n";
    
    sqlite3* db = nullptr;
    sqlite3_open(":memory:", &db);
    CreateTables(db);
    
    // Create a temporary file on disk
    string path = "temp_test_file.txt";
    ofstream f(path);
    f << "test data\n";
    f.close();
    
    cout << "Created temp file on disk: " << path << "\n";
    
    cout << "\nScenario 1: File is not in the database yet\n";
    bool needs_index = Need_To_Change_OR_Create_indexing(db, path);
    cout << "Result: " << (needs_index ? "True (Needs indexing)" : "False (Skip)") << "\n";
    
    // Get actual modification time from the filesystem
    auto ftime = fs::last_write_time(path);
    long long actual_mtime = std::abs((long long)std::chrono::duration_cast<std::chrono::seconds>(ftime.time_since_epoch()).count());
    
    cout << "\nScenario 2: Simulating we just indexed it (adding exact mtime to DB)...\n";
    modify_DB_indexing_time(db, path, actual_mtime);
    needs_index = Need_To_Change_OR_Create_indexing(db, path);
    cout << "Result: " << (needs_index ? "True (Needs indexing)" : "False (Skip)") << "\n";
    
    cout << "\nScenario 3: Simulating file was edited (adding an old mtime to DB)...\n";
    modify_DB_indexing_time(db, path, actual_mtime - 1000);
    needs_index = Need_To_Change_OR_Create_indexing(db, path);
    cout << "Result: " << (needs_index ? "True (Needs indexing)" : "False (Skip)") << "\n";
    
    // Cleanup
    fs::remove(path);
    sqlite3_close(db);
    cout << "\nTemp file removed and DB closed.\n";
}
