#include <iostream>      // cout, cerr
#include <fstream>       // ifstream
#include <string>        // string
#include <vector>        // vector
#include <filesystem>    // filesystem (if used)
#include <sqlite3.h>     // sqlite3

#include "../../tokenize/tokeniser.h"     // tokenize()
#include "../../Database/Database.h"      // InsertWord()
#include "../Readfile.h"                  // corresponding header
#include "../config/config.h"

using namespace std;
using namespace std::filesystem;



//MAX_UNIQUE_WORDS_PER_FILE


void Freeing_map(sqlite3* db,long long file_id,unordered_map<string,int>& freq){
    if(!db)return;
    
    if(!freq.empty()){
        // for every word 
         BulkInsert(db,file_id,freq);
         freq.clear();
    }
 
}

void Incrementing_Word_freq(string &word,unordered_map<string,int>& freq,long long file_id,sqlite3* db){
          //increasing frequency of word
        freq[word]++;
        if(freq.size()==MAX_UNIQUE_WORDS_PER_FILE){
          Freeing_map(db,file_id,freq);
         }
}


void Read_File2(string path,sqlite3* db,long long file_id){
    ifstream file(path);

    // ERROR Handeling
    if(!file.is_open()){
    cerr << "Failed to open: " << path << '\n';
     return;
    }

    // used to store frequency of words in a file
    unordered_map<string,int> freq;

    string word;
    string buffer;


   
    
    char* err = nullptr;
//starting SQLite Transaction

    sqlite3_exec(
        db,
        "BEGIN TRANSACTION;",
        nullptr,
        nullptr,
        &err
    );





    while(file>>word){
        //adding each word to buffer string & creating space btw words by " "
      buffer+=word+" ";

// Flush after adding a complete word.
// Buffer size may exceed LIMIT slightly,
// but words are never split.

     if(buffer.size()>=LIMIT){
        //tokenise fixed size buffer
        vector<string> words=tokenize(buffer);

        // for each word ,insert in Inverted Index 
        // We pass file_id (which will serve as a Post List for that Word)        
        for(auto &word : words) { 

          Incrementing_Word_freq(word,freq,file_id,db);
          //InsertWord(db,word,file_id);
         }


        //clear buffer to prevent overflow
        buffer.clear();
     }
    }
    // flushing the remaining buffer if not empty
    if(!buffer.empty()){
        vector<string> words=tokenize(buffer);
        // for each word ,insert in Inverted Index 
        // We pass file_id (which will serve as a Post List for that Word)        
        for(auto &word : words) { 
            Incrementing_Word_freq(word,freq,file_id,db);
           // InsertWord(db,word,file_id);
         }
        //clear buffer to prevent overflow
        buffer.clear();
    }


    // freeing map memory
    if(!freq.empty()){
        Freeing_map(db,file_id,freq);
    }


    file.close();


//Sql Commit 
    sqlite3_exec(
        db,
        "COMMIT;",
        nullptr,
        nullptr,
        &err
    );



}