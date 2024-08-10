#include "../include/Symbol.hpp"


vector<Symbol*> Symbol::symbolList;

Symbol::Symbol(string name, int value, Scope scope, bool defined, Section* section){
    this->name = name;
    this->value = value;
    this->scope = scope;
    this->defined = defined;
    this->section = section;
    symbolList.push_back(this);
    
}

Symbol* Symbol::getSymbol(string name){
    for(Symbol* tmp: symbolList){
        if(tmp->name == name)
            return tmp;
    }
    return nullptr;
}

void Symbol::printSymbolList(){
    std::cout<<"-------------------------Symbol table-------------------------------------"<<endl;
    std::cout<<"      Name           Value          Scope           Section        Defined"<<endl;
    for(Symbol* symbol:symbolList){
        cout<<symbol->name<<"        "<<symbol->value<<"        "<<symbol->scope<<"        "
            <<symbol->section->getId()<<"        "<<symbol->defined<<endl;
    }
    std::cout<<"--------------------------------------------------------------------------"<<endl;
}


Symbol::~Symbol(){}