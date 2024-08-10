#include "../include/Assembler.hpp"


Assembler::Assembler(){}

int Assembler::handleDirectives(std::vector<string> parsedLine){

    if(parsedLine[0] == ".global")
    {
        for(int i = 1; i<parsedLine.size(); i++){
            Symbol* existingSymbol = Symbol::getSymbol(parsedLine[i]);

            if(existingSymbol != nullptr){
                existingSymbol->setScope(Scope::GLOBAL);
            }else{
                bool success = addSymbolToTheSymbolList(parsedLine[i], 0, Scope::GLOBAL, false, Section::getUndefinedSection());
                if(success == false)
                    return -1;
            }
        }
    }
    else if (parsedLine[0] == ".extern")
    {
        for(int i = 1; i<parsedLine.size(); i++){
            Symbol* existingSymbol = Symbol::getSymbol(parsedLine[i]);

            if(existingSymbol != nullptr){
                // TODO ne bi trebalo da bacam gresku ovde, trebalo bi da mogu da se naprave i extern i lokalni simbol sa istim imenom,
                // ali da li postoji u testovima
                cout<<"--------------------------------------------------------------"<<endl;
                cerr<<"Error: Symbol already defined" + parsedLine[i]<<endl;
                cout<<"--------------------------------------------------------------"<<endl;
                return -1;
            }else{
                bool success = addSymbolToTheSymbolList(parsedLine[i], 0, Scope::GLOBAL, true, Section::getUndefinedSection());
                if(success == false)
                    return -1;
            }
        }
    }
    else if (parsedLine[0] == ".section")
    {
        if(parsedLine.size()!=2){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        if(isValidIdentifier(parsedLine[1].substr(1))==false){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error: Invalid section name."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        if(Section::getActiveSection()!= nullptr){
            //ovo mozda ne treba ako uvecavam size posle svake instrukcije
            Section::getActiveSection()->setSize(currentLocation);
        }

        currentLocation = 0;
        Section* newSection = new Section(parsedLine[1]);
        Section::setActiveSection(newSection);
    }
    else if (parsedLine[0] == ".word")
    {
        if(parsedLine.size()<2){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }
        
        for(int i = 1; i < parsedLine.size(); i++){
            int typeOfParam = isNumber(parsedLine[i]);
            int valueOfWord = 0;

            if(typeOfParam == -1){ //symbol
                Symbol* symbol = Symbol::getSymbol(parsedLine[i]);
                if(symbol == nullptr){
                    cout<<"--------------------------------------------------------------"<<endl;
                    cerr<<"Error: Invalid parameter:" + parsedLine[i]<<endl;
                    cout<<"--------------------------------------------------------------"<<endl;
                    return -1;
                }
                valueOfWord = symbol->getValue();
            }else if(typeOfParam == 0){ //decimal
                valueOfWord = stoi(parsedLine[i]);
            }else if(typeOfParam == 1){ //hex
                valueOfWord = hexToDecimal(parsedLine[i]);
            }else{
                cout<<"--------------------------------------------------------------"<<endl;
                cerr<<"Error: Function isNumber doesn't work properly."<<endl;
                cout<<"--------------------------------------------------------------"<<endl;
                return -1;
            }
            
            if (valueOfWord > 2147483647 || valueOfWord < -2147483648){
                cout<<"--------------------------------------------------------------"<<endl;
                cerr<<"Error: Invalid parameter. The value is out of range:" + parsedLine[i]<<endl;
                cout<<"--------------------------------------------------------------"<<endl;
                return -1;
            }
            string code = decimalToLittleEndianHexString(valueOfWord);
            Section::appendCode(code);
            currentLocation += 4;
        }
    }
    else if (parsedLine[0] == ".skip")
    {
        if(parsedLine.size() != 2){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        if(Section::getActiveSection() == nullptr){
                cout<<"------------------------------------------"<<endl;
                std::cerr<<"Error: Skip directive needs to be in a section."<<endl;
                cout<<"------------------------------------------"<<endl;
                return -1;
            }

        int isNum = isNumber(parsedLine[1]);

        if(isNum == -1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error: The argument has to be a number."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }else if(isNum == 0){
            currentLocation += stoi(parsedLine[1]);
            string code = string(stoi(parsedLine[1]) * 2, '0');
            Section::appendCode(code);
        }else if(isNum == 1){
            int decimal = hexToDecimal(parsedLine[1]);
            if(decimal == -1)return -1;
            currentLocation += decimal;
            string code = string(decimal * 2, '0');
            Section::appendCode(code);
        }
        

        

    }
    else if (parsedLine[0] == ".end")
    {
        if(Section::getActiveSection()!= nullptr){
            //ovo mozda ne treba ako uvecavam size posle svake instrukcije
            Section::getActiveSection()->setSize(currentLocation);
        }
        cout<<Section::getActiveSection()->getCode()<<endl;
        return 1;
    }
    return 0;
    
}

int Assembler::handleInstructions(std::vector<string> parsedLine){

    if(parsedLine[0] == "halt")
    {

    }
    else if (parsedLine[0] == "int")
    {
        
    }
    else if (parsedLine[0] == "iret")
    {
        
    }
    else if (parsedLine[0] == "call")
    {
        
    }
    else if (parsedLine[0] == "ret")
    {
        
    }
    else if (parsedLine[0] == "jmp")
    {
        
    }
    else if (parsedLine[0] == "beq")
    {
        
    }
    else if (parsedLine[0] == "bne")
    {
        
    }
    else if (parsedLine[0] == "bgt")
    {
        
    }
    else if (parsedLine[0] == "push")
    {
        
    }
    else if (parsedLine[0] == "pop")
    {
        
    }
    else if (parsedLine[0] == "xchg")
    {
        
    }
    else if (parsedLine[0] == "add")
    {
        
    }
    else if (parsedLine[0] == "sub")
    {
        
    }
    else if (parsedLine[0] == "mul")
    {
        
    }
    else if (parsedLine[0] == "div")
    {
        
    }
    else if (parsedLine[0] == "not")
    {
        
    }
    else if (parsedLine[0] == "and")
    {
        
    }
    else if (parsedLine[0] == "or")
    {
        
    }
    else if (parsedLine[0] == "xor")
    {
        
    }
    else if (parsedLine[0] == "shl")
    {
        
    }
    else if (parsedLine[0] == "shr")
    {
        
    }
    else if (parsedLine[0] == "ld")
    {
        
    }
    else if (parsedLine[0] == "st")
    {
        
    }
    else if (parsedLine[0] == "csrrd")
    {
        
    }
    else if (parsedLine[0] == "csrwr")
    {
        
    }

    return 0;
}

bool Assembler::addSymbolToTheSymbolList(string name, int value, Scope scope, bool defined, Section* section){
    // check if the symbol can be added and is valid (i.e. can't be 343ff55)
    if(isValidIdentifier(name) == false){
        cout<<"-----------------------------------------"<<endl;
        std::cerr<<"Error: identifier name is not valid: " + name<<endl;
        cout<<"-----------------------------------------"<<endl;
        return false;
    }


    // check if the symbol already exists
    Symbol* existingSymbol = Symbol::getSymbol(name);
    if(existingSymbol == nullptr){
        Symbol* newSymbol = new Symbol(name, value, scope, defined, section);
        return true;

    }else{

        if(existingSymbol->isDefined()){
            cout<<"-----------------------------------------"<<endl;
            std::cerr<<"Error: identifier already defined: " + name<<endl;
            cout<<"-----------------------------------------"<<endl;

            return false;
        }else{
            existingSymbol->setValue(value);
            existingSymbol->setDefined(true);
            existingSymbol->setSection(section); // TODO da li ovo treba??? - Mora
            return true;
        }
    }


    
}

bool Assembler::isValidIdentifier(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    if (!std::isalpha(name[0]) && name[0] != '_') {
        return false;
    }
    for (char ch : name) {
        if (!std::isalnum(ch) && ch != '_') {
            return false;
        }
    }

    return true;
}

int Assembler::isNumber(std::string arg) {
    if (arg.empty()) {
        return -1; // Error: empty string
    }

    // Check for hexadecimal format (starts with "0x" or "0X")
    if (arg.size() > 2 && arg[0] == '0' && (arg[1] == 'x' || arg[1] == 'X')) {
        // Check if the rest of the string are valid hex digits
        for (size_t i = 2; i < arg.size(); ++i) {
            if (!isxdigit(arg[i])) {
                return -1; // Error: invalid hex number
            }
        }
        return 1; // It's a hexadecimal number
    }

    // Check for decimal format
    for (size_t i = 0; i < arg.size(); ++i) {
        if (!isdigit(arg[i])) {
            return -1; // Error: invalid decimal number
        }
    }

    return 0; // It's a decimal number
}

int Assembler::hexToDecimal(string hex){
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
            cerr << "Error: Invalid hexadecimal digit: " << digit <<endl;
            cout<<"---------------------------------"<<endl;
            return -1; 
        }
    }

    return decimal;
}

string Assembler::decimalToLittleEndianHexString(int num) {
    unsigned int unsignedNum = static_cast<unsigned int>(num);

    std::stringstream ss;

    // Convert the unsigned integer to a hexadecimal string
    ss << std::hex << std::setw(8) << std::setfill('0') << unsignedNum;

    std::string hexStr = ss.str();

    // Convert to little endian
    std::string littleEndianHexStr;
    for (size_t i = 0; i < hexStr.size(); i += 2) {
        littleEndianHexStr.insert(0, hexStr.substr(i, 2));
    }

    return littleEndianHexStr;
}

void Assembler::compile(string inputFileName, string outputFileName){

    Parser *parser = new Parser();  

    std::vector<string> allLines = parser->readLinesFromFile(inputFileName);

    for(string line : allLines){

        // handle comments
        std::size_t hashtagPosition = line.find('#');
        if(hashtagPosition != std::string::npos){
            line.erase(hashtagPosition);
        }
        if(line.length() == 0)continue;
        // parse a single line
        std::vector<string> parsedLine = parser->splitLine(line, ' ');

        // trim all the words in the line
        for(int i = 0; i < parsedLine.size(); i++){
            parsedLine[i] = parser->trimString(parsedLine[i]);
        }

        if(parsedLine.front().find(':') != std::string::npos){
            //label: ...

            // label needs to be in a section
            if(Section::getActiveSection() == nullptr){
                cout<<"------------------------------------------"<<endl;
                std::cerr<<"Error: Label needs to be in a section:" + parsedLine[0]<<endl;
                cout<<"------------------------------------------"<<endl;
                return;
            }

            //add a symbol to the symbol table
            parsedLine[0].pop_back();  // remove the ":" at the end of a label
            bool isSymbolAdded = addSymbolToTheSymbolList(parsedLine[0], currentLocation, Scope::LOCAL, true, Section::getActiveSection());
            if(isSymbolAdded == false)return;

            //delete the first word of the line in case there is an instruction after the label
            parsedLine.erase(parsedLine.begin());
        }
        if(parsedLine.empty())continue;

        if(parsedLine.front().find('.') == 0){
            // directives (.section, .equ itd.)
            int success = handleDirectives(parsedLine);
            if(success !=0)break;
        
        }else{
            // instructions 
            int success = handleInstructions(parsedLine);
            if(success !=0)break;
            currentLocation +=4;       
        }

    }

    Section::printSectionList();
    Symbol::printSymbolList();
}