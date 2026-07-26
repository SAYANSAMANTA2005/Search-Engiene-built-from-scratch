#include <iostream>
#include "check.h"

// Forward declarations for our test functions


void test_stemmer();
void test_tokenizer();
void test_trie();
void test_intersection();
void test_database();
void test_file_change_detector();

int main() {
    std::cout << "========================================\n";
    std::cout << "       Search Engine Automated Tests\n";
    std::cout << "========================================\n";

    // Every test function below runs unconditionally, every time --
    // no menu, no manual input required. This is what lets this binary
    // be dropped straight into a CI job (GitHub Actions, a pre-commit
    // hook, etc.) with no human watching the output.
    test_stemmer();
    test_tokenizer();
    test_trie();
    test_intersection();
    test_database();
    test_file_change_detector();

    std::cout << "\n========================================\n";
    if (g_failures == 0) {
        std::cout << "ALL " << g_checks << " CHECKS PASSED\n";
    } else {
        std::cout << g_failures << " OF " << g_checks << " CHECKS FAILED\n";
    }
    std::cout << "========================================\n";

    // This exit code is the whole point: 0 means every check passed,
    // nonzero means something is broken. A script or CI job can act on
    // this without a human ever reading the printed output.
    return g_failures == 0 ? 0 : 1;
}
