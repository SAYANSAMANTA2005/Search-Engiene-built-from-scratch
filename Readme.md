#THERE IS A PROBLEM IN CMAKE AS OF 27/07/2026 -- IF BUILD WITH CMAKE , CURRENTLY AVALABLE WORDS IN DATABASE CAN NOT BE FETCHED WITHOUT TRAVERSING THAT DIRECTORY AGAIN
** SO for Now RUN the programme in **windows** by -->  ./run **

# Building the Search Engine (Cross-Platform)

This project uses **CMake** as its build system, allowing it to be built
on **Windows, Linux, and macOS** using the same commands.

## Prerequisites

-   CMake 3.15+
-   A C++17 compatible compiler
    -   **Windows:** MSVC (Visual Studio) or MinGW
    -   **Linux:** GCC or Clang
    -   **macOS:** Apple Clang (Xcode Command Line Tools)

------------------------------------------------------------------------

## 1. Clone the Repository

``` bash
git clone <repository-url>
cd SearchEngine
```

------------------------------------------------------------------------

## 2. Create a Build Directory

``` bash
mkdir build
cd build
```

Building outside the source directory keeps generated files separate
from the source code.

------------------------------------------------------------------------

## 3. Configure the Project

``` bash
cmake ..
```

This command:

-   Reads `CMakeLists.txt`
-   Detects your compiler
-   Downloads and builds SQLite automatically
-   Generates the native build files for your operating system

------------------------------------------------------------------------

## 4. Build the Project

``` bash
cmake --build .
```

CMake automatically invokes the correct build tool for your platform.

------------------------------------------------------------------------

## 5. Run the Application

### Windows

``` bash
./search_engine.exe
```

### Linux

``` bash
./search_engine
```

### macOS

``` bash
./search_engine
```

------------------------------------------------------------------------

## Running the Test Suite

### Windows

``` bash
./run_tests.exe
```

### Linux / macOS

``` bash
./run_tests
```

Or simply use:

``` bash
ctest
```

To show detailed failures:

``` bash
ctest --output-on-failure
```

------------------------------------------------------------------------

## Debug Build

``` bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

## Release Build

``` bash
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

For Visual Studio generators:

``` bash
cmake --build . --config Release
```

------------------------------------------------------------------------

## Clean Build

Delete the `build` directory and recreate it.

Linux/macOS:

``` bash
rm -rf build
```

Windows PowerShell:

``` powershell
Remove-Item -Recurse -Force build
```

Then:

``` bash
mkdir build
cd build
cmake ..
cmake --build .
```

------------------------------------------------------------------------

## Why CMake?

-   Cross-platform (Windows, Linux, macOS)
-   Automatically downloads SQLite
-   No platform-specific build scripts
-   Works with Visual Studio, VS Code, and CLion
-   Easy integration with GitHub Actions CI

------------------------------------------------------------------------

## Typical Workflow

``` text
git clone Repository
        ↓
mkdir build
        ↓
cd build
        ↓
cmake ..
        ↓
cmake --build .
        ↓
Run search_engine
        ↓
Run run_tests (or ctest)
```
