#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include "../tokenize/tokeniser.h"
#include "ChunkedProcessing/Chunked_Reading.h"
#include<sqlite3.h>

using namespace std;
using namespace filesystem;



void Read_File(string path, sqlite3* db, long long file_id);
void Read_File2(string path, sqlite3* db, long long file_id);
