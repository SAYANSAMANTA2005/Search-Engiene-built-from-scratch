#include<iostream>
#include<string>
#include<vector>
#include<utility>
#include<sqlite3.h>
#include"../Database/Database.h"
#include "Multi_Word/Multi_Search.h"
#include "Single_Word/Search.h"
#include "Fuzzy_Search/Fuzzy_handeler.h"
#include "../config/config.h"
using namespace std;


void Specify_Search_Handeler(sqlite3* db){
    cout<<"Enter (M) if you want to search Multi-word / Enter (S) if you want to search Single-word / Enter (F) if u want to Fuzzy Search Word : "<<endl;
    string choice;
    getline(cin,choice);
    cin.clear();
    vector<pair<string,int>>Final_Search_Result_After_Processing;
    if(choice.size() &&(choice[0]=='m'||choice[0]=='M')){
         Final_Search_Result_After_Processing=  Multi_Search(db);
    }
    else if(choice.size() &&(choice[0]=='s'||choice[0]=='S')){
        Final_Search_Result_After_Processing=  Search_Single_Word_in_Database(db);
    }
    else if(choice.size() &&(choice[0]=='f'||choice[0]=='F')){
        Final_Search_Result_After_Processing=  fuzzy_search_handeler(db);
    }
    else{
        cout<<"Invalid Choice\n";
    }



   // after storing final result of query  --> i wii print the result

 
   
cout<<endl;
cout<<" Top [ " << min((size_t)MAX_NO_OF_FILES_TO_SHOW_IN_SEARCH,
 Final_Search_Result_After_Processing.size())<< " ] files found :"<<endl;
   int cnt=0;

   // This will show only top  [ MAX_NO_OF_FILES_TO_SHOW_IN_SEARCH ]  files, of search result




   
   for(auto & Search_result: Final_Search_Result_After_Processing){
    if(cnt>=MAX_NO_OF_FILES_TO_SHOW_IN_SEARCH)break;
    cout << Search_result.first << " (" << Search_result.second << " matches)"<< '\n';
   cnt++;
   }
    

}