#include "../include/LinkerSymbol.hpp"
#include "../include/LinkerSection.hpp"

vector<LinkerSymbol*> LinkerSymbol::symbolList;
vector<LinkerSymbol*> LinkerSymbol::globalSymbolTable;


LinkerSymbol::LinkerSymbol(int fileID, string id, string value, string type, string scope, string sectionID, string name){

    // cout<<"New LinkerSymbol: FileID: "<<fileID<<" - SymbolID: "<<id<<" - Value: "<<value<<" - Type: "<<type<<" - Scope: "<<scope<<
    //         " - SectionID: "<<sectionID<< " - Name: "<<name<<endl;
    
    this->fileID = fileID;
    this->id = stoi(id);
    this->value = value;
    this->type = type == "SCTN" ? LinkerSymbolType::SCTN : LinkerSymbolType::NOTYP;
    this->scope = scope == "GLOB" ? Scope::GLOBAL : Scope::LOCAL;
    this->sectionID = stoi(sectionID);
    this->name = name;

    // TODO da li treba ovo drugacije uraditi?
    this->defined = this->sectionID == 0 ? false : true;
    this->resolved = false;

    if(this->type == LinkerSymbolType::SCTN){
        new LinkerSection(fileID, this->id, name);
    }

    symbolList.push_back(this);
}

LinkerSymbol* LinkerSymbol::getSymbol(string name){
    for(LinkerSymbol* tmp: symbolList){
        if(tmp->name == name)
            return tmp;
    }
    return nullptr;
}

void LinkerSymbol::printGlobalSymTable(){
    cout<< "FileID\tSectionID\tSymbolID\tValue\tType\tScope\tName"<<endl;
    for(LinkerSymbol* sym : globalSymbolTable){
        cout<< sym->fileID << "\t" << sym->sectionID << "\t" << sym->id << "\t" << sym->value << "\t" << sym->type << "\t" <<
                sym->scope << "\t" << sym->name<<endl;
    }
}


LinkerSymbol::~LinkerSymbol(){}