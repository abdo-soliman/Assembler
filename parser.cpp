#include "parser.h"

parser::parser() {
    inVariablesArea = false;
}

void parser::parse(std::string input_filename, std::string output_filename) {
    const std::size_t w = 2;
    std::ofstream temp("binaryStrings.txt");
    std::vector<std::uint16_t> data;
    input_file.open(input_filename);

    firstPass();
    secondPass();

    for (unsigned int i = 0; i < output.size(); i++) {
        temp << output[i] << "\n";
        data.push_back(std::stoi(output[i].c_str(), nullptr, 2));
    }

    write(output_filename, data, w*data.size());
    temp.close();
    input_file.close();
}
/*-----------------------------------------------------------------------------------------*/
bool parser::isValidVariable(std::string variabale, std::string& error) {
    if (variables_table.find(variabale) != variables_table.end()) {
        error = "variable *" + variabale + "* is already defined at line " + std::to_string(variables_table.find(variabale)->second);
        return false;
    }

    if (labels_table.find(variabale) != labels_table.end()) {
        error = "variable *" + variabale + "* is already used as a label at line " + std::to_string(labels_table.find(variabale)->second);
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
bool parser::isKeyWord(std::string variabale) {
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

bool parser::isVariable(std::string operand) {
    if (variables_table.find(operand) != variables_table.end())
        return true;

    return false;
}

bool parser::isLabel(std::string operand) {
    if (labels_table.find(operand) != labels_table.end())
        return true;

    return false;
}

bool parser::isIndex(std::string str, std::int16_t& index) {
    int tmp = 632767;
    string2int(str, tmp);

    if (tmp < -32768 || tmp > 32767)
        return false;

    index = tmp;
    return true;
}

int parser::isRegister(std::string operand) {
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
bool parser::isTwoOpsInstruction(std::string instruction) {
    instruction = toLower(instruction);
    if (instrucitons_two_operand.find(instruction) != instrucitons_two_operand.end())
        return true;

    return false;
}

bool parser::isOneOpInstruction(std::string instruction) {
    instruction = toLower(instruction);
    if (instrucitons_one_operand.find(instruction) != instrucitons_one_operand.end())
        return true;

    return false;
}

bool parser::isBranchInstruction(std::string instruction) {
    instruction = toLower(instruction);
    if (instrucitons_branch.find(instruction) != instrucitons_branch.end())
        return true;

    return false;
}

bool parser::isNoOpInstruction(std::string instruction) {
    instruction = toLower(instruction);
    if (instrucitons_no_operand.find(instruction) != instrucitons_no_operand.end())
        return true;

    return false;
}

bool parser::isSubRoutineInstruction(std::string instruction) {
    instruction = toLower(instruction);
    if (instructions_sub_routine.find(instruction) != instructions_sub_routine.end())
        return true;

    return false;
}
/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/
std::string parser::regCode(int reg) {
    if (reg == 0)
        return "000";
    if (reg == 1)
        return "001";
    if (reg == 2)
        return "010";
    if (reg == 3)
        return "011";
    if (reg == 4)
        return "100";
    if (reg == 5)
        return "101";
    if (reg == 6)
        return "110";

    return "111";
}

bool parser::line2fields(const std::string& s, std::vector<std::string>& tokens, char delimiter, int& two_lines_label) {
    int num_fields = 0;
    std::string token;
    std::istringstream tokenStream(s);

    while (std::getline(tokenStream, token, delimiter)) {
        if (num_fields > 5)
            return false;

        if ((token.size() == 1 && token[0] == delimiter) || token.size() == 0)
            continue;
        else if (token[0] == ';')
            break;
        else {
            tokens.push_back(token);
            num_fields++;
        }

        if (num_fields == 2 && toLower(tokens[0]) == "define") {
            std::getline(tokenStream, token);
            stringErase(token, ' ');
            if(token.find(';') != -1) {
                token = token.substr(0, token.find(';'));
            }
            tokens.push_back(token);
            break;
        }
    }

    if (num_fields == 1 && tokens.size() == 1 && (tokens[0])[(tokens[0]).size() - 1] == ':') {
        if(two_lines_label)
            return false;

        std::string label = tokens[0];
        label = label.substr(0, label.size() - 1);
        tmp_label = label;
        two_lines_label = 1;
    }

    return true;
}

bool parser::readLine(std::string line, int& line_number, std::string& error, int& two_lines_label) {
    // skip empty lines
    if (line.size() == 0) {
        --line_number;
        return true;
    }

    // skip comment lines
    if (line[0] == ';') {
        --line_number;
        return true;
    }

    stringReplaceAll(line, '\t', ' ');
    std::vector<std::string> splited_line;

    if (line2fields(line, splited_line, ' ', two_lines_label)) {
        if (two_lines_label == 1) {
            --line_number;
            two_lines_label++;
            return true;
        }
        if (two_lines_label == 2) {
            splited_line.insert(splited_line.begin(), tmp_label);
            two_lines_label = 0;
        }
        if (toLower(splited_line[0]) == "define") {
            inVariablesArea = true;
            if (splited_line.size() != 3) {
                error = "invalid number of arguments expected 2 arguments after keyword DEFINE, found " + std::to_string(splited_line.size()) + " instead";
                return false;
            }

            if (isValidVariable(splited_line[1], error))
                variables_table.insert({splited_line[1], line_number});
            else
                return false;
        }
        else if (inVariablesArea) {
            error = "expected DEFINE at line " + std::to_string(line_number) + " found *" + splited_line[0] + "* instead";
            return false;
        }
        else if (isKeyWord(splited_line[0]) && splited_line.size() >= 1 && splited_line.size() <= 4) {
            return true;
        }
        else {
            if (splited_line.size() < 3) {
                error = "invalid number of fields in lines starting with label must be a least 3";
                return false;
            }

            std::string label = splited_line[0];
            if (label[label.size() - 1] == ':')
                label = label.substr(0, label.size() - 1);

            if (isValidVariable(label, error))
                labels_table.insert({label, line_number});
            else
                return false;
        }
    }
    else {
        error = "invalid number of fields";
        return false;
    }

    return true;
}

bool parser::parseLine(std::string line, int& line_number, std::string& error, int& two_lines_label) {
    // skip empty lines
    if (line.size() == 0) {
        --line_number;
        return true;
    }

    // skip comment lines
    if (line[0] == ';') {
        --line_number;
        return true;
    }

    stringReplaceAll(line, '\t', ' ');
    std::vector<std::string> splited_line;

    if (line2fields(line, splited_line, ' ', two_lines_label)) {
        if (two_lines_label == 1) {
            --line_number;
            two_lines_label++;
            return true;
        }
        if (two_lines_label == 2) {
            splited_line.insert(splited_line.begin(), tmp_label);
            two_lines_label = 0;
        }
        if (toLower(splited_line[0]) == "define") {
            return parseVariables(splited_line, line_number, error);
        }
        else if (isKeyWord(splited_line[0])) {
            return parseKeyWords(splited_line, line_number, error);
        }
        else {
            std::string label = splited_line[0];
            if (label[label.size() - 1] == ':')
                label = label.substr(0, label.size() - 1);

            if (isLabel(label)) {
                labels_table[label] = line_number;
                splited_line.erase(splited_line.begin());
                return parseKeyWords(splited_line, line_number, error);
            }
            else {
                error = "invalid instruction '" + splited_line[0] + "' at line " + std::to_string(line_number);
                return false;
            }
        }
    }
    else {
        error = "invalid number of fields";
        return false;
    }

    return true;
}
/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/
bool parser::parseOperandAuto(std::string operand, std::string& mode, std::string& reg_code, int line_number, std::string& error, bool indirect) {
    std::string reg;

    if (std::count(operand.begin(), operand.end(), '(') != 1 ||
        std::count(operand.begin(), operand.end(), ')') != 1 ||
        operand.find('(') > operand.find(')')) {
        error = operand + " is not a valid operand at line " + std::to_string(line_number);
        return false;
    }

    else if (operand[0] == '(' && operand.substr(operand.size() - 2) == ")+") {
        mode = (indirect) ? "101" : "001";
        reg = operand.substr(1, operand.size() - 3);

        if (isRegister(reg) != -1)
            reg_code = regCode(isRegister(reg));
        else {
            error = "expected a register name after ( at line " + std::to_string(line_number) + " found *" + reg + "* instead";
            return false;
        }
    }

    else if (operand.substr(0, 2) == "-(" && operand[operand.size() -1] == ')') {
        mode = (indirect) ? "110" : "010";
        reg = operand.substr(2, operand.size() - 3);

        if (isRegister(reg) != -1)
            reg_code = regCode(isRegister(reg));
        else {
            error = "expected a register name after ( at line " + std::to_string(line_number) + " found *" + reg + "* instead";
            return false;
        }
    }

    return true;
}

bool parser::parseOperandIndexed(std::string operand, std::string& mode, std::string& reg_code, std::int16_t& index, std::string& variable, int line_number, std::string& error, bool indirect) {
    index = 0;
    variable = "";
    std::vector<std::string> data;

    if (operand[operand.size() - 1] != ')') {
        error = "expected ) at the end of operand at line " + std::to_string(line_number) + "found *" + operand[operand.size()-  1] + "* instead";
        return false;
    }

    operand.erase(operand.end()-1);
    split(operand, data, '(');

    if (data.size() != 2) {
        error = "invalid indexing mode in operand '" + operand + "', index not found. At line " + std::to_string(line_number);
        return false;
    }

    if (isIndex(data[0], index))
        mode = (indirect) ? "111" : "011";
    else if (isVariable(data[0])){
        mode = (indirect) ? "111" : "011";
        variable = data[0];
    }
    else {
        error = "invalid index *" + data[0] + "* provided in operand '" + operand + "'. At line " + std::to_string(line_number);
        return false;
    }

    if (isRegister(data[1]) != -1)
        reg_code = regCode(isRegister(data[1]));
    else {
        error = "expected a register name after ( at line " + std::to_string(line_number) + " found *" + data[1] + "* instead";
        return false;
    }

    return true;
}

bool parser::parseOperand(std::string operand, std::string& code, std::int16_t& index, std::string& variable, int line_number, std::string& error, bool source, bool indirect) {
    std::string mode = "";
    std::string reg_code = "";
    variable = "";
    index = 0;

    if (isRegister(operand) != -1) {
        reg_code = regCode(isRegister(operand));
        mode = (indirect) ? "100" : "000";
    }
    else if (isVariable(operand)) {
        variable = operand;
        reg_code = "111";
        mode = (indirect) ? "111" : "011";
    }
    else if (operand[0] == '#' && indirect) {
        error = "Can't use imediate value with indirect indexed mode";
        return false;
    }
    else if (operand[0] == '#' && isIndex(operand.substr(1), index) && source) {
        reg_code = "111";
        mode = "100";
    }
    else if (!parseOperandAuto(operand, mode, reg_code, line_number, error, indirect))
        return false;
    else if(reg_code == "" && mode == "") {
        if (!parseOperandIndexed(operand, mode, reg_code, index, variable, line_number, error, indirect))
            return false;
    }

    code = mode + reg_code;
    return true;
}

bool parser::parseTwoOpsInstruction(std::vector<std::string> instruction, int& line_number, std::string& error) {
    std::int16_t index_1;
    std::int16_t index_2;
    std::string code_1;
    std::string code_2;
    std::string variable_1;
    std::string variable_2;
    bool indirect_1 = false;
    bool indirect_2 = false;
    std::string operands = "";
    std::vector<std::string> operands_vec;

    for (unsigned int i = 1; i < instruction.size(); i++)
        operands += instruction[i];

    split(operands, operands_vec, ',');

    if (operands_vec.size() != 2) {
        error = "invalid number of operands for instruction '" + instruction[0] + "' at line " + std::to_string(line_number);
        return false;
    }

    if ((operands_vec[0])[0] == '@') {
        indirect_1 = true;
        operands_vec[0] = (operands_vec[0]).substr(1);
    }
    if ((operands_vec[1])[0] == '@') {
        indirect_2 = true;
        operands_vec[1] = (operands_vec[1]).substr(1);
    }

    if (!(parseOperand(operands_vec[0], code_1, index_1, variable_1, line_number, error, true, indirect_1) && parseOperand(operands_vec[1], code_2, index_2, variable_2, line_number, error, false, indirect_2)))
        return false;

    output.push_back(instrucitons_two_operand.find(toLower(instruction[0]))->second + code_1 + code_2);

    if (code_1 == "100111") {
        line_number++;
        output.push_back(std::bitset< 16 >(index_1).to_string());
    }

    if (code_1.substr(1,2) == "11") {
        if (variable_1 != "")
            fix_variables_table.insert({variable_1, line_number});

        output.push_back(std::bitset< 16 >(index_1).to_string());
        line_number++;
    }

    if (code_2.substr(1,2) == "11") {
        if (variable_2 != "")
            fix_variables_table.insert({variable_2, line_number});

        output.push_back(std::bitset< 16 >(index_2).to_string());
        line_number++;
    }

    return true;
}

bool parser::parseOneOpInstruction(std::vector<std::string> instruction, int& line_number, std::string& error) {
    std::int16_t index;
    std::string code;
    std::string variable;

    if (instruction.size() == 2) {
        bool indirect = ((instruction[1])[0] == '@') ? true : false;
        if (!parseOperand(instruction[1], code, index, variable, line_number, error, false, indirect))
            return false;
    }
    else if (instruction.size() == 1) {
        error = "expected operand after " + instruction[0] + "at line " + std::to_string(line_number) + " found new line instead";
        return false;
    }
    else if (instruction.size() > 2) {
        error = "expected new line after " + instruction[1] + "at line " + std::to_string(line_number) + " found *" + instruction[2] + "* instead";
        return false;
    }

    output.push_back(instrucitons_one_operand.find(toLower(instruction[0]))->second + code);

    if (code.substr(1,2) == "11") {
        if (variable != "")
            fix_variables_table.insert({variable, line_number});

        output.push_back(std::bitset< 16 >(index).to_string());
        line_number++;
    }

    return true;
}

bool parser::parseBranchInstruction(std::vector<std::string> instruction, int line_number, std::string& error) {
    char is_reg = '0';
    std::pair<int, bool> p;
    std::string code;

    if (instruction.size() == 2) {
        if (isLabel(instruction[1])) {
            p.first = line_number-1;
            p.second = true;
            fix_labels_table.insert({instruction[1], p});
            code = "00000000";
        }
        else if (isRegister(instruction[1])) {
            is_reg = '1';
            code = "00000" + regCode(isRegister(instruction[1]));
        }
        else {
            error = "expected label name after " + instruction[0] + "at line " + std::to_string(line_number) + " found *" + instruction[1] + "*instead";
            return false;
        }
    }
    else if (instruction.size() == 1) {
        error = "expected label name after " + instruction[0] + "at line " + std::to_string(line_number) + " found new line instead";
        return false;
    }
    else {
        error = "expected new line after " + instruction[1] + "at line " + std::to_string(line_number) + " found *" + instruction[2] + "* instead";
        return false;
    }

    output.push_back(instrucitons_branch.find(toLower(instruction[0]))->second + is_reg + code);
    return true;
}

bool parser::parseNoOpInstruction(std::vector<std::string> instruction, int line_number, std::string& error) {
    if (instruction.size() != 1) {
        error = "expected new line after " + instruction[0] + " at line " + std::to_string(line_number) + " found *" + instruction[1] + "*instead";
        return false;
    }

    output.push_back(instrucitons_no_operand.find(toLower(instruction[0]))->second);
    return true;
}

bool parser::parseSubRoutineInstruction(std::vector<std::string> instruction, int line_number, std::string& error) {
    char is_reg = '0';
    std::string code;
    std::pair<int, bool> p;
    std::string key_word = toLower(instruction[0]);

    if (key_word == "jsr") {
        if (instruction.size() == 2 && isLabel(instruction[1])) {
            p.first = line_number-1;
            p.second = false;
            fix_labels_table.insert({instruction[1], p});
            code = "00000000000";
        }
        else if (instruction.size() == 2 && isRegister(instruction[1])) {
            is_reg = '1';
            code = "00000000" + regCode(isRegister(instruction[1]));
        }
        else if (instruction.size() == 1) {
            error = "expected label name after jsr instruction at line " + std::to_string(line_number) + " found new line instead";
            return false;
        }
        else {
            error = "expected label name after jsr instruction at line " + std::to_string(line_number) + " found *" + instruction[1] + "* instead";
            return false;
        }
    }
    else if (instruction.size() == 1 && (key_word == "rts" || key_word == "iret")) {
        output.push_back(instructions_sub_routine.find(key_word)->second);
        return true;
    }
    else {
        error = "expected new line after instruction " + instruction[0] + " at line " + std::to_string(line_number) + " found *" + instruction[1] + "* instead";
        return false;
    }

    output.push_back(instructions_sub_routine.find("jsr")->second + is_reg + code);
    return true;
}
/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/
bool parser::parseVariables(std::vector<std::string> instruction, int& line_number, std::string& error) {
    std::int16_t value;
    std::string string_data = "";
    std::vector<std::string> data;
    std::vector<std::int16_t> values;

    for (unsigned int i = 2; i < instruction.size(); i++)
        string_data += instruction[i];

    stringErase(string_data, ' ');
    split(string_data, data, ',');

    for (unsigned int i = 0; i < data.size(); i++) {
        if((data[i].size() <= 5 || (data[i].size() == 6 && data[i][0] == '-')) && isIndex(data[i], value)) {
            values.push_back(value);
            continue;
        }

        error = "syntax error: *" + data[i] + "* is not a word at line " + std::to_string(line_number);
        return false;
    }

    variables_table[instruction[1]] = line_number;

    for (unsigned int i = 0; i < values.size(); i++) {
        output.push_back(std::bitset< 16 >(values[i]).to_string());
        line_number++;
    }

    --line_number;
    return true;
}

bool parser::parseKeyWords(std::vector<std::string> instruction, int& line_number, std::string& error) {
    if (isTwoOpsInstruction(instruction[0]))
        return parseTwoOpsInstruction(instruction, line_number, error);

    else if (isOneOpInstruction(instruction[0]))
        return parseOneOpInstruction(instruction, line_number, error);

    else if (isBranchInstruction(instruction[0]))
        return parseBranchInstruction(instruction, line_number, error);

    else if (isNoOpInstruction(instruction[0]))
        return parseNoOpInstruction(instruction, line_number, error);

    else if (isSubRoutineInstruction(instruction[0]))
        return parseSubRoutineInstruction(instruction, line_number, error);

    error = "invalid instruction *" + instruction[0] + "* at line " + std::to_string(line_number);
    return false;
}
/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/
void parser::firstPass() {
    int line_number = 0;
    std::string line;
    std::string error = "";
    int two_lines_label = 0;
    if (input_file.is_open()) {
        while(std::getline(input_file, line)) {
            line_number++;
            if (line.size() > 255)
                throw std::logic_error("syntax error: line size exceeded, number of characters must less then or equal to 255");
            if(!readLine(line,line_number, error, two_lines_label))
                throw std::logic_error("syntax error: " + error + " at line: " + std::to_string(line_number));
        }
    }
    else {
        throw std::runtime_error("invalid file");
    }

    input_file.clear();
    input_file.seekg(0, std::ios::beg);
}

void parser::secondPass() {
    int line_number = 0;
    std::string line;
    std::string error = "";
    int two_lines_label = 0;

    if (input_file.is_open()) {
        while(std::getline(input_file, line)) {
            line_number++;
            if(!parseLine(line,line_number, error, two_lines_label))
                throw std::logic_error("syntax error: " + error + " at line: " + std::to_string(line_number));
        }
    }
    else {
        throw std::runtime_error("invalid file");
    }

    for (std::map<std::string, int>::iterator it = fix_variables_table.begin(); it != fix_variables_table.end(); it++) {
        std::int16_t offset;
        int tmp = ((variables_table[it->first] - it->second - 1) * 2) - 2;

        if (isIndex(std::to_string(tmp), offset))
            output[it->second] = std::bitset< 16 >(offset).to_string();
        else
            throw std::logic_error("semantics error: variable offset value out of range at line " + std::to_string(it->second));
    }

    for (std::map<std::string, std::pair<int, bool> >::iterator it = fix_labels_table.begin(); it != fix_labels_table.end(); it++) {
        int offset = ((labels_table[it->first] - (it->second).first - 1) * 2) - 2;
        std::string code;
        if ((it->second).second) {
            code = (output[(it->second).first]).substr(0, 8);
            if (offset >= -128 && offset <= 127)
                output[(it->second).first] = code + std::bitset< 8 >(offset).to_string();
            else
                throw std::logic_error("semantics error: variable offset value out of range at line " + std::to_string((it->second).first));
        }
        else {
            code = (output[(it->second).first]).substr(0, 5);
            if (offset >= -1024 && offset <= 1023)
                output[(it->second).first] = code + std::bitset< 11 >(offset).to_string();
            else
                throw std::logic_error("semantics error: variable offset value out of range at line " + std::to_string((it->second).first));
        }
    }
}
/*-----------------------------------------------------------------------------------------*/
