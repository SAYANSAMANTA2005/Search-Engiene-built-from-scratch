#include <iostream>
#include <string>
#include <vector>
#include "../Search/Fuzzy_Search/Fuzzy_Trie/Fuzzy_Trie.h"

using namespace std;

void test_trie() {
    cout << "\n--- Testing Fuzzy Trie ---\n";
    Trie trie;
    
    cout << "Inserting sample words into Trie: 'database', 'hello', 'world', 'index', 'test'\n";
    string words[] = {"database", "hello", "world", "index", "test"};
    for (const string& w : words) {
        string copy_w = w; // trie.insert takes non-const reference in this codebase
        trie.insert(copy_w, 1);
    }
    
    cout << "Enter a word to fuzzy search (or type 'exit' to stop):\n";
    string target;
    while (true) {
        cout << "Word > ";
        if (!(cin >> target)) break;
        if (target == "exit") break;
        
        cout << "Max edit distance (e.g. 1 or 2) > ";
        int k;
        if (!(cin >> k)) break;
        
        vector<pair<string, int>> results = trie.fuzzy_search(target, k);
        cout << "Results:\n";
        if (results.empty()) {
            cout << " - No matches found.\n";
        } else {
            for (const auto& p : results) {
                cout << " - " << p.first << " (freq: " << p.second << ")\n";
            }
        }
    }
}
