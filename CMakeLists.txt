cmake_minimum_required(VERSION 3.15)
project(SearchEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# 1. Get SQLite3 -- built from source via FetchContent, identically on
#    Windows/Mac/Linux. No system package install needed, no binaries
#    committed to git. This replaces the old approach of bundling
#    sqlite3.dll/.lib (Windows-only) or requiring `apt install libsqlite3-dev`.
# ---------------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
    sqlite3
    GIT_REPOSITORY https://github.com/azadkuh/sqlite-amalgamation.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(sqlite3)
# The fetched project defines a plain target named "SQLite3" (not the
# SQLite::SQLite3 alias, which only exists after a real install+find_package).
# Add that alias ourselves so the rest of this file can use one consistent
# name regardless of how SQLite was obtained.
if(NOT TARGET SQLite::SQLite3)
    add_library(SQLite::SQLite3 ALIAS SQLite3)
endif()


# ---------------------------------------------------------------------------
# 2. Library target: every .cpp EXCEPT Crawler.cpp (which holds main())
# ---------------------------------------------------------------------------
# Explicit file list on purpose (no GLOB) -- CMake won't auto-detect new
# files added later with glob without re-running cmake, which causes
# confusing "why isn't my new file compiling" bugs. Add new .cpp files
# to this list as you create them.
# ---------------------------------------------------------------------------
add_library(search_engine_lib STATIC
    Reindexing/ReindexFile.cpp
    Reindexing/FileChangeDetector.cpp
    # NOTE: Explorer/ is legacy/dead code -- run.bat never compiles it,
    # Crawler.cpp only includes it inside a commented-out block.
    # Left out of the build on purpose. Either delete the folder or
    # finish/fix it later -- don't let dead code silently rot in the tree.
    Database/Chunked_Bulk_Insertion/DeleteFilePostings.cpp
    Database/Chunked_Bulk_Insertion/BulkInsert.cpp
    Database/SearchWord/searchwordbyfileid.cpp
    Database/SearchWord/searchword.cpp
    Database/Database.cpp
    Database/ModifiedTime/ModifiedTime.cpp
    # NOTE: run.bat does NOT compile this file, but ReindexFile.cpp calls
    # Read_File2(), which is only defined here -- this is a real bug in
    # run.bat (it links only because you're running an old prebuilt .exe).
    # Fix run.bat by adding this line to it too.
    ReadFile/ChunkedProcessing/Chunked_Reading.cpp
    ReadFile/Readfile.cpp
    Search/Single_Word/Search.cpp
    Search/Multi_Word/Multi_Search.cpp
    Search/Multi_Word/And/and.cpp
    Search/Fuzzy_Search/Fuzzy_Trie/Fuzzy_Trie.cpp
    Search/Fuzzy_Search/Fuzzy_handeler.cpp
    Search/Specify_Search_Handeler.cpp
    tokenize/tokeniser.cpp
    tokenize/stemming/stemmer.cpp
)

# Anything in this project can #include "Database/Database.h" etc.
# using a path relative to the repo root.
target_include_directories(search_engine_lib PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${sqlite3_SOURCE_DIR}
)

target_link_libraries(search_engine_lib PUBLIC SQLite::SQLite3)

# ---------------------------------------------------------------------------
# 3. Main program: Crawler.cpp + the library
# ---------------------------------------------------------------------------
add_executable(search_engine Crawler.cpp)
target_link_libraries(search_engine PRIVATE search_engine_lib)
# No more DLL-copying step needed: SQLite is now compiled directly into
# search_engine_lib from source, so there's no separate sqlite3.dll to ship.

# ---------------------------------------------------------------------------
# 4. Tests (only built if you run cmake with -DBUILD_TESTS=ON, see below)
# ---------------------------------------------------------------------------
option(BUILD_TESTS "Build the test suite" ON)

if(BUILD_TESTS)
    enable_testing()

    add_executable(run_tests
        Tests/test_main.cpp
        Tests/test_tokenizer.cpp
        Tests/test_stemmer.cpp
        Tests/test_trie.cpp
        Tests/test_intersection.cpp
        Tests/test_database.cpp
        Tests/test_file_change_detector.cpp
        # add new Tests/test_*.cpp files here as you write them
    )
    target_link_libraries(run_tests PRIVATE search_engine_lib)

    # Lets you run `ctest` from the build folder instead of the exe directly
    add_test(NAME AllTests COMMAND run_tests)
endif()
