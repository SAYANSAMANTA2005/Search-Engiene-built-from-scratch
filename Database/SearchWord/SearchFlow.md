Crawler.cpp
    │
    ▼
Directory_Explorer()
    │
    │  Recursively traverses user-selected directories.
    │  Skips ignored folders (.git, node_modules, etc.).
    │  Finds valid text/code files for indexing.
    │
    ▼
GetOrCreateFileId()
    │
    │  Checks whether the file already exists in DB.
    │
    │  If present:
    │      returns existing file_id
    │
    │  Otherwise:
    │      creates a new row in files table
    │      generates a new file_id
    │
    ▼
DeleteFilePostings()
    │
    │  Removes all previous index entries
    │  associated with this file_id.
    │
    │  Prevents frequency inflation when
    │  the same file is indexed again.
    │
    ▼
Read_File2()
    │
    │  Reads file incrementally using a fixed-size buffer.
    │
    │  Avoids loading very large files
    │  completely into memory.
    │
    ▼
Tokenizer
    │
    │  Converts raw text into searchable tokens.
    │
    │  Example:
    │
    │  "Stocks are Rising!"
    │
    │      ↓
    │
    │  ["stock","rise"]
    │
    │  Includes:
    │      • Normalization
    │      • Stopword filtering
    │      • Stemming
    │
    ▼
Frequency Map
(unordered_map<string,int>)
    │
    │  Counts occurrences of each word
    │  inside the current file/chunk.
    │
    │  Example:
    │
    │  stock stock market stock
    │
    │      ↓
    │
    │  stock  → 3
    │  market → 1
    │
    │  When unique-word limit is reached,
    │  map is flushed to database.
    │
    ▼
BulkInsert()
    │
    │  Writes (word,file_id,frequency)
    │  entries into SQLite.
    │
    │  Uses:
    │
    │  ON CONFLICT(word,file_id)
    │
    │  to increment existing frequencies.
    │
    │  Example:
    │
    │  stock | 5 | 200
    │
    │  +
    │
    │  stock | 5 | 50
    │
    │  =
    │
    │  stock | 5 | 250
    │
    ▼
SQLite Database
(search_engine.db)
    │
    ├── files
    │
    │      file_id
    │      file_path
    │
    └── inverted_index
           word
           file_id
           frequency
    │
    │  Stores the searchable index.
    │
    ▼
search_word()
    │
    │  Accepts a user query.
    │
    │  Example:
    │
    │  stock
    │
    │  Executes SQL JOIN between:
    │
    │  files
    │      +
    │  inverted_index
    │
    ▼
Ranked Results
    │
    │  Results sorted by:
    │
    │  frequency DESC
    │
    │  Example:
    │
    │  finance.txt  (17 matches)
    │  market.txt   (6 matches)
    │  notes.txt    (2 matches)
    │
    ▼
Console Output