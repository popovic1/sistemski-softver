#include "Parser.hpp"
#include "Section.hpp"
#include "Symbol.hpp"
#include <string>

using namespace std;

#ifndef ASSEMBLER_HPP_
#define ASSEMBLER_HPP_

enum Operations {
    HALT, INT, IRET, CALL, RET, JMP, BEQ, BNE, BGT, PUSH, POP, XCHG, ADD, SUB,
    MUL, DIV, NOT, AND, OR, XOR, SHL, SHR, LD, ST, CSRRD, CSRWR, ERR_INS
};

// enum Directives {
//     GLOBAL, EXTERN, SECTION, WORD, SKIP, END, ERROR
// };

class Assembler{
private:
    int currentLocation = 0;

public:
    Assembler();

    void compile(string inputFileName, string outputFileName);

    int handleDirectives(std::vector<string> parsedLine);
    int handleInstructions(std::vector<string> parsedLine);

    bool addSymbolToTheSymbolList(string symbol, int value, Scope scope, bool defined, Section* section);

    bool isValidIdentifier(const std::string& name);

    int isNumber(std::string arg);

    int hexToDecimal(string hex);
    string decimalToLittleEndianHexString(int num);
    string intToHexString(int num);

    int isRegister(const std::string& reg);
    int isCSR(string csr);

    ~Assembler();
};

#endif



