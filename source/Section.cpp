#include "../include/Section.hpp"
#include "../include/Symbol.hpp"
#include "../include/ReallocationTable.hpp"
#include "../include/LiteralPool.hpp"

Section *Section::activeSection = nullptr;

Section *Section::undefinedSection = nullptr;

std::vector<Section*> Section::sectionList;

int Section::numberOfSections = 0;

Section::Section(std::string name){
    this->name = name;
    this->id = ++numberOfSections;
    new Symbol(name, 0, Scope::LOCAL, true, this, SymbolType::SCTN);
    literalPool = new LiteralPool();
    reallocationTable = new ReallocationTable();
    sectionList.push_back(this);
}

Section::Section(string name, int number){
    this->name = name;
    this->id = number;
    new Symbol(name, 0, Scope::LOCAL, true, this, SymbolType::SCTN);
    literalPool = new LiteralPool();
    reallocationTable = new ReallocationTable();    
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

            
        }
        
    }
    Symbol::updateFLinkLocationsInLiteralPool(this);
    addLiteralPoolToCode();
    
    return 0;
}

void Section::addLiteralPoolToCode(){
    for(int i = 0; i< literalPool->getAllEntries().size(); i++){
        LiteralPoolEntry* entry = literalPool->getEntryByLocation(i*4);
        if(entry == nullptr){
            cout<<"------------------------------------------"<<endl;
            std::cerr<<"Error: Literal pool wasn't populated properly. Missing a location." <<endl;
            cout<<"------------------------------------------"<<endl;
            return;
        }
        string hex = entry->value;

        if(isNumber(hex) == -1){
            hex = "00000000";
        }

        if(hex.length()>8){
            cout<<"------------------------------------------"<<endl;
            std::cerr<<"Error: Entry to the literal pool bigger than 4 bytes." <<endl;
            cout<<"------------------------------------------"<<endl;
            return;
        }

        while(hex.length()<8){
            hex = "0" + hex;
        }

        appendCode(hex);

    }
}

int Section::isNumber(std::string arg) {
    if (arg.empty()) {
        return -1; // Error: empty string
    }

    // Check for hexadecimal format (starts with "0x" or "0X")
    if (arg.size() > 2 && arg[0] == '0' && (arg[1] == 'x' || arg[1] == 'X')) {
        // Check if the rest of the string are valid hex digits
        for (size_t i = 2; i < arg.size(); ++i) {
            if (!isxdigit(arg[i])) {
                return -1; // Error: invalid hex number
            }
        }
        return 1; // It's a hexadecimal number
    }

    // Check for decimal format
    for (size_t i = 0; i < arg.size(); ++i) {
        if (!isdigit(arg[i])) {
            return -1; // Error: invalid decimal number
        }
    }

    return 0; // It's a decimal number
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

// void Section::determineSectionSizesWithLiteralPools(){
//     for(int i = 0; i < Section::getSectionList().size(); i ++){
//         Section* section = Section::getSection(i);
//         section->size += section->getLiteralPool()->getAllEntries().size() * 4;
//     }
// }

void Section::modifyCode(int startPosition, int length, string modifiedCode){
    for(int i = 0; i<length; i++){
        this->code[startPosition + i] = modifiedCode[i];
    }
}

Section::~Section(){}
