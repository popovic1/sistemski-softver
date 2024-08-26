#ifndef LINKER_SYMBOL_HPP_
#define LINKER_SYMBOL_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class LinkerSection;

enum Scope{
    LOCAL, GLOBAL
};


enum LinkerSymbolType{
    NOTYP, SCTN
};

class LinkerSymbol{
private:
    int fileID;
    int id;
    string name;
    string value;
    LinkerSymbolType type;
    int sectionID;
    Scope scope;
    bool defined;
    bool resolved;

    static vector<LinkerSymbol*> symbolList;

public:
    LinkerSymbol(int fileID, string id, string value, string type, string scope, string sectionID, string name);

    ~LinkerSymbol();

    static LinkerSymbol* getSymbol(string name);

    static void printLinkerSymbolList();

    string getName(){
        return name;
    }

    string getValue(){
        return value;
    }

    int getID(){
        return id;
    }

    void setValue(string value){
        this->value = value;
    }

    bool isDefined(){
        return defined;
    }

    void setDefined(bool defined){
        this->defined = defined;
    }

    bool isResolved(){
        return resolved;
    }

    void setResolved(bool resolved){
        this->resolved = resolved;
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

    LinkerSymbolType getType(){
        return type;
    }

    string getTypeString(){
        if(type == LinkerSymbolType::NOTYP){
            return "NOTYP";
        }else if(type == LinkerSymbolType::SCTN){
            return "SCTN";
        }
        return "";
    }

    static vector<LinkerSymbol*> getAllSymbols(){
        return symbolList;
    }

    static string intToHexString(int num) {
        stringstream ss;
        ss << std::hex << num; // Convert integer to hexadecimal
        return ss.str();       // Return the hexadecimal string
    }

    

};

#endif