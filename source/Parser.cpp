#include "../include/Parser.hpp"

Parser::Parser(){}

std::vector<std::string> Parser::readLinesFromFile(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return lines;
    }

    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    
    file.close();
    return lines;
}

std::vector<std::string> Parser::splitLine(const std::string& line, char separator) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    
    while (std::getline(ss, token, separator)) {
        tokens.push_back(token);
    }
    
    return tokens;
}