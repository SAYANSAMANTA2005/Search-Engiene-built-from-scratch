# Trie vs Trie* in C++

When learning Tries, one of the most confusing things is understanding when to use:

```cpp
Trie
```

and when to use:

```cpp
Trie*
```

This document explains the difference.

---

# 1. What is Trie?

```cpp
Trie root;
```

This creates an actual Trie object.

Example:

```cpp
Trie root;
```

Memory:

```text
root
 ├── iscomplete
 └── child
```

The variable itself contains all the data of the object.

Access members using:

```cpp
root.insert(word);
```

or

```cpp
root.iscomplete
```

using the dot (`.`) operator.

---

# 2. What is Trie*?

```cpp
Trie* node;
```

This creates a pointer.

A pointer does not contain a Trie object.

Instead, it stores the address of a Trie object.

Example:

```cpp
Trie* node = new Trie();
```

Memory:

```text
node
  |
  v
Trie object
```

Access members using:

```cpp
node->insert(word);
```

or

```cpp
node->iscomplete
```

using the arrow (`->`) operator.

---

# 3. Why Do Tries Use Trie* Children?

A beginner may try:

```cpp
struct Trie
{
    Trie child[26];
};
```

This cannot work.

Reason:

Each child contains:

```cpp
26 Trie objects
```

Each of those contains:

```cpp
26 Trie objects
```

And so on forever.

This creates infinite recursion in the class definition.

The compiler rejects it.

---

# 4. Correct Approach

Store pointers instead:

```cpp
struct Trie
{
    vector<Trie*> child;
};
```

Now each child stores:

```text
Address of another Trie node
```

instead of storing an entire Trie object.

This is finite and valid.

---

# 5. Example

Root node:

```cpp
Trie root;
```

Insert:

```cpp
database
```

During insertion:

```cpp
root.child[3] = new Trie();
```

Memory becomes:

```text
root
 |
 +----> node('d')
             |
             +----> node('a')
                        |
                        +----> node('t')
```

Every edge stores a pointer.

---

# 6. Understanding "this"

Consider:

```cpp
class Trie
{
public:

    void insert(const string& word)
    {
        Trie* cur = this;
    }
};
```

Inside every non-static member function, C++ automatically provides:

```cpp
this
```

Its type is:

```cpp
Trie*
```

which means:

```text
Pointer to the object that called the function.
```

---

# 7. Example of "this"

```cpp
Trie root;

root.insert("database");
```

Suppose:

```text
root is stored at address 1000
```

Inside insert():

```cpp
this == 1000
```

Therefore:

```cpp
Trie* cur = this;
```

means:

```cpp
cur points to root
```

---

# 8. Why We Write

```cpp
Trie* cur = this;
```

Instead of:

```cpp
Trie cur = this;
```

Because:

```cpp
this
```

is already a pointer.

Its type is:

```cpp
Trie*
```

So:

```cpp
Trie* cur = this;
```

is valid.

But:

```cpp
Trie cur = this;
```

would be invalid because a pointer cannot be assigned to an object.

---

# 9. Object vs Pointer

## Object

```cpp
Trie root;
```

Access:

```cpp
root.insert(word);
```

Operator:

```cpp
.
```

---

## Pointer

```cpp
Trie* node = new Trie();
```

Access:

```cpp
node->insert(word);
```

Operator:

```cpp
->
```

---

# 10. Rule of Thumb

Use:

```cpp
Trie
```

when you want an actual object.

Examples:

```cpp
Trie root;
Trie temp;
```

---

Use:

```cpp
Trie*
```

when:

* Linking Trie nodes
* Traversing a Trie
* Dynamically creating nodes
* Using `this`

Examples:

```cpp
Trie* cur;
Trie* child;
Trie* newNode = new Trie();
```

---

# 11. Typical Trie Implementation

```cpp
class Trie
{
    bool iscomplete = false;
    vector<Trie*> child;

public:

    Trie()
    {
        child.resize(26, nullptr);
    }

    void insert(const string& word)
    {
        Trie* cur = this;

        for(char ch : word)
        {
            int idx = ch - 'a';

            if(cur->child[idx] == nullptr)
            {
                cur->child[idx] = new Trie();
            }

            cur = cur->child[idx];
        }

        cur->iscomplete = true;
    }
};
```

Usage:

```cpp
Trie root;

root.insert("database");
root.insert("sqlite");
root.insert("search");
```

This is the standard approach used in most Trie implementations.
