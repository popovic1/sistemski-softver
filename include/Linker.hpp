#ifndef LINKER_HPP_
#define LINKER_HPP_

#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

class Linker{
private:
    static int numberOfFIles;
    static int nextSectionAddr;

    unordered_map<string, string> places;
    string outputFileName;
    vector<string> inputFileNames;
public:
    Linker(){}

    void addPlace(string key, string value);

    void setOutputFileName(string name);

    void addInputFileName(string name);

    vector<string> getInputFileNames();

    std::vector<std::string> splitLine(const std::string& line, char separator);

    std::string trimString(const std::string& str);

    int isNumber(std::string arg);
    int hexToDecimal(string hex);
    uint32_t hexToUint32(const std::string& hex);
    std::string uint32ToHexString(uint32_t value);

    bool overlap(uint32_t start1, uint32_t size1, uint32_t start2, uint32_t size2);

    int loadDataFromFiles();

    void writeToFile();

    void link();

    int mapSections();

    int resolveSymbolValues();

    int resolveReallocations();

};

#endif