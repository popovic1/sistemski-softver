#include "../include/Section.hpp"
#include "../include/Symbol.hpp"

Section *Section::activeSection = nullptr;

Section *Section::undefinedSection = nullptr;

std::vector<Section*> Section::sectionList;

int Section::numberOfSections = 0;

Section::Section(std::string name){
    this->name = name;
    this->id = ++numberOfSections;
    sectionList.push_back(this);
}

Section::Section(string name, int number){
    this->name = name;
    this->id = number;
    sectionList.push_back(this);
}

int Section::finalize(){

    // resolve offsets to the literal pool

    for(LiteralPoolEntry* entry : literalPool->getAllEntries()){

        for(int pos : entry->positionsInCode){
            int offset = size - pos - 4 + entry->location;
            string hexOffset = intToHexString(offset);

            if(hexOffset.length()>3){
                cout<<"------------------------------------------"<<endl;
                std::cerr<<"Error: Offset to the literal pool can't fit into 12 bits." <<endl;
                cout<<"------------------------------------------"<<endl;
                return -1;
            }

            while (hexOffset.length() < 3) {
                hexOffset = "0" + hexOffset;
            }

            code[pos * 2 + 5] = hexOffset[0];
            code[pos * 2 + 6] = hexOffset[1];
            code[pos * 2 + 7] = hexOffset[2];

            Symbol::updateFLinkLocationsInLiteralPool(this);
        }
    }
    return 0;
}

string Section::intToHexString(int num) {
    std::stringstream ss;
    ss << std::hex << num; // Convert integer to hexadecimal
    return ss.str();       // Return the hexadecimal string
}

void Section::printCode(){
    for(int i = 0; i<code.length(); i += 8){
        cout<<code.substr(i, 8)<<endl;
    }
}

void Section::printSectionList(){
    std::cout<<"-------------------------Section table-------------------------------------"<<endl;
    std::cout<<"      Id                   Name                  Size"<<endl;
    for(Section* section:sectionList){
        cout<<section->id<<"               "<<section->name<<"               "<<section->size<<endl;
    }
    std::cout<<"---------------------------------------------------------------------------"<<endl;
}

void Section::determineSectionSizesWithLiteralPools(){
    for(int i = 0; i < Section::getSectionList().size(); i ++){
        Section* section = Section::getSection(i);
        section->size += section->getLiteralPool()->getAllEntries().size() * 4;
    }
}

void Section::modifyCode(int startPosition, int length, string modifiedCode){
    for(int i = 0; i<length; i++){
        this->code[startPosition + i] = modifiedCode[i];
    }
}

Section::~Section(){}
