TITLE-->High-Performance Local File Search Engine (C++17)

description -->
.Designed and implemented a from-scratch local search engine in C++17 with a SQLite-backed inverted index, supporting single-word, multi-word (AND), and typo-tolerant fuzzy search via a custom Trie with Levenshtein-distance-bounded DFS pruning — not a brute-force or library-based edit-distance scan.
.Engineered memory-bounded file ingestion using chunked streaming reads and disable_recursion_pending() during directory traversal to skip irrelevant subtrees (e.g. node_modules, .git) at the filesystem-API level, avoiding wasted I/O on large trees rather than filtering after the fact.
.Built a cross-platform CMake + FetchContent build pipeline (Windows/Linux/macOS) with a GitHub Actions CI matrix validating clean builds and a 30-case automated test suite on all three OSes.
.Used parameterized SQLite queries throughout with ON CONFLICT...DO UPDATE for atomic frequency accumulation, and diagnosed a live re-indexing logic bug by writing real assertion-based tests, replacing an earlier print-and-eyeball manual test process.
