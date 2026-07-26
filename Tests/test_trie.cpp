#include <iostream>
#include <string>
#include <vector>
#include "../Search/Fuzzy_Search/Fuzzy_Trie/Fuzzy_Trie.h"
#include "check.h"

static bool contains_word(const std::vector<std::pair<std::string,int>>& results, const std::string& word) {
    for (const auto& p : results) {
        if (p.first == word) return true;
    }
    return false;
}

void test_trie() {
    std::cout << "\n--- Testing Fuzzy Trie ---\n";

    Trie trie;
    std::string words[] = {"database", "hello", "world", "index", "test"};
    for (const std::string& w : words) {
        std::string copy_w = w; // trie.insert takes a non-const reference in this codebase
        trie.insert(copy_w, 1);
    }

    // Exact match, 0 edit distance
    auto exact = trie.fuzzy_search("hello", 0);
    check(contains_word(exact, "hello"), "fuzzy_search('hello', 0) finds exact match 'hello'");

    // One-character typo, edit distance 1
    auto typo = trie.fuzzy_search("helo", 1);
    check(contains_word(typo, "hello"), "fuzzy_search('helo', 1) finds 'hello' within edit distance 1");

    // Something with no close match in the trie should return nothing
    auto none = trie.fuzzy_search("zzzzzzzzzz", 1);
    check(none.empty(), "fuzzy_search('zzzzzzzzzz', 1) returns no matches");
}
