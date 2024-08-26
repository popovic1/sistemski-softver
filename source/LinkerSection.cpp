#include "../include/LinkerSection.hpp"
#include "../include/LinkerSymbol.hpp"


std::vector<LinkerSection*> LinkerSection::sectionList;
std::vector<LinkerSection*> LinkerSection::mappedSectionsList;

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

void LinkerSection::printSectionList(){
    cout << "FileID\tSectionID\tName\tMapped\tStartAddr\tSize"<<endl;
    for(LinkerSection* sctn : sectionList){
        cout << sctn->fileID << "\t" << sctn->id << "\t" << sctn->name << "\t" << sctn->mapped << "\t" << sctn->startAdress << "\t" << sctn->size <<endl;
    }
}

LinkerSection::~LinkerSection(){}
