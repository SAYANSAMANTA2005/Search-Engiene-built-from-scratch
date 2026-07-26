TITLE-->High-Performance Local File Search Engine (C++17)

description -->
.Designed and implemented a from-scratch local search engine in C++17 with a SQLite-backed inverted index,
supporting single-word, multi-word (AND), and typo-tolerant fuzzy search via a 
custom Trie with Levenshtein-distance-bounded DFS pruning — not a brute-force or library-based edit-distance scan.

.Engineered memory-bounded file ingestion using chunked streaming reads and disable_recursion_pending()
during directory traversal to skip irrelevant subtrees (e.g. node_modules, .git) at the filesystem-API level,
avoiding wasted I/O on large trees rather than filtering after the fact.

.Built a cross-platform CMake + FetchContent build pipeline (Windows/Linux/macOS) with a GitHub Actions CI matrix
validating clean builds and a 30-case automated test suite on all three OSes.

.Used parameterized SQLite queries throughout with ON CONFLICT...DO UPDATE for atomic frequency 
accumulation, and diagnosed a live re-indexing logic bug by 
writing real assertion-based tests, replacing an earlier print-and-eyeball manual test process.


BEFORE 20/07/2026-- DESCRIPTION IN RESUME --> 

Local File Search Engine C++, SQLite, doctest
github.com/SAYANSAMANTA2005/Search-Engiene-built-from-scratch
• Built a full-text local file search engine from scratch in 
C++ — crawls directories using C++17 recursive directory iterator,
builds an inverted index in SQLite, and supports single-word, multi-word AND, and 
fuzzy search via Trie + Levenshtein distance (edit distance ≤ 2).

• Implemented incremental indexing using per-file modified-time comparison; on a 70 GB / 1M+ file corpus, reduced
repeat-traversal time from 45 minutes → 40 seconds (∼ 60× speedup) by skipping unchanged files entirely.

• Bounded memory usage via chunked tokenization — files are never fully loaded into RAM; content is tokenized,
stemmed, and flushed to SQLite in fixed-size windows using batched BEGIN/COMMIT transactions with INSERT
OR CONFLICT DO UPDATE for correct frequency accumulation.

• Added unit test suite (doctest, 23 test cases, 49 assertions) covering tokenizer, stemmer, 
and edit-distance; refactored Levenshtein distance out of an untestable local lambda into a standalone tested module,
and replaced stringconcatenated SQL with prepared statements to fix a SQL injection vulnerability
