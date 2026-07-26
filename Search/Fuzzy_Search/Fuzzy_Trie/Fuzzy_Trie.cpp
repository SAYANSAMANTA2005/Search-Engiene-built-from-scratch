#include "Fuzzy_Trie.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

Trie::Trie() : iscomplete(false), frequency(0) {
    child.resize(26, nullptr);
}

Trie::~Trie() {
    for (Trie* c : child) {
        if (c != nullptr) {
            delete c;
        }
    }
}

void Trie::fuzzy_search_helper(Trie* node, const string& target, int k, const vector<int>& prev_row, string& curr_word, vector<pair<string,int>>& results) {
    if (node->iscomplete) {
        if (prev_row.back() <= k) {
            results.push_back({curr_word, node->frequency});
        }
    }

    int m = target.size();
    for (int i = 0; i < 26; i++) {
        if (node->child[i] != nullptr) {
            char ch = 'a' + i;
            vector<int> next_row(m + 1);
            next_row[0] = prev_row[0] + 1;
            int min_val = next_row[0];

            for (int j = 1; j <= m; j++) {
                int cost = (target[j - 1] == ch) ? 0 : 1;
                next_row[j] = min({
                    next_row[j - 1] + 1,        // Insertion
                    prev_row[j] + 1,            // Deletion
                    prev_row[j - 1] + cost      // Substitution
                });
                min_val = min(min_val, next_row[j]);
            }

            if (min_val <= k) {
                curr_word.push_back(ch);
                fuzzy_search_helper(node->child[i], target, k, next_row, curr_word, results);
                curr_word.pop_back();
            }
        }
    }
}

void Trie::insert(string &word, int freq) {
    Trie *root = this;
    
    for(int i = 0; i < word.size(); i++) {
        int index = word[i] - 'a';
        if(root->child[index] == nullptr) {
             root->child[index] = new Trie();
        }
        root = root->child[index];
    }
    root->iscomplete = true;
    root->frequency += freq; // Accumulate frequency in case stems match
}

vector<pair<string,int>> Trie::fuzzy_search(const string& target, int k) {
    vector<pair<string,int>> results;
    int m = target.size();
    vector<int> current_row(m + 1);
    for (int i = 0; i <= m; i++) {
        current_row[i] = i;
    }
    string curr_word = "";
    fuzzy_search_helper(this, target, k, current_row, curr_word, results);
    return results;
}