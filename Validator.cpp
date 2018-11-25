#include "Validator.h"

bool Validator::isValidVariable(string variabale, string& error) {
    if (variables_table.find(variabale) != variables_table.end()) {
        error = "variable *" + variabale + "* is already defined at line " + to_string(variables_table.find(variabale)->second);
        return false;
    }

    if (labels_table.find(variabale) != labels_table.end()) {
        error = "variable *" + variabale + "* is already used as a label at line " + to_string(labels_table.find(variabale)->second);
        return false;
    }

    if (isKeyWord(variabale)) {
        error = "*" + variabale + "* is a keyword and not allowed to be used as a variable";
        return false;
    }

    if (variabale.size() > 255) {
        error = "invalid variable name *" + variabale + "* must be less than 255 characters";
        return false;
    }

    if (!(variabale[0] == '_' || (variabale[0] >= 65 && variabale[0] <= 90) || (variabale[0] >= 97 && variabale[0] <= 122))) {
        error = "invalid variable name *" + variabale + "* must be start with an upper letter, lower letter or underscore";
        return false;
    }

    if (variabale.size() > 1) {
        for (unsigned int i = 1; i < variabale.size(); i++) {
            if (!( variabale[0] == '_' || (variabale[0] >= 65 && variabale[0] <= 90) || (variabale[0] >= 97 && variabale[0] <= 122) || (variabale[0] >= 48 && variabale[0] <= 57) )) {
                error = "invalid variable name *" + variabale + "* can contain only upper letters, lower letters, numbers and underscore";
                return false;
            }
        }
    }

    return true;
}
/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/
bool Validator::isKeyWord(string variabale) {
    variabale = toLower(variabale);
    if (instrucitons_two_operand.find(variabale) != instrucitons_two_operand.end() ||
        instrucitons_one_operand.find(variabale) != instrucitons_one_operand.end() ||
        instrucitons_branch.find(variabale) != instrucitons_branch.end() ||
        instrucitons_no_operand.find(variabale) != instrucitons_no_operand.end() ||
        instructions_sub_routine.find(variabale) != instructions_sub_routine.end()) {

        return true;
    }

    return false;
}

bool Validator::isVariable(string operand) {
    if (variables_table.find(operand) != variables_table.end())
        return true;

    return false;
}

bool Validator::isLabel(string operand) {
    if (labels_table.find(operand) != labels_table.end())
        return true;

    return false;
}

bool Validator::isIndex(string str, int16_t& index) {
    int tmp = 632767;
    string2int(str, tmp);

    if (tmp < -32768 || tmp > 32767)
        return false;

    index = tmp;
    return true;
}

int parser::isRegister(string operand) {
    if(operand.size() != 2)
        return -1;

    operand = toLower(operand);

    if (operand[0] != 114)
        return -1;

    if (operand[1] >= 48 && operand[1] <= 55)
        return operand[1] - 48;

    return -1;
}
/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/
bool Validator::isTwoOpsInstruction(string instruction) {
    instruction = toLower(instruction);
    if (instrucitons_two_operand.find(instruction) != instrucitons_two_operand.end())
        return true;

    return false;
}

bool Validator::isOneOpInstruction(string instruction) {
    instruction = toLower(instruction);
    if (instrucitons_one_operand.find(instruction) != instrucitons_one_operand.end())
        return true;

    return false;
}

bool Validator::isBranchInstruction(string instruction) {
    instruction = toLower(instruction);
    if (instrucitons_branch.find(instruction) != instrucitons_branch.end())
        return true;

    return false;
}

bool Validator::isNoOpInstruction(string instruction) {
    instruction = toLower(instruction);
    if (instrucitons_no_operand.find(instruction) != instrucitons_no_operand.end())
        return true;

    return false;
}

bool Validator::isSubRoutineInstruction(string instruction) {
    instruction = toLower(instruction);
    if (instructions_sub_routine.find(instruction) != instructions_sub_routine.end())
        return true;

    return false;
}
