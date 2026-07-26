@echo off
REM ============================================================================
REM run_tests.bat - Build and execute the Search Engine test menu
REM
REM Usage: run_tests.bat
REM
REM Requires: g++ with C++17 support on PATH, and sqlite3.dll in repo root
REM ============================================================================

echo.
echo ============================================================
echo   Building Search Engine Test Menu ...
echo ============================================================
echo.

g++ ^
    Tests\test_main.cpp ^
    Tests\test_stemmer.cpp ^
    Tests\test_tokenizer.cpp ^
    Tests\test_trie.cpp ^
    Tests\test_intersection.cpp ^
    Tests\test_database.cpp ^
    Tests\test_file_change_detector.cpp ^
    tokenize\tokeniser.cpp ^
    tokenize\stemming\stemmer.cpp ^
    Database\Database.cpp ^
    Database\Chunked_Bulk_Insertion\BulkInsert.cpp ^
    Database\Chunked_Bulk_Insertion\DeleteFilePostings.cpp ^
    Database\SearchWord\searchword.cpp ^
    Database\SearchWord\searchwordbyfileid.cpp ^
    Database\ModifiedTime\ModifiedTime.cpp ^
    Search\Fuzzy_Search\Fuzzy_Trie\Fuzzy_Trie.cpp ^
    Search\Multi_Word\And\and.cpp ^
    Reindexing\FileChangeDetector.cpp ^
    -I include ^
    -I . ^
    -L lib ^
    -std=c++17 ^
    -lsqlite3 ^
    -static ^
    -o Tests\run_tests.exe

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Build FAILED. Fix compiler errors above and try again.
    pause
    exit /b 1
)

echo.
echo Build SUCCESS. Running tests menu ...
echo.

Tests\run_tests.exe

set EXIT_CODE=%errorlevel%
echo.

if %EXIT_CODE% equ 0 (
    echo  Tests closed.
) else (
    echo  Tests closed with errors.
)

echo.
pause
exit /b %EXIT_CODE%
