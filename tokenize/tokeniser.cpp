#include "tokeniser.h"
#include "stemming/stemmer.h"
#include "../config/config.h"
#include<vector>
#include <string>
#include <cctype>
#include <sqlite3.h>

using namespace std;


/// for Faster tokenise remember to keep text len<=1k words or sentence length


/*

Tokenizer handeles --> Tokenize + stemming + Normalization   +stopwordfilter

*/
vector<string> tokenize(const string& text)
{
    vector<string> words;
    string cur;

    for(char c : text)
    {
        //checks if alphanumeric character or not 
        //REMOVES PUNCUCATIONS ,BRACKETS ETC
        if(isalnum(c))
        {
            //normalise words
            cur += tolower(c);
        }
        else
        {
            if(!cur.empty())
            {
            //stemming word before storing it
            string Current_Word=stem(cur);
           
            //avoids to store frequent words ie stop words
            if(STOP_WORDS.count(Current_Word)){
                cur.clear();
                continue;
            }

            words.push_back(Current_Word);
            cur.clear();
            }
        }
    }

    if(!cur.empty()){
        //stemming word before storing it
        string Current_Word=stem(cur);
            //avoids to store frequent words ie stop words
            if(STOP_WORDS.count(Current_Word)){
                cur.clear();
            }
            else{
               words.push_back(Current_Word);
            }
         
        cur.clear();
    }
    

    return words;
}