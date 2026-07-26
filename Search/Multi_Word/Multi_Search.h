 
#include "../../Database/Database.h"
#include "../../Database/SearchWord/searchword.h"
#include"../../config/config.h"
#include "../../tokenize/tokeniser.h"
#include  "../Single_Word/Search.h"
#include "And/and.h"

#include<sqlite3.h>
#include<iostream> 
#include<string.h>
#include<cctype>
#include<vector>
#include<tuple>
#include<utility>

 vector<pair<string,int>> Multi_Search(sqlite3* db);

 vector<pair<string,int>>Answer_Intersection(vector<tuple<int,int,string>>Intersection_result);

 vector<pair<string,int>>intersection(vector<vector<tuple<int,int,string>>>All_Word_Search_Results);