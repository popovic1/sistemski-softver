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

    static vector<LinkerSymbol*> globalSymbolTable;

public:
    LinkerSymbol(int fileID, string id, string value, string type, string scope, string sectionID, string name);

    ~LinkerSymbol();

    static LinkerSymbol* getSymbol(string name);

    static LinkerSymbol* getLinkerSymbol(int fileID, int symbolID){
        for(LinkerSymbol* s : symbolList){
            if(s->fileID == fileID && s->id == symbolID){
                return s;
            }
        }
        return nullptr;
    }

    static LinkerSymbol* getGlobalSymbol(string name){
        for(LinkerSymbol* s : globalSymbolTable){
            if(s->name == name){
                return s;
            }
        }
        return nullptr;
    }

    static void printGlobalSymTable();

    string getName(){
        return name;
    }

    int getFileID(){
        return fileID;
    }

    string getValue(){
        return value;
    }

    bool isResolved(){
        return resolved;
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

    int getSectionID(){
        return sectionID;
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

    static vector<LinkerSymbol*> getGlobalSymbolTable(){
        return globalSymbolTable;
    }

    static int addToGlobalSymTable(LinkerSymbol* s){
        if(s->scope == Scope::GLOBAL){
            for(LinkerSymbol* sym : globalSymbolTable){
                if(sym->getName() == s->getName() && sym->getScope() == Scope::GLOBAL){
                    cout<<"------------------------------------------------"<<endl;
                    std::cerr << "Error - Linker: Global symbol with the same name already exists: "<<sym->getName() << std::endl;
                    cout<<"------------------------------------------------"<<endl;
                    return -1;
                }
            }
        }
        globalSymbolTable.push_back(s);
        return 0;
    }

    static string intToHexString(int num) {
        stringstream ss;
        ss << std::hex << num; // Convert integer to hexadecimal
        return ss.str();       // Return the hexadecimal string
    }

    

};

#endif