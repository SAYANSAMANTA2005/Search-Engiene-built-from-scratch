@echo off

g++ ^
Crawler.cpp ^
tokenize/tokeniser.cpp ^
ReadFile/Readfile.cpp ^
tokenize/stemming/stemmer.cpp ^
Database/Database.cpp ^
Database/SearchWord/searchword.cpp ^
Search/Single_Word/Search.cpp ^
-I include -L lib -std=c++17 -lsqlite3 -static -o search_engine.exe

if %errorlevel% neq 0 (
    echo Build FAILED!
    pause
    exit /b
)

echo Build SUCCESS! Running...
search_engine.exe

pause
