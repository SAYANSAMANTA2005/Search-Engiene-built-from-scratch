#pragma once

#include <unordered_set>
#include <unordered_map>
#include <string>
using namespace std;
const size_t LIMIT = 1024;//1e3
const int MAX_UNIQUE_WORDS_PER_FILE = 100000;//1e5
const int MAX_NO_OF_FILES_TO_SHOW_IN_SEARCH=3;//
inline long long file_id = 0;


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

    // Git / Version Control
    ".git",
    ".github",
    ".gitlab",
    ".svn",
    ".hg",

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

    // Java / Kotlin / JVM
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

    // Operating Systems (Windows / macOS / Linux)
    "$Recycle.Bin",
    "System Volume Information",
    ".Trash-1000",
    ".Trashes",
    "RECYCLER",
    ".fseventsd",
    ".Spotlight-V100",

    "vcpkg",
    ".vcpkg"
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