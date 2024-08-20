#ifndef SYMBOL_HPP_
#define SYMBOL_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class Section;

enum Scope{
    LOCAL, GLOBAL, EXTERN
};

enum FlinkType{
    LITERAL_POOL, TWELVE_BIT_VALUE, THIRTY_TWO_BIT_VALUE
};

enum SymbolType{
    NOTYP, SCTN
};

struct FLinkEntry{
    int location;
    Section* section;
    FlinkType type;
};

class Symbol{
private:
    static int symbolID;
    int id;
    string name;
    int value;
    SymbolType type;
    Section* section;
    Scope scope;
    bool defined;
    vector<FLinkEntry*> flinks;

    static vector<Symbol*> symbolList;

public:
    Symbol(string name, int value, Scope scope, bool defined, Section* section, SymbolType type);

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

    int getID(){
        return id;
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

    Scope getScope(){
        return scope;
    }

    string getScopeString(){
        if(scope == Scope::GLOBAL){
            return "GLOB";
        }else if(scope == Scope::LOCAL){
            return "LOC";
        }
        return "";
    }

    SymbolType getType(){
        return type;
    }

    string getTypeString(){
        if(type == SymbolType::NOTYP){
            return "NOTYP";
        }else if(type == SymbolType::SCTN){
            return "SCTN";
        }
        return "";
    }

    static vector<Symbol*> getAllSymbols(){
        return symbolList;
    }

    void addFLinkEntry(int location, Section* section, FlinkType type){
        FLinkEntry* entry = new FLinkEntry{location, section, type};
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