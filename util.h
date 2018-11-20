#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

bool is_number(const std::string& s);
std::string toLower(std::string str);
void split(const std::string& s, std::vector<std::string>& tokens, char delimiter);
bool string2int(std::string str, int& integer);
void stringErase(std::string& str, char c);
void stringReplaceAll(std::string& str, char c1, char c2);
void write(std::string filename, std::vector<std::uint16_t> data, std::size_t bytes);

#endif
