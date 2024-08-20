#ifndef REALLOCATIONTABLE_HPP_
#define REALLOCATIONTABLE_HPP_

#include <vector>

using namespace std;

class Symbol;

enum ReallocationType{
    EXT_SYM_REALLOC, LOCAL_SYM_REALLOC_THIRTY_TWO_BIT, LOCAL_SYM_REALLOC_TWELVE_BIT, GLOB_SYM_REALLOC, 
    GLOB_SYM_REALLOC_TWELVE_BIT, EXT_SYM_REALLOC_TWELVE_BIT
};

struct Reallocation{
    int location;
    Symbol* symbol;
    ReallocationType type;
};

class ReallocationTable{

private:
    std::vector<Reallocation*> reallocations;

public:
    ReallocationTable();

    vector<Reallocation*> getReallocations(){
        return reallocations;
    }

    static string getTypeString(ReallocationType type){
        if(type == EXT_SYM_REALLOC){
            return "EXT_SYM_REALLOC";
        }
        else if(type == LOCAL_SYM_REALLOC_THIRTY_TWO_BIT){
            return "LOCAL_SYM_REALLOC_THIRTY_TWO_BIT";
        }else if(type == LOCAL_SYM_REALLOC_TWELVE_BIT){
            return "LOCAL_SYM_REALLOC_TWELVE_BIT";
        }else if(type == GLOB_SYM_REALLOC){
            return "GLOB_SYM_REALLOC";
        }else if(type == GLOB_SYM_REALLOC_TWELVE_BIT){
            return "GLOB_SYM_REALLOC_TWELVE_BIT";
        }else if(type == EXT_SYM_REALLOC_TWELVE_BIT){
            return "EXT_SYM_REALLOC_TWELVE_BIT";
        }else{
            return "";
        }
    }

    void addReallocation(int location, Symbol* symbol, ReallocationType type);

};


#endif