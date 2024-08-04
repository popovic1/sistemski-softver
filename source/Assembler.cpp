#include "../include/Assembler.hpp"

Assembler::Assembler(){}

void Assembler::compile(string inputFileName, string outputFileName){

    Parser *parser = new Parser();  

    std::vector<string> allLines = parser->readLinesFromFile(inputFileName);

    for(string line : allLines){

        // handle comments
        // if # is found, delete everything after it, then check if the line is empty
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
            //labela: ...
            //dodaj novi simbol u tabelu simbola, ako vec ne postoji. Ako postoji, vrati gresku
            parsedLine.erase(parsedLine.begin());
        }
        if(parsedLine.empty())continue;

        if(parsedLine.front().find('.') == 0){
            // direktive (.section, .equ itd.)
        
        }else{
            // instrukcije        
        }

        

        cout<<'\n';
    }
}