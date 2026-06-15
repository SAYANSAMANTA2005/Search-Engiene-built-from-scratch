Tokenizing evry sentence ,inseted of Tokenizing whole file


// Process one line at a time.
// This significantly reduces memory usage because
// the whole file is never loaded into RAM.
// For large files it is usually more scalable and
// may also improve performance.


# Traversal Optimization

## Directory Skipping Strategy

Certain directories contain generated code, dependencies, caches, or build artifacts that are not useful for search indexing.

Examples:

```text
node_modules
.git
venv
__pycache__
build
dist
```

Instead of traversing these directories and then ignoring their files, the crawler uses:

```cpp
it.disable_recursion_pending();
```

to prevent entering the directory subtree entirely.

### Benefits

* Faster traversal
* Reduced disk I/O
* Lower CPU usage
* Better scalability on large projects

---

## File Skipping Strategy

Only text-based files are useful for tokenization and indexing.

Indexed extensions:

```text
.txt
.md
.cpp
..
...
```

### Benefits

* Avoids processing binary files
* Prevents invalid tokenization
* Reduces indexing time
* Saves memory and storage

---

## Result

The crawler focuses only on:

1. Relevant directories
2. Indexable text files

This significantly improves indexing performance while reducing unnecessary filesystem operations.

-----------------------------------------------------------------------------------------------------

# Chunked Indexing Approach in Search Engine

## Overview

While indexing a file, storing the frequency of every unique word in memory can become expensive for large files. To prevent excessive memory usage, the indexing pipeline uses a **chunked processing strategy**.

The approach combines:

1. Chunked file reading.
2. Chunked tokenization.
3. Chunked frequency aggregation.
4. Batched SQLite updates.

---

# Workflow

## Step 1: Read File Incrementally

Instead of loading the entire file into memory:

```cpp
while(file >> word)
{
    buffer += word + " ";
}
```

words are appended to a temporary buffer.

---

## Step 2: Tokenize Fixed-Size Chunks

When the buffer reaches a predefined size:

```cpp
if(buffer.size() >= LIMIT)
```

the buffer is tokenized:

```cpp
vector<string> words = tokenize(buffer);
```

After processing:

```cpp
buffer.clear();
```

This ensures memory consumption remains bounded regardless of file size.

---

## Step 3: Build Local Frequency Map

For every token:

```cpp
freq[word]++;
```

The map:

```cpp
unordered_map<string,int> freq;
```

stores:

```text
word -> frequency within current chunk
```

Example:

```text
database -> 15
sqlite   -> 9
engine   -> 4
```

---

## Step 4: Flush Large Frequency Maps

When the number of unique words reaches a threshold:

```cpp
if(freq.size() == MAX_UNIQUE_WORDS_PER_FILE)
```

the map is flushed:

```cpp
BulkInsert(db,file_id,freq);
freq.clear();
```

This prevents the frequency map from growing indefinitely.

---

## Step 5: Bulk Insert into SQLite

Current implementation:

```cpp
BEGIN TRANSACTION;

for(each word)
{
    INSERT ...
}

COMMIT;
```

SQLite performs batched updates instead of one transaction per word.

---

# Database Update Strategy

The SQL command:

```sql
INSERT INTO inverted_index
(word,file_id,frequency)
VALUES(?,?,?)
ON CONFLICT(word,file_id)
DO UPDATE SET
frequency = frequency + excluded.frequency;
```

ensures that:

```text
(database, file1) -> 10
```

followed later by:

```text
(database, file1) -> 7
```

becomes:

```text
(database, file1) -> 17
```

instead of creating duplicate rows.

---

# Advantages

## 1. Bounded Memory Usage

Without chunking:

```text
Large File
      ↓
Huge unordered_map
      ↓
Potential memory explosion
```

With chunking:

```text
Large File
      ↓
Many small maps
      ↓
Stable memory usage
```

Memory consumption remains approximately bounded by:

```text
O(MAX_UNIQUE_WORDS_PER_FILE)
```

instead of:

```text
O(total unique words in file)
```

---

## 2. Supports Very Large Files

Files several gigabytes in size can be processed without loading them entirely into RAM.

---

## 3. Better Scalability

Indexing performance remains predictable even when file sizes vary significantly.

---

## 4. Reduced Risk of Allocation Failures

Very large frequency maps can trigger:

```text
Excessive allocations
Heap fragmentation
Memory pressure
```

Chunking minimizes these risks.

---

## 5. Efficient Word Aggregation

Word frequencies are aggregated in memory before hitting SQLite.

Instead of:

```text
database -> write
database -> write
database -> write
```

the system performs:

```text
database -> frequency = 100
```

followed by a single database update.

---

# Disadvantages

## 1. More Transactions

Current implementation executes:

```cpp
BEGIN TRANSACTION;
...
COMMIT;
```

inside every flush.

If a file generates:

```text
10 flushes
```

then:

```text
10 transactions
```

are executed.

SQLite generally prefers fewer transactions.

---

## 2. Repeated Updates for the Same Word

Example:

```text
Chunk 1:
database -> 10

Chunk 2:
database -> 7
```

Results in:

```text
INSERT/UPDATE
INSERT/UPDATE
```

for the same word.

A single final flush would perform only one update.

---

## 3. Increased Journal Activity

Each transaction may create journal or WAL records.

More transactions imply:

```text
More COMMIT operations
More disk synchronization
```

which can reduce indexing speed.

---

# Complexity Analysis

Let:

```text
T = total tokens
U = unique words
M = MAX_UNIQUE_WORDS_PER_FILE
```

Tokenization:

```text
O(T)
```

Frequency counting:

```text
O(T)
```

Hash map operations:

```text
Average O(1)
```

Flush operations:

```text
O(U / M)
```

Database insertion:

```text
O(U log N)
```

approximately, where:

```text
N = postings in SQLite index
```

---

# Recommended Improvements

## Improvement 1: Move Transaction Management Outside BulkInsert

Current:

```text
Flush
 ├─ BEGIN
 ├─ INSERT
 └─ COMMIT
```

Proposed:

```text
BEGIN

Flush
Flush
Flush
Flush

COMMIT
```

Advantages:

```text
Fewer commits
Less journaling
Faster indexing
```

Potential speedup can be substantial.

---

## Improvement 2: Commit Per Batch of Files

Instead of:

```text
1 file -> 1 transaction
```

use:

```text
1000 files -> 1 transaction
```

Benefits:

```text
Reduced commit overhead
Improved throughput
```

while maintaining manageable rollback size.

---

## Improvement 3: Enable WAL Mode

```sql
PRAGMA journal_mode=WAL;
```

Benefits:

```text
Better write throughput
Improved concurrency
Reduced journal overhead
```

---

## Improvement 4: Cache Document Statistics

Store:

```sql
documents(
    file_id,
    total_terms
)
```

during indexing.

This prepares the system for:

```text
TF-IDF
BM25
```

ranking algorithms.

---

## Improvement 5: Future Posting List Architecture

Current architecture:

```text
Word
 ↓
SQLite Row
```

Future architecture:

```text
Dictionary
 ↓
Posting List
 ↓
Compressed Storage
```

Benefits:

```text
Smaller index
Faster intersections
Better ranking performance
```

for large-scale search systems.

---

# Conclusion

The chunked indexing strategy primarily optimizes:

```text
Memory usage
Scalability
Large-file support
```

rather than raw SQLite insertion speed.

The current design is a strong engineering tradeoff because it guarantees bounded memory consumption while maintaining correctness through incremental database updates.

The next major optimization should be reducing transaction frequency rather than increasing chunk size.
