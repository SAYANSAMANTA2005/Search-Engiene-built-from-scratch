#include "Database.h"
#include "SearchWord/searchword.h"
#include "Chunked_Bulk_Insertion/BulkInsert.h"

#include <iostream>

using namespace std;

sqlite3* OpenDB()
{

    sqlite3* db = nullptr;
    int rc =
        sqlite3_open(
            "search_engine.db",
            &db
        );

    if(rc != SQLITE_OK)
    {
        cerr
            << "Failed to open database: "
            << sqlite3_errmsg(db)
            << '\n';

        return nullptr;
    }

    cout
        << "Database Connected Successfully\n";

    return db;
}

void CreateTables(sqlite3* db)
{
    const char* sql = R"(
    CREATE TABLE IF NOT EXISTS files
    (   
        file_id INTEGER PRIMARY KEY AUTOINCREMENT,
        file_path TEXT UNIQUE NOT NULL,
        modified_time INTEGER NOT NULL
    );

   CREATE TABLE IF NOT EXISTS inverted_index
   (   
       word TEXT NOT NULL,
       file_id INTEGER NOT NULL,
       frequency INTEGER NOT NULL DEFAULT 0,

       PRIMARY KEY(word,file_id),

       FOREIGN KEY(file_id)
          REFERENCES files(file_id)
);
    )";

    char* err = nullptr;

    int rc =
        sqlite3_exec(
            db,
            sql,
            nullptr,
            nullptr,
            &err
        );

    if(rc != SQLITE_OK)
    {
        cerr
            << "Create Table Error: "
            << err
            << '\n';

        sqlite3_free(err);
    }
    else
    {
        cout
            << "Tables Ready\n";
    }
}

long long GetOrCreateFileId(
    sqlite3* db,
    const std::string& path,
    long long modified_time
)
{
    sqlite3_stmt* stmt = nullptr;

    //--------------------------------------------------
    // Try finding existing file_id
    //--------------------------------------------------

    const char* select_sql =
        "SELECT file_id "
        "FROM files "
        "WHERE file_path = ?;";

    sqlite3_prepare_v2(
        db,
        select_sql,
        -1,
        &stmt,
        nullptr
    );

    sqlite3_bind_text(
        stmt,
        1,
        path.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    if(sqlite3_step(stmt) == SQLITE_ROW)
    {
        long long file_id =
            sqlite3_column_int64(stmt, 0);

        sqlite3_finalize(stmt);

        return file_id;
    }

    sqlite3_finalize(stmt);

    //--------------------------------------------------
    // File not present
    // Insert new file
    //--------------------------------------------------

    const char* insert_sql =
        "INSERT INTO files "
        "(file_path, modified_time) "
        "VALUES(?, ?);";

    sqlite3_prepare_v2(
        db,
        insert_sql,
        -1,
        &stmt,
        nullptr
    );

    sqlite3_bind_text(
        stmt,
        1,
        path.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_int64(
        stmt,
        2,
        modified_time
    );

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        cerr
            << "Insert File Error: "
            << sqlite3_errmsg(db)
            << '\n';
    }

    sqlite3_finalize(stmt);

    return sqlite3_last_insert_rowid(db);
}

void InsertWord(
    sqlite3* db,
    const std::string& word,
    long long file_id
)
{
    sqlite3_stmt* stmt = nullptr;

    const char* sql =
        "INSERT OR IGNORE "
        "INTO inverted_index("
        "word, file_id"
        ") VALUES(?, ?)";

    sqlite3_prepare_v2(
        db,
        sql,
        -1,
        &stmt,
        nullptr
    );

    sqlite3_bind_text(
        stmt,
        1,
        word.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_int64(
        stmt,
        2,
        file_id
    );

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        cerr
            << "InsertWord Error: "
            << sqlite3_errmsg(db)
            << '\n';
    }

    sqlite3_finalize(stmt);
}

void CloseDB(sqlite3* db)
{
    if(db)
    {
        sqlite3_close(db);

        cout
            << "Database Closed\n";
    }
}