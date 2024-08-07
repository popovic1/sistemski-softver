#ifndef SYMBOL_HPP_
#define SYMBOL_HPP_

#include <string>
#include "Section.hpp"
#include <iostream>

using namespace std;

enum Scope{
    LOCAL, GLOBAL
};

class Symbol{
private:
    string name;
    int value;
    Section* section;
    Scope scope;
    bool defined;

    static vector<Symbol*> symbolList;

public:
    Symbol(string name, int value, Scope scope, bool defined, Section* section);

    ~Symbol();

    static void printSymbolList();

};

#endif