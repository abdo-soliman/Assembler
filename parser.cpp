#include "parser.h"

parser::parser() {
    inVariablesArea = false;
}

bool parser::isKeyWord(std::string variabale) {
    if (instrucitons_two_operand.find(variabale) != instrucitons_two_operand.end() ||
        instrucitons_one_operand.find(variabale) != instrucitons_one_operand.end() ||
        instrucitons_branch.find(variabale) != instrucitons_branch.end() ||
        instrucitons_no_operand.find(variabale) != instrucitons_no_operand.end() ||
        instructions_sub_routine.find(variabale) != instructions_sub_routine.end()) {

        return true;
    }

    return false;
}

bool parser::isValidVariable(std::string variabale, std::string& error) {
    if (variables_table.find(variabale) != variables_table.end()) {
        error = "variable *" + variabale + "* is already defined at line " + variables_table.find(variabale)->second;
        return false;
    }

    if (labels_table.find(variabale) != labels_table.end()) {
        error = "variable *" + variabale + "* is already used as a label at line " + labels_table.find(variabale)->second;
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
        for (int i = 1; i < variabale.size(); i++) {
            if (!( variabale[0] == '_' || (variabale[0] >= 65 && variabale[0] <= 90) || (variabale[0] >= 97 && variabale[0] <= 122)|| (variabale[0] >= 48 && variabale[0] <= 57) )) {
                error = "invalid variable name *" + variabale + "* can contain only upper letters, lower letters, numbers and underscore";
                return false;
            }
        }
    }

    return true;
}

bool parser::split(const std::string& s, std::vector<std::string>& tokens, char delimiter) {
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
    }

    return true;
}

bool parser::readLine(std::string line, int line_number, std::string& error) {
    // skip empty lines
    if (line.size() == 0)
        return true;

    // skip comment lines
    if (line[0] == ';')
        return true;

    std::vector<std::string> splited_line;

    if (split(line, splited_line, ' ')) {
        if (toLower(splited_line[0]) == "define") {
            inVariablesArea = true;
            if (splited_line.size() != 3) {
                error = "invalid number of arguments expected 2 arguments after keyword DEFINE, found " + std::to_string(splited_line.size()) + " instead";
                return false;
            }
            
            if (isValidVariable(splited_line[0], error))
                variables_table.insert({splited_line[0], line_number});
            else
                return false;
        }
        else if (inVariablesArea) {
            error = "expected DEFINE at line " + line_number + " found *" + splited_line[0] + "* instead";
            return false;
        }
        else if (isKeyWord(splited_line[0]) && splited_line.size() >= 2 && splited_line.size() >= 4) {
            return true;
        }
        else {
            if (splited_line.size() < 3) {
                error = "invalid number of fields in lines starting with label must be a least 3";
                return false;
            }

            if (isValidVariable(splited_line[0], error))
                labels_table.insert({splited_line[0], line_number});
            else
                return false;
        }
    }
    else {
        error = "invalid number of fields";
        return false;
    }

}

void parser::firstPass(std::ifstream file) {
    int line_number = 0;
    std::string line;
    std::string error = "";

    if (file.is_open()) {
        while(std::getline(file, line)) {
            line_number++;
            if(!readLine(line,line_number, error))
                throw std::logic_error("syntax error: " + error + " at line: " + std::to_string(line_number));
        }
    }
    else {
        throw std::runtime_error("invalid file");
    }
    
    file.clear();
    file.seekg(0, std::ios::beg);
}

bool parser::parseLine(std::string line, int line_number, std::string error) {
    // to second pass parse line into there respective binary code
}

void parser::secondPass(std::ifstream file) {
    int line_number = 0;
    std::string line;
    std::string error = "";

    if (file.is_open()) {
        while(std::getline(file, line)) {
            line_number++;
            if(!parseLine(line,line_number, error))
                throw std::logic_error("syntax error: " + error + " at line: " + std::to_string(line_number));
        }
    }
    else {
        throw std::runtime_error("invalid file");
    }
}