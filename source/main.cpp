using namespace std;

#include <stdio.h>
#include <iostream>
#include <string>
#include "../include/Parser.hpp"
#include "../include/Assembler.hpp"

int main(int argc, char** argv)
{
    char* o = argv[1];
    char* outputNilename = argv[2];
    char* inputFileName = argv[3];

     if (strcmp(o, "-o") != 0 && (!inputFileName || !outputNilename))        
    {
        cout << "Command not valid." << endl;
        return -1;
    }

    Parser *parser = new Parser();
    Assembler *assembler = new Assembler();

}