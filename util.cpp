#include "util.h"

bool is_number(const std::string& s) {
    if (s[0] == '-')
        return !s.empty() && std::find_if(s.begin()+1,
            s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
    else
        return !s.empty() && std::find_if(s.begin(),
            s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

std::string toLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

void split(const std::string& s, std::vector<std::string>& tokens, char delimiter) {
    std::string token;
    std::istringstream tokenStream(s);

    while (std::getline(tokenStream, token, delimiter)) {
        if ((token.size() == 1 && token[0] == delimiter) || token.size() == 0)
            continue;
        else
            tokens.push_back(token);
    }
}

bool string2int(std::string str, int& integer) {
    if (is_number(str)) {
        std::stringstream geek(str);
        geek >> integer;
        return true;
    }

    return false;
}

void stringErase(std::string& str, char c) {
    str.erase(std::remove(str.begin(), str.end(), c), str.end());
}

void stringReplaceAll(std::string& str, char c1, char c2) {
  std::replace(str.begin(), str.end(), c1, c2);
}

void write(std::string filename, std::vector<std::uint16_t> data, std::size_t bytes) {
    try {
        auto myfile = std::fstream(filename, std::ios::out | std::ios::binary);
        myfile.write((char*)&data[0], bytes);
        myfile.close();
    }
    catch(...) {
        throw std::runtime_error("Error while exporting binary file please make sure the output file is a valid filename");
    }
}
