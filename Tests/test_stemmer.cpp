#include <iostream>
#include <string>
#include "../tokenize/stemming/stemmer.h"

using namespace std;

void test_stemmer() {
    cout << "\n--- Testing Stemmer ---\n";
    cout << "Enter a word to stem (or type 'exit' to stop):\n";
    string word;
    while (true) {
        cout << "> ";
        if (!(cin >> word)) break;
        if (word == "exit") break;
        cout << "Stemmed result: " << stem(word) << "\n";
    }
}
