 
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

 vector<pair<string,int>> Multi_Search(sqlite3* db);