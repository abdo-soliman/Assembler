#ifndef UTIL_H
#define UTIL_H

#include <algorithm>
#include <string>
#include <sstream>
#include <vector>

int findChar(std::vector<char> arr, char letter);
int findString(std::vector<std::string> arr, std::string str);
std::string toLower(std::string str);
void split(const std::string& s, std::vector<std::string>& tokens, char delimiter);

#endif