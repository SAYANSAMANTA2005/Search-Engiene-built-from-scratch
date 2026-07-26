#include<sqlite3.h>
#include<iostream>
#include<string>

using namespace std;

/*
get_db_last_modified  ---->  get the last_modified_time of a file from the database.
                        ----> also handels the case when The file is not present in DB(returns -1)


modify_DB_indexing_time --> updates the last_modified_time of a file in the database.
                        ----> also handels the case when The file is not present in DB
    -->case 1: (inserts the file and updates the last_modified_time of the file in the database.)
    -->case 2: (updates the last_modified_time of the file in the database.)

*/
void modify_DB_indexing_time(
    sqlite3* db,
    const std::string& path,
    long long modified_time
)
{
    sqlite3_stmt* stmt = nullptr;

    const char* sql =
        "INSERT INTO files "
        "(file_path, modified_time) "
        "VALUES(?, ?) "
        "ON CONFLICT(file_path) "
        "DO UPDATE SET "
        "modified_time = excluded.modified_time;";

    if(sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &stmt,
            nullptr) != SQLITE_OK)
    {
        std::cerr
            << "modify_DB_indexing_time Error: "
            << sqlite3_errmsg(db)
            << '\n';
        return;
    }

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

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);
}

long long get_db_last_modified(
    sqlite3* db,
    const std::string& path
)
{
    sqlite3_stmt* stmt = nullptr;

    const char* sql =
        "SELECT modified_time "
        "FROM files "
        "WHERE file_path = ?;";

    if(sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &stmt,
            nullptr) != SQLITE_OK)
    {
        return -1;
    }

    sqlite3_bind_text(
        stmt,
        1,
        path.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    long long modified_time = -1;

    if(sqlite3_step(stmt) == SQLITE_ROW)
    {
        modified_time =
            sqlite3_column_int64(
                stmt,
                0
            );
    }

    sqlite3_finalize(stmt);

    return modified_time;
}