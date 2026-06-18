#pragma once

#include <string>
#include <vector>
#include <utility>

class Trie {
private:
    bool iscomplete;
    int frequency;
    std::vector<Trie*> child;

    void fuzzy_search_helper(Trie* node, const std::string& target, int k, const std::vector<int>& prev_row, std::string& curr_word, std::vector<std::pair<std::string,int>>& results);

public:
    Trie();
    ~Trie();
    void insert(std::string &word, int freq);
    std::vector<std::pair<std::string,int>> fuzzy_search(const std::string& target, int k);
};
