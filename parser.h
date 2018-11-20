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

class parser {
    private:
        bool inVariablesArea;
        const std::map<std::string, std::string> instrucitons_two_operand = {
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
        const std::map<std::string, std::string> instrucitons_one_operand = {
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
        const std::map<std::string, std::string> instrucitons_branch = {
            {"br",  "0101000"},
            {"beq", "0101001"},
            {"bne", "0101010"},
            {"blo", "0101011"},
            {"bls", "0101100"},
            {"bhi", "0101101"},
            {"bhs", "0101110"}
        };
        const std::map<std::string, std::string> instrucitons_no_operand = {
            {"hlt", "0001000000000000"},
            {"nop", "0000000001000000"}
        };
        const std::map<std::string, std::string> instructions_sub_routine = {
            {"jsr", "0100"},
            {"rts", "0011000000000000"},
            {"iret", "0010000000000000"}
        };

        std::ifstream input_file;
        std::map<std::string,int> labels_table;
        std::string tmp_label;
        std::map<std::string,int> variables_table;
        std::map< std::string,std::pair<int, bool> > fix_labels_table;
        std::map<std::string,int> fix_variables_table;
        std::vector<std::string> output;

        bool isValidVariable(std::string variabale, std::string& error);

        bool isKeyWord(std::string variabale);
        bool isVariable(std::string operand);
        bool isLabel(std::string operand);
        int isRegister(std::string operand);
        bool isIndex(std::string str, std::int16_t& index);

        bool isTwoOpsInstruction(std::string instruction);
        bool isOneOpInstruction(std::string instruction);
        bool isBranchInstruction(std::string instruction);
        bool isNoOpInstruction(std::string instruction);
        bool isSubRoutineInstruction(std::string instruction);

        std::string regCode(int reg);
        bool line2fields(const std::string& s, std::vector<std::string>& tokens, char delimiter, int& two_lines_label);
        bool readLine(std::string line, int& line_number, std::string& error, int& two_lines_label);
        bool parseLine(std::string line, int& line_number, std::string& error, int& two_lines_label);

        bool parseOperandAuto(std::string operand, std::string& mode, std::string& reg_code, int line_number, std::string& error, bool indirect=false);
        bool parseOperandIndexed(std::string operand, std::string& mode, std::string& reg_code, std::int16_t& index, std::string& variable, int line_number, std::string& error, bool indirect=false);
        bool parseOperand(std::string operand, std::string& code, std::int16_t& index, std::string& variable, int line_number, std::string& error, bool source=false, bool indirect=false);
        bool parseTwoOpsInstruction(std::vector<std::string> instruction, int& line_number, std::string& error);
        bool parseOneOpInstruction(std::vector<std::string> instruction, int& line_number, std::string& error);
        bool parseBranchInstruction(std::vector<std::string> instruction, int line_number, std::string& error);
        bool parseNoOpInstruction(std::vector<std::string> instruction, int line_number, std::string& error);
        bool parseSubRoutineInstruction(std::vector<std::string> instruction, int line_number, std::string& error);

        bool parseVariables(std::vector<std::string> instruction, int& line_number, std::string& error);
        bool parseKeyWords(std::vector<std::string> instruction, int& line_number, std::string& error);

        void firstPass();
        void secondPass();

    public:
        parser();
        void parse(std::string input_filename, std::string output_filename="output.bin");

};

#endif
