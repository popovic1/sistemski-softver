#ifndef SYMBOL_HPP_
#define SYMBOL_HPP_

#include <string>
#include "Section.hpp"
#include <iostream>

using namespace std;

enum Scope{
    LOCAL, GLOBAL, EXTERN
};

struct FLinkEntry{
    int location;
    bool fourBytes;
    Section* section;
    bool word;
};

class Symbol{
private:
    string name;
    int value;
    Section* section;
    Scope scope;
    bool defined;
    vector<FLinkEntry*> flinks;

    static vector<Symbol*> symbolList;

public:
    Symbol(string name, int value, Scope scope, bool defined, Section* section);

    ~Symbol();

    static Symbol* getSymbol(string name);

    static int updateFLinkLocationsInLiteralPool(Section* section); 

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

    void addFLinkEntry(int location, bool isLiteralPool, Section* section, bool word = false){
        FLinkEntry* entry = new FLinkEntry{location, isLiteralPool, section, word};
        flinks.push_back(entry);
    }

    static int resolveSymbolValuesAndFLinks();

    static string intToHexString(int num) {
        stringstream ss;
        ss << std::hex << num; // Convert integer to hexadecimal
        return ss.str();       // Return the hexadecimal string
    }

    

};

#endif