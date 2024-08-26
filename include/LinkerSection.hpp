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
    int fileID;
    int id;
    std::string name;
    int size = 0;
    std::string code = "";

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

    void setSize(int size){
        this->size = size;
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

    static LinkerSection* getLinkerSection(int fileID, int id){
        for(LinkerSection* section: sectionList){
            if(section->getId() == id && section->getFileID() == fileID){
                return section;
            }
        }
        return nullptr;
    }

    void printCode();

    void modifyCode(int startPosition, int length, string modifiedCode);

    ~LinkerSection();
};


#endif