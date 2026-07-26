#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include "../Search/Multi_Word/And/and.h"
#include "check.h"

void test_intersection() {
    std::cout << "\n--- Testing Multi-Word Intersection (AND) ---\n";

    // (file_id, frequency, path) -- mirrors what search_single_word_give_result_by_file_id returns
    std::vector<std::tuple<int,int,std::string>> word1_results = {
        std::make_tuple(1, 5, "file1.txt"),
        std::make_tuple(2, 3, "file2.txt"),
        std::make_tuple(3, 1, "file3.txt")
    };
    std::vector<std::tuple<int,int,std::string>> word2_results = {
        std::make_tuple(2, 6, "file2.txt"),
        std::make_tuple(3, 2, "file3.txt"),
        std::make_tuple(4, 8, "file4.txt")
    };

    // Word 1 is in files {1,2,3}, word 2 is in files {2,3,4} -> intersection is {2,3}.
    // intersection_of_A_and_B() combines frequency via min(freqA, freqB):
    //   file2: min(3,6) = 3
    //   file3: min(1,2) = 1
    std::vector<std::vector<std::tuple<int,int,std::string>>> all_results = {word1_results, word2_results};
    std::vector<std::pair<std::string,int>> result = intersection(all_results);

    check_eq<size_t>(result.size(), 2, "intersection returns exactly 2 common files");
    if (result.size() == 2) {
        check_eq<std::string>(result[0].first, "file2.txt", "first result is file2.txt");
        check_eq<int>(result[0].second, 3, "file2.txt combined frequency is min(3,6)=3");
        check_eq<std::string>(result[1].first, "file3.txt", "second result is file3.txt");
        check_eq<int>(result[1].second, 1, "file3.txt combined frequency is min(1,2)=1");
    }

    // No overlap at all -> empty result
    std::vector<std::tuple<int,int,std::string>> word3_results = {
        std::make_tuple(99, 1, "file99.txt")
    };
    std::vector<std::vector<std::tuple<int,int,std::string>>> no_overlap = {word1_results, word3_results};
    check(intersection(no_overlap).empty(), "intersection of non-overlapping file sets is empty");
}
