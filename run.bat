@echo off

g++ Crawler.cpp tokenize/tokeniser.cpp ReadFile/Readfile.cpp tokenize/stemming/stemmer.cpp Database/Database.cpp -I "..\SQLite_Search_Engiene\vcpkg\installed\x64-windows\include" -L "..\SQLite_Search_Engiene\vcpkg\installed\x64-windows\lib" -std=c++17 -lsqlite3 -static -o search_engine.exe

if %errorlevel% neq 0 (
    echo Build FAILED!
    pause
    exit /b
)

echo Build SUCCESS! Running...
search_engine.exe

pause
