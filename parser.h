#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <exception>

#include "util.h"

using std::map;
using std::pair;
using std::vector;
using std::string;
using std::int16_t;
using std::ifstream;
using std::ofstream;
using std::to_string;
using std::logic_error;
using std::runtime_error;
using std::bitset;
using std::getline;

class parser {
    private:
        bool inVariablesArea;
        const map<string, string> instrucitons_two_operand = {
            {"mov", "1111"},
            {"add", "1110"},
            {"adc", "1101"},
            {"sub", "1100"},
            {"sbc", "1011"},
            {"and", "1010"},
            {"or", "1001"},
            {"xnor", "1000"},
            {"cmp", "0111"}
        };
        const map<string, string> instrucitons_one_operand = {
            {"inc", "0110000000"},
            {"dec", "0110000100"},
            {"clr", "0110001000"},
            {"inv", "0110001100"},
            {"lsr", "0110010000"},
            {"ror", "0110010100"},
            {"rrc", "0110011000"},
            {"asr", "0110011100"},
            {"lsl", "0110100000"},
            {"rol", "0110100100"},
            {"rlc", "0110101000"}
        };
        const map<string, string> instrucitons_branch = {
            {"br",  "0101000"},
            {"beq", "0101001"},
            {"bne", "0101010"},
            {"blo", "0101011"},
            {"bls", "0101100"},
            {"bhi", "0101101"},
            {"bhs", "0101110"}
        };
        const map<string, string> instrucitons_no_operand = {
            {"hlt", "0001000000000000"},
            {"nop", "0000000001000000"}
        };
        const map<string, string> instructions_sub_routine = {
            {"jsr", "0100"},
            {"rts", "0011000000000000"},
            {"iret", "0010000000000000"}
        };

        ifstream input_file;
        map<string,int> labels_table;
        string tmp_label;
        map<string,int> variables_table;
        map<pair<int, bool>, string> fix_labels_table;
        map<int, string> fix_variables_table;
        vector<string> output;

        bool isValidVariable(string variabale, string& error);

        bool isKeyWord(string variabale);
        bool isVariable(string operand);
        bool isLabel(string operand);
        int isRegister(string operand);
        bool isIndex(string str, int16_t& index);

        bool isTwoOpsInstruction(string instruction);
        bool isOneOpInstruction(string instruction);
        bool isBranchInstruction(string instruction);
        bool isNoOpInstruction(string instruction);
        bool isSubRoutineInstruction(string instruction);

        string regCode(int reg);
        bool line2fields(const string& s, vector<string>& tokens, char delimiter, int& two_lines_label);
        bool readLine(string line, int& line_number, string& error, int& two_lines_label);
        bool parseLine(string line, int& line_number, string& error, int& two_lines_label);

        bool parseOperandAuto(string operand, string& mode, string& reg_code, int line_number, string& error, bool indirect=false);
        bool parseOperandIndexed(string operand, string& mode, string& reg_code, int16_t& index, string& variable, int line_number, string& error, bool indirect=false);
        bool parseOperand(string operand, string& code, int16_t& index, string& variable, int line_number, string& error, bool source=false, bool indirect=false);
        bool parseTwoOpsInstruction(vector<string> instruction, int& line_number, string& error);
        bool parseOneOpInstruction(vector<string> instruction, int& line_number, string& error);
        bool parseBranchInstruction(vector<string> instruction, int line_number, string& error);
        bool parseNoOpInstruction(vector<string> instruction, int line_number, string& error);
        bool parseSubRoutineInstruction(vector<string> instruction, int line_number, string& error);

        bool parseVariables(vector<string> instruction, int& line_number, string& error);
        bool parseKeyWords(vector<string> instruction, int& line_number, string& error);

        void firstPass();
        void secondPass();

    public:
        parser();
        void parse(string input_filename, string output_filename="output.bin");

};

#endif
