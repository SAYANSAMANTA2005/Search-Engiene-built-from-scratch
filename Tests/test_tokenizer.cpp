#include <iostream>
#include <string>
#include <vector>
#include "../tokenize/tokeniser.h"
#include "check.h"

void test_tokenizer() {
    std::cout << "\n--- Testing Tokenizer ---\n";

    // Stopwords ("the", "and") should be removed. "cat"/"dog" have no
    // suffix the stemmer strips, so they pass through unchanged.
    std::vector<std::string> t1 = tokenize("the cat and the dog");
    check_eq<size_t>(t1.size(), 2, "tokenize('the cat and the dog') has 2 tokens (stopwords removed)");
    if (t1.size() == 2) {
        check_eq<std::string>(t1[0], "cat", "first token is 'cat'");
        check_eq<std::string>(t1[1], "dog", "second token is 'dog'");
    }

    // Punctuation is stripped, everything lowercased.
    std::vector<std::string> t2 = tokenize("Hello, World!");
    check_eq<size_t>(t2.size(), 2, "tokenize('Hello, World!') has 2 tokens");
    if (t2.size() == 2) {
        check_eq<std::string>(t2[0], "hello", "punctuation stripped + lowercased -> 'hello'");
        check_eq<std::string>(t2[1], "world", "punctuation stripped + lowercased -> 'world'");
    }

    // Edge cases
    check_eq<size_t>(tokenize("").size(), 0, "tokenize('') returns no tokens");
    check_eq<size_t>(tokenize("!!! ,,, ...").size(), 0, "tokenize of only punctuation returns no tokens");
}
