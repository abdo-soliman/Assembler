#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
using std::string;
using std::to_string;

class Validator {
    bool isValidVariable(string variabale, string& error);

    bool isKeyWord(string variabale);
    bool isVariable(string operand);
    bool isLabel(string operand);
    int isRegister(string operand);
    bool isIndex(string str, std::int16_t& index);

    bool isTwoOpsInstruction(string instruction);
    bool isOneOpInstruction(string instruction);
    bool isBranchInstruction(string instruction);
    bool isNoOpInstruction(string instruction);
    bool isSubRoutineInstruction(string instruction);
};

#endif