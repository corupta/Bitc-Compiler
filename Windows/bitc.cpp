#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <set>

#define endrn "\r\n"
// endl for windows

using namespace std;

bool errOnLine = false;
// this is true if the current line has a syntax error

set<string> variableNames;
// a set to hold unique variable names
ofstream outFile;
// ofstream to print the .asm file

/*
 * given a variable name t, (such as $x)
 * put it into the set variableNames
 * return the variable name for assembly, (such as vx)
 */
string addVariable(string t) {
    string res = "v" + t.substr(1);
    variableNames.insert(res);
    return res;
}

/*
 * prints all variable definitions for assembly
 */
void printVariables() {
    outFile << ";;;\tvariables\t;;;" << endrn;
    for (__typeof(variableNames.begin()) it = variableNames.begin(); it != variableNames.end(); ++it) {
        outFile << "\t" << *it << " dw 0" << endrn;
    }
}

/*
 * removes all whitespace / control characters in the given string str
 * also changes any uppercase into its lowercase one
 */
string removeWhitespace(string &str) {
    int len = str.length();
    int newLen = 0;
    char *buffer = new char[len];
    for (int i = 0; i < len; ++i) {
        if (isspace(str[i]) == 0 && iscntrl(str[i]) == 0) {
            buffer[newLen++] = str[i];
            if (str[i] >= 'A' && str[i] <= 'Z') {
                buffer[newLen - 1] += 'o' - 'O';
            }
        }
    }
    buffer[newLen] = '\0';
    string ans = buffer;
    delete buffer;
    return ans;
}

void parseExpression (string &, int &);

/*
 * returns true if character c is a hexadecimal character
 */
bool isHexadecimal(char c) {
    return (c <= 'f' && c >= 'a') || (c <= '9' && c >= '0');
}

/*
 * returns true if character c is an alphanumeric character
 */
bool isAlphanumeric(char c) {
    return (c <= 'z' && c >= 'a') || (c <= '9' && c >= '0');
}

/*
 * parseFactor function
 *
 * tries to parse xor(expr,expr), ls(expr,expr), rs(expr,expr), lr(expr,expr), rr(expr,expr), not(expr), (expr)
 * sets errOnLine = true when matches an operation but fails to match ',' or ')'
 *
 * tries to parse a variable or an immediate value
 *
 * if the first character of the variable / immediate value is '$'
 *      that means it is matching a variable
 *      sets errOnLine = true if matches only '$' not something like '$x'
 *      sets errOnLine = true if encounters any non alphanumeric character except: ',' , '|' , '&', ')'
 *          such as ')'
 *      parses the variable until reaching ',' , '|' , '&' , ')' or the end of string.
 *
 * if the first character of the variable / immediate value is not '$'
 *      that means it is matching an immediate value
 *      sets errOnLine = true if encounters any non hexadecimal character except: ',' , '|' , '&', ')'
 *          such as ')' or 'g'
 *      parses the variable until reaching ',' , '|' , '&' , ')' or the end of string.
 *
 * prints any operation parsed, to assembly out file in a postfix notation
 */
void parseFactor(string &line, int &pos) {
    static string opCodes[] = { "xor(", "ls(", "rs(", "lr(", "rr(", "not(", "(" };
    static char opCharacters[] = { ',', '|', '&', ')' };
    static int beginOffset[] = { 4, 3, 3, 3, 3, 4, 1 };
    static int nofExpressions[] = { 2, 2, 2, 2, 2, 1, 1 };
    bool isNumOrVar = true;
    for (int i = 0 ; i < 7; ++i) {
        if (line.substr(pos, beginOffset[i]) == opCodes[i]) {
            pos += beginOffset[i];
            for (int j = 0; j < nofExpressions[i]; ++j) {
                if (j) {
                    if (line[pos] != ',') {
                        errOnLine = true;
                        return;
                    }
                    ++pos; // for skipping ','
                }
                parseExpression(line, pos);
                if (errOnLine) {
                    return;
                }
            }
            if (line[pos] != ')') {
                errOnLine = true;
                return;
            }
            ++pos;
            if (i != 6) { // (expr) does nothing to expr, so no need to pop or push anything
                if (nofExpressions[i] > 1) {
                    outFile << "\tpop cx" << endrn;
                }
                outFile << "\tpop ax" << endrn;
                switch (i) {
                    case 0: // xor(ax,cx)
                        outFile << "\txor ax, cx" << endrn;
                        break;
                        /*
                         * for rotations and shifts, since the destination is 16 bits,
                         * only the last 4 bits of the source of such operation (number of rotations / shifts)
                         * is important, as the destination number repeats itself after each 16 rotations / shifts
                         * as a result ls(ax,cx) is equivalent to ls(ax,cl) and ls(ax, last 4 bits of cl)
                         */
                    case 1: // ls(ax,cx)
                        outFile << "\tshl ax, cl" << endrn;
                        break;
                    case 2: // rs(ax, cx)
                        outFile << "\tshr ax, cl" << endrn;
                        break;
                    case 3: // lr(ax, cx)
                        outFile << "\trol ax, cl" << endrn;
                        break;
                    case 4: // rr(ax, cx)
                        outFile << "\tror ax, cl" << endrn;
                        break;
                    case 5: // not(ax)
                        outFile << "\tnot ax" << endrn;
                        break;
                    default:
                        cerr << "Weird ERROR !!!" << endrn;
                }
                outFile << "\tpush ax" << endrn;
            }
            isNumOrVar = false;
            break;
        }
    }
    if (isNumOrVar) {
        int startPos = pos;
        while (++pos < line.length()) {
            bool isEnd = false;
            for (int i = 0; i < 4; ++i) {
                if (line[pos] == opCharacters[i]) {
                    isEnd = true;
                    break;
                }
            }
            if (isEnd) {
                break;
            } else if (line[startPos] == '$' ? !isAlphanumeric(line[pos]) : !isHexadecimal(line[pos])) {
                errOnLine = true;
                return;
            }
        }
        string myNumOrVar = line.substr(startPos, pos - startPos);
        if (line[startPos] == '$') {
            if (pos == startPos) {
                errOnLine = true;
                return;
            }
            outFile << "\tpush " << addVariable(myNumOrVar) << " w" << endrn;
        } else {
            if (!isHexadecimal(line[startPos])) {
                errOnLine = true;
                return;
            }
            outFile << "\tpush 0" << myNumOrVar << "h" << endrn;
        }
    }
}

/*
 * if line[pos] is '&' parse factor and print out in assembly as an and operation.
 * return if there was a syntax error while parsing the factor
 */
void parseMoreFactors(string &line, int &pos) {
    if (pos < line.length() && line[pos] == '&') {
        ++pos;
        parseFactor(line, pos);
        if (errOnLine) {
            return;
        }
        parseMoreFactors(line, pos);
        outFile << "\tpop bx" << endrn;
        outFile << "\tpop ax" << endrn;
        outFile << "\tand ax, bx" << endrn;
        outFile << "\tpush ax" << endrn;
    }
}

/*
 * term = factor:morefactors (factor&factor&factor...)
 * parse a factor
 * if syntax error was found while parsing that factor return
 * then, try to parse more
 */
void parseTerm(string &line, int &pos) {
    parseFactor(line, pos);
    if (errOnLine) {
        return;
    }
    parseMoreFactors(line, pos);
}

/*
 * if line[pos] is '|' parse term and print out in assembly as an or operation.
 * return if there was a syntax error while parsing the term
 */
void parseMoreTerms(string &line, int &pos) {
    if (pos < line.length() && line[pos] == '|') {
        ++pos;
        parseTerm(line, pos);
        if (errOnLine) {
            return;
        }
        parseMoreTerms(line, pos);
        outFile << "\tpop bx" << endrn;
        outFile << "\tpop ax" << endrn;
        outFile << "\tor ax, bx" << endrn;
        outFile << "\tpush ax" << endrn;
    }
}

/*
 * expr = term:moreterms (term|term|term...)
 * parse a term
 * if syntax error was found while parsing that term return
 * then, try to parse more
 */
void parseExpression(string &line, int &pos) {
    parseTerm(line, pos);
    if (errOnLine) {
        return;
    }
    parseMoreTerms(line, pos);
}

/*
 * parse a line
 * if an '=' exists in the line
 *      check the left part of '=' if it is not a valid variable set error
 *      parse the right part of '=' by parseExpression
 * if no '=' exists in the line
 *      parse the whole line by parseExpression
 *      then, print the result (the only value left in the stack)
 *
 * also return if any error is encountered
 */
void parseLine(string &line) {
    int len = line.length();
    int currPos = 0;
    for (int i = 0; i < len; ++i) {
        if (line[i] == '=') {
            string myVariable = line.substr(0, i);
            bool err = false;
            if (myVariable[0] != '$') {
                err = true;
            } else {
                for (int j = 1; j < i ; ++j) {
                    if (!isAlphanumeric(myVariable[j])) {
                        err = true;
                    }
                }
            };
            if (err) {
                errOnLine = true;
                return;
            }
            ++i;
            outFile << "\tpush offset " << addVariable(myVariable) << endrn;
            parseExpression(line, i);
            if (i != len) {
                errOnLine = true;
            }
            outFile << "\tpop ax" << endrn;
            outFile << "\tpop bx" << endrn;
            outFile << "\tmov [bx], ax" << endrn;
            return;
        }
    }
    parseExpression(line, currPos);
    if (currPos != len) {
        errOnLine = true;
    }
    outFile << "\tpop ax" << endrn;
    outFile << "\tcall printword" << endrn;
}

/*
 * print the beginning line, "code segment" to the assembly file
 */
void printInitialCode() {
    outFile << "code segment" << endrn;
}

/*
 * print printer functions for assembly
 *
 * consists of 5 functions
 *
 * printword: prints the hexadecimal number at ax
 * (2 printbyte + 1 println)
 *
 * printbyte: prints the hexadecimal number at ch
 * (2 printchar)
 *
 * printchar: prints the last 4 bits of dl (prints 0-f)
 * (either prints a-f directly or 0-9 via printnum function)
 *
 * printnum: prints a number character between 0-9 in the last 4 bits of dl
 * (prints 0-9 directly)
 *
 * println: prints an endline
 * (\r\n for windows version and \n for ubuntu/linux version)
 */
void printPrinterStuff() {
    string printThose[] = {
        "printword:\t; print the hexadecimal number at ax",
            "\tmov cx, ax",
            "\tcall printbyte",
            "\tmov ch, cl",
            "\tcall printbyte",
            "\tcall println",
            "\tret",
        "printbyte: \t\t; print the hexadecimal number at ch",
            "\tmov dl, ch",
            "\tshr dl, 4",
            "\tcall printchar",
            "\tmov dl, ch",
            "\tshl dl, 4",
            "\tshr dl, 4",
            "\tcall printchar",
            "\tret",
        "printchar:\t\t; print 0-f at last 4 bits of dl",
            "\tmov ah, 02h",
            "\tcmp dl, 0ah",
            "\tjb printnum",
            "\tsub dl, 0ah",
            "\tadd dl, 97d",
            "\tint 21h",
            "\tret",
        "printnum:\t\t; print 0-9 at dl",
            "\tadd dl, 48d",
            "\tint 21h",
            "\tret",
        "println:\t\t; print a newline (\\r\\n)",
            "\tmov ah, 02h",
            "\tmov dl, 13d",
            "\tint 21h",
            "\tmov dl, 10d",
            "\tint 21h",
            "\tret"
    };
    int nofLines = 35;
    for (int i = 0; i < nofLines; ++i) {
        outFile << printThose[i] << endrn;
    }
}

/*
 * prints "int 20h" for assembly to return to os
 * prints print functions for assembly
 * prints variables
 * prints "code ends"
 */
void printEndCode() {
    outFile << "\tint 20h" << endrn;
    printPrinterStuff();
    printVariables();
    outFile << "code ends"; // << endrn;
}

/*
 * main function, works only when ./bitc [input_file].bc is called
 * gives an error if there aren't exactly one argument used with ./bitc
 * gives an error if the argument given to ./bitc does not end with .bc
 *
 * reads the [input_file].bc line by line, removes any whitespace / control characters and parses it
 * prints each parsed line in assembly, also checks if the line contains any syntax errors
 * if there is a syntax error en error is printed to cerr and program is halted, so the output is half written (not finished)
 * after printing each line, prints the print functions for assembly, and variables
 * finally prints the last line "code ends"
 */
int main (int argc, char* argv[]) {
    bool error = false;
    int len;
    string inFileName;
    if (argc == 2) {
        inFileName = argv[1];
        len = inFileName.length();
        if (inFileName.substr(len - 3).compare(".bc")) {
            error = true;
        }
    } else {
        error = true;
    }
    if (error) {
        cerr << "Run the code with the following command: ./bitc [input_file].bc" << endrn;
        return 1;
    }
    ifstream inFile(argv[1]);
    string outFileName = inFileName.substr(0, len - 3) + ".asm";
    outFile.open(outFileName.c_str());
    string line;
    printInitialCode();
    int currLineNo = 0;
    do {
        getline(inFile, line);
        line = removeWhitespace(line);
        if (line.length() > 1) {
            parseLine(line);
            ++currLineNo;
            if (errOnLine) {
                cerr << "Syntax error on line " << currLineNo << endrn;
                return 0;
            }
        }
    } while (inFile.good());
    printEndCode();
    return 0;
}