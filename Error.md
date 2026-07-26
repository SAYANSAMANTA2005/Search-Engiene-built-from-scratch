# 🔍 Search Engine — Error Report & Improvement Suggestions

> **Analyzed by:** Antigravity AI  
> **Date:** 2026-07-26  
> **Scope:** All `.cpp` and `.h` files in the repository

---

## Table of Contents

1. [🔴 Critical / Logical Errors](#-critical--logical-errors)
2. [🟠 Medium Severity Bugs](#-medium-severity-bugs)
3. [🟡 Minor Issues & Code Smells](#-minor-issues--code-smells)
4. [🔵 Feature Suggestions & Improvements](#-feature-suggestions--improvements)

---

## 🔴 Critical / Logical Errors

---

### 1. SQL Injection Vulnerability — `searchword.cpp` & `searchwordbyfileid.cpp`

**Files:**
- `Database/SearchWord/searchword.cpp` — Line 111
- `Database/SearchWord/searchwordbyfileid.cpp` — Line 118

**Problem:**  
The SQL query is built by **directly concatenating the user's input word** into the query string:

```cpp
// searchword.cpp, line 111
std::string sql =
    "SELECT file_path, frequency "
    "FROM files "
    "JOIN inverted_index "
    "ON files.file_id = inverted_index.file_id "
    "WHERE word = '" + word + "' "   // DANGER: direct string concat
    "ORDER BY frequency DESC, files.file_id ASC;";
```

**What Can Go Wrong:**  
A word containing a single-quote (e.g., `O'Brian`) will **crash the SQL query** with a syntax error. A malicious input like `'; DROP TABLE inverted_index; --` would **destroy your entire index**.

**Fix — Use Prepared Statements:**

```cpp
// Use sqlite3_prepare_v2 + sqlite3_bind_text instead of sqlite3_exec
const char* sql =
    "SELECT file_path, frequency FROM files "
    "JOIN inverted_index ON files.file_id = inverted_index.file_id "
    "WHERE word = ? ORDER BY frequency DESC;";

sqlite3_stmt* stmt = nullptr;
sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
sqlite3_bind_text(stmt, 1, word.c_str(), -1, SQLITE_TRANSIENT);
// ... step and finalize
```

This is the same pattern already correctly used in `Database.cpp`, `ModifiedTime.cpp`, and others — just not here.

---

### 2. Crash on Empty Tokenizer Result — `Search/Single_Word/Search.cpp`

**File:** `Search/Single_Word/Search.cpp` — Line 65

**Problem:**  
`tokenize(s)` can return an **empty vector** (e.g., if user enters only digits, punctuation, or a single stop word). Accessing `[0]` on an empty vector is **undefined behavior / crash**:

```cpp
s = tokenize(s)[0];   // CRASH if tokenize() returns empty vector!
```

**Fix:**

```cpp
auto tokens = tokenize(s);
if (tokens.empty()) {
    cout << "No valid search term. Please enter a real word.\n";
    return {};
}
s = tokens[0];
```

> **Note:** The `fuzzy_search_handeler` already handles this correctly — apply the same guard here.

---

### 3. Wrong Comparison Logic in `FileChangeDetector.cpp`

**File:** `Reindexing/FileChangeDetector.cpp` — Line 36

**Problem:**  
The condition to decide whether re-indexing is needed is **logically inverted**:

```cpp
// Current (WRONG):
if(((DB_Modified_time == -1) || (llabs(modified_time) < DB_Modified_time))) {
    return true;  // re-index
}
```

`llabs(modified_time) < DB_Modified_time` means *"the file's modification time is OLDER than what's in the DB"* — this triggers re-indexing for **old/unmodified files** but **skips actually modified files**.

**What it should be:**

```cpp
// Correct:
if ((DB_Modified_time == -1) || (llabs(modified_time) != DB_Modified_time)) {
    return true;  // file is new OR its modification time changed -> re-index
}
```

Using `!=` correctly catches both newly modified files AND new files. Using `>` instead of `<` would also work (file is newer than DB record -> re-index), but `!=` is the safest.

---

### 4. Global `file_id` Used as Shared Mutable State

**Files:**
- `config/config.h` — Line 10: `inline long long file_id = 0;`
- `Reindexing/ReindexFile.cpp` — Line 22
- `Crawler.cpp` — Lines 151, 153

**Problem:**  
`file_id` is a **global mutable variable** assigned in `ReindexFile.cpp` and then read back in `Crawler.cpp`. The reset `file_id = 0` in Crawler.cpp (line 153) only happens every `INTERVAL_OF_PROCESSED_FILE_SHOW` (100) files, not per-file, meaning the displayed `file_id` in the log is always stale/wrong. This pattern is also fragile for any future multi-threaded use.

**Fix:**  
Make `ReindexFile()` return the `file_id` it assigned:

```cpp
// ReindexFile.h
long long ReindexFile(sqlite3* db, std::string& Path, int count);

// ReindexFile.cpp
long long ReindexFile(...) {
    long long fid = GetOrCreateFileId(db, Path, llabs(modified_time));
    DeleteFilePostings(db, fid);
    Read_File2(Path, db, fid);
    modify_DB_indexing_time(db, Path, llabs(modified_time));
    return fid;
}

// Crawler.cpp
long long fid = ReindexFile(db, Path, Processed_files_count);
// Log fid directly
```

---

### 5. `OpenDB()` Returns `nullptr` But Caller Never Checks — `Crawler.cpp`

**File:** `Crawler.cpp` — Lines 189–190

**Problem:**  
`OpenDB()` returns `nullptr` on failure, but `main()` immediately passes it to `CreateTables(db)` without a null check:

```cpp
sqlite3* db = OpenDB();  // can be nullptr
CreateTables(db);        // CRASH: passes nullptr to sqlite3 functions
```

**Fix:**

```cpp
sqlite3* db = OpenDB();
if (!db) {
    cerr << "Fatal: Cannot open database. Exiting.\n";
    return 1;
}
CreateTables(db);
```

---

### 6. No Atomic Transaction Around `DeleteFilePostings` + `Read_File2` — Data Loss Risk

**Files:** `Reindexing/ReindexFile.cpp`, `Database/Chunked_Bulk_Insertion/DeleteFilePostings.cpp`

**Problem:**  
`ReindexFile` does:
1. `DeleteFilePostings(db, file_id)` — deletes all index entries for the file
2. `Read_File2(...)` — re-reads and re-inserts entries

If the **program crashes or is killed between step 1 and step 2**, the file's index entries are **permanently deleted** with nothing to replace them. The file becomes invisible to all future searches until the next full re-crawl.

**Fix:**  
Wrap the entire `ReindexFile` operation in a single outer transaction:

```cpp
void ReindexFile(sqlite3* db, std::string& Path, int count) {
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    // ... delete postings, re-read file, bulk insert ...
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
}
```

---

### 7. `BulkInsert` Transaction Is Commented Out — Severe Performance Issue

**File:** `Database/Chunked_Bulk_Insertion/BulkInsert.cpp` — Lines 31–44, 106–117

**Problem:**  
The `BEGIN TRANSACTION` / `COMMIT` block inside `BulkInsert` is **commented out**. Every single INSERT now runs in its own auto-commit transaction. SQLite auto-commits are extremely slow — indexing large files may be **10–100x slower** than necessary.

The transaction is managed in `Read_File2` (Chunked_Reading.cpp), so this is intentional — but it should be clearly documented, not left as dead code. If `BulkInsert` is ever called from outside `Read_File2`, it will be critically slow.

**Suggestion:**  
Delete the commented-out block and add a comment: `// Transaction is managed by the caller (Read_File2).`

---

### 8. `cnt` Global Not Reset Before Use in `searchword.cpp`

**File:** `Database/SearchWord/searchword.cpp` — Lines 75, 135

**Problem:**  
`cnt` is a file-scope global. It is reset to `0` *after* the query runs (line 135), but NOT *before* the callback fires. If `search_single_word` is called and a prior call left `cnt` non-zero (e.g., on exception or error path), the result count and the early-exit behavior inside `callback()` will be wrong.

**Fix:**

```cpp
vector<pair<string,int>> search_single_word(sqlite3* db, string& word) {
    SearchWordResult.clear();
    cnt = 0;  // Reset BEFORE the query runs, not after
    ...
}
```

---

## 🟠 Medium Severity Bugs

---

### 9. Numbers Are Completely Stripped from Tokenizer

**File:** `tokenize/tokeniser.cpp` — Line 29

**Problem:**

```cpp
if (isalnum(c) && (!(c >= '0' && c <= '9')))
```

This drops ALL digits. Version numbers (`c++17`, `python3`, `gcc-12`), error codes (`error404`), identifiers like `sha256` — all get their digits stripped.

**Why it Matters:**  
Searching for `"python3"` or `"c++17"` will fail because these tokens were never stored with their digits.

**Fix:**  
Keep digits in tokens. Optionally filter purely numeric strings afterward:

```cpp
if (isalnum(c)) {
    cur += tolower(c);
}
```

---

### 10. Stemmer Produces Wrong / Harmful Stems

**File:** `tokenize/stemming/stemmer.cpp`

**Problem:**  
The naive stemmer mangles many common English words:

| Input | Produced | Expected |
|---|---|---|
| `"class"` | `"clas"` | `"class"` |
| `"process"` | `"proces"` | `"process"` |
| `"access"` | `"acces"` | `"access"` |
| `"address"` | `"addres"` | `"address"` |
| `"success"` | `"succes"` | `"success"` |
| `"stress"` | `"stres"` | `"stress"` |
| `"glass"` | `"glas"` | `"glass"` |
| `"bus"` | `"bu"` | `"bus"` |

Words ending in double-s (`ss`) or short words ending in `s` become unfindable after stemming.

**Fix:**  
Add a guard: only strip trailing `s` if the result has 3+ chars AND the word doesn't end in `ss`:

```cpp
else if (word.size() > 3 && word.back() == 's' &&
         !(word.size() > 1 && word[word.size()-2] == 's'))
{
    word.pop_back();
}
```

Better solution: use an established **Porter Stemmer** implementation.

---

### 11. `stoi(argv[1])` in Fuzzy Callback Can Throw on NULL

**File:** `Search/Fuzzy_Search/Fuzzy_handeler.cpp` — Line 79

**Problem:**

```cpp
int freq = std::stoi(argv[1]);
```

If `SUM(frequency)` returns NULL for any reason, `argv[1]` will be `nullptr` and `std::stoi(nullptr)` causes undefined behavior / crash.

**Fix:**

```cpp
int freq = argv[1] ? std::stoi(argv[1]) : 0;
```

---

### 12. Variable Shadowing Bug — Timer Not Updated in `searchwordbyfileid.cpp`

**File:** `Database/SearchWord/searchwordbyfileid.cpp` — Line 147

**Problem:**

```cpp
auto start_time_db2 = now;  // Creates a NEW LOCAL variable, doesn't update global!
```

Using `auto` here declares a brand new local variable that shadows the global `start_time_db2` (declared at line 12) and immediately goes out of scope. The global is never updated.

**Fix:**

```cpp
start_time_db2 = now;  // Remove `auto` to assign to the global
```

---

### 13. Wrong Include Path in `Chunked_Reading.cpp`

**File:** `ReadFile/ChunkedProcessing/Chunked_Reading.cpp` — Line 11

**Problem:**

```cpp
#include "../config/config.h"
```

This file is at `ReadFile/ChunkedProcessing/`. One `..` goes to `ReadFile/`, not the project root. Should be:

```cpp
#include "../../config/config.h"
```

---

### 14. Multi-word Intersection Uses Min Frequency — Suboptimal Ranking

**File:** `Search/Multi_Word/And/and.cpp` — Line 21

**Problem:**

```cpp
get<1>(A[i]) = min(get<1>(A[i]), get<1>(B[j]));
```

Using `min` for scoring means a document highly relevant to both words scores lower than one relevant to only one. Sum is a better relevance metric.

**Suggestion:**

```cpp
get<1>(A[i]) += get<1>(B[j]);  // Sum frequencies = better relevance score
```

---

## 🟡 Minor Issues & Code Smells

---

### 15. `#include <bits/stdc++.h>` Used in Multiple Files

**Files:** `Crawler.cpp` (line 2), `ReadFile/Readfile.cpp` (line 2), `ReadFile/Readfile.h` (line 3)

`bits/stdc++.h` is GCC-specific, non-standard, massively increases compile time, and is not portable to MSVC or Clang. Replace with specific standard headers.

---

### 16. Duplicate `#include <iostream>` in `Crawler.cpp`

**File:** `Crawler.cpp` — Lines 1 and 15 both include `<iostream>`. No functional harm, but messy.

---

### 17. `using namespace std;` in Header Files

**Files:** `config/config.h`, `Database/Database.h`, `ReadFile/Readfile.h`, `Reindexing/Combined.h`, `Search/Specify_Search_Handeler.h`

This pollutes the global namespace of every `.cpp` that includes them. Remove from all headers and use explicit `std::` qualification.

---

### 18. Duplicate Entries in `SKIP_DIRECTORIES`

**File:** `config/config.h`

The following strings appear more than once in the `unordered_set`:

| String | Lines |
|---|---|
| `"out"` | 129, 152 |
| `"target"` | 151, 233 |
| `"__pycache__"` | 102, 335 |
| `"Library"` | 302, 321 |
| `"bin"` | 131, 294 |

`unordered_set` deduplicates silently, so no bug — but indicates the list was never audited.

---

### 19. Glob Patterns Like `"*.egg-info"` in `SKIP_DIRECTORIES` Don't Work

**File:** `config/config.h` — Lines 112 (`"*.egg-info"`), 170 (`"*.suo"`), 171 (`"*.user"`)

These use wildcard `*`, but the skip check uses exact string matching via `.count(dirname)`. A directory named `mypackage.egg-info` will **never be skipped**. A suffix-match check is needed for these patterns.

---

### 20. `Processed_files_count` Parameter in `ReindexFile` Is Never Used

**File:** `Reindexing/ReindexFile.cpp` — Line 16

The parameter is received but never used inside the function body. Either use it for logging or remove it from the signature.

---

### 21. `Search_Single_WordResult` Is an Unnecessary Global

**File:** `Search/Single_Word/Search.cpp` — Line 13

```cpp
vector<pair<string,int>> Search_Single_WordResult;  // file-scope global
```

This is immediately returned from the function. It doesn't need to be global. Declare it as a local inside `Search_Single_Word_in_Database()`.

---

### 22. `Explorer/File_Reader.cpp` Is Empty (0 bytes)

**File:** `Explorer/File_Reader.cpp`

This appears to be a leftover empty stub file. Either implement it or delete it.

---

### 23. `Tests/` Directory Is Completely Empty

**File:** `Tests/`

There are zero automated tests. The critical bugs in #1, #2, #3, and #5 above would have been caught by even basic unit tests. Consider adding at least:
- A tokenizer test (empty string, digits, stop words)
- A stemmer test (words ending in `ss`, `ed`, `ing`)
- A DB connection failure test
- A file-change-detection logic test

---

## 🔵 Feature Suggestions & Improvements

---

### F1 — Add OR Search Mode to Multi-Word Search

The code currently only supports AND (intersection). Add an OR (union) mode so users can find files containing **any** of the query words. The comment in `Multi_Search.cpp` line 87–94 already marks this as a known TODO.

---

### F2 — TF-IDF Scoring Instead of Raw Frequency

Current ranking uses raw term frequency. TF-IDF gives much better relevance:
- Rare words in a file rank much higher than common words
- The `inverted_index` already stores TF; add a `doc_count` column or compute IDF at query time.

---

### F3 — Cache the Trie Between Fuzzy Searches

Every fuzzy search rebuilds the entire Trie from the DB (can be slow for large indexes). Build the Trie once at startup and invalidate it only after a new crawl.

```cpp
static Trie* g_trie = nullptr;
static bool g_trie_dirty = true;

Trie* get_trie(sqlite3* db) {
    if (g_trie_dirty || !g_trie) {
        delete g_trie;
        g_trie = load_trie_from_db(db);
        g_trie_dirty = false;
    }
    return g_trie;
}
```

---

### F4 — Add SQLite PRAGMA Optimizations

Add these PRAGMAs after opening the DB for dramatically faster indexing and searching:

```cpp
sqlite3_exec(db, "PRAGMA journal_mode = WAL;", nullptr, nullptr, nullptr);
sqlite3_exec(db, "PRAGMA synchronous = NORMAL;", nullptr, nullptr, nullptr);
sqlite3_exec(db, "PRAGMA cache_size = -64000;", nullptr, nullptr, nullptr);
sqlite3_exec(db, "PRAGMA temp_store = MEMORY;", nullptr, nullptr, nullptr);
```

WAL mode alone can give a **3–5x speedup** for mixed read/write workloads.

---

### F5 — Replace Naive Stemmer with Porter Stemmer

The current 3-rule stemmer is too aggressive and incorrect on many words (see Bug #10). The **Porter Stemmer** is a well-tested, widely used algorithm available as a single `.cpp` file with no dependencies. It would immediately improve search accuracy.

---

### F6 — Add Ranked Pagination for Search Results

Currently the top-N results are printed and discarded. Adding pagination ("show next 10") would make the tool much more usable when a word appears in hundreds of files.

---

### F7 — Add a `--reindex-all` Flag

Allow users to force a full re-index of all files regardless of modification time — useful after stemmer upgrades or config changes that would affect all stored tokens.

---

### F8 — Multithreaded Crawling

The crawler is single-threaded. For large directories, parallelizing file reading with `std::async` / a thread pool would give significant speed gains. SQLite supports concurrent writes in WAL mode (see F4).

---

### F9 — Add `stats` Command

Add a command to display DB statistics at startup:
- Total files indexed
- Total unique words in index
- DB file size
- Last crawl timestamp

This helps users understand the state of their index without re-crawling.

---

### F10 — Better REPL-style Interface

Replace the current simple `Y/N` loop with a command-based REPL:

```
search-engine> search "error handling"
search-engine> crawl C:\Projects
search-engine> stats
search-engine> help
search-engine> exit
```

This is more user-friendly and makes it easy to do multiple operations in one session.

---

## Summary Table

| # | File | Severity | Type |
|---|------|----------|------|
| 1 | `searchword.cpp`, `searchwordbyfileid.cpp` | 🔴 Critical | SQL Injection |
| 2 | `Search/Single_Word/Search.cpp` | 🔴 Critical | Crash (UB on empty vector) |
| 3 | `Reindexing/FileChangeDetector.cpp` | 🔴 Critical | Wrong Comparison Logic |
| 4 | `config.h`, `Crawler.cpp`, `ReindexFile.cpp` | 🔴 Critical | Shared Mutable Global State |
| 5 | `Crawler.cpp` | 🔴 Critical | Null Pointer Dereference |
| 6 | `Reindexing/ReindexFile.cpp` | 🔴 Critical | Atomicity / Data Loss on Crash |
| 7 | `BulkInsert.cpp` | 🟠 Medium | Performance (Commented Transaction) |
| 8 | `searchword.cpp` | 🟠 Medium | Global Counter Not Reset Before Use |
| 9 | `tokenizer.cpp` | 🟠 Medium | Digits Stripped Incorrectly |
| 10 | `stemmer.cpp` | 🟠 Medium | Wrong Stems for Common Words |
| 11 | `Fuzzy_handeler.cpp` | 🟠 Medium | Potential NULL Dereference in stoi |
| 12 | `searchwordbyfileid.cpp` | 🟠 Medium | Variable Shadowing (auto bug) |
| 13 | `Chunked_Reading.cpp` | 🟠 Medium | Wrong Include Path |
| 14 | `and.cpp` | 🟠 Medium | Suboptimal Relevance Scoring (min vs sum) |
| 15 | Multiple | 🟡 Minor | Non-portable bits/stdc++.h |
| 16 | `Crawler.cpp` | 🟡 Minor | Duplicate Include |
| 17 | Multiple Headers | 🟡 Minor | `using namespace std` in Headers |
| 18–19 | `config.h` | 🟡 Minor | Duplicate Entries & Non-working Globs |
| 20 | `ReindexFile.cpp` | 🟡 Minor | Unused Parameter |
| 21 | `Search.cpp` | 🟡 Minor | Unnecessary File-scope Global |
| 22 | `Explorer/File_Reader.cpp` | 🟡 Minor | Empty File |
| 23 | `Tests/` | 🟡 Minor | No Tests |
| F1–F10 | — | 💡 Feature | Suggestions for New Features |

---

*End of Error Report*
