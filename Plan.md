
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
 if file extention is valid --> calls tokeniser(here we dont bo stemming) --> tokeniser tokenise the file --> tokeniser removes punctuation and calls stemmer -->stemmer stems the  words --> tokeniser check & removes Stop words -->
  sends filtered & stemmed words to inverted index 
  --> inverted indexer  sends the words to sql data base for storing &
   later answering of queries --> Implemented Ranked Search of single Word (frequency based ans ordering of file paths, among files containing searched word)

     --> implemeted incremental indexing ( using file Change Detector & last modified time )  startegy that saves the programme to Read & Process a lot of programmes


      ** HUGE ACHIVEMENT** --> AFTER APPLYING THIS it processed a 3lakh+ files's containing Folder(16.5 Gb Folder) in 7.5 seconds
      --> Before Implementing **incremental indexing** it took 7 minutes to do that
          **60 TIMES FASTER**



user search 
  │
  ▼
Normalisation
  │
  ▼
Stopword Filter
  │
  ▼
Stemmer (optional)
  │
  ▼
Retrieval from Index
  │
  ▼
Ranking
  │
  ▼
Results








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




For your search engine project, a 10k unique-word buffer is a sensible engineering choice, even though most files won't need it. It shows you're thinking about memory scalability, which is exactly how production indexing systems are designed.