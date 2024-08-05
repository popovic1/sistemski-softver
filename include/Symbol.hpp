#ifndef SYMBOL_HPP_
#define SYMBOL_HPP_

#include <string>
#include "Section.hpp"

using namespace std;

class Symbol{
private:
    string name;
    int value;
    Section* section;
    int scope;
    int defined;

};

#endif