# Fuzzy Search Enhancements Summary

This document outlines the recent bug fixes and enhancements made to the Fuzzy Search pipeline of the Search Engine.

## 1. Input Buffer Bug Fix (`std::cin` issue)
**Problem:** After entering a fuzzy search query, the main application loop would abruptly skip the "Do you want to search again?" prompt and exit.
**Cause:** `cin >> input` successfully read the searched word but left a trailing newline (`\n`) character in the input buffer. The subsequent `getline(cin, want)` in `Crawler.cpp` instantly consumed the newline, read an empty string, and gracefully exited the loop.
**Fix:** Replaced `cin >> input` with `getline(cin, input)` and added a check to ignore empty strings from stray newlines, ensuring the prompt works properly.

## 2. Trie Index Out-of-Bounds Crash Fix
**Problem:** The Trie crashed or printed nothing when loading certain words from the database.
**Cause:** The Trie array is strictly sized to 26 children (for lowercase `a-z`). However, `load_word_callback` pulled raw words from the database that could contain digits or symbols.
**Fix:** Processed words using the existing `tokenize()` function before Trie insertion. Words are verified to only be inserted if they map nicely to `a-z`, avoiding the negative array index crash.

## 3. Returning Frequencies from the Trie
**Feature:** The user requested that fuzzy search suggestions be ranked taking the word frequency into consideration.
**Implementation:**
- **`Fuzzy_Trie.h` & `Fuzzy_Trie.cpp`**: 
  - Modified the signature of `insert(string &word, int freq)` to store word frequency at the leaf nodes. Since multiple derivations of a word can stem to the same root, frequencies are accumulated (`root->frequency += freq`).
  - Modified `fuzzy_search` and `fuzzy_search_helper` to return a `vector<pair<string, int>>` rather than just strings.
- **`Fuzzy_handeler.cpp`**:
  - Altered the SQLite SQL query to group and aggregate global frequencies: `SELECT word, SUM(frequency) FROM inverted_index GROUP BY word;`
  - The callback function now extracts the frequency (`stoi(argv[1])`) and passes it into the Trie insertion.

## 4. Enhanced Ranking Logic
With the new frequency data, the Levenshtein sorting logic in `fuzzy_search_handeler` was upgraded:
1. **Edit Distance:** Prioritizes exact matches or closer fuzzy matches (e.g., Edit Distance 0 beats Edit Distance 1).
2. **Frequency:** On an edit distance tie, it heavily prioritizes words with the higher global frequency in the database.
3. **Alphabetical:** On a frequency tie, it falls back to lexicographical order.

## 5. Wiring and Build Errors
- Ensured `#include <algorithm>` was present across files (`Multi_Search.cpp`, `Fuzzy_handeler.cpp`) to satisfy standard library requirements for `std::sort` and `std::min`.
- Cleaned up broken experimental code appended to the end of `Fuzzy_Trie.cpp` (`Fuzzy_Search_Query`) which was causing compilation failures.
- Ensured `run.bat` correctly references all `.cpp` objects, including `Search/Fuzzy_Search/Fuzzy_handeler.cpp` and the correctly pathed `Search/Fuzzy_Search/Fuzzy_Trie/Fuzzy_Trie.cpp`.
