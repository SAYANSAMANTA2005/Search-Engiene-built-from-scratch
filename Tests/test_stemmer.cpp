#include <iostream>
#include <string>
#include "../tokenize/stemming/stemmer.h"
#include "check.h"

// Expected values below are derived directly from stemmer.cpp's actual rules
// (strip trailing "ing" if len>4, else strip trailing "ed" if len>3, else
// strip trailing "s" if len>3) -- NOT from what a "correct" linguistic
// stemmer would produce. This stemmer is a simple suffix-stripper, so e.g.
// stem("running") == "runn", not "run". Update these if stemmer.cpp changes.
void test_stemmer() {
    std::cout << "\n--- Testing Stemmer ---\n";

    check_eq<std::string>(stem("running"), "runn", "stem(running) strips trailing 'ing'");
    check_eq<std::string>(stem("tested"), "test", "stem(tested) strips trailing 'ed'");
    check_eq<std::string>(stem("cats"), "cat", "stem(cats) strips trailing 's'");
    check_eq<std::string>(stem("hello"), "hello", "stem(hello) is unchanged (no matching suffix)");
    check_eq<std::string>(stem("as"), "as", "stem(as) is unchanged (too short to strip 's', len<=3)");
}
