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

// Remove commas and whitespaces from the beginning and end of a string
std::string Parser::trimString(const std::string& str) {
    // Include all whitespace characters and comma in the chars string
    const std::string chars = " \t\n\r\f\v,";

    // Find the position of the first character not in the trim set
    size_t start = str.find_first_not_of(chars);
    if (start == std::string::npos) {
        return ""; // The string is all whitespace or commas
    }

    // Find the position of the last character not in the trim set
    size_t end = str.find_last_not_of(chars);

    // Return the substring from start to end
    return str.substr(start, end - start + 1);
}