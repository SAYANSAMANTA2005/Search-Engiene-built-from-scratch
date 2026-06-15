
 
#include "../../Database/Database.h"
#include "../../Database/SearchWord/searchword.h"
#include"../../config/config.h"
#include "../../tokenize/tokeniser.h"
#include  "../Single_Word/Search.h"

#include<sqlite3.h>
#include<iostream> 
#include<string.h>
#include<cctype>
#include<vector>

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
 Single Word Search
 */
    

// normalise_string  expects only first word of user search

// it returns stemmed & filtered word

void printString(std::string &s){
   // padding front & end of string 
   cout<<" ";
   for(auto &it:s){
      cout<<it;
   }
   cout<<" ";
}
    
 vector<pair<string,int>> Multi_Search(sqlite3* db){



  // Use getline() instead of cin >>
    // cin >> stops at the first space and leaves '\n' in the input buffer.
   // getline() reads the complete line until Enter is press
    //searching a word
    cout<<"Search Any Number of Words : "<<endl;
    string s;
    getline(cin,s);


    vector<string>words=tokenize(s);
    vector<vector<pair<string,int>>>All_Word_Search_Results;
    for(auto &word:words){
        //for each word i want Single_Word_Search's ans
        //it will be stored in a vector of vector of pairs
        
        All_Word_Search_Results.push_back(search_single_word(db,word));
    }
    
    vector<pair<string,int>>Multi_Search_Result=All_Word_Search_Results[0];

    /*
    NOW thi will call "AND","OR" then it will return the final search result of multi-word search query
    

    THE AND,OR --> return vector<pair<string,int>> ;

    this i will implement later
    */


     cout<<"<<<<<<<<<<--- Searched Words :  [ ";
     printString(s);
     cout<<" ] Occured in these file paths: --->>>>>>>>>>"<<endl;
    return Multi_Search_Result;
    //for now i am only returning the search result of first word in the multi-word search query
    

}
    
  