
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

          now avg word search query time -->300ms 
      
      OPtimisation --> 
      -->  after updating the sql transaction & sql commit in (Each file's starting --> starts Transcation)
       (each file's end -->commits )

      --> NOW i also implemented  **Multi Word Search** 



      SEARCH LOGIC---->

 



F:\APP DEVELOPMENT -- > first time traverse 4.5 minutes to process --> second time traverse <10 second

06:16 am --> F:\JU SUPER FOLDER --> traversed in <10 sec


06:19 am F:\JAVASCRIPT PROJECTS --> traversed in <20 sec 

06:25 am ---> F:\Oops_LLd_HLD  --> Traversed in ABOUT 30-35 sec(newly read &added 200 files in db)




07:08 am --> F:\REACT PROJECTS  --> 1st time traversed in 58 seconds (244 files read & added in db)

07:18  am -->F:\FUTURE_INTERN Company tasks 1st time traveral take 10 sec(30 files read & insert in db)



first time reading -->F:\\ whole disk traversed in 45 min(70 Gb space, 10 lakh+ files read & stored in db)
2nd time reading --> F:\\ whole disk traversed in 40 seconds (traversed 10 lakh + files, 70 GB memory spaced)

/* another version of this code (15/06/26)   */
/* code Ran from E Drive*/
after delting the db & run again --> F:\\ whole disk traversed in 3 min,20 sec   only.

when i Travrsed whole F:\\  2nd time -6:55

current code (Ran from F drive with LIMIT=1e3)
F:// trveral 2nd time -> 1 minute
third time trveral --> 16.5 seconds
F:// --> after deleting 1st trvereal took 4 min 45 sec


current code (Ran from F drive with LIMIT=1e4)
F:// --> after deleting 1st trvereal took 4 min 14 sec
      ---> 2nd traversal took 14 sec only
      -->3rd trversal 12.80 sec
      --> 4th time  11 sec

current code (Ran from F drive with LIMIT=5e4)
F:// --> after deleting 1st trvereal took  4 min 1 second
      ---> 2nd traversal took 11 sec only
     


     after updating the sql transaction & sql commit in (Each file's starting --> starts Transcation)
     (each file's end -->commits )
16/06/26  3:44 am --> F:// took first time traversal =5 min 3 sec
                                2nd traversal took = 6sec only
                                3rd traversal 4.80 sec only
                                4th traversal 4.5  sec


 after LIMIT=1e5
  1st time travesal F:// 3min 30 sec
  2nd time traveral F:// 4.5 sec only 










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
Ranking(Term[word] Frequency Only) -- TF
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






# Fuzzy Search Approaches for My Search Engine

## What is Fuzzy Search?

Fuzzy Search allows users to find results even when the query contains spelling mistakes.

Example:

```text
User Query: databse

Expected Result:
database
```

Instead of requiring an exact match, the search engine attempts to find similar words.

---

# Approach 1: Levenshtein Distance

## Idea

Levenshtein Distance measures the minimum number of edits needed to transform one word into another.

Allowed operations:

1. Insert a character
2. Delete a character
3. Replace a character

Example:

```text
database -> databse
```

Required operations:

```text
Delete 'a'
```

Distance:

```text
1
```

Another example:

```text
database -> databaze
```

Distance:

```text
1
```

(one character replacement)

---

## Algorithm

Dynamic Programming is used.

Let:

```text
dp[i][j]
```

represent the minimum edits required to convert:

```text
a[0...i]
```

into:

```text
b[0...j]
```

Time Complexity:

```text
O(n × m)
```

Space Complexity:

```text
O(n × m)
```

where:

```text
n = length of first word
m = length of second word
```

---

## Problem

Suppose the vocabulary contains:

```text
500,000 words
```

To find similar words we would need:

```cpp
for(word in vocabulary)
{
    distance(query, word);
}
```

Complexity:

```text
O(V × L²)
```

where:

```text
V = vocabulary size
L = average word length
```

This becomes very slow.

---

# Approach 2: BK Tree (Recommended)

## Idea

BK Tree (Burkhard-Keller Tree) is a data structure designed specifically for fuzzy matching using edit distance.

Instead of comparing against every word, it eliminates large portions of the vocabulary.

---

## Example Vocabulary

```text
database
sqlite
index
search
engine
```

These words are inserted into a BK Tree.

---

## Query

```text
databse
```

Maximum allowed distance:

```text
2
```

The BK Tree explores only relevant branches.

Result:

```text
database
```

---

## Why BK Tree is Better

Instead of checking:

```text
500,000 words
```

it may only examine:

```text
a few hundred words
```

depending on the search radius.

---

## Complexity

Worst Case:

```text
O(V)
```

Average Case:

```text
Much better than linear scan
```

Practical performance is excellent for spell correction.

---

## Recommended Usage

Workflow:

```text
User Query
      ↓
Exact Match?
      ↓
No
      ↓
BK Tree Search
      ↓
Suggested Word
      ↓
Normal Search
```

Example:

```text
databse
    ↓
database
    ↓
Search(database)
```

---

# Approach 3: Trie-Based Fuzzy Search

## Idea

Store all vocabulary words inside a Trie.

Example:

```text
data
database
dataset
date
```

Trie:

```text
root
 └── d
      └── a
           └── t
```

---

## Advantages

Useful for:

* Autocomplete
* Prefix Search
* Search Suggestions

Example:

```text
User Types:

dat
```

Suggestions:

```text
data
database
dataset
```

---

## Fuzzy Search with Trie

A Trie can be combined with edit distance calculations while traversing the tree.

This avoids scanning the entire dictionary.

---

## Complexity

Depends on:

```text
Word Length
Allowed Edit Distance
Trie Structure
```

Generally faster than checking every word.

---

# Comparison

| Method           | Easy to Implement | Fast | Memory Usage | Best Use Case               |
| ---------------- | ----------------- | ---- | ------------ | --------------------------- |
| Levenshtein Scan | Yes               | No   | Low          | Small vocabularies          |
| BK Tree          | Medium            | Yes  | Medium       | Spell correction            |
| Trie             | Medium            | Yes  | Higher       | Autocomplete + fuzzy search |

---

# Recommended Plan for This Search Engine

Current Roadmap:

```text
Crawler
    ↓
Inverted Index
    ↓
Single Word Search
    ↓
Multi Word Search
    ↓
TF-IDF Ranking
    ↓
Phrase Search
    ↓
Vocabulary Table
    ↓
BK Tree
    ↓
Fuzzy Search
```

 implementation Plan :

1. Build a vocabulary table.
2. Store every unique indexed word.
3. Load vocabulary into a BK Tree during startup.
4. If exact search fails:

   * Search BK Tree with distance ≤ 2.
   * Suggest closest words.
5. Run normal search on the selected word.

This provides efficient fuzzy search without significantly impacting search performance.
