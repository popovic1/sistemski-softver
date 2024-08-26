#ifndef LINKER_REALLOCATION_HPP_
#define LINKER_REALLOCATION_HPP_

#include <string>
#include <vector>
#include <iostream>

using namespace std;

enum ReallocationType{
    EXT_SYM_REALLOC, LOCAL_SYM_REALLOC_THIRTY_TWO_BIT, LOCAL_SYM_REALLOC_TWELVE_BIT, GLOB_SYM_REALLOC, 
    GLOB_SYM_REALLOC_TWELVE_BIT, EXT_SYM_REALLOC_TWELVE_BIT
};

class LinkerReallocation{

private:
    static vector<LinkerReallocation*> reallocations;
    string offset;
    ReallocationType type;
    int symbolID;
    int fileID;

public:

    LinkerReallocation(int fileID, string offset, string type, string symbolID);
};

#endif