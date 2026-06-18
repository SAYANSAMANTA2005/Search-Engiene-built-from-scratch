
#include<sqlite3.h>
#include<iostream>  
#include<chrono>
#include<algorithm>

#include "../../tokenize/tokeniser.h"
#include "../../Database/Database.h"
#include "../../Database/SearchWord/searchword.h"
#include "../../config/config.h"
#include<string.h>
#include<cctype>
#include "Fuzzy_Trie/Fuzzy_Trie.h"

using namespace std;


/*

Directory Traversal
        │
        ▼
     Read File
        │
        ▼
     Tokenizer
        │
        ▼
┌───────────────────────┐
│      Normalisation     │
│           ↓            │
│    Stopword Filter     │--->in my case normalisation does all of thses 3 jobs
│           ↓            │
│        Stemmer         │
└───────────────────────┘
        │
        ▼
  Inverted Index
        │
        ▼
      SQLite
        │
        ▼
   Fuzzy Search Pipeline:

   User types misspelled word (e.g. "databse")
        │
        ▼
   Tokenize + Normalize input
        │
        ▼
   Load all unique words from DB into Trie
        │
        ▼
   Fuzzy search Trie (edit distance <= k)
        │
        ▼
   Find closest matching word (e.g. "databas")
        │
        ▼
   Run normal search_single_word(db, corrected_word)
        │
        ▼
   Return ranked results
 */
    


/*
   ─── Helper: Load all distinct words from inverted_index into a Trie ───
   
   This queries "SELECT DISTINCT word FROM inverted_index" 
   and inserts each word into the Trie for fuzzy matching.
*/
static int load_word_callback(void* data, int argc, char** argv, char** colNames) {
    Trie* trie = static_cast<Trie*>(data);
    if (argv[0] && argv[1]) {
        string word(argv[0]);
        int freq = std::stoi(argv[1]);
        //send into trie tokenized words only as trie only supports a-z
        if(tokenize(word).size()){
            word=tokenize(word)[0];
            trie->insert(word, freq);
        }

    }
    return 0;
}

static Trie* load_trie_from_db(sqlite3* db) {
    Trie* trie = new Trie();

    const char* sql = "SELECT word, SUM(frequency) FROM inverted_index GROUP BY word;";
    char* err = nullptr;

    auto start = chrono::steady_clock::now();

    int rc = sqlite3_exec(db, sql, load_word_callback, trie, &err);
    if (rc != SQLITE_OK) {
        cerr << "Error loading words into Trie: " << err << endl;
        sqlite3_free(err);
        delete trie;
        return nullptr;
    }

    auto elapsed = chrono::duration_cast<chrono::milliseconds>(
        chrono::steady_clock::now() - start
    );
    cout << "  Trie built from DB in " << elapsed.count() << " ms" << endl;

    return trie;
}


    
vector<pair<string,int>> fuzzy_search_handeler(sqlite3* db){

    // ── Step 1: Get user input ──
    cout<<"Search a word (fuzzy): ";
    string s;
    getline(cin,s);

    // Tokenize & normalize the input (lowercase, stem, etc.)
    vector<string> tokens = tokenize(s);
    if (tokens.empty()) {
        cout << "No valid word found after tokenization." << endl;
        return {};
    }
    s = tokens[0];
    cin.clear();

    cout << endl;
    cout << "  Searching (fuzzy) for: [ " << s << " ]" << endl;

    // ── Step 2: Build Trie from all indexed words in DB ──
    Trie* trie = load_trie_from_db(db);
    if (!trie) {
        cout << "  Failed to build Trie from database." << endl;
        return {};
    }

    // ── Step 3: Fuzzy search in Trie (edit distance k=2) ──
    //    k=1 catches single typos, k=2 catches double typos
    
    auto fuzzy_start = chrono::steady_clock::now();

    vector<pair<string,int>> fuzzy_matches = trie->fuzzy_search(s, MAX_EDIT_DISTANCE);

    auto fuzzy_elapsed = chrono::duration_cast<chrono::milliseconds>(
        chrono::steady_clock::now() - fuzzy_start
    );
    cout << "  Fuzzy search completed in " << fuzzy_elapsed.count() << " ms" << endl;

    // Done with the Trie
    delete trie;

    if (fuzzy_matches.empty()) {
        cout << "  No fuzzy matches found within edit distance " << MAX_EDIT_DISTANCE << "." << endl;
        return {};
    }

    // ── Step 4: Pick the best match ──
    //    Prefer exact match first, then shortest edit-distance match.
    //    Since fuzzy_search returns all within k, pick the closest one.
    //    We'll compute actual Levenshtein distance to rank them.

    // Simple Levenshtein distance lambda
    auto levenshtein = [](const string& a, const string& b) -> int {
        int n = a.size(), m = b.size();
        vector<vector<int>> dp(n + 1, vector<int>(m + 1));
        for (int i = 0; i <= n; i++) dp[i][0] = i;
        for (int j = 0; j <= m; j++) dp[0][j] = j;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                int cost = (a[i-1] == b[j-1]) ? 0 : 1;
                dp[i][j] = min({dp[i-1][j] + 1, dp[i][j-1] + 1, dp[i-1][j-1] + cost});
            }
        }
        return dp[n][m];
    };

    // Sort by edit distance, then frequency, then alphabetically
    sort(fuzzy_matches.begin(), fuzzy_matches.end(),
        [&](const pair<string,int>& a, const pair<string,int>& b) {
            int da = levenshtein(s, a.first);
            int db_dist = levenshtein(s, b.first);
            if (da != db_dist) return da < db_dist;
            if (a.second != b.second) return a.second > b.second;
            return a.first < b.first;
        }
    );

    // Show top fuzzy suggestions
    cout << endl;
    cout << "  Fuzzy matches found (" << fuzzy_matches.size() << " total):" << endl;
    int show_count = min((int)fuzzy_matches.size(), max_no_of_fuzzy_search_suggestions);
    for (int i = 0; i < show_count; i++) {
        cout << "    " << (i+1) << ". \"" << fuzzy_matches[i].first 
             << "\" (edit distance: " << levenshtein(s, fuzzy_matches[i].first) 
             << ", frequency: " << fuzzy_matches[i].second << ")" << endl;
    }
    if ((int)fuzzy_matches.size() > max_no_of_fuzzy_search_suggestions) {
        cout << "    ... and " << (fuzzy_matches.size() - max_no_of_fuzzy_search_suggestions) << " more" << endl;
    }

    // ── Step 5: Use the best match to do a normal DB search ──
    cout<<"Write Which  word u want to saerch ?"<<endl;
    std::string input;
    getline(cin, input);
    // If input is empty (e.g. if there was a stray newline), try reading again
    if (input.empty()) {
        getline(cin, input);
    }
    string best_match = input;

    cout << endl;
    cout << "  Using best match: [ " << best_match << " ]" << endl;
    cout << "  <<<<<<<<<< Fuzzy corrected: [ " << s << " ] --> [ " << best_match << " ] >>>>>>>>>>" << endl;

    // Run the normal single-word search with the corrected word
    vector<pair<string,int>> result = search_single_word(db, best_match);
    return result;
    

}
    
  