#include "../include/Assembler.hpp"

Assembler::Assembler(){}

void Assembler::handleDirectives(std::vector<string> parsedLine){
    cout<<"Direktiva\n";

    if(parsedLine[0] == ".global")
    {

    }
    else if (parsedLine[0] == ".extern")
    {
        
    }
    else if (parsedLine[0] == ".section")
    {
        
    }
    else if (parsedLine[0] == ".word")
    {
        
    }
    else if (parsedLine[0] == ".skip")
    {
        
    }
    else if (parsedLine[0] == ".end")
    {
        
    }
    
}

void Assembler::handleInstructions(std::vector<string> parsedLine){
    cout<<"Instrukcija\n";

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
}

void Assembler::addSymbolToTheSymbolTable(string symbol){
    // TODO check if the symbol can be added and is valid (i.e. can't be 343ff55)
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
        cout<<line + '\n';
        // parse a single line
        std::vector<string> parsedLine = parser->splitLine(line, ' ');

        // trim all the words in the line
        for(int i = 0; i < parsedLine.size(); i++){
            parsedLine[i] = parser->trimString(parsedLine[i]);
        }

        if(parsedLine.front().find(':') != std::string::npos){
            //label: ...

            // label needs to be in a section
            if(!Section::getActiveSection()){
                // TODO generate an error
            }

            //add a symbol to the symbol table
            parsedLine[0].pop_back();  // remove the ":" at the end of a label
            addSymbolToTheSymbolTable(parsedLine[0]);

            //delete the first word of the line in case there is an instruction after the label
            parsedLine.erase(parsedLine.begin());
        }
        if(parsedLine.empty())continue;

        if(parsedLine.front().find('.') == 0){
            // directives (.section, .equ itd.)
            handleDirectives(parsedLine);
        
        }else{
            // instructions 
            handleInstructions(parsedLine);       
        }

    }
}