/*
 * test_framework.h  — Zero-dependency, header-only unit-test micro-framework.
 *
 * Usage:
 *   TEST("description") { ASSERT_EQ(foo(), 42); }
 *   int main() { return RUN_ALL_TESTS(); }
 *
 * Compile with -std=c++17 or later.
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <cstdlib>        // EXIT_FAILURE / EXIT_SUCCESS
#include <sstream>

// ─────────────────────────────────────────────────────────────────────────────
// ANSI colour helpers (work on Windows 10+ with VT mode enabled in the bat)
// ─────────────────────────────────────────────────────────────────────────────
#define CLR_RESET  "\033[0m"
#define CLR_GREEN  "\033[32m"
#define CLR_RED    "\033[31m"
#define CLR_YELLOW "\033[33m"
#define CLR_CYAN   "\033[36m"
#define CLR_BOLD   "\033[1m"

// ─────────────────────────────────────────────────────────────────────────────
// Internal registry
// ─────────────────────────────────────────────────────────────────────────────
namespace test_fw {

struct TestCase {
    std::string          name;
    std::string          file;
    int                  line;
    std::function<void()> fn;
};

inline std::vector<TestCase>& registry() {
    static std::vector<TestCase> cases;
    return cases;
}

// Thrown (and caught) for each ASSERT failure so we can continue running other tests
struct AssertionError {
    std::string msg;
};

// Thread-local flag set per test — set to true when an assertion fires
inline bool g_test_failed = false;

// ─────────────────────────────────────────────────────────────────────────────
// RUN_ALL_TESTS
// ─────────────────────────────────────────────────────────────────────────────
inline int run_all() {
    auto& cases = registry();
    int passed = 0, failed = 0;

    std::cout << CLR_BOLD CLR_CYAN
              << "\n╔══════════════════════════════════════════════════════╗\n"
              << "║          Search Engine — Unit Test Suite             ║\n"
              << "╚══════════════════════════════════════════════════════╝\n"
              << CLR_RESET;
    std::cout << "  Running " << cases.size() << " test(s)...\n\n";

    for (auto& tc : cases) {
        g_test_failed = false;
        try {
            tc.fn();
        } catch (const AssertionError& e) {
            g_test_failed = true;
            std::cout << CLR_RED << "  [FAIL] " << CLR_RESET << tc.name << "\n"
                      << "         " << CLR_YELLOW << e.msg << CLR_RESET
                      << "  (" << tc.file << ":" << tc.line << ")\n";
        } catch (const std::exception& e) {
            g_test_failed = true;
            std::cout << CLR_RED << "  [EXCP] " << CLR_RESET << tc.name << "\n"
                      << "         " << CLR_YELLOW << e.what() << CLR_RESET << "\n";
        }

        if (!g_test_failed) {
            ++passed;
            std::cout << CLR_GREEN << "  [PASS] " << CLR_RESET << tc.name << "\n";
        } else {
            ++failed;
        }
    }

    std::cout << "\n" << CLR_BOLD;
    if (failed == 0) {
        std::cout << CLR_GREEN << "  All " << passed << " test(s) passed.\n" << CLR_RESET;
    } else {
        std::cout << CLR_RED
                  << "  " << failed << " FAILED  /  " << passed << " passed  /  "
                  << cases.size() << " total\n"
                  << CLR_RESET;
    }
    std::cout << "\n";
    return (failed == 0) ? 0 : 1;
}

// ─────────────────────────────────────────────────────────────────────────────
// Registrar helper (RAII trick)
// ─────────────────────────────────────────────────────────────────────────────
struct Registrar {
    Registrar(const char* name, const char* file, int line,
              std::function<void()> fn) {
        registry().push_back({name, file, line, fn});
    }
};

} // namespace test_fw

// ─────────────────────────────────────────────────────────────────────────────
// Public macros
// ─────────────────────────────────────────────────────────────────────────────

/*
 * Register and define a test.
 * We use __LINE__ (unique within a file) to generate unique symbol names.
 * CONCAT2 / CONCAT force macro expansion before token-pasting.
 */
#define _TFW_CONCAT2(a, b) a##b
#define _TFW_CONCAT(a, b)  _TFW_CONCAT2(a, b)

#define TEST(description)                                                        \
    static void _TFW_CONCAT(_test_body_, __LINE__)();                            \
    static test_fw::Registrar _TFW_CONCAT(_reg_, __LINE__)(                      \
        description, __FILE__, __LINE__,                                         \
        _TFW_CONCAT(_test_body_, __LINE__));                                      \
    static void _TFW_CONCAT(_test_body_, __LINE__)()

/* Assertion macros — throw AssertionError on failure */
#define ASSERT_TRUE(expr)                                                        \
    do {                                                                         \
        if (!(expr)) {                                                           \
            std::ostringstream _oss;                                             \
            _oss << "ASSERT_TRUE failed: (" #expr ") is false";                 \
            throw test_fw::AssertionError{_oss.str()};                           \
        }                                                                        \
    } while (0)

#define ASSERT_FALSE(expr)                                                       \
    do {                                                                         \
        if ((expr)) {                                                            \
            std::ostringstream _oss;                                             \
            _oss << "ASSERT_FALSE failed: (" #expr ") is true";                 \
            throw test_fw::AssertionError{_oss.str()};                           \
        }                                                                        \
    } while (0)

#define ASSERT_EQ(a, b)                                                          \
    do {                                                                         \
        auto _a = (a); auto _b = (b);                                            \
        if (!(_a == _b)) {                                                       \
            std::ostringstream _oss;                                             \
            _oss << "ASSERT_EQ failed: " #a " == " #b                           \
                 << "  |  got: [" << _a << "] vs [" << _b << "]";               \
            throw test_fw::AssertionError{_oss.str()};                           \
        }                                                                        \
    } while (0)

#define ASSERT_NEQ(a, b)                                                         \
    do {                                                                         \
        auto _a = (a); auto _b = (b);                                            \
        if (_a == _b) {                                                          \
            std::ostringstream _oss;                                             \
            _oss << "ASSERT_NEQ failed: " #a " != " #b                          \
                 << "  |  both equal: [" << _a << "]";                          \
            throw test_fw::AssertionError{_oss.str()};                           \
        }                                                                        \
    } while (0)

#define ASSERT_CONTAINS(vec, val)                                                \
    do {                                                                         \
        bool _found = false;                                                     \
        for (const auto& _x : (vec)) if (_x == (val)) { _found = true; break; } \
        if (!_found) {                                                           \
            std::ostringstream _oss;                                             \
            _oss << "ASSERT_CONTAINS failed: [" #val "] not found in " #vec;    \
            throw test_fw::AssertionError{_oss.str()};                           \
        }                                                                        \
    } while (0)

#define ASSERT_NOT_CONTAINS(vec, val)                                            \
    do {                                                                         \
        for (const auto& _x : (vec))                                             \
            if (_x == (val)) {                                                   \
                std::ostringstream _oss;                                         \
                _oss << "ASSERT_NOT_CONTAINS failed: [" #val "] found in " #vec;\
                throw test_fw::AssertionError{_oss.str()};                       \
            }                                                                    \
    } while (0)

#define RUN_ALL_TESTS() test_fw::run_all()
