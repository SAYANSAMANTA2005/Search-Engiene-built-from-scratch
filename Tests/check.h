#pragma once
#include <iostream>
#include <string>

// Tracks how many checks have failed across the whole test run.
// main() reads this at the end to decide the process exit code.
inline int g_failures = 0;
inline int g_checks = 0;

// Call this for every single expected-vs-actual comparison in a test.
// `description` should say what was being checked, e.g. "stem(running) == runn".



//colorfull printing in termianls

// Text Colors
#define RESET   "\033[0m"

#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// Bright Colors
#define BRIGHT_RED     "\033[91m"
#define BRIGHT_GREEN   "\033[92m"
#define BRIGHT_YELLOW  "\033[93m"
#define BRIGHT_BLUE    "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN    "\033[96m"

// Text Styles
#define BOLD      "\033[1m"
#define UNDERLINE "\033[4m"



///
inline void check(bool condition, const std::string& description) {
    g_checks++;
    if (condition) {
        std::cout << CYAN <<"  [PASS] "<<RESET  << description <<RESET<< "\n";
    } else {
        std::cout << RED <<"  [FAIL] " <<RESET  << description <<RESET<< "\n";
        g_failures++;
    }
}

// Convenience overload for the common case of comparing two values directly.
// Prints what was expected vs what was actually returned on failure.
template <typename T>
inline void check_eq(const T& actual, const T& expected, const std::string& label) {
    g_checks++;
    if (actual == expected) {
        std::cout << CYAN <<"  [PASS] " <<RESET<< label << "\n";
    } else {
        std::cout << RED<<" [FAIL] " <<RESET<< label
                   << " -- expected [" << expected << "], got [" << actual <<RESET<< "\n";
        g_failures++;
    }
}
