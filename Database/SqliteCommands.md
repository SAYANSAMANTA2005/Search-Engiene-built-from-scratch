# SQLite Commands Required For My Search Engine

## Goal

Use SQLite to persist:

* File IDs
* File Paths
* File Metadata
* Inverted Index

This allows the search engine to retain information across program restarts.

---

# 1. Open Database

```cpp
sqlite3* db;

sqlite3_open(
    "search_engine.db",
    &db
);
```

Purpose:

```text
Create database if it doesn't exist.
Open existing database if it already exists.
```

---

# 2. Close Database

```cpp
sqlite3_close(db);
```

Purpose:

```text
Flush pending writes.
Release resources.
```

---

# 3. Execute SQL Query

```cpp
sqlite3_exec(
    db,
    sql,
    nullptr,
    nullptr,
    nullptr
);
```

Purpose:

```text
Run SQL directly.
```

Used for:

* CREATE TABLE
* DELETE
* DROP TABLE
* Bulk operations

---

# 4. Create Files Table

```sql
CREATE TABLE IF NOT EXISTS files(
    file_id INTEGER PRIMARY KEY AUTOINCREMENT,
    file_path TEXT UNIQUE NOT NULL
);
```

Purpose:

```text
Store persistent file IDs.
```

Example:

| file_id | file_path |
| ------- | --------- |
| 1       | a.cpp     |
| 2       | b.cpp     |

---

# 5. Create Inverted Index Table

```sql
CREATE TABLE IF NOT EXISTS inverted_index(
    word TEXT NOT NULL,
    file_id INTEGER NOT NULL
);
```

Purpose:

```text
Store word → file relationships.
```

Example:

| word  | file_id |
| ----- | ------- |
| hello | 1       |
| hello | 2       |
| world | 1       |

---

# 6. Prepare Query

```cpp
sqlite3_stmt* stmt;

sqlite3_prepare_v2(
    db,
    sql.c_str(),
    -1,
    &stmt,
    nullptr
);
```

Purpose:

```text
Compile SQL query.
```

Required for:

* SELECT
* INSERT
* UPDATE
* DELETE

with parameters.

---

# 7. Bind Parameter

```cpp
sqlite3_bind_text(
    stmt,
    1,
    path.c_str(),
    -1,
    SQLITE_STATIC
);
```

Purpose:

```text
Replace ? placeholders.
```

Example:

```sql
SELECT file_id
FROM files
WHERE file_path=?
```

---

# 8. Execute Prepared Statement

```cpp
sqlite3_step(stmt);
```

Purpose:

```text
Run query.
```

Returns:

```cpp
SQLITE_ROW
```

or

```cpp
SQLITE_DONE
```

---

# 9. Read Result

```cpp
long long id =
    sqlite3_column_int64(
        stmt,
        0
    );
```

Purpose:

```text
Read value from SELECT result.
```

Example:

```sql
SELECT file_id
FROM files
```

---

# 10. Free Statement

```cpp
sqlite3_finalize(stmt);
```

Purpose:

```text
Prevent memory leaks.
```

Always call after:

```cpp
sqlite3_prepare_v2(...)
```

---

# 11. Insert File

```sql
INSERT INTO files(file_path)
VALUES(?)
```

Purpose:

```text
Add new file.
```

---

# 12. Get Auto Generated File ID

```cpp
sqlite3_last_insert_rowid(db);
```

Purpose:

```text
Get newly assigned file_id.
```

Example:

```text
Inserted file

file_id = 57
```

---

# 13. Find Existing File

```sql
SELECT file_id
FROM files
WHERE file_path=?
```

Purpose:

```text
Reuse file ID if file already exists.
```

---

# 14. Update File Metadata

```sql
UPDATE files
SET size=?,
    modified_time=?
WHERE file_id=?
```

Purpose:

```text
Store latest file state.
```

---

# 15. Delete File

```sql
DELETE FROM files
WHERE file_id=?
```

Purpose:

```text
Remove deleted files.
```

---

# 16. Check If File Exists

```sql
SELECT EXISTS(
    SELECT 1
    FROM files
    WHERE file_path=?
)
```

Purpose:

```text
Quick existence check.
```

---

# 17. Insert Word Into Inverted Index

```sql
INSERT INTO inverted_index(
    word,
    file_id
)
VALUES(?,?)
```

Purpose:

```text
Store word occurrence.
```

Example:

```text
hello -> file 1
hello -> file 2
```

---

# 18. Search Word

```sql
SELECT file_id
FROM inverted_index
WHERE word=?
```

Purpose:

```text
Find all files containing a word.
```

Example:

```text
Search: hello

Returns:
1
2
7
15
```

---

# 19. Delete Existing Posting List

```sql
DELETE
FROM inverted_index
WHERE file_id=?
```

Purpose:

```text
Re-index modified file.
```

Workflow:

```text
Delete old words
→ Insert new words
```

---

# 20. Transactions (Important)

```sql
BEGIN TRANSACTION;
```

Insert many rows.

```sql
COMMIT;
```

Purpose:

```text
Much faster bulk insertion.
```

Without transaction:

```text
100000 inserts
=
100000 disk writes
```

With transaction:

```text
100000 inserts
=
1 disk write
```

Huge performance improvement.

---

# Minimum SQLite Features Needed For Version 1

1. sqlite3_open()
2. CREATE TABLE
3. SELECT
4. INSERT
5. sqlite3_last_insert_rowid()
6. sqlite3_finalize()
7. sqlite3_close()

These are sufficient to implement:

```text
Persistent File IDs
+
Persistent Inverted Index
+
Basic Search
```

Future versions can add:

```text
Metadata Tracking
Incremental Indexing
TF-IDF Ranking
Phrase Search
```
