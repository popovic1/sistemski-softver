#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

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
};

#endif



