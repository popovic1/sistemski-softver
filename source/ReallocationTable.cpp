#include "../include/ReallocationTable.hpp"
#include "../include/Symbol.hpp"

ReallocationTable::ReallocationTable(){}

void ReallocationTable::addReallocation(int location, Symbol* symbol, ReallocationType type){
    Reallocation* reallocation = new Reallocation{location, symbol, type};
    reallocations.push_back(reallocation);
}