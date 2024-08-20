#ifndef ASSEMBLER_HPP_
#define ASSEMBLER_HPP_

#include "Parser.hpp"
#include "Section.hpp"
#include "Symbol.hpp"
#include <string>

using namespace std;

class Assembler{
private:
    int currentLocation = 0;

public:
    Assembler();

    void compile(string inputFileName, string outputFileName);

    int handleDirectives(std::vector<string> parsedLine);
    int handleInstructions(std::vector<string> parsedLine);

    bool addSymbolToTheSymbolList(string symbol, int value, Scope scope, bool defined, Section* section, SymbolType type);

    bool isValidIdentifier(const std::string& name);

    int isNumber(std::string arg);

    int hexToDecimal(string hex);
    string decimalToLittleEndianHexString(int num);
    string intToHexString(int num);

    int isRegister(const std::string& reg);
    int isCSR(string csr);

    void writeToFile(string inputFileName, string outputFileName);

    ~Assembler();
};

#endif



