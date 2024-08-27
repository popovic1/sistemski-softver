#include "../include/Linker.hpp"
#include "../include/LinkerSymbol.hpp"
#include "../include/LinkerSection.hpp"
#include "../include/LinkerReallocation.hpp"

int Linker::numberOfFIles = 0;
int Linker::nextSectionAddr = 1073741824;

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

int Linker::hexToDecimal(string hex){
    int decimal = 0;
    for (size_t i = 2; i < hex.size(); ++i) {
        char digit = toupper(hex[i]); 
        decimal *= 16; 

        if (digit >= '0' && digit <= '9') {
            decimal += digit - '0';
        } else if (digit >= 'A' && digit <= 'F') {
            decimal += digit - 'A' + 10;
        } else {
            cout<<"---------------------------------"<<endl;
            cerr << "Error - Linker: Invalid hexadecimal digit: " << digit <<endl;
            cout<<"---------------------------------"<<endl;
            return -1; 
        }
    }

    return decimal;
}

uint32_t Linker::hexToUint32(const std::string& hex) {
    return static_cast<uint32_t>(std::stoul(hex, nullptr, 16));
}

bool Linker::overlap(uint32_t start1, uint32_t size1, uint32_t start2, uint32_t size2) {
    int end1 = start1 + size1;
    int end2 = start2 + size2;

    return (start1 < end2) && (start2 < end1);
}

std::string Linker::uint32ToHexString(uint32_t value) {
    std::stringstream ss;
    ss << std::hex << std::setw(8) << std::setfill('0') << value;
    return ss.str();
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


        int thisFileID = ++numberOfFIles;
        int stageOfFile = 0;
        int cnt = 0;
        int cntRela = 0;
        int numOfSymbols = 0;
        int numOfSections = 0;
        int numOfReallocations = 0;
        int currSectionID = 0;
        int currSectionSize = 0;
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
                    currSectionSize = stoi(parsedLine[1]);
                }
                else
                {

                    LinkerSection* sctn = LinkerSection::getLinkerSection(thisFileID, currSectionID);
                    sctn->setCode(parsedLine[0]);
                    sctn->setSize(currSectionSize);
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
                    
                    new LinkerReallocation(thisFileID, parsedLine[0], parsedLine[1], parsedLine[2], currSectionID);
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

void Linker::writeToFile() {
    // Sort the mapped sections by startAddress
    std::vector<LinkerSection*> sortedSections = LinkerSection::getMappedSectionsList();

    // Sort the new vector by startAddress
    std::sort(sortedSections.begin(), sortedSections.end(),
        [](LinkerSection* a, LinkerSection* b) {
            return a->getStartAdress() < b->getStartAdress();
        });

    std::ofstream outFile(this->outputFileName);
    if (!outFile.is_open()) {
        std::cerr << "Error opening file: " << outputFileName << std::endl;
        return;
    }

    // Previous section information
    uint32_t prevEndAddress = 0;
    bool firstSection = true;
    size_t remainingBytesInLine = 8; // Track how many bytes are left in the current line

    for (auto* section : sortedSections) {
        string code = section->toLittleEndian();
        uint32_t address = section->getStartAdress();
        size_t codeIndex = 0;

        while (codeIndex < code.length()) {
            if (!firstSection && address == prevEndAddress && remainingBytesInLine > 0) {
                // If the current section starts immediately after the previous one, continue on the same line
                size_t bytesToWrite = std::min(remainingBytesInLine * 2, code.length() - codeIndex); // Multiply by 2 because each byte is represented by two characters

                for (size_t j = 0; j < bytesToWrite; j += 2) {
                    outFile << code.substr(codeIndex + j, 2) << " ";
                }

                codeIndex += bytesToWrite;
                remainingBytesInLine -= bytesToWrite / 2;
                address += bytesToWrite / 2;
            } else {
                // Start a new line if either the sections are not contiguous or the line is full
                if (!firstSection) {
                    outFile << std::endl;
                }
                
                // Write the address in hex format
                outFile << std::hex << std::setw(8) << std::setfill('0') << address << ": ";
                
                size_t bytesInLine = std::min(size_t(8), (code.length() - codeIndex) / 2);
                remainingBytesInLine = 8 - bytesInLine;

                for (size_t j = 0; j < bytesInLine * 2; j += 2) {
                    outFile << code.substr(codeIndex + j, 2) << " ";
                }

                codeIndex += bytesInLine * 2;
                address += bytesInLine;
            }

            prevEndAddress = address;
            firstSection = false;
        }
    }

    outFile.close();
}

int Linker::mapSections(){

    for(const auto& place : places){ // resolve -place argument by putting sections in those spots
        string sectionName = place.first;
        string location = place.second;
        uint32_t locationInt = hexToUint32(location);
        bool found = false;

        for(int i = 1; i <= numberOfFIles; i++){

            int sctnID = LinkerSection::findSectionID(i, sectionName);
            if(sctnID == -1)continue;
            LinkerSection* sctn = LinkerSection::getLinkerSection(i, sctnID);
            found = true;

            sctn->map(locationInt);
            if(locationInt + sctn->getSize() > nextSectionAddr){
                nextSectionAddr = locationInt + sctn->getSize();
            }
            locationInt += sctn->getSize();

        }
        if(!found){
            cout<<"------------------------------------------------"<<endl;
            std::cerr << "Error - Linker: Section not found for the place argument: " << std::endl;
            cout<<"------------------------------------------------"<<endl;
            return -1;
        }
    }

    // check if they overlap
    if(LinkerSection::getMappedSectionsList().size()!=0){

        for(LinkerSection* sctn : LinkerSection::getMappedSectionsList()){

            for(LinkerSection* tmp : LinkerSection::getMappedSectionsList()){
                if(sctn == tmp)continue;

                if(overlap(sctn->getStartAdress(), sctn->getSize(), tmp->getStartAdress(), tmp->getSize())){
                    cout<<"------------------------------------------------"<<endl;
                    std::cerr << "Error - Linker: Sections overlap: " <<sctn->getFileID() << ":" << sctn->getName()<<
                        " and "<<tmp->getFileID() << ":" << tmp->getName()<< std::endl;
                    cout<<"------------------------------------------------"<<endl;
                    return -1;
                }
            }
        }
    }
    // map all other sections
    for(int i = 1; i <= numberOfFIles; i++){
        for(LinkerSection* sctn : LinkerSection::getSectionList()){
            // find the next section to be mapped
            if(sctn->getFileID() == i && !sctn->isMapped() && sctn->getId() != 0){
                sctn->map(nextSectionAddr);
                nextSectionAddr += sctn->getSize();

                // find sections in other files that have the same name
                for(int j = i; j <= numberOfFIles; j++){

                    for(LinkerSection* tmp : LinkerSection::getSectionList()){
                        if(tmp->getFileID() == j && !tmp->isMapped() && tmp->getName() == sctn->getName()){
                            tmp->map(nextSectionAddr);
                            nextSectionAddr += tmp->getSize();
                        }
                    }
                }
            }
        }
    }

    return 0;
}

int Linker::resolveSymbolValues(){

    for(int i = 1; i <= numberOfFIles; i++){
        for(LinkerSymbol* sym : LinkerSymbol::getAllSymbols()){
            if(sym->getFileID() == i && sym->getType() == LinkerSymbolType::NOTYP && !sym->isResolved() && sym->isDefined()){
                //resolve symbol
                int sectionID = sym->getSectionID();
                LinkerSection* sctn = LinkerSection::getLinkerSection(i, sectionID);
                uint32_t addend = sctn->getStartAdress();

                uint32_t finalValue = addend + hexToUint32(sym->getValue());

                sym->setValue(uint32ToHexString(finalValue));
                sym->setResolved(true);
                int success = LinkerSymbol::addToGlobalSymTable(sym);
                if(success == -1)return -1;

            }
        }
    }
    LinkerSymbol::printGlobalSymTable();
    return 0;
}

int Linker::resolveReallocations(){
    for(LinkerReallocation* rela : LinkerReallocation::getReallocations()){
        if(rela->isResolved())continue;
        
        if(rela->getType() == ReallocationType::LOCAL_SYM_REALLOC_THIRTY_TWO_BIT || rela->getType() == ReallocationType::GLOB_SYM_REALLOC){
            LinkerSection* sctn = LinkerSection::getLinkerSection(rela->getFileID(), rela->getSectionID());
            uint32_t offset = hexToUint32(rela->getOffset());
            LinkerSymbol* symbol = LinkerSymbol::getLinkerSymbol(rela->getFileID(), rela->getSymbolID());
            if(symbol == nullptr || !symbol->isResolved()){
                cout<<"------------------------------------------------"<<endl;
                std::cerr << "Error - Linker: No such symbol in rela: " << rela->getSymbolID()<< std::endl;
                cout<<"------------------------------------------------"<<endl;
                return -1;
            }
            string correctValue = symbol->getValue();
            string code = sctn->getCode();
            for(int i = 0; i<8; i++){
                code[offset*2 + i] = correctValue[i];
            }

            sctn->setCode(code);
            rela->setResolved(true);

        }else if(rela->getType() == ReallocationType::EXT_SYM_REALLOC){
            LinkerSection* sctn = LinkerSection::getLinkerSection(rela->getFileID(), rela->getSectionID());
            uint32_t offset = hexToUint32(rela->getOffset());
            
            LinkerSymbol* undSymbol = LinkerSymbol::getLinkerSymbol(rela->getFileID(), rela->getSymbolID());
            string symName = undSymbol->getName();
            cout<<"Offset to symbol: " << symName << " " << offset<<endl;
            LinkerSymbol* symbol = LinkerSymbol::getGlobalSymbol(symName);
            if(symbol == nullptr || !symbol->isResolved()){
                cout<<"------------------------------------------------"<<endl;
                std::cerr << "Error - Linker: No such symbol in rela: " << rela->getSymbolID()<< std::endl;
                cout<<"------------------------------------------------"<<endl;
                return -1;
            }
            string correctValue = symbol->getValue();
            string code = sctn->getCode();

            cout<<code.substr(offset, 8)<<endl;

            for(int i = 0; i<8; i++){
                code[offset*2 + i] = correctValue[i];
            }

            sctn->setCode(code);
            rela->setResolved(true);
        }else{
            cout<<"------------------------------------------------"<<endl;
            std::cerr << "Error - Linker: Unsupported reallocation type: " << rela->getType()<< std::endl;
            cout<<"------------------------------------------------"<<endl;
            return -1;
        }
    }
    return 0;
}

void Linker::link(){
    loadDataFromFiles();
    mapSections();
    resolveSymbolValues();
    resolveReallocations();
    writeToFile();
}