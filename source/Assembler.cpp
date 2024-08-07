#include "../include/Assembler.hpp"


Assembler::Assembler(){}

int Assembler::handleDirectives(std::vector<string> parsedLine){

    if(parsedLine[0] == ".global")
    {

    }
    else if (parsedLine[0] == ".extern")
    {
        
    }
    else if (parsedLine[0] == ".section")
    {
        if(parsedLine.size()!=2){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }
        // if(parsedLine[1].find('.')!=0){
        //     cout<<"--------------------------------------------------------------"<<endl;
        //     cerr<<"Error: Invalid section name. It must start with a '.'"<<endl;
        //     cout<<"--------------------------------------------------------------"<<endl;
        //     return -1;
        // }

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
        
    }
    else if (parsedLine[0] == ".skip")
    {
        // uvecaj currentLocation   
    }
    else if (parsedLine[0] == ".end")
    {
        if(Section::getActiveSection()!= nullptr){
            //ovo mozda ne treba ako uvecavam size posle svake instrukcije
            Section::getActiveSection()->setSize(currentLocation);
        }
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

bool Assembler::addSymbolToTheSymbolList(string symbol, int value, Scope scope, bool defined, Section* section){
    // TODO check if the symbol can be added and is valid (i.e. can't be 343ff55)
    if(isValidIdentifier(symbol) == false){
        cout<<"-----------------------------------------"<<endl;
        std::cerr<<"Error: identifier name is not valid: " + symbol<<endl;
        cout<<"-----------------------------------------"<<endl;
        return false;
    }
    //TODO check if the symbol already exists and what shall be done in that case?
    Symbol* newSymbol = new Symbol(symbol, value, scope, defined, section);
    return true;
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
                std::cerr<<"Error: Label needs to be in a section."<<endl;
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
            if(success !=0)return;
        
        }else{
            // instructions 
            int success = handleInstructions(parsedLine);
            if(success !=0)return;
            currentLocation +=4;       
        }

    }

    Section::printSectionList();
    Symbol::printSymbolList();
}