#pragma once

#include <unordered_set>
#include <unordered_map>
#include <string>
using namespace std;
const size_t LIMIT = 1e5;//1e5
const int MAX_UNIQUE_WORDS_PER_FILE = 100000;//1e5
const int MAX_NO_OF_FILES_TO_SHOW_IN_SEARCH=10;//
inline long long file_id = 0;
const int INTERVAL_OF_PROCESSED_FILE_SHOW=100;//how many files to process at once
const int MAX_LIMIT_OF_STORING_SEARCH_RESULT=10;//in case of single word search

const int MAX_LIMIT_OF_STORING_MULTI_WORD_SEARCH_RESULT=100;
const int  MAX_EDIT_DISTANCE = 2;
const int max_no_of_fuzzy_search_suggestions=10;



inline unordered_map<long long,string> file_id_to_path;
inline unordered_map<string,long long> path_to_file_id;

const std::unordered_set<std::string> text_extensions = {
    // Plain text
    ".txt", ".md",".log",

    // C/C++
    ".c",".cpp",".h",".hpp",

    // Java
    ".java",

    // Python
    ".py",

    // JavaScript / TypeScript
    ".js",".ts",

    // Web
    ".html",".css",

    // Data formats
    ".json",".xml",

    // Config files
    ".ini",".cfg",".yaml",".yml",

    // Database scripts
    ".sql",

    // Shell scripts
    ".sh",".bat",".ps1",

    //pdf files --> IS IGNORED BCZ THEY NEEED 
    // a seperate new pipeline to extract text from them
   // ".pdf"

   // images
   //".jpg",".png","jpeg","gif","bmp","svg","webp"
};

inline const std::unordered_set<std::string>
SKIP_DIRECTORIES = {

    // JavaScript / Node.js
    "node_modules",
    ".npm",
    ".yarn",
    ".pnpm-store",
    ".next",
    ".nuxt",
    ".svelte-kit",
    ".turbo",
    "bower_components",
    ".cache/yarn",

    // Git / Version Control
    ".git",
    ".github",
    ".gitlab",
    ".svn",
    ".hg",

   // Compilers & Toolchains (Gcc, MinGW, Clang)
    "Gcc-16",
    "Gcc-15",
    "Gcc-14",
    "Gcc-13",              // <-- NEWLY ADDED
    "MinGW",
    "msys64",
    "msys32",
    "TDM-GCC-32",
    "MinGW32",
    "MinGW64",
    "LLVM",                // <-- NEWLY ADDED
    "Clang",               // <-- NEWLY ADDED


      

    // Python
    "__pycache__",
    ".venv",
    "venv",
    "env",
    ".pytest_cache",
    ".mypy_cache",
    ".ruff_cache",
    ".tox",
    "eggs",
    ".eggs",
    "*.egg-info",
    "htmlcov",
    "site-packages",      // <-- NEWLY ADDED (Crucial for local pip libraries)
    "dist-packages",      // <-- NEWLY ADDED
    ".conda",             // <-- NEWLY ADDED
    "conda-meta",         // <-- NEWLY ADDED
    ".ipynb_checkpoints", // <-- NEWLY ADDED (Jupyter notebook caches)
    "miniconda",          // <-- NEWLY ADDED
    "miniconda3",         // <-- NEWLY ADDED
    "anaconda",           // <-- NEWLY ADDED
    "anaconda3",          // <-- NEWLY ADDED

    // C / C++ / Build Folders
    "build",
    "build64",
    "cmake-build-debug",
    "cmake-build-release",
    "dist",
    "out",
    "bin",
    "obj",
    "x64",
    "Win32",
    "Release",
    "Debug",
    "ipch",
    "vcpkg",
    ".vcpkg",
    "vcpkg_installed",   // <-- NEWLY ADDED (Local project dependency caches)
    ".conan",            // <-- NEWLY ADDED
    ".conan2",           // <-- NEWLY ADDED


   // .NET & Java / Kotlin / JVM
    ".nuget",            // <-- NEWLY ADDED (Global C# package storage)
    "packages",          // <-- NEWLY ADDED
    ".gradle",
    "buildSrc",
    ".target",
    "target",
    "out",


    // Ruby / PHP / Go
    ".bundle",
    "vendor",
    "composer",

    // Mobile (Android / iOS / Flutter)
    ".dart_tool",
    "Pods",
    ".symlinks",
    "DerivedData",

    // IDE / Editors
    ".vscode",
    ".idea",
    ".vs",
    "*.suo",
    "*.user",
    ".eclipse",
    ".metadata",



    // Operating Systems & Low-Level Kernels
    "$Recycle.Bin",
    "System Volume Information",
    "Windows",                 // <-- NEWLY ADDED (Prevents deep OS lockup crashes)
    "Program Files",           // <-- NEWLY ADDED
    "Program Files (x86)",     // <-- NEWLY ADDED
    "ProgramData",             // <-- NEWLY ADDED
    "AppData",                 // <-- NEWLY ADDED
    "Config.Msi",              // <-- NEWLY ADDED
    "RECYCLER",
    ".Trash-1000",
    ".Trashes",
    ".fseventsd",
    ".Spotlight-V100",
    "usr",                     // <-- NEWLY ADDED
    "var",                     // <-- NEWLY ADDED
    "proc",                    // <-- NEWLY ADDED
    "sys",                     // <-- NEWLY ADDED
    "dev",                     // <-- NEWLY ADDED
    "run",                     // <-- NEWLY ADDED



     // Caches & Logs
    ".cache",
    "logs",
    "log",
    "tmp",
    "temp",
    ".sass-cache",

    // DevOps & Containers
    ".terraform",
    ".vagrant",
    ".docker",

    // VS CODE
    "vs",






// ─── Compilers & Toolchains (additions) ───────────────────────────────────
    "Cygwin",
    "Cygwin64",
    "arm-none-eabi",
    "xtensa-esp32-elf",
    "riscv32-esp-elf",
    "x86_64-w64-mingw32",


// ─── Rust ─────────────────────────────────────────────────────────────────
    ".cargo",
    ".rustup",
    "target",


// ─── Java / JVM (additions) ───────────────────────────────────────────────
    ".m2",
    ".ivy2",
    ".sbt",
    ".kotlin",


// ─── Go ───────────────────────────────────────────────────────────────────
    ".gopath",
    "pkg",
    "mod",


// ─── Database Files & Dumps ───────────────────────────────────────────────
    ".db-journal",
    "pgdata",
    "mysql",
    "redis-data",
    "mongodb-data",
    "elasticsearch-data",


// ─── Browser & Electron App Caches ────────────────────────────────────────
    "GPUCache",
    "ShaderCache",
    "Code Cache",
    "CachedData",
    "blob_storage",
    "IndexedDB",
    "Local Storage",
    "Session Storage",
    "Service Worker",
    "CacheStorage",


// ─── Windows System (additions) ───────────────────────────────────────────
    "WinSxS",
    "SoftwareDistribution",
    "Prefetch",
    "SysWOW64",
    "System32",
    "Installer",
    "assembly",
    "Microsoft.NET",
    "WpSystem",
    "WindowsApps",
    "MSOCache",
    "$WinREAgent",
    "$Windows.~BT",
    "$Windows.~WS",
    "PerfLogs",


// ─── Linux System (additions) ─────────────────────────────────────────────
    "boot",
    "lib",
    "lib64",
    "sbin",
    "bin",
    "etc",
    "snap",
    "flatpak",
    "lost+found",


// ─── macOS System (additions) ─────────────────────────────────────────────
    "Library",
    "System",
    "private",
    "cores",
    ".DocumentRevisions-V100",
    ".TemporaryItems",
    ".PKInstallSandboxManager",


// ─── Virtual Machines & Emulators ─────────────────────────────────────────
    ".VirtualBox",
    "VirtualBox VMs",
    ".vmware",
    "Virtual Machines",
    "Snapshots",
    ".qemu",


// ─── Game Engines & Asset Pipelines ───────────────────────────────────────
    "Library",
    "Temp",
    ".import",
    ".godot",
    "DerivedDataCache",
    "Intermediate",
    "Saved",
    "Binaries",


// ─── DevOps & Containers (additions) ──────────────────────────────────────
    ".minikube",
    ".kube",
    ".helm",
    "__pycache__",
    ".serverless",
    ".pulumi",
    ".ansible",


// ─── Misc Build & Tooling ─────────────────────────────────────────────────
    ".parcel-cache",
    ".rollup-cache",
    ".esbuild",
    ".swc",
    ".nx",
    ".angular",
    "storybook-static",
    ".storybook-out",
    ".docusaurus",
    ".vuepress",
    "public",
    ".hugo_build.lock",
    "_site",
    ".jekyll-cache",

};

inline const std::unordered_set<std::string> STOP_WORDS = {
    // Articles & Conjunctions
    "a", "an", "the", "and", "but", "or", "nor", "for", "yet", "so",
    
    // Prepositions
    "in", "on", "at", "to", "from", "by", "with", "of", "about", "into", "through",
    
    // Pronouns
    "i", "me", "my", "we", "our", "you", "your", "he", "him", "his", "she", "her", 
    "it", "its", "they", "them", "their", "this", "that", "these", "those",
    
    // Verbs
    "is", "am", "are", "was", "were", "be", "been", "being", "have", "has", "had", 
    "do", "does", "did", "can", "could", "will", "would", "should",
    
    // Code & Doc Specific Layout Junk
    "todo", "fixme", "note", "example", "sample", "test", "demo", "placeholder",
    "param", "return", "returns", "function", "method", "class", "variable",
    "page", "chapter", "section", "figure", "fig", "table", "copyright"
};