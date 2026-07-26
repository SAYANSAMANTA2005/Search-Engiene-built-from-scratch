

Vocabulary Table
        ↓
Load Trie
        ↓
User types "databse"
        ↓
Trie + Edit Distance DFS
        ↓
database
        ↓
Run normal inverted-index search
        ↓
Show results










# Complete Search Engine Workflow

## From Crawling → Indexing → Ranking → Phrase Search → Fuzzy Search

---

# Phase 1: File Discovery

Goal:

```text
Find every user file that should be indexed.
```

Pipeline:

```text
User Selected Directories
          ↓
Directory Explorer
          ↓
Recursive Traversal
          ↓
Skip System Folders
(node_modules, .git, etc.)
          ↓
Valid File Extensions Check
          ↓
Candidate Files
```

Example:

```text
C:/Notes/db.txt
C:/Books/sqlite.pdf
C:/Projects/main.cpp
```

---

# Phase 2: Incremental Indexing

Goal:

```text
Avoid re-reading unchanged files.
```

For every file:

```text
Path
Size
Modified Time
```

Check:

```text
Already Indexed?
       ↓
      Yes
       ↓
Modified?
       ↓
   No → Skip
       ↓
   Yes
       ↓
Reindex
```

Database:

```sql
files(
    file_id,
    path,
    size,
    modified_time
)
```

---

# Phase 3: File Reading

Goal:

```text
Extract text.
```

Pipeline:

```text
File
 ↓
Reader
 ↓
Chunk Reader
 ↓
Text Stream
```

Supported:

```text
txt
cpp
py
java
pdf
docx
pptx
...
```

---

# Phase 4: Tokenization

Input:

```text
SQLite is a database engine.
```

Output:

```text
sqlite
is
a
database
engine
```

---

# Phase 5: Normalization

Convert:

```text
SQLite
SQLITE
Sqlite
```

to:

```text
sqlite
```

Pipeline:

```text
Token
 ↓
tolower()
 ↓
Normalized Token
```

---

# Phase 6: Stemming

Convert:

```text
running
runs
runner
```

into:

```text
run
```

Pipeline:

```text
Word
 ↓
Stemmer
 ↓
Root Form
```

---

# Phase 7: Frequency Counting

For one file:

```text
database
database
sqlite
```

Create:

```text
database → 2
sqlite → 1
```

Store temporarily:

```cpp
unordered_map<string,int>
```

---

# Phase 8: Inverted Index Construction

Store:

```sql
inverted_index(
    word,
    file_id,
    frequency
)
```

Example:

```text
database → (1,5)
database → (7,3)

sqlite → (1,2)
sqlite → (9,4)
```

Meaning:

```text
(word, file_id, frequency)
```

---

# Phase 9: Vocabulary Construction

Store every unique word.

```sql
vocabulary(
    word,
    document_frequency
)
```

Example:

```text
database → 150
sqlite → 80
engine → 400
```

Meaning:

```text
word appears in X files
```

Used for:

```text
TF-IDF
Autocomplete
Fuzzy Search
```

---

# Phase 10: Trie Construction

Startup:

```text
Vocabulary Table
        ↓
Load Vocabulary
        ↓
Build Trie
```

Trie contains:

```text
database
data
dataset
sqlite
search
engine
```

Used for:

```text
Autocomplete
Fuzzy Search
```

---

# SEARCH PIPELINE

====================================

# Exact Single Word Search

User:

```text
database
```

Pipeline:

```text
database
     ↓
Lookup Posting List
     ↓
(file_id, frequency)
     ↓
TF-IDF Ranking
     ↓
Sort Descending
     ↓
Show Results
```

---

# Multi Word Search

User:

```text
database sqlite
```

Pipeline:

```text
database
     ↓
Posting List A

sqlite
     ↓
Posting List B

A ∩ B
     ↓
Common Files
     ↓
TF-IDF Ranking
     ↓
Show Results
```

Current Complexity:

```text
O(f1 + f2 + ... + fk)
```

---

# Phrase Search

User:

```text
"database engine"
```

Pipeline:

```text
database
      ↓
Posting List

engine
      ↓
Posting List

Intersection
      ↓
Candidate Files
      ↓
Open Candidate Files
      ↓
Verify Exact Phrase
      ↓
Return Matches
```

No positional index needed.

---

# Boolean Search

User:

```text
database AND sqlite
```

Pipeline:

```text
Intersection
```

User:

```text
database OR sqlite
```

Pipeline:

```text
Union
```

User:

```text
database NOT mysql
```

Pipeline:

```text
Difference
```

---

# TF-IDF Ranking

For every matching document:

```text
score =
Σ(tf × idf)
```

Compute:

```text
tf = word frequency in file

idf = log(
        total_files
        /
        files_containing_word
      )
```

Store:

```cpp
unordered_map<int,double>
```

Sort descending.

---

# Fuzzy Search

User:

```text
databse
```

Pipeline:

```text
No Exact Match
        ↓
Trie + Edit Distance DFS
        ↓
database
        ↓
Normal Search(database)
        ↓
Show Results
```

Example:

```text
databse
```

↓

```text
database
```

---

# Autocomplete

User types:

```text
dat
```

Pipeline:

```text
Trie
 ↓
Prefix Search
 ↓
Suggestions
```

Output:

```text
data
database
dataset
```

---

# Search Result Display

Final Result:

```text
database sqlite
```

↓

```text
1. db_notes.txt
   Score: 145

2. sqlite_guide.pdf
   Score: 120

3. search_engine.cpp
   Score: 98
```

Optionally:

```text
Snippet:
"...SQLite uses an inverted index..."
```

---

# Future Optimizations

Current:

```text
Posting Lists
     ↓
Two Pointer Intersection
```

Future:

```text
Posting Lists
     ↓
Sort By Size
     ↓
Skip Pointers
     ↓
Faster Intersection
```

---

# Future Real-Time Search

File Changed
↓
File Watcher
(ReadDirectoryChangesW)
↓
Reindex File
↓
Update SQLite
↓
Update Trie

````

Result:

```text
Always Up-To-Date Search Index
````

---

# Final Architecture

```text
File System
     ↓
Crawler
     ↓
Incremental Indexing
     ↓
Tokenizer
     ↓
Stemmer
     ↓
Frequency Counter
     ↓
SQLite Inverted Index
     ↓
Vocabulary Table
     ↓
Trie

================================

Single Word Search
Multi Word Search
Phrase Search
Boolean Search
TF-IDF Ranking
Autocomplete
Fuzzy Search

================================

GUI
     ↓
User Search Experience
```
