
#include<sqlite3.h>
#include<iostream>  
#include "../../Database/Database.h"
#include "../../Database/SearchWord/searchword.h"
#include"../../config/config.h"
#include "../../tokenize/tokeniser.h"
#include<string.h>
#include<cctype>

using namespace std;

vector<pair<string,int>>Search_Single_WordResult;

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
 Single Word Search
 */
    


    
vector<pair<string,int>> Search_Single_Word_in_Database(sqlite3* db){



  // Use getline() instead of cin >>
    // cin >> stops at the first space and leaves '\n' in the input buffer.
   // getline() reads the complete line until Enter is press
    //searching a word
    cout<<"Search a word: ";
    string s;
    getline(cin,s);

    //here input s-> 1 word ,tokenizer --> filters the input & returns the word
    /*
    Tokenizer --> gives Vector of all Words In input string
    As S=string , hence i seletecd 0th element of output vector 

    */
    s= tokenize(s)[0];
    cin.clear();
    cout<<"<<<<<<<<<<--- Word :  [ "<<s<<" ] Occured in these file paths: --->>>>>>>>>>"<<endl;
 
    Search_Single_WordResult = search_single_word(db,s);
    return Search_Single_WordResult;
    

}
    
  