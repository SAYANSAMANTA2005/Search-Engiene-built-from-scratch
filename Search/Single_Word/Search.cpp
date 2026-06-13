
#include<sqlite3.h>
#include<iostream>  
#include "../../Database/Database.h"
#include "../../Database/SearchWord/searchword.h"
#include"../../config/config.h"
#include "../../tokenize/stemming/stemmer.h"
#include<string.h>
#include<cctype>

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

    string normalise_string(std::string &s){
    string res="";
    // we takes only the first word in word user inters in searching
    // and coverts it into lower case
    for(auto it:s){
        if(it==' ')break;
        //if(it<='9'&& it>='0')continue;
        if(isalnum(it)){
            res+=tolower(it);
        } 
    }

    //stemming of word
    res= stem(res);

    // stop words filter
    if(STOP_WORDS.count(res))return "";


    return res;
}

    
void Search_in_Database(sqlite3* db, bool UserWant_To_Search){

  if(!UserWant_To_Search)return;

  // Use getline() instead of cin >>
    // cin >> stops at the first space and leaves '\n' in the input buffer.
   // getline() reads the complete line until Enter is press
    //searching a word
    cout<<"Search a word: "<<endl;
    string s;
    getline(cin,s);
    //normalise string
    s=normalise_string(s);
    cin.clear();
    cout<<"<<<<<<<<<<--- Word :  [ "<<s<<" ] Occured in these file paths: --->>>>>>>>>>"<<endl;
    search_word(db,s);
    

}
    
  