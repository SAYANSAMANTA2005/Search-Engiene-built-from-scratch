#include <iostream>
#include<bits/stdc++.h>
#include <filesystem>
#include "../config/config.h"
#include "../tokenize/tokeniser.h"
#include "Readfile.h"
#include "../Database/Database.h"
#include <sqlite3.h>
using namespace std;
using namespace filesystem;


// APPRoch1 --> tokenizing each sentence (Line) instead of whole file.



void Read_File(string path,sqlite3* db,long long file_id){
    ifstream file(path);
    // ERROR Handeling
    if(!file.is_open()){
    cerr << "Failed to open: " << path << '\n';
     return;
    }
    string line;
    while(getline(file,line)){


        //instead of tokenizing whole file ,
        // This Tokenize each line ,which reduce memory usage
        //and faster for large files 


        
    vector<string> words = tokenize(line);

    // for each word ,insert in Inverted Index 
    // We pass file_id (which will serve as a Post List for that Word)        
    for(auto &word : words) { 
        InsertWord(db,word,file_id);
    }


    }
    file.close();
}


//Approach-2 Tokenize a fixed lenths of strings (let N=1024),
// so that each chunk has at most 1024 words 
// This is usefull for very large file 
//

void Read_File2(string path,sqlite3* db,long long file_id){
    ifstream file(path);
   

    // ERROR Handeling
    if(!file.is_open()){
    cerr << "Failed to open: " << path << '\n';
     return;
    }
    string word;
    string buffer;
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
            InsertWord(db,word,file_id);
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
            InsertWord(db,word,file_id);
         }
        //clear buffer to prevent overflow
        buffer.clear();
    }
    file.close();
}

