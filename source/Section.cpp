#include "../include/Section.hpp"

Section *Section::activeSection = nullptr;

std::vector<Section*> Section::sectionList;

int Section::numberOfSections = 0;

Section::Section(std::string name){
    this->name = name;
    this->id = ++numberOfSections;
    sectionList.push_back(this);
}

void Section::printSectionList(){
    std::cout<<"-------------------------Section table-------------------------------------"<<endl;
    std::cout<<"      Id                   Name                  Size"<<endl;
    for(Section* section:sectionList){
        cout<<section->id<<"               "<<section->name<<"               "<<section->size<<endl;
    }
    std::cout<<"---------------------------------------------------------------------------"<<endl;
}

Section::~Section(){}
