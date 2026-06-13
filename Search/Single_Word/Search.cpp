
#include<sqlite3.h>
#include<iostream>  
#include "../../Database/Database.h"
#include "../../Database/SearchWord/searchword.h"
#include<string.h>


using namespace std;

    



    string normalise_string(std::string &s){
    string res="";
    // we takes only the first word in word user inters in searching
    // and coverts it into lower case
    for(auto it:s){
        if(it==' ')break;
        if(isalnum(it)){
            res+=tolower(it);
        } 
    }
    return res;
}

    
void Search_in_Database(sqlite3* db){


  // Use getline() instead of cin >>
    // cin >> stops at the first space and leaves '\n' in the input buffer.
   // getline() reads the complete line until Enter is press
    //searching a word
    cout<<"Search a word: "<<endl;
    string s;
    getline(cin,s);
    //normalise string
    s=normalise_string(s);
    cout<<"<<<<<<<<<<--- Word :"<<s<<" Occured in these file paths: --->>>>>>>>>>"<<endl;
    search_word(db,s);
    

}
    
  