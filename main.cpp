#include <iostream>
#include <string>
#include "parser.h"
using namespace std;

int main (int argc, char** argv) {
    if (argc > 4) {
        cout << "Error: invalid number of arguments see --help for extra information" << endl;
        return -1;
    }
    if (argc == 1) {
        cout << "Error: missing input file name" << endl;
        return -1;
    }

    string help = "I'm too lazy to type it now :D";
    string parameter1 = argv[1];
    string parameter2 = (argc == 3) ? argv[2] : "";
    string input_file;
    string output_file = "output.bin";
    if (parameter1 == "-o" || parameter1 == "--output") {
        if (argc < 3) {
            cout << "Error: missing parameter for " << parameter1 << " option" << endl;
            return -1;
        }
        if (argc < 4) {
            cout << "Error: missing input file name" << endl;
        }

        input_file = argv[3];
        output_file = argv[2];
    }
    else if (parameter1 == "-h" || parameter1 == "--help") {
        if (argc > 2) {
            cout << "Error: " << parameter1 << " doesn't take any parameters" << endl;
            return -1;
        }

        cout << help << endl;
        return 0;
    }
    else if (parameter1[0] == '-') {
        cout << "Error: " << parameter1 << " is not a valid option see --help for extra information" << endl;
        return -1;
    }
    else {
        input_file = argv[1];
        if (argc == 3 && (parameter2 == "-o" || parameter2 == "--output")) {
            cout << "Error: missing paramaters for option " << argv[2] << endl;
            return -1;
        }
        else if (argc == 3 && (parameter2 == "-h" || parameter2 == "--help"))
            cout << help << endl;
        else if (argc == 4 && (parameter2 == "-o" || parameter2 == "--output"))
            output_file = argv[3];
        else if (argc >= 3) {
            cout << "Error: " << argv[2] << " is not a valid option" << endl;
            return -1;
        }
    }

    parser p;
    p.parse(input_file, output_file);

    return 0;
}
