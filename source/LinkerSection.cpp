#include "../include/LinkerSection.hpp"
#include "../include/LinkerSymbol.hpp"


std::vector<LinkerSection*> LinkerSection::sectionList;

int LinkerSection::findSectionID(int fileID, string name){
    for(LinkerSection* sctn : sectionList){
        if(sctn->fileID == fileID && sctn->name == name){
            return sctn->id;
        }
    }
    return -1;
}

LinkerSection::LinkerSection(int fileID, int id, std::string name){
    this->name = name;
    this->id = id;
    this->fileID = fileID;
    sectionList.push_back(this);
}


void LinkerSection::printCode(){
    for(int i = 0; i<code.length(); i += 8){
        cout<<code.substr(i, 8)<<endl;
    }
}



void LinkerSection::modifyCode(int startPosition, int length, string modifiedCode){
    for(int i = 0; i<length; i++){
        this->code[startPosition + i] = modifiedCode[i];
    }
}

LinkerSection::~LinkerSection(){}
