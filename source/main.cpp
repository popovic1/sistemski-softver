#include <stdio.h>
#include <iostream>
#include <string>
#include "../include/Parser.hpp"
#include "../include/Assembler.hpp"

using namespace std;

int main(int argc, char** argv)
{
    char* o = argv[1];
    char* outputFileName = argv[2];
    char* inputFileName = argv[3];
    
    if (strcmp(o, "-o") != 0 && (!inputFileName || !outputFileName)){
        cout << "Command not valid." << endl;
        return -1;
    }

    Assembler *assembler = new Assembler();
    assembler->compile(inputFileName, outputFileName);

}