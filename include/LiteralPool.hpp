

#ifndef LITEREALPOOLENTRY_HPP_
#define LITEREALPOOLENTRY_HPP_

#include <string>
#include <vector>

using namespace std;

struct LiteralPoolEntry{
    int location;
    string value;
    vector<int> positionsInCode;
};

class LiteralPool{
private:
    std::vector<LiteralPoolEntry*> entries;
    

public:
    LiteralPool(){}

    int findEntry(string value);
    LiteralPoolEntry* getEntry(string value);
    LiteralPoolEntry* insertEntry(string value);
    
    std::vector<LiteralPoolEntry*> getAllEntries(){
        return entries;
    }

    ~LiteralPool(){}
};


#endif