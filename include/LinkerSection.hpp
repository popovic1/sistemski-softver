#ifndef LINKER_SECTION_HPP_
#define LINKER_SECTION_HPP_

#include <vector>
#include <iostream>
#include <sstream>



using namespace std;

class LinkerSymbol;

class LinkerSection{
private:
    static std::vector<LinkerSection*> sectionList;
    static std::vector<LinkerSection*> mappedSectionsList;
    int fileID;
    int id;
    std::string name;
    uint32_t size = 0;
    std::string code = "";
    bool mapped = false;
    uint32_t startAdress = 0;

public:
    LinkerSection(int fileID, int id, std::string name);

    static int findSectionID(int fileID, string name);

    int getId(){
        return id;
    }

    int getFileID(){
        return fileID;
    }

    std::string getName(){
        return name;
    }

    void setName(string name){
        this->name = name;
    }

    int getSize(){
        return size;
    }

    void setSize(uint32_t size){
        this->size = size;
    }

    void setStartAddress(uint32_t address){
        this->startAdress = address;
    }

    uint32_t getStartAdress(){
        return startAdress;
    }

    void map(int address){
        this->startAdress = address;
        mapped = true;
        mappedSectionsList.push_back(this);
    }

    bool isMapped(){
        return mapped;
    }

    string getCode(){
        return code;
    }

    void setCode(string code){
        this->code = code;
    }

    static std::vector<LinkerSection*> getSectionList(){
        return sectionList;
    }

    static std::vector<LinkerSection*> getMappedSectionsList(){
        return mappedSectionsList;
    }

    static LinkerSection* getLinkerSection(int fileID, int id){
        for(LinkerSection* section: sectionList){
            if(section->getId() == id && section->getFileID() == fileID){
                return section;
            }
        }
        return nullptr;
    }

    void printCode();

    static void printSectionList();

    void modifyCode(int startPosition, int length, string modifiedCode);

    ~LinkerSection();
};


#endif