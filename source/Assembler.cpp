#include "../include/Assembler.hpp"

Assembler::Assembler(){}

void Assembler::compile(string inputFileName, string outputFileName){

    Parser *parser = new Parser();

    std::vector<string> allLines = parser->readLinesFromFile(inputFileName);

    for(string line : allLines){

        //handle comments
        // if # is found, delete everything after it, then check if the line is empty
        std::size_t hashtagPosition = line.find('#');
        if(hashtagPosition != std::string::npos){
            line.erase(hashtagPosition);
        }

        //parse a single line
        std::vector<string> parsedLine = parser->splitLine(line, ' ');

        //handle labels
        for(string word : parsedLine){
            cout<< word + ' ';
        }
        cout<<'\n';
        //process the line based on the op code
    }
}