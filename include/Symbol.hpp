#ifndef SYMBOL_HPP_
#define SYMBOL_HPP_

#include <string>
#include "Section.hpp"
#include <iostream>

using namespace std;

enum Scope{
    LOCAL, GLOBAL
};

struct TNSEntry{
    int location;
    bool literalPool;
    Section* section;
};

class Symbol{
private:
    string name;
    int value;
    Section* section;
    Scope scope;
    bool defined;
    vector<TNSEntry*> tns;

    static vector<Symbol*> symbolList;

public:
    Symbol(string name, int value, Scope scope, bool defined, Section* section);

    ~Symbol();

    static Symbol* getSymbol(string name);

    static void printSymbolList();

    string getName(){
        return name;
    }

    int getValue(){
        return value;
    }

    void setValue(int value){
        this->value = value;
    }

    bool isDefined(){
        return defined;
    }

    void setDefined(bool defined){
        this->defined = defined;
    }

    Section* getSection(){
        return section;
    }

    void setSection(Section* section){
        this->section = section;
    }

    void setScope(Scope scope){
        this->scope = scope;
    }

    void addTNSEntry(int location, bool isLiteralPool, Section* section){
        TNSEntry* entry = new TNSEntry{location, isLiteralPool, section};
        tns.push_back(entry);
    }

};

#endif