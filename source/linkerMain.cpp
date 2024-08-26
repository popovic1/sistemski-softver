#include <stdio.h>
#include <iostream>
#include <string.h>
#include "../include/Linker.hpp"

using namespace std;

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        cout<<"--------------------------------------------------------------"<<endl;
        cerr<<"Error - Linker: Insufficient arguments."<<endl;
        cout<<"--------------------------------------------------------------"<<endl;
        return -1;
    }

    bool hexArg = false;
    bool outputArg = false;
    Linker* linker = new Linker();

    for(int i = 1; i < argc; i++)
    {
        string current = argv[i];

        if(current == "-hex") hexArg = true;
        else if(current == "-o")
        {
            if(argc <= i + 1){
                cout<<"--------------------------------------------------------------"<<endl;
                cerr<<"Error - Linker: No output file in arguments."<<endl;
                cout<<"--------------------------------------------------------------"<<endl;
                return -1;
            }
            outputArg = true;
            linker->setOutputFileName(argv[++i]);
        }
        else if(current.find("-place=") == 0)
        {
            int addressIndex = current.find("@") + 1;
            if(addressIndex <= 7)
            {
                cout<<"--------------------------------------------------------------"<<endl;
                cerr<<"Error - Linker: Invalid place argument."<<endl;
                cout<<"--------------------------------------------------------------"<<endl;
                return -1;
            }
            string section = current.substr(7, addressIndex - 8);
            string address = current.substr(addressIndex);
            linker->addPlace(section, address);
        }
        else
        {
            linker->addInputFileName(current);
        }
    }

    if(!hexArg)
    {
        cout<<"--------------------------------------------------------------"<<endl;
        cerr<<"Error - Linker: No -hex argument."<<endl;
        cout<<"--------------------------------------------------------------"<<endl;
        return -1;
    }

    if(!outputArg)
    {
        cout<<"--------------------------------------------------------------"<<endl;
        cerr<<"Error - Linker: No output file argument."<<endl;
        cout<<"--------------------------------------------------------------"<<endl;
        return -1;
    }

    if(linker->getInputFileNames().size() == 0)
    {
        cout<<"--------------------------------------------------------------"<<endl;
        cerr<<"Error - Linker: No input files provided."<<endl;
        cout<<"--------------------------------------------------------------"<<endl;
        return -1;
    }

    linker->link();

    return 0;
}