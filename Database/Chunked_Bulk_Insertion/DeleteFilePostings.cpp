#include "../Database.h"
#include "DeleteFilePostings.h"


#include<sqlite3.h>
#include<string.h>
#include <iostream>

using namespace std;

// Remove old inverted-index entries for this file before re-indexing.
// Prevents frequency inflation when the same file is indexed again.
// Example:
// Previous run: stock -> 500
// New run:      stock -> 500
// Without deletion: stock -> 1000 (wrong)
// With deletion:    stock -> 500 (correct)



// Prevents duplicate frequency accumulation across crawler runs.
// Tradeoff: if indexing fails after deletion, file must be re-indexed.
void DeleteFilePostings(
    sqlite3* db,
    long long file_id
)
{
    sqlite3_stmt* stmt = nullptr;

    const char* sql =
        "DELETE FROM inverted_index "
        "WHERE file_id = ?;";

    sqlite3_prepare_v2(
        db,
        sql,
        -1,
        &stmt,
        nullptr
    );

    sqlite3_bind_int64(
        stmt,
        1,
        file_id
    );

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);
}