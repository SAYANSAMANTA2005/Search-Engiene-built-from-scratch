#include <iostream>
#include <string>
#include <vector>
#include "../tokenize/tokeniser.h"

using namespace std;

void test_tokenizer() {
    cout << "\n--- Testing Tokenizer ---\n";
    cout << "Enter a sentence to tokenize (or type 'exit' to stop):\n";
    string line;
    
    // Clear newline character from any previous cin operations
    cin.ignore(10000, '\n'); 
    
    while (true) {
        cout << "> ";
        if (!getline(cin, line)) break;
        if (line == "exit") break;
        
        vector<string> tokens = tokenize(line);
        cout << "Tokens: [ ";
        for (const string& t : tokens) {
            cout << "'" << t << "' ";
        }
        cout << "]\n";
    }
}
