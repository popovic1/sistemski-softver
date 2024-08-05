#include "Parser.hpp"
#include "Section.hpp"
#include <string>

using namespace std;

#ifndef ASSEMBLER_HPP_
#define ASSEMBLER_HPP_

enum Operations {
    HALT, INT, IRET, CALL, RET, JMP, BEQ, BNE, BGT, PUSH, POP, XCHG, ADD, SUB,
    MUL, DIV, NOT, AND, OR, XOR, SHL, SHR, LD, ST, CSRRD, CSRWR, ERR_INS
};

enum Directives {
    GLOBAL, EXTERN, SECTION, WORD, SKIP, END, ERROR
};

class Assembler{

public:
    Assembler();

    void compile(string inputFileName, string outputFileName);

    void handleDirectives(std::vector<string> parsedLine);
    void handleInstructions(std::vector<string> parsedLine);

    void addSymbolToTheSymbolTable(string symbol);

    ~Assembler();
};

#endif



