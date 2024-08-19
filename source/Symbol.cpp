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

int Symbol::updateFLinkLocationsInLiteralPool(Section* section){
    for(Symbol* symbol : symbolList){
        for(FLinkEntry* entry : symbol->flinks){
            if(entry->fourBytes && !entry->word){
                if(entry->section == section){
                    entry->location += section->getSize();
                }
            }
        }
    }
    return 0;
}

int Symbol::resolveSymbolValuesAndFLinks(){
    for(Symbol* symbol : Symbol::symbolList){
        if(!symbol->isDefined() && symbol->scope != Scope::EXTERN){
            cout<<"------------------------------------------"<<endl;
            std::cerr<<"Error: Symbol not defined:" + symbol->getName()<<endl;
            cout<<"------------------------------------------"<<endl;
            return -1;
        }

        if(symbol->scope == Scope::EXTERN){
            // TODO kako napraviit relokaciju za extern simbole
            symbol->scope = Scope::GLOBAL;

        }else{
            // ovde razresavam sve flinkove, ali ih ne stavljam u realloc tabelu? Ili ipak stavljam i za global i za lokal
            string hexValue = intToHexString(symbol->value);
            for(FLinkEntry* entry : symbol->flinks){
                if(entry->fourBytes){

                    while(hexValue.length()<8){
                        hexValue = "0" + hexValue;
                    }

                    entry->section->modifyCode(entry->location*2, 8, hexValue);
                }else{
                    while(hexValue.length()<3){
                        hexValue = "0" + hexValue;
                    }

                    entry->section->modifyCode(entry->location*2 + 5, 3, hexValue);
                }
            }

        }

        
    }
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