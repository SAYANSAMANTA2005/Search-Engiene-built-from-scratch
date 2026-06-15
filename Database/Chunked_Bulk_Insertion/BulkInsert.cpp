#include "../Database.h"
#include "../SearchWord/searchword.h"
#include "BulkInsert.h"
#include <sqlite3.h>
#include <iostream>
#include <unordered_map>

using namespace std;
/*

Building the frequency map is O(T) average using an unordered_map.
Writing the postings to SQLite is O(U log N), where U is the
number of unique words in the document and N is the total number
of postings in the inverted index.
Therefore the overall indexing complexity is:

O(T + U log N)

*/
void BulkInsert(
    sqlite3* db,
    long long file_id,
    const std::unordered_map<std::string,int>& freq
)
{
    if(!db || freq.empty())
        return;

    char* err = nullptr;
/*

    sqlite3_exec(
        db,
        "BEGIN TRANSACTION;",
        nullptr,
        nullptr,
        &err
    );





*/

    const char* sql =
        "INSERT INTO inverted_index "
        "(word,file_id,frequency) "
        "VALUES(?,?,?) "
        "ON CONFLICT(word,file_id) "
        "DO UPDATE SET "
        "frequency = frequency + excluded.frequency;";

    sqlite3_stmt* stmt = nullptr;

    if(sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &stmt,
            nullptr) != SQLITE_OK)
    {
        cerr
            << "BulkInsert Prepare Error: "
            << sqlite3_errmsg(db)
            << '\n';

        return;
    }

    for(const auto& [word,count] : freq)
    {
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

        sqlite3_bind_int(
            stmt,
            3,
            count
        );

        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            cerr
                << "BulkInsert Error: "
                << sqlite3_errmsg(db)
                << '\n';
        }

        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
    }

    sqlite3_finalize(stmt);
/*


    sqlite3_exec(
        db,
        "COMMIT;",
        nullptr,
        nullptr,
        &err
    );

*/

}