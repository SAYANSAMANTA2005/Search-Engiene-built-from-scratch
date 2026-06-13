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
