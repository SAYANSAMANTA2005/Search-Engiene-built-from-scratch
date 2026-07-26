Where it'll get exposed under interview pressure
An interviewer who actually probes will find these soft spots:

"Why C++ over Python?" — your own Plan.md has a slightly defensive answer to this ("doesn't require C++ for performance, but..."). That's honest, but be ready for a follow-up like "show me where it would actually bottleneck in Python" — if you can't answer that crisply it looks rehearsed.
"What happens if a file is deleted from disk?" — currently nothing; stale rows stay forever. This is a textbook "tell me about a limitation" question and you should have a ready answer (you already do, in problems.md — that's good, just be ready to say it out loud).
"How do you rank results?" — pure term frequency. Any interviewer who's seen TF-IDF will ask why you didn't normalize for document length or term rarity. Right now your honest answer is "I haven't gotten there yet," which is fine once, but you should know why TF-IDF matters, not just that it's on your roadmap.
"Walk me through your stemmer" — it's a 15-line suffix stripper, not real stemming. If asked "does this stemmer handle irregular words?" the honest answer is no, and that's a fair thing to be asked to defend.
"Why is file_id incrementing by 2?" — this is in your own problems.md as an unsolved bug. If asked to debug live, this is a good one to actually fix before interviews, since "I have an unexplained off-by-N bug in production" is not a great thing to volunteer.
No tests. Fresher interviewers increasingly ask "how did you verify correctness?" — right now your evidence is manual runs and println timing, not unit tests.


# Directory Traversal Optimization
**Crawler.cpp**

## Problem

The search engine needs to recursively traverse the filesystem and discover files that should be indexed.

Initially, traversal was implemented using:

```cpp
for(auto &entry :
    recursive_directory_iterator(root))
{
    ...
}
```

This successfully visits every file and directory under the root path.

However, modern software projects often contain very large generated directories such as:

```text
node_modules
.git
venv
__pycache__
build
dist
```

These directories can contain:

* Tens of thousands of files
* Third-party dependencies
* Generated artifacts
* Cache files

which are typically not useful for search indexing.

---

## Initial Approach

A set of ignored directories was created:

```cpp
unordered_set<string> SKIP_DIRECTORIES;
```

and paths were filtered using:

```cpp
if(SKIP_DIRECTORIES.count(dirname))
{
    continue;
}
```

This prevented indexing files inside those directories.

### Limitation

Although files were ignored, the filesystem iterator still entered the directory and visited every file within it.

Example:

```text
Project/
│
├── src/
│   └── main.cpp
│
└── node_modules/
    ├── file1.js
    ├── file2.js
    ├── ...
    └── 50000 more files
```

The traversal still touched all 50,000 files before skipping them.

Result:

* Unnecessary filesystem operations
* Slower indexing
* Increased disk access

---

## Optimized Solution

Traversal was changed from:

```cpp
for(auto &entry :
    recursive_directory_iterator(root))
```

to:

```cpp
for(
    auto it =
        recursive_directory_iterator(root);
    it != recursive_directory_iterator();
    ++it
)
```

This provides direct access to the iterator object.

When a directory is encountered:

```cpp
if(SKIP_DIRECTORIES.count(dirname))
{
    it.disable_recursion_pending();
}
```

the iterator is instructed not to descend into that directory.

---

## Why This Works

The filesystem library separates two concepts:

### directory_entry

Represents the current file or directory.

It can provide:

* Path
* Extension
* File size
* Metadata

but it cannot control traversal.

---

### recursive_directory_iterator

Represents the traversal engine itself.

It controls:

* Current traversal depth
* Which directory will be visited next
* Whether recursion should continue

The method:

```cpp
it.disable_recursion_pending();
```

belongs to the iterator because only the iterator controls traversal.

---

## Example

Filesystem:

```text
Project/
│
├── src/
│   └── main.cpp
│
└── node_modules/
    ├── a.js
    ├── b.js
    └── thousands of files
```

Without optimization:

```text
Visit node_modules
Visit a.js
Visit b.js
Visit thousands more files
```

With optimization:

```text
Visit node_modules
Disable recursion
Skip entire subtree
```

Only:

```text
src/main.cpp
```

is processed.

---

## Benefits

### Faster Indexing

Large dependency folders are never traversed.

### Reduced Disk I/O

The filesystem is not queried for every file in ignored directories.

### Better Scalability

Projects with:

```text
100,000+
1,000,000+
```

files can be handled more efficiently.

### Cleaner Search Results

Only user-relevant files are indexed.

---

## Interview Discussion

A significant performance issue was discovered while crawling large developer projects.

Directories such as:

```text
node_modules
.git
venv
build
```

contained tens of thousands of irrelevant files.

Initially these files were filtered after traversal, but this still incurred filesystem traversal costs.

The solution was to switch from range-based iteration to direct iterator access and use:

```cpp
disable_recursion_pending()
```

to prevent the iterator from entering unwanted directory trees entirely.

This reduced unnecessary filesystem operations and significantly improved indexing performance on large projects.
