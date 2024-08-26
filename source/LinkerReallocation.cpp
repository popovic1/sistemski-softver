#include "../include/LinkerReallocation.hpp"

vector<LinkerReallocation*> LinkerReallocation::reallocations;

LinkerReallocation::LinkerReallocation(int fileID, string offset, string type, string symbolID){
    // cout<<"New linkerReallocation: SymbolID = "<<symbolID<< " - fileID = " <<fileID<<" - Offset = "<<offset<<" - type = "<<type<<endl;
    this->fileID = fileID;
    this->offset = offset;
    this->symbolID = stoi(symbolID);
    this->type = type == "EXT_SYM_REALLOC" ? EXT_SYM_REALLOC : 
                    type == "LOCAL_SYM_REALLOC_THIRTY_TWO_BIT" ? LOCAL_SYM_REALLOC_THIRTY_TWO_BIT :
                    type == "LOCAL_SYM_REALLOC_TWELVE_BIT" ? LOCAL_SYM_REALLOC_TWELVE_BIT :
                    type == "GLOB_SYM_REALLOC" ? GLOB_SYM_REALLOC :
                    type == "GLOB_SYM_REALLOC_TWELVE_BIT" ? GLOB_SYM_REALLOC_TWELVE_BIT :
                                                            EXT_SYM_REALLOC_TWELVE_BIT;

    reallocations.push_back(this);
}