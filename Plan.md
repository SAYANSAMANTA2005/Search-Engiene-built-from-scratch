
workflow -->

Files
  │
  ▼
Crawler (done)
  │
  ▼
Tokenizer (done)
  │
  ▼
Inverted Index 
  │
  ▼
SQLite Database
  │
  ▼
Search API
  │
  ▼
CLI / Web UI






Why i choose C++?
ans-->
"The current dataset doesn't require C++ for performance, but I chose it because search engines often involve CPU-intensive indexing and ranking workloads. Building the core in C++ provides a path to scale to larger datasets while also demonstrating systems programming skills."







workflow of code -->

 user given an directory to iterate --> it recursively goes to all the files and directories --> check
 if file extention is valid --> calls tokeniser(here we dont bo stemming) --> tokeniser tokenise the file --> tokeniser removes punctuation and calls stemmer -->stemmer stems the  words --> tokeniser sends words to inverted index --> inverted indexer sends the words to sql data base for storing & later answering of queries .








 Filesystem

    │
    ▼

Crawler

    │
    ▼

Tokenizer

    │
    ▼

Stemmer

    │
    ▼

SQLite Storage

(files)
(inverted_index)

    │
    ▼

Query Engine

    │
    ▼

Ranking Engine

    │
    ▼

Real-Time Updater

    │
    ▼

Web UI