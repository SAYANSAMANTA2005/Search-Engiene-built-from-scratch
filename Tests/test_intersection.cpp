#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include "../Search/Multi_Word/And/and.h"

using namespace std;

void test_intersection() {
    cout << "\n--- Testing Multi-Word Intersection (AND) ---\n";
    
    // Create some dummy data mimicking what we get from the database
    vector<tuple<int, int, string>> word1_results = {
        make_tuple(1, 5, "file1.txt"),
        make_tuple(2, 3, "file2.txt"),
        make_tuple(3, 1, "file3.txt")
    };
    
    vector<tuple<int, int, string>> word2_results = {
        make_tuple(2, 6, "file2.txt"),
        make_tuple(3, 2, "file3.txt"),
        make_tuple(4, 8, "file4.txt")
    };
    
    cout << "Word 1 is present in: file1, file2, file3\n";
    cout << "Word 2 is present in: file2, file3, file4\n";
    cout << "Calculating intersection (files containing BOTH words)...\n";
    
    vector<vector<tuple<int, int, string>>> all_results = {word1_results, word2_results};
    
    vector<pair<string, int>> final_result = intersection(all_results);
    
    cout << "\nResulting Intersection:\n";
    if (final_result.empty()) {
        cout << " - No common files found.\n";
    } else {
        for (const auto& p : final_result) {
            cout << " - " << p.first << " (score: " << p.second << ")\n";
        }
    }
}
