#include <iostream>

using namespace std;

// Forward declarations for our test functions
void test_stemmer();
void test_tokenizer();
void test_trie();
void test_intersection();
void test_database();
void test_file_change_detector();

int main() {
    int choice;
    while (true) {
        cout << "\n========================================\n";
        cout << "       Search Engine Test Menu\n";
        cout << "========================================\n";
        cout << "1. Test Stemmer\n";
        cout << "2. Test Tokenizer\n";
        cout << "3. Test Fuzzy Trie\n";
        cout << "4. Test Multi-word Intersection (AND)\n";
        cout << "5. Test Database Insert/Delete\n";
        cout << "6. Test File Change Detector\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        
        if (!(cin >> choice)) {
            break; // Handle EOF or invalid input
        }

        switch (choice) {
            case 1: test_stemmer(); break;
            case 2: test_tokenizer(); break;
            case 3: test_trie(); break;
            case 4: test_intersection(); break;
            case 5: test_database(); break;
            case 6: test_file_change_detector(); break;
            case 0: return 0;
            default: cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}
