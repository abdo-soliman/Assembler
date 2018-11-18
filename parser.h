#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <exception>

#include "util.h"

class parser {
    private:
        bool inVariablesArea;
        const std::map<std::string, std::string> instrucitons_two_operand = {
            {"mov", "00000"},
            {"add", "00001"},
            {"adc", "00010"},
            {"sub", "00011"},
            {"sbc", "00100"},
            {"and", "00101"},
            {"or", "00110"},
            {"xnor", "00111"},
            {"cmp", "01000"}
        };
        const std::map<std::string, std::string> instrucitons_one_operand = {
            {"inc", "01001"},
            {"dec", "01010"},
            {"clr", "01011"},
            {"inv", "01100"},
            {"lsr", "01101"},
            {"ror", "01110"},
            {"rrc", "01111"},
            {"asr", "10000"},
            {"lsl", "10001"},
            {"rol", "10010"},
            {"rlc", "10011"}
        };
        const std::map<std::string, std::string> instrucitons_branch = {
            {"br", "10100"},
            {"beq", "10101"},
            {"bne", "10110"},
            {"blo", "10111"},
            {"bls", "11000"},
            {"bhi", "11001"},
            {"bhs", "11010"}
        };
        const std::map<std::string, std::string> instrucitons_no_operand = {
            {"hlt", "11011"},
            {"nop", "11100"}
        };
        const std::map<std::string, std::string> instructions_sub_routine = {
            {"jsr", "11101"},
            {"rts", "11110"},
            {"interrupt", "11111"},
            {"iret", "000000"}
        };
        
        std::map<std::string,int> labels_table;
        std::map<std::string,int> variables_table;

        bool isValidVariable(std::string variabale, std::string& error);
        bool isKeyWord(std::string variabale);
        bool readLine(std::string line, int line_number, std::string& error);
        bool split(const std::string& s, std::vector<std::string>& tokens, char delimiter);
        bool parseLine(std::string line, int line_number, std::string error);
        
        void firstPass(std::ifstream file);
        void secondPass(std::ifstream file);
    public:
        parser();

};

#endif