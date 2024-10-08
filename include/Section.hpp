#ifndef SECTION_HPP_
#define SECTION_HPP_

#include <vector>
#include <iostream>
#include <sstream>



using namespace std;

class ReallocationTable;
class LiteralPool;
class Symbol;

class Section{
private:

    static Section *activeSection;
    static Section *undefinedSection;
    static std::vector<Section*> sectionList;
    static int numberOfSections;
    int id;
    std::string name;
    int size = 0;
    std::string code = "";
    LiteralPool* literalPool;;
    ReallocationTable* reallocationTable;

    Section(string name, int number);

public:
    Section(std::string name);

    int finalize();
    
    static Section* getActiveSection(){
        return activeSection;
    }

    static Section* getUndefinedSection(){
        if(undefinedSection == nullptr){
            undefinedSection = new Section("UND", 0);
        }
        return undefinedSection;
    }

    static void setActiveSection(Section *section){
        activeSection = section;
    }

    static void determineSectionSizesWithLiteralPools();

    int getId(){
        return id;
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

    LiteralPool* getLiteralPool(){
        return literalPool;
    }

    ReallocationTable* getReallocationTable(){
        return reallocationTable;
    }

    static void appendCode(string code){
        activeSection->code.append(code);
        activeSection->size += 4;
    }

    void addLiteralPoolToCode();

    static std::vector<Section*> getSectionList(){
        return sectionList;
    }

    static Section* getSection(int id){
        for(Section* section: sectionList){
            if(section->getId() == id){
                return section;
            }
        }
        return nullptr;
    }

    void printCode();

    static void printSectionList();

    string intToHexString(int num);
    int isNumber(std::string arg);

    void modifyCode(int startPosition, int length, string modifiedCode);

    ~Section();
};


#endif