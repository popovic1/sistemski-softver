#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


using namespace std;

class Parser{
public:
    Parser();

    std::vector<std::string> readLinesFromFile(const std::string& filename);

    std::vector<std::string> splitLine(const std::string& line, char separator);

    std::string trimString(const std::string& str) ;

    ~Parser();
};

#endif