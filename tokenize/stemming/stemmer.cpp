#include "stemmer.h"
using namespace std;



string stem(string word)
{
    //removing common suffixes
    if(word.size() > 4 &&
       word.substr(word.size()-3) == "ing")
    {
        word.erase(word.size()-3);
    }
    else if(word.size() > 3 &&
            word.substr(word.size()-2) == "ed")
    {
        word.erase(word.size()-2);
    }
    else if(word.size() > 3 &&
            word.back() == 's')
    {
        word.pop_back();
    }

    return word;
}
