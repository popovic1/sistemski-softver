#ifndef SECTION_HPP_
#define SECTION_HPP_

#include <vector>

class Section{
private:

    static Section *activeSection;
    static std::vector<Section> sections;

public:
    static Section* getActiveSection(){
        return activeSection;
    }

    static void setActiveSection(Section *section){
        activeSection = section;
    }
};


#endif