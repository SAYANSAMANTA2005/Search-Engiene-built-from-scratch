# Major Engineering Challenges Faced While Building a Local Search Engine

## 1. Efficiently Traversing Millions of Files

### Problem

A naive recursive scan of the filesystem can encounter:

* Millions of files
* Deep directory hierarchies
* Permission restricted folders
* Symbolic link cycles

### Challenge

How to ensure:

* No infinite traversal loops
* No crashes due to permission errors
* Efficient discovery of all indexable files

### Solution

Used:

```cpp
std::filesystem::recursive_directory_iterator
```

with filtering and error handling.

---

## 2. Memory-Efficient Processing of Large Files

### Problem

Some files can be extremely large:

* Log files
* XML dumps
* Minified JSON
* Generated source code

Reading the entire file into memory can cause:

* High RAM usage
* Performance degradation
* Out-of-memory failures

### Challenge

Process arbitrarily large files while keeping memory usage bounded.

### Solution

Implemented streaming-style processing:

```text
Read file
→ Process chunks/lines
→ Tokenize incrementally
→ Discard processed data
```

instead of loading entire files into memory.

---

## 3. Supporting Multiple File Types

### Problem

Not all files are text files.

Examples:

```text
.txt   → readable
.cpp   → readable
.json  → readable

.pdf   → special parser needed
.docx  → special parser needed
.jpg   → binary
.mp4   → binary
```

### Challenge

Determine which files can be indexed directly and which require specialized extraction pipelines.

### Solution

Created extension-based filtering and designed the system to support pluggable parsers later.

---

## 4. Designing the Inverted Index

### Problem

Searching every file during every query is:

```text
O(number_of_files)
```

which becomes prohibitively slow.

### Challenge

Design a structure that enables near-instant keyword lookup.

### Solution

Implemented an inverted index:

```text
word
  →
list of files containing word
```

Example:

```text
hello → [1,4,8]
world → [2,8]
```

Reducing query complexity dramatically.

---

## 5. Incremental Index Updates

### Problem

Rebuilding the entire index after every modification is expensive.

Example:

```text
1 million indexed files
1 file modified
```

Re-indexing everything is wasteful.

### Challenge

Detect only changed files and update affected index entries.

### Planned Solution

Track:

```text
path
size
last_modified_time
```

and selectively re-index changed files.

---

## 6. Maintaining Index Consistency

### Problem

Files may be:

* Deleted
* Renamed
* Moved
* Modified

after indexing.

### Challenge

Prevent stale entries from remaining in the search index.

### Solution

Designing metadata tracking and index cleanup mechanisms.

---

## 7. Scaling Search Performance

### Problem

As indexed documents grow:

```text
10K files
100K files
1M files
```

search latency increases.

### Challenge

Maintain sub-second query performance.

### Solution

Use:

* Inverted indexes
* Hash-based lookups
* Efficient posting lists
* Future ranking structures

---

## 8. Real-Time File Monitoring

### Problem

Filesystem contents continuously change.

### Challenge

Update the search index automatically without periodic full rescans.

### Planned Solution

Use Windows:

```cpp
ReadDirectoryChangesW
```

to receive filesystem change events and update the index in real time.

---

## 9. Persistent Storage of Index Data

### Problem

Keeping the entire index in RAM limits scalability.

### Challenge

Persist:

* File metadata
* Terms
* Posting lists

while maintaining fast retrieval.

### Planned Solution

SQLite-backed storage layer.

---

## 10. Search Relevance

### Problem

Keyword matching alone produces poor results.

Example:

```text
query: search engine
```

Many documents match.

### Challenge

Return the most relevant documents first.

### Planned Solution

TF-IDF based ranking.
