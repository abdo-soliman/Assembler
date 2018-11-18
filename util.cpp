#include "util.h"

int findChar(std::vector<char> arr, char letter) {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == letter)
            return i;
    }

    return -1;
}

int findString(std::vector<std::string> arr, std::string str) {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == str)
            return i;
    }

    return -1;
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
