@echo off

taskkill /F /IM search_engine.exe >nul 2>&1

g++ ^
Crawler.cpp ^
tokenize/tokeniser.cpp ^
tokenize/stemming/stemmer.cpp ^
ReadFile/Readfile.cpp ^
ReadFile/ChunkedProcessing/Chunked_Reading.cpp ^
Database/Database.cpp ^
Database/SearchWord/searchword.cpp ^
Database/SearchWord/searchwordbyfileid.cpp ^
Database/Chunked_Bulk_Insertion/BulkInsert.cpp ^
Database/Chunked_Bulk_Insertion/DeleteFilePostings.cpp ^
Database/ModifiedTime/ModifiedTime.cpp ^
Search/Single_Word/Search.cpp ^
Search/Multi_Word/And/and.cpp ^
Search/Multi_Word/Multi_Search.cpp ^
Search/Fuzzy_Search/Fuzzy_Trie/Fuzzy_Trie.cpp ^
Search/Fuzzy_Search/Fuzzy_handeler.cpp ^
Search/Specify_Search_Handeler.cpp ^
Reindexing/FileChangeDetector.cpp ^
Reindexing/ReindexFile.cpp ^
-I include -L lib -std=c++17 -lsqlite3 -static -o search_engine.exe

if %errorlevel% neq 0 (
    echo Build FAILED!
    pause
    exit /b
)

echo Build SUCCESS! Running...
search_engine.exe

pause
