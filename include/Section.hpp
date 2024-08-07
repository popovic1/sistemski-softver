#ifndef SECTION_HPP_
#define SECTION_HPP_

#include <vector>
#include <iostream>

using namespace std;

class Section{
private:

    static Section *activeSection;
    static std::vector<Section*> sectionList;
    static int numberOfSections;
    int id;
    std::string name;
    int size = 0;
    std::string code = "";

public:
    Section(std::string name);
    
    static Section* getActiveSection(){
        return activeSection;
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

    void appendCode(string code){
        this->code.append(code);
    }

    static std::vector<Section*> getSectionList(){
        return sectionList;
    }

    static void printSectionList();

    ~Section();
};


#endif