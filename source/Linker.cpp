#include "../include/Linker.hpp"
#include "../include/LinkerSymbol.hpp"
#include "../include/LinkerSection.hpp"
#include "../include/LinkerReallocation.hpp"

int Linker::fileNumber = 0;

void Linker::addPlace(string key, string value){
    places[key] = value;
}

void Linker::setOutputFileName(string name){
    outputFileName = name;
}

void Linker::addInputFileName(string name){
    inputFileNames.push_back(name);
}

vector<string> Linker::getInputFileNames(){
    return inputFileNames;
}

std::vector<std::string> Linker::splitLine(const std::string& line, char separator) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    
    while (std::getline(ss, token, separator)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

// Remove commas and whitespaces from the beginning and end of a string
std::string Linker::trimString(const std::string& str) {
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

int Linker::isNumber(std::string arg) {
    if (arg.empty()) {
        return -1; // Error - Assembler: empty string
    }

    // Check for hexadecimal format (starts with "0x" or "0X")
    if (arg.size() > 2 && arg[0] == '0' && (arg[1] == 'x' || arg[1] == 'X')) {
        // Check if the rest of the string are valid hex digits
        for (size_t i = 2; i < arg.size(); ++i) {
            if (!isxdigit(arg[i])) {
                return -1; // Error - Assembler: invalid hex number
            }
        }
        return 1; // It's a hexadecimal number
    }

    // Check for decimal format
    for (size_t i = 0; i < arg.size(); ++i) {
        if (!isdigit(arg[i])) {
            return -1; // Error - Assembler: invalid decimal number
        }
    }

    return 0; // It's a decimal number
}

int Linker::loadDataFromFiles(){

    for(string inputFile : inputFileNames)
    {
        std::vector<std::string> lines;
        std::ifstream file(inputFile);
        
        if (!file.is_open()) {
            cout<<"------------------------------------------------"<<endl;
            std::cerr << "Error - Linker: Unable to open file: " << inputFile << std::endl;
            cout<<"------------------------------------------------"<<endl;
            return -1;
        }

        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        
        file.close();

        if(lines.size() == 0){
            cout<<"------------------------------------------------"<<endl;
            std::cerr << "Error - Linker: Empty file: " << inputFile << std::endl;
            cout<<"------------------------------------------------"<<endl;
            return -1;
        }


        int thisFileID = ++fileNumber;
        int stageOfFile = 0;
        int cnt = 0;
        int cntRela = 0;
        int numOfSymbols = 0;
        int numOfSections = 0;
        int numOfReallocations = 0;
        int currSectionID = 0;
        vector<string> parsedLine;
        for(string line : lines){

            parsedLine = splitLine(line, '\t');

            for(int i = 0; i < parsedLine.size(); i++){
                parsedLine[i] = trimString(parsedLine[i]);
            }

            while(line.length()>0 && parsedLine[0] == ""){
                parsedLine.erase(parsedLine.begin());
            }
            if(line.length() == 0)continue;

            if(stageOfFile == 0) // file name and empty line
            {  
                stageOfFile = 1;
                continue;

            }else if(stageOfFile == 1) // symbol table
            {
                if(parsedLine[0].find("#") == 0)
                {

                    numOfSymbols = std::stoi(parsedLine[1]);
                    // cout<<parsedLine[0] <<" - "<<parsedLine[1]<<endl;
                    // cout<<"Number of symbols in stage 1: "<<numOfSymbols<<endl;
                    if(numOfSymbols == 0)
                        stageOfFile = 2;
                        continue;
    
                }
                else if(parsedLine[0] == "ID"){}
                else
                {

                    new LinkerSymbol(thisFileID, parsedLine[0], parsedLine[1], parsedLine[2], parsedLine[3], parsedLine[4], parsedLine[5]);

                    cnt++;

                    if(cnt == numOfSymbols){
                        cnt = 0;
                        stageOfFile = 2;
                        continue;
                    }
                }
            }else if(stageOfFile == 2) //sections and code
            {
                if(parsedLine[0] == "[]")
                {
                    numOfSections = stoi(parsedLine[2]);
                    // std::cout<<"Number of sections in stage 2: "<<numOfSections<<endl;
                    if(numOfSections == 0){
                        stageOfFile = 3;
                        continue;
                    }
                }
                else if(parsedLine[0].find("#") == 0)
                {
                    string sectionName = parsedLine[0].substr(2);
                    currSectionID = LinkerSection::findSectionID(thisFileID, sectionName);
                }
                else
                {

                    LinkerSection* sctn = LinkerSection::getLinkerSection(thisFileID, currSectionID);
                    sctn->setCode(parsedLine[0]);
                    cnt ++;
                    if(cnt == numOfSections){
                        cnt = 0;
                        stageOfFile = 3;
                        continue;
                    }
                }


            }else if(stageOfFile == 3) // reallocation tables
            {
                if(parsedLine[0] == "[]")
                {
                    numOfSections = stoi(parsedLine[2]);
                    // std::cout<<"Number of sections in stage 3: "<<numOfSections<<endl;
                    if(numOfSections == 0){
                        stageOfFile = 4;
                        continue;
                    }
                }
                else if (parsedLine[0].find("#") == 0)
                {
                    
                    string sectionName = parsedLine[0].substr(7);
                    currSectionID = LinkerSection::findSectionID(thisFileID, sectionName);
                    numOfReallocations = stoi(parsedLine[1]);
                    cnt++;
                    
                }
                else if(parsedLine[0] == "Offset"){}
                else
                {
                    
                    new LinkerReallocation(thisFileID, parsedLine[0], parsedLine[1], parsedLine[2]);
                    cntRela++;

                    if(cntRela == numOfReallocations){
                        cntRela = 0;
                        if(cnt == numOfSections){
                            cnt = 0;
                            stageOfFile = 4;
                            continue;
                        }

                    }
                }
            }
            else{
                cout<<"------------------------------------------------"<<endl;
                std::cerr << "Error - Linker: Line not handled: " << line << std::endl;
                cout<<"------------------------------------------------"<<endl;
                return -1;
            }
            
        }

    }
    return 0;
}

int Linker::mapSections(){
    loadDataFromFiles();
    return 0;
}

int Linker::resolveSymbolValues(){
    return 0;

}

int Linker::resolveReallocations(){
    return 0;
}

void Linker::link(){
    mapSections();
}