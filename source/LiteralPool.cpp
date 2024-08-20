#include "../include/LiteralPool.hpp"

int LiteralPool::findEntry(string value){
    for (auto& entry : entries) {
        if (entry->value == value) {
            return entry->location;
        }
    }
    return -1;
}

LiteralPoolEntry* LiteralPool::insertEntry(string value){
    int offset = entries.size()*4;
    LiteralPoolEntry* newEntry = new LiteralPoolEntry{offset, value};
    entries.push_back(newEntry);

    return newEntry;
    
}

LiteralPoolEntry* LiteralPool::getEntry(string value){
    for (LiteralPoolEntry* entry : entries) {
        if (entry->value == value) {
            return entry;
        }
    }
    return nullptr;
}

LiteralPoolEntry* LiteralPool::getEntryByLocation(int location){
    for (LiteralPoolEntry* entry : entries) {
        if (entry->location == location) {
            return entry;
        }
    }
    return nullptr;
}