#include "../include/Symbol.hpp"
#include "../include/Section.hpp"
#include "../include/ReallocationTable.hpp"

int Symbol::symbolID = 0;
vector<Symbol*> Symbol::symbolList;

Symbol::Symbol(string name, int value, Scope scope, bool defined, Section* section, SymbolType type){
    Section::getUndefinedSection();
    this->id = ++symbolID;
    this->name = name;
    this->value = value;
    this->type = type;
    this->scope = scope;
    this->defined = defined;
    this->section = section;
    symbolList.push_back(this);
    
}

Symbol::Symbol(string name, int value, Scope scope, bool defined, Section* section, SymbolType type, bool undSection){
    this->id = 0;
    this->name = name;
    this->value = value;
    this->type = type;
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
            if(entry->type == FlinkType::LITERAL_POOL){
                if(entry->section == section){
                    entry->location += section->getSize();
                }
            }

            if(entry->section == section){
            // TODO proveri da li su dobro ugnjezdene operacije
                if(symbol->getScope() == Scope::LOCAL){
                    section->getReallocationTable()->addReallocation(entry->location, 
                        Symbol::getSymbol(symbol->getSection()->getName()), ReallocationType::LOCAL_SYM_REALLOC_THIRTY_TWO_BIT);
                }else if(symbol->getScope() == Scope::EXTERN){
                    section->getReallocationTable()->addReallocation(entry->location, symbol, ReallocationType::EXT_SYM_REALLOC);
                }else if(symbol->getScope() == Scope::GLOBAL){
                    section->getReallocationTable()->addReallocation(entry->location, symbol, ReallocationType::GLOB_SYM_REALLOC);
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
            std::cerr<<"Error - Assembler: Symbol not defined:" + symbol->getName()<<endl;
            cout<<"------------------------------------------"<<endl;
            return -1;
        }

        if(symbol->scope == Scope::EXTERN){

            symbol->scope = Scope::GLOBAL;
        }else{

            string hexValue = intToHexString(symbol->value);
            for(FLinkEntry* entry : symbol->flinks){
                if(entry->type == FlinkType::LITERAL_POOL || entry->type == FlinkType::THIRTY_TWO_BIT_VALUE){

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
    return 0;
}


void Symbol::printSymbolList(){
    std::cout<<"-------------------------Symbol table-------------------------------------"<<endl;
    std::cout<<"      Name      Type     Value          Scope           Section        Defined"<<endl;
    for(Symbol* symbol:symbolList){
        cout<<symbol->name<<"        "<<symbol->type<<"        "<<symbol->value<<"        "<<symbol->scope<<"        "
            <<symbol->section->getId()<<"        "<<symbol->defined<<endl;
    }
    std::cout<<"--------------------------------------------------------------------------"<<endl;
}


Symbol::~Symbol(){}