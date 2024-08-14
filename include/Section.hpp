#ifndef SECTION_HPP_
#define SECTION_HPP_

#include <vector>
#include <iostream>
#include "LiteralPool.hpp"

using namespace std;

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
    LiteralPool* literalPool = new LiteralPool();

    Section(string name, int number);

public:
    Section(std::string name);
    
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

    static void appendCode(string code){
        activeSection->code.append(code);
    }

    static std::vector<Section*> getSectionList(){
        return sectionList;
    }

    static void printSectionList();

    ~Section();
};


#endif