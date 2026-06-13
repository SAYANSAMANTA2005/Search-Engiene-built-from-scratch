#include"../Database.h"
#include<sqlite3.h>
#include<iostream>
using namespace std;

void DeleteFilePostings(
    sqlite3* db,
    long long file_id
);