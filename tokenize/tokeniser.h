#pragma once
#include <vector>
#include <string>

using namespace std;

std::vector<std::string> tokenize(const std::string& text);
std::string stem(std::string word);