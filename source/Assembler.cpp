#include "../include/Assembler.hpp"
#include "../include/LiteralPool.hpp"
#include "../include/ReallocationTable.hpp"

#include <iostream>
#include <fstream>



Assembler::Assembler(){}

int Assembler::handleDirectives(std::vector<string> parsedLine){

    if(parsedLine[0] == ".global")
    {
        for(int i = 1; i<parsedLine.size(); i++){
            Symbol* existingSymbol = Symbol::getSymbol(parsedLine[i]);

            if(existingSymbol != nullptr){
                existingSymbol->setScope(Scope::GLOBAL);
            }else{
                bool success = addSymbolToTheSymbolList(parsedLine[i], 0, Scope::GLOBAL, false, Section::getUndefinedSection(), SymbolType::NOTYP);
                if(success == false)
                    return -1;
            }
        }
    }
    else if (parsedLine[0] == ".extern")
    {
        for(int i = 1; i<parsedLine.size(); i++){
            Symbol* existingSymbol = Symbol::getSymbol(parsedLine[i]);

            if(existingSymbol != nullptr){
                cout<<"--------------------------------------------------------------"<<endl;
                cerr<<"Error - Assembler: Symbol already defined" + parsedLine[i]<<endl;
                cout<<"--------------------------------------------------------------"<<endl;
                return -1;
            }else{
                bool success = addSymbolToTheSymbolList(parsedLine[i], 0, Scope::EXTERN, true, Section::getUndefinedSection(), SymbolType::NOTYP);
                if(success == false)
                    return -1;
            }
        }
    }
    else if (parsedLine[0] == ".section")
    {
        if(parsedLine.size()!=2){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        if(isValidIdentifier(parsedLine[1].substr(1))==false){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Invalid section name."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        if(Section::getActiveSection()!= nullptr){
            //ovo mozda ne treba ako uvecavam size posle svake instrukcije
            Section::getActiveSection()->finalize();
        }

        currentLocation = 0;
        Section* newSection = new Section(parsedLine[1]);
        Section::setActiveSection(newSection);
    }
    else if (parsedLine[0] == ".word")
    {
        if(parsedLine.size()<2){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        if(Section::getActiveSection() == nullptr){
            cout<<"------------------------------------------"<<endl;
            std::cerr<<"Error - Assembler: Word directive needs to be in a section."<<endl;
            cout<<"------------------------------------------"<<endl;
            return -1;
        }
        
        for(int i = 1; i < parsedLine.size(); i++){
            int typeOfParam = isNumber(parsedLine[i]);
            int valueOfWord = 0;

            if(typeOfParam == -1){ //symbol
                // TODO resolve when symbol is not yet defined
                Symbol* symbol = Symbol::getSymbol(parsedLine[i]);
                if(symbol == nullptr || !symbol->isDefined()){
                    if(symbol == nullptr){
                        bool success = addSymbolToTheSymbolList(parsedLine[1], 0, Scope::LOCAL, false, Section::getUndefinedSection(), SymbolType::NOTYP);
                        if(success == false){
                            return -1;
                        }
                        symbol = Symbol::getSymbol(parsedLine[1]);
                    }
                    symbol->addFLinkEntry(currentLocation, Section::getActiveSection(), FlinkType::THIRTY_TWO_BIT_VALUE);
                    valueOfWord = 0;
                }else{
                    valueOfWord = symbol->getValue();
                }

                if(symbol->getScope() == Scope::LOCAL){
                    Section::getActiveSection()->getReallocationTable()->addReallocation(currentLocation, 
                        symbol, ReallocationType::LOCAL_SYM_REALLOC_THIRTY_TWO_BIT);
                }else if(symbol->getScope() == Scope::EXTERN){
                    Section::getActiveSection()->getReallocationTable()->addReallocation(currentLocation, symbol, ReallocationType::EXT_SYM_REALLOC);
                }else if(symbol->getScope() == Scope::GLOBAL){
                    Section::getActiveSection()->getReallocationTable()->addReallocation(currentLocation, symbol, ReallocationType::GLOB_SYM_REALLOC);
                }
                
                
            }else if(typeOfParam == 0){ //decimal
                valueOfWord = stoi(parsedLine[i]);
            }else if(typeOfParam == 1){ //hex
                valueOfWord = hexToDecimal(parsedLine[i]);
            }else{
                cout<<"--------------------------------------------------------------"<<endl;
                cerr<<"Error - Assembler: Function isNumber doesn't work properly."<<endl;
                cout<<"--------------------------------------------------------------"<<endl;
                return -1;
            }
            
            if (valueOfWord > 2147483647 || valueOfWord < -2147483648){
                cout<<"--------------------------------------------------------------"<<endl;
                cerr<<"Error - Assembler: Invalid parameter. The value is out of range:" + parsedLine[i]<<endl;
                cout<<"--------------------------------------------------------------"<<endl;
                return -1;
            }
            string code = intToHexString(valueOfWord);
            while(code.length()<8){
                code = "0" + code;
            }
            Section::appendCode(code);
            currentLocation += 4;
        }
    }
    else if (parsedLine[0] == ".skip")
    {
        if(parsedLine.size() != 2){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        if(Section::getActiveSection() == nullptr){
                cout<<"------------------------------------------"<<endl;
                std::cerr<<"Error - Assembler: Skip directive needs to be in a section."<<endl;
                cout<<"------------------------------------------"<<endl;
                return -1;
            }

        int isNum = isNumber(parsedLine[1]);

        if(isNum == -1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: The argument has to be a number."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }else if(isNum == 0){
            currentLocation += stoi(parsedLine[1]);
            string code = string(stoi(parsedLine[1]) * 2, '0');
            Section::appendCode(code);
        }else if(isNum == 1){
            int decimal = hexToDecimal(parsedLine[1]);
            if(decimal == -1)return -1;
            currentLocation += decimal;
            string code = string(decimal * 2, '0');
            Section::appendCode(code);
        }
        

        

    }
    else if (parsedLine[0] == ".end")
    {
        if(Section::getActiveSection()!= nullptr){
            //ovo mozda ne treba ako uvecavam size posle svake instrukcije
            Section::getActiveSection()->finalize();
        }
        
        return 1;
    }
    else
    {
        cout<<"--------------------------------------------------------------"<<endl;
        cerr<<"Error - Assembler: Invalid directive."<<endl;
        cout<<"--------------------------------------------------------------"<<endl;
        return -1;
    }
    return 0;
    
}

int Assembler::handleInstructions(std::vector<string> parsedLine){

    const char hexDigits[] = "0123456789ABCDEF";

    if(parsedLine[0] == "halt")
    {
        if(parsedLine.size() != 1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }
        Section::appendCode(string(8, '0'));
    }
    else if (parsedLine[0] == "int")
    {
        if(parsedLine.size() != 1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }
        Section::appendCode("10000000");
    }
    else if (parsedLine[0] == "iret")
    {
        if(parsedLine.size() != 1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }
        // TODO koja treba prvo da se radi
        Section::appendCode("960E0004");
        Section::appendCode("93FE0008");
        
        currentLocation += 4;
    }
    else if (parsedLine[0] == "call")
    {
        if(parsedLine.size() != 2){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int isLiteral = isNumber(parsedLine[1]);
        if(isLiteral == 0){ // decimal
            // convert to hex
            parsedLine[1] = "0x" + intToHexString(stoi(parsedLine[1]));
            isLiteral = 1;
        }

        if(isLiteral == 1){ // hex
            string hex = parsedLine[1].substr(2);
            if(hex.length() <= 3){
                while (hex.length() < 3) {
                    hex = "0" + hex;
                }
                string code = "20000" + hex;
                Section::appendCode(code);

            }else{
                LiteralPoolEntry* entry = Section::getActiveSection()->getLiteralPool()->insertEntry(hex);
                
                entry->positionsInCode.push_back(currentLocation);

                Section::appendCode("21F00000");
            }

        }else{ //symbol
            Symbol* symbol = Symbol::getSymbol(parsedLine[1]);
            if(symbol == nullptr){
                bool success = addSymbolToTheSymbolList(parsedLine[1], 0, Scope::LOCAL, false, Section::getUndefinedSection(), SymbolType::NOTYP);
                if(!success) return -1;
                symbol = Symbol::getSymbol(parsedLine[1]);
            }

            LiteralPoolEntry* entry = Section::getActiveSection()->getLiteralPool()->insertEntry(parsedLine[1]);
            entry->positionsInCode.push_back(currentLocation);

            // this will later add a reallocation for the location in the literal pool
            symbol->addFLinkEntry(entry->location, Section::getActiveSection(), FlinkType::LITERAL_POOL);


            Section::appendCode("21F00000");
        }
    }
    else if (parsedLine[0] == "ret")
    {
        if(parsedLine.size() != 1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }
        Section::appendCode("93FE0004");   
    }
    else if (parsedLine[0] == "jmp")
    {
        if(parsedLine.size() != 2){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int isLiteral = isNumber(parsedLine[1]);
        if(isLiteral == 0){ // decimal
            // convert to hex
            parsedLine[1] = "0x" + intToHexString(stoi(parsedLine[1]));
            isLiteral = 1;
        }

        if(isLiteral == 1){ // hex
            string hex = parsedLine[1].substr(2);
            if(hex.length() <= 3){
                while (hex.length() < 3) {
                    hex = "0" + hex;
                }
                string code = "30000" + hex;
                Section::appendCode(code);

            }else{
                LiteralPoolEntry* entry = Section::getActiveSection()->getLiteralPool()->insertEntry(hex);
                
                entry->positionsInCode.push_back(currentLocation);

                Section::appendCode("38F00000");
            }

        }else{ //symbol
            Symbol* symbol = Symbol::getSymbol(parsedLine[1]);
            if(symbol == nullptr){
                bool success = addSymbolToTheSymbolList(parsedLine[1], 0, Scope::LOCAL, false, Section::getUndefinedSection(), SymbolType::NOTYP);
                if(!success) return -1;
                symbol = Symbol::getSymbol(parsedLine[1]);
            }

            LiteralPoolEntry* entry = Section::getActiveSection()->getLiteralPool()->insertEntry(parsedLine[1]);
            entry->positionsInCode.push_back(currentLocation);

            // this will later add a reallocation for the location in the literal pool
            symbol->addFLinkEntry(entry->location, Section::getActiveSection(), FlinkType::LITERAL_POOL);

            Section::appendCode("38F00000");
        }
    }
    else if (parsedLine[0] == "beq")
    {
        if(parsedLine.size() != 4){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax: " + to_string(parsedLine.size())<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int regNum1 = isRegister(parsedLine[1].substr(1));
        int regNum2 = isRegister(parsedLine[2].substr(1));

        if(parsedLine[1][0] != '%' || parsedLine[2][0] != '%' || regNum1 == -1 || regNum2 == -1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Parameter is not a register or doesn't start with '%'."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int isLiteral = isNumber(parsedLine[3]);
        if(isLiteral == 0){ // decimal
            // convert to hex
            parsedLine[3] = "0x" + intToHexString(stoi(parsedLine[3]));
            isLiteral = 1;
        }

        if(isLiteral == 1){ // hex
            string hex = parsedLine[3].substr(2);
            if(hex.length() <= 3){
                while (hex.length() < 3) {
                    hex = "0" + hex;
                }
                string code = "310" + string(1, hexDigits[regNum1]) + string(1, hexDigits[regNum2]) + hex;
                Section::appendCode(code);

            }else{
                LiteralPoolEntry* entry = Section::getActiveSection()->getLiteralPool()->insertEntry(hex);
                
                entry->positionsInCode.push_back(currentLocation);

                Section::appendCode("39F" + string(1, hexDigits[regNum1]) + string(1, hexDigits[regNum2]) + "000");
            }

        }else{ //symbol
            Symbol* symbol = Symbol::getSymbol(parsedLine[3]);
            if(symbol == nullptr){
                bool success = addSymbolToTheSymbolList(parsedLine[3], 0, Scope::LOCAL, false, Section::getUndefinedSection(), SymbolType::NOTYP);
                if(!success) return -1;
                symbol = Symbol::getSymbol(parsedLine[3]);
            }

            LiteralPoolEntry* entry = Section::getActiveSection()->getLiteralPool()->insertEntry(parsedLine[3]);
            entry->positionsInCode.push_back(currentLocation);

            // this will later add a reallocation for the location in the literal pool
            symbol->addFLinkEntry(entry->location, Section::getActiveSection(), FlinkType::LITERAL_POOL);

            Section::appendCode("39F" + string(1, hexDigits[regNum1]) + string(1, hexDigits[regNum2]) + "000");
        }
    }
    else if (parsedLine[0] == "bne")
    {
        if(parsedLine.size() != 4){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int regNum1 = isRegister(parsedLine[1].substr(1));
        int regNum2 = isRegister(parsedLine[2].substr(1));

        if(parsedLine[1][0] != '%' || parsedLine[2][0] != '%' || regNum1 == -1 || regNum2 == -1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Parameter is not a register or doesn't start with '%'."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int isLiteral = isNumber(parsedLine[3]);
        if(isLiteral == 0){ // decimal
            // convert to hex
            parsedLine[3] = "0x" + intToHexString(stoi(parsedLine[3]));
            isLiteral = 1;
        }

        if(isLiteral == 1){ // hex
            string hex = parsedLine[3].substr(2);
            if(hex.length() <= 3){
                while (hex.length() < 3) {
                    hex = "0" + hex;
                }
                string code = "320" + string(1, hexDigits[regNum1]) + string(1, hexDigits[regNum2]) + hex;
                Section::appendCode(code);

            }else{
                LiteralPoolEntry* entry = Section::getActiveSection()->getLiteralPool()->insertEntry(hex);
                
                entry->positionsInCode.push_back(currentLocation);

                Section::appendCode("3AF" + string(1, hexDigits[regNum1]) + string(1, hexDigits[regNum2]) + "000");
            }

        }else{ //symbol
            Symbol* symbol = Symbol::getSymbol(parsedLine[3]);
            if(symbol == nullptr){
                bool success = addSymbolToTheSymbolList(parsedLine[3], 0, Scope::LOCAL, false, Section::getUndefinedSection(), SymbolType::NOTYP);
                if(!success) return -1;
                symbol = Symbol::getSymbol(parsedLine[3]);
            }

            LiteralPoolEntry* entry = Section::getActiveSection()->getLiteralPool()->insertEntry(parsedLine[3]);
            
            entry->positionsInCode.push_back(currentLocation);

            // this will later add a reallocation for the location in the literal pool
            symbol->addFLinkEntry(entry->location, Section::getActiveSection(), FlinkType::LITERAL_POOL);
            Section::appendCode("3AF" + string(1, hexDigits[regNum1]) + string(1, hexDigits[regNum2]) + "000");
        }
    }
    else if (parsedLine[0] == "bgt")
    {
        if(parsedLine.size() != 4){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int regNum1 = isRegister(parsedLine[1].substr(1));
        int regNum2 = isRegister(parsedLine[2].substr(1));

        if(parsedLine[1][0] != '%' || parsedLine[2][0] != '%' || regNum1 == -1 || regNum2 == -1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Parameter is not a register or doesn't start with '%'."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int isLiteral = isNumber(parsedLine[3]);
        if(isLiteral == 0){ // decimal
            // convert to hex
            parsedLine[3] = "0x" + intToHexString(stoi(parsedLine[3]));
            isLiteral = 1;
        }

        if(isLiteral == 1){ // hex
            string hex = parsedLine[3].substr(2);
            if(hex.length() <= 3){
                while (hex.length() < 3) {
                    hex = "0" + hex;
                }
                string code = "330" + string(1, hexDigits[regNum1]) + string(1, hexDigits[regNum2]) + hex;
                Section::appendCode(code);

            }else{
                LiteralPoolEntry* entry = Section::getActiveSection()->getLiteralPool()->insertEntry(hex);
                
                entry->positionsInCode.push_back(currentLocation);

                Section::appendCode("3BF" + string(1, hexDigits[regNum1]) + string(1, hexDigits[regNum2]) + "000");
            }

        }else{ //symbol
            Symbol* symbol = Symbol::getSymbol(parsedLine[3]);
            if(symbol == nullptr){
                bool success = addSymbolToTheSymbolList(parsedLine[3], 0, Scope::LOCAL, false, Section::getUndefinedSection(), SymbolType::NOTYP);
                if(!success) return -1;
                symbol = Symbol::getSymbol(parsedLine[3]);
            }

            LiteralPoolEntry* entry = Section::getActiveSection()->getLiteralPool()->insertEntry(parsedLine[3]);
            entry->positionsInCode.push_back(currentLocation);

            // this will later add a reallocation for the location in the literal pool
            symbol->addFLinkEntry(entry->location, Section::getActiveSection(), FlinkType::LITERAL_POOL);
            Section::appendCode("3BF" + string(1, hexDigits[regNum1]) + string(1, hexDigits[regNum2]) + "000");
        }
    }
    else if (parsedLine[0] == "push")
    {
        if(parsedLine.size() != 2){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int regNum = isRegister(parsedLine[1].substr(1));

        if(parsedLine[1][0] != '%'|| regNum == -1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Parameter is not a register or doesn't start with '%'."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        string code = "81E0" + string(1, hexDigits[regNum]) + "FFC";

        Section::appendCode(code);   
    }
    else if (parsedLine[0] == "pop")
    {
        if(parsedLine.size() != 2){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int regNum = isRegister(parsedLine[1].substr(1));

        if(parsedLine[1][0] != '%'|| regNum == -1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Parameter is not a register or doesn't start with '%'."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        string code = "93" + string(1, hexDigits[regNum]) + "E0004";

        Section::appendCode(code);   
    }
    else if (parsedLine[0] == "xchg")
    {
        if(parsedLine.size() != 3){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int regNum1 = isRegister(parsedLine[1].substr(1));
        int regNum2 = isRegister(parsedLine[2].substr(1));

        if(parsedLine[1][0] != '%' || parsedLine[2][0] != '%' || regNum1 == -1 || regNum2 == -1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Parameter is not a register or doesn't start with '%'."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        string code = "400" + string(1, hexDigits[regNum2]) + string(1, hexDigits[regNum1]) + "000";

        Section::appendCode(code);
    }
    else if (parsedLine[0] == "add")
    {
        if(parsedLine.size() != 3){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int regNum1 = isRegister(parsedLine[1].substr(1));
        int regNum2 = isRegister(parsedLine[2].substr(1));

        if(parsedLine[1][0] != '%' || parsedLine[2][0] != '%' || regNum1 == -1 || regNum2 == -1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Parameter is not a register or doesn't start with '%'."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        string code = "50" + string(1, hexDigits[regNum2]) + string(1, hexDigits[regNum2]) + string(1, hexDigits[regNum1]) + "000";

        Section::appendCode(code);
    }
    else if (parsedLine[0] == "sub")
    {
        if(parsedLine.size() != 3){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int regNum1 = isRegister(parsedLine[1].substr(1));
        int regNum2 = isRegister(parsedLine[2].substr(1));

        if(parsedLine[1][0] != '%' || parsedLine[2][0] != '%' || regNum1 == -1 || regNum2 == -1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Parameter is not a register or doesn't start with '%'."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        string code = "51" + string(1, hexDigits[regNum2]) + string(1, hexDigits[regNum2]) + string(1, hexDigits[regNum1]) + "000";

        Section::appendCode(code);
    }
    else if (parsedLine[0] == "mul")
    {
        if(parsedLine.size() != 3){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int regNum1 = isRegister(parsedLine[1].substr(1));
        int regNum2 = isRegister(parsedLine[2].substr(1));

        if(parsedLine[1][0] != '%' || parsedLine[2][0] != '%' || regNum1 == -1 || regNum2 == -1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Parameter is not a register or doesn't start with '%'."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        string code = "52" + string(1, hexDigits[regNum2]) + string(1, hexDigits[regNum2]) + string(1, hexDigits[regNum1]) + "000";

        Section::appendCode(code);
    }
    else if (parsedLine[0] == "div")
    {
        if(parsedLine.size() != 3){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int regNum1 = isRegister(parsedLine[1].substr(1));
        int regNum2 = isRegister(parsedLine[2].substr(1));

        if(parsedLine[1][0] != '%' || parsedLine[2][0] != '%' || regNum1 == -1 || regNum2 == -1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Parameter is not a register or doesn't start with '%'."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        string code = "53" + string(1, hexDigits[regNum2]) + string(1, hexDigits[regNum2]) + string(1, hexDigits[regNum1]) + "000";

        Section::appendCode(code);
    }
    else if (parsedLine[0] == "not")
    {
        if(parsedLine.size() != 2){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int regNum1 = isRegister(parsedLine[1].substr(1));

        if(parsedLine[1][0] != '%' || regNum1 == -1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Parameter is not a register or doesn't start with '%'."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        string code = "60" + string(1, hexDigits[regNum1]) + string(1, hexDigits[regNum1]) + "0000";

        Section::appendCode(code);
    }
    else if (parsedLine[0] == "and")
    {
        if(parsedLine.size() != 3){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int regNum1 = isRegister(parsedLine[1].substr(1));
        int regNum2 = isRegister(parsedLine[2].substr(1));

        if(parsedLine[1][0] != '%' || parsedLine[2][0] != '%' || regNum1 == -1 || regNum2 == -1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Parameter is not a register or doesn't start with '%'."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        string code = "61" + string(1, hexDigits[regNum2]) + string(1, hexDigits[regNum2]) + string(1, hexDigits[regNum1]) + "000";

        Section::appendCode(code);
    }
    else if (parsedLine[0] == "or")
    {
        if(parsedLine.size() != 3){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int regNum1 = isRegister(parsedLine[1].substr(1));
        int regNum2 = isRegister(parsedLine[2].substr(1));

        if(parsedLine[1][0] != '%' || parsedLine[2][0] != '%' || regNum1 == -1 || regNum2 == -1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Parameter is not a register or doesn't start with '%'."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        string code = "62" + string(1, hexDigits[regNum2]) + string(1, hexDigits[regNum2]) + string(1, hexDigits[regNum1]) + "000";

        Section::appendCode(code);
    }
    else if (parsedLine[0] == "xor")
    {
        if(parsedLine.size() != 3){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int regNum1 = isRegister(parsedLine[1].substr(1));
        int regNum2 = isRegister(parsedLine[2].substr(1));

        if(parsedLine[1][0] != '%' || parsedLine[2][0] != '%' || regNum1 == -1 || regNum2 == -1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Parameter is not a register or doesn't start with '%'."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        string code = "63" + string(1, hexDigits[regNum2]) + string(1, hexDigits[regNum2]) + string(1, hexDigits[regNum1]) + "000";

        Section::appendCode(code);
    }
    else if (parsedLine[0] == "shl")
    {
        if(parsedLine.size() != 3){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int regNum1 = isRegister(parsedLine[1].substr(1));
        int regNum2 = isRegister(parsedLine[2].substr(1));

        if(parsedLine[1][0] != '%' || parsedLine[2][0] != '%' || regNum1 == -1 || regNum2 == -1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Parameter is not a register or doesn't start with '%'."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        string code = "70" + string(1, hexDigits[regNum2]) + string(1, hexDigits[regNum2]) + string(1, hexDigits[regNum1]) + "000";

        Section::appendCode(code);
    }
    else if (parsedLine[0] == "shr")
    {
        if(parsedLine.size() != 3){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int regNum1 = isRegister(parsedLine[1].substr(1));
        int regNum2 = isRegister(parsedLine[2].substr(1));

        if(parsedLine[1][0] != '%' || parsedLine[2][0] != '%' || regNum1 == -1 || regNum2 == -1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Parameter is not a register or doesn't start with '%'."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        string code = "71" + string(1, hexDigits[regNum2]) + string(1, hexDigits[regNum2]) + string(1, hexDigits[regNum1]) + "000";

        Section::appendCode(code);
    }
    else if (parsedLine[0] == "ld")
    {
        int regNum = 0;
        if(parsedLine.size() < 3){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }else if(parsedLine.size() == 3){

            regNum = isRegister(parsedLine[2].substr(1));

            if(parsedLine[2][0] != '%' || regNum == -1){
                cout<<"--------------------------------------------------------------"<<endl;
                cerr<<"Error - Assembler: Last parameter is not a register or doesn't start with '%'."<<endl;
                cout<<"--------------------------------------------------------------"<<endl;
                return -1;
            }

            if(parsedLine[1][0]=='$'){ // vrednost literala ili simbola

                int isLiteral = isNumber(parsedLine[1].substr(1));
                if(isLiteral == 0){ // decimal
                    // convert to hex
                    parsedLine[1] = "$0x" + intToHexString(stoi(parsedLine[1].substr(1)));
                    isLiteral = 1;
                }

                if(isLiteral == 1){ // hex
                    string hex = parsedLine[1].substr(3);
                    if(hex.length() <= 3){
                        while (hex.length() < 3) {
                            hex = "0" + hex;
                        }
                        string code = "91" + string(1, hexDigits[regNum]) + "00" + hex;
                        Section::appendCode(code);

                    }else{
                        LiteralPoolEntry* entry = Section::getActiveSection()->getLiteralPool()->insertEntry(hex);
                        cout<<"Literal: "<<hex<<endl;
                        entry->positionsInCode.push_back(currentLocation);
                        string code = "92" + string(1, hexDigits[regNum]) + "F0000";
                        Section::appendCode(code);
                    }

                }else{ //symbol
                    Symbol* symbol = Symbol::getSymbol(parsedLine[1].substr(1));
                    if(symbol == nullptr){
                        bool success = addSymbolToTheSymbolList(parsedLine[1].substr(1), 0, Scope::LOCAL, false, Section::getUndefinedSection(), SymbolType::NOTYP);
                        if(!success) return -1;
                        symbol = Symbol::getSymbol(parsedLine[1].substr(1));                    
                    }
                    LiteralPoolEntry* entry = Section::getActiveSection()->getLiteralPool()->insertEntry(parsedLine[1].substr(1));

                    entry->positionsInCode.push_back(currentLocation);

                    // this will later add a reallocation for the location in the literal pool
                    symbol->addFLinkEntry(entry->location, Section::getActiveSection(), FlinkType::LITERAL_POOL);

                    string code = "92" + string(1, hexDigits[regNum]) + "F0000";
                    Section::appendCode(code);
                }
                        
            }else if(parsedLine[1][0] == '%'){ // vrednost u registru

                int regNum2 = isRegister(parsedLine[1].substr(1));
                if(parsedLine[1][0] != '%' || regNum2 == -1){
                    cout<<"--------------------------------------------------------------"<<endl;
                    cerr<<"Error - Assembler: First parameter is not a register or doesn't start with '%'."<<endl;
                    cout<<"--------------------------------------------------------------"<<endl;
                    return -1;
                }

                string code = "91" + string(1, hexDigits[regNum]) +  string(1, hexDigits[regNum2]) + "0000";
                Section::appendCode(code);

            }else if(parsedLine[1][0] == '['){ // vrednost u memoriji na mestu reg
                
                int regNum2 = isRegister(parsedLine[1].substr(2, parsedLine[1].length() - 3));

                if(parsedLine[1][1] != '%' || parsedLine[1][parsedLine[1].length() -1] != ']' || regNum2 == -1){
                    cout<<"--------------------------------------------------------------"<<endl;
                    cerr<<"Error - Assembler: First parameter is not a register or isn't properly written."<<endl;
                    cout<<"--------------------------------------------------------------"<<endl;
                    return -1;
                }

                string code = "92" + string(1, hexDigits[regNum]) +  string(1, hexDigits[regNum2]) + "0000";
                Section::appendCode(code);

            }else{ // vrednost u memoriji na mestu literal ili simbol
                int isLiteral = isNumber(parsedLine[1]);
                if(isLiteral == 0){ // decimal
                    // convert to hex
                    parsedLine[1] = "0x" + intToHexString(stoi(parsedLine[1]));
                    isLiteral = 1;
                }

                if(isLiteral == 1){ // hex
                    string hex = parsedLine[1].substr(2);
                    if(hex.length() <= 3){
                        while (hex.length() < 3) {
                            hex = "0" + hex;
                        }
                        string code = "92" + string(regNum, 1) + "00" + hex;
                        Section::appendCode(code);

                    }else{
                        LiteralPoolEntry* entry = Section::getActiveSection()->getLiteralPool()->insertEntry(hex);

                        
                        entry->positionsInCode.push_back(currentLocation);
                        string code = "92" + string(1, hexDigits[regNum]) + "F0000";
                        Section::appendCode(code);
                        currentLocation += 4;

                        code = "92" + string(1, hexDigits[regNum]) + string(1, hexDigits[regNum]) + "0000";
                        Section::appendCode(code);
                    }

                }else{ //symbol
                    Symbol* symbol = Symbol::getSymbol(parsedLine[1]);
                    if(symbol == nullptr){
                        bool success = addSymbolToTheSymbolList(parsedLine[1], 0, Scope::LOCAL, false, Section::getUndefinedSection(), SymbolType::NOTYP);
                        if(!success) return -1;
                        symbol = Symbol::getSymbol(parsedLine[1]);
                    }

                    LiteralPoolEntry* entry = Section::getActiveSection()->getLiteralPool()->insertEntry(parsedLine[1]);
                    entry->positionsInCode.push_back(currentLocation);

                    // this will later add a reallocation for the location in the literal pool
                    symbol->addFLinkEntry(entry->location, Section::getActiveSection(), FlinkType::LITERAL_POOL);

                    string code = "92" + string(1, hexDigits[regNum]) + "F0000";
                    Section::appendCode(code);
                    currentLocation += 4;

                    code = "92" + string(1, hexDigits[regNum]) + string(1, hexDigits[regNum]) + "0000";
                    Section::appendCode(code);
                }
            }

        }else{ //izraz

            regNum = isRegister(parsedLine[parsedLine.size()-1].substr(1));

            if(parsedLine[parsedLine.size()-1][0] != '%' || regNum == -1){
                cout<<"--------------------------------------------------------------"<<endl;
                cerr<<"Error - Assembler: Last parameter is not a register or doesn't start with '%'."<<endl;
                cout<<"--------------------------------------------------------------"<<endl;
                return -1;
            }

            int regNum2 = isRegister(parsedLine[1].substr(2));

            if(parsedLine[1][0] != '[' || parsedLine[1][1] != '%' || regNum2 == -1 || parsedLine[2] != "+" || 
            parsedLine[3][parsedLine[3].length()-1] != ']')
            {
                cout<<"--------------------------------------------------------------"<<endl;
                cerr<<"Error - Assembler: Incorrect expression."<<endl;
                cout<<"--------------------------------------------------------------"<<endl;
                return -1;
            }

            int isLiteral = isNumber(parsedLine[3].substr(0, parsedLine[3].length() - 1));
            if(isLiteral == 0){ // decimal
                // convert to hex
                parsedLine[3] = "0x" + intToHexString(stoi(parsedLine[3])) + "]";
                isLiteral = 1;
            }

            if(isLiteral == 1){ // hex
                string hex = parsedLine[3].substr(2, parsedLine[3].length() - 3);
                if(hex.length() <= 3){
                    while (hex.length() < 3) {
                        hex = "0" + hex;
                    }
                    string code = "92" + string(1, hexDigits[regNum]) + string(1, hexDigits[regNum2]) + "0" + hex;
                    Section::appendCode(code);

                }else{
                    cout<<"--------------------------------------------------------------"<<endl;
                    cerr<<"Error - Assembler: Literal can't fit in 12 bits."<<endl;
                    cout<<"--------------------------------------------------------------"<<endl;
                    return -1;
                }
            }else{ //symbol

                Symbol* symbol = Symbol::getSymbol(parsedLine[3].substr(0, parsedLine[3].length() - 1));
                if(symbol != nullptr and symbol->isDefined()){
                    int value = symbol->getValue();
                    string hex = intToHexString(value);

                    if(hex.length() <= 3){
                        while (hex.length() < 3) {
                            hex = "0" + hex;
                        }
                        
                        // TODO proveri da li ovu vrednost treba realocirati
                        if(symbol->getScope() == Scope::LOCAL){
                            Section::getActiveSection()->getReallocationTable()->addReallocation(currentLocation, 
                                Symbol::getSymbol(symbol->getSection()->getName()), ReallocationType::LOCAL_SYM_REALLOC_TWELVE_BIT);
                        }else if(symbol->getScope() == Scope::EXTERN){
                            Section::getActiveSection()->getReallocationTable()->addReallocation(currentLocation, symbol, ReallocationType::EXT_SYM_REALLOC_TWELVE_BIT);
                        }else if(symbol->getScope() == Scope::GLOBAL){
                            Section::getActiveSection()->getReallocationTable()->addReallocation(currentLocation, symbol, ReallocationType::GLOB_SYM_REALLOC_TWELVE_BIT);
                        }

                        string code = "92" + string(1, hexDigits[regNum]) + string(1, hexDigits[regNum2]) + "0" + hex;
                        Section::appendCode(code);

                    }else{
                        cout<<"--------------------------------------------------------------"<<endl;
                        cerr<<"Error - Assembler: Symbol value can't fit in 12 bits."<<endl;
                        cout<<"--------------------------------------------------------------"<<endl;
                        return -1;
                    }                    

                }else{
                    cout<<"--------------------------------------------------------------"<<endl;
                    cerr<<"Error - Assembler: Symbol doesn't exist or isn't defined."<<endl;
                    cout<<"--------------------------------------------------------------"<<endl;
                    return -1;
                }
            }


        }
    }
    else if (parsedLine[0] == "st")
    {
        int regNum = 0;
        if(parsedLine.size() < 3){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }else if(parsedLine.size() == 3){

            regNum = isRegister(parsedLine[1].substr(1));

            if(parsedLine[1][0] != '%' || regNum == -1){
                cout<<"--------------------------------------------------------------"<<endl;
                cerr<<"Error - Assembler: First parameter is not a register or doesn't start with '%'."<<endl;
                cout<<"--------------------------------------------------------------"<<endl;
                return -1;
            }

            if(parsedLine[2][0]=='$'){ // vrednost literala ili simbola

                cout<<"--------------------------------------------------------------"<<endl;
                cerr<<"Error - Assembler: Can't store to an immidate value."<<endl;
                cout<<"--------------------------------------------------------------"<<endl;
                return -1;
                        
            }else if(parsedLine[2][0] == '%'){ // vrednost u registru

                cout<<"--------------------------------------------------------------"<<endl;
                cerr<<"Error - Assembler: Can't store into a register."<<endl;
                cout<<"--------------------------------------------------------------"<<endl;
                return -1;

            }else if(parsedLine[2][0] == '['){ // vrednost u memoriji na mestu reg
                
                int regNum2 = isRegister(parsedLine[2].substr(2, parsedLine[2].length() - 3));

                if(parsedLine[2][1] != '%' || parsedLine[2][parsedLine[2].length() -1] != ']' || regNum2 == -1){
                    cout<<"--------------------------------------------------------------"<<endl;
                    cerr<<"Error - Assembler: First parameter is not a register or isn't properly written."<<endl;
                    cout<<"--------------------------------------------------------------"<<endl;
                    return -1;
                }

                string code = "80" + string(1, hexDigits[regNum2]) + "0" +  string(1, hexDigits[regNum]) + "000";
                Section::appendCode(code);

            }else{ // vrednost u memoriji na mestu literal ili simbol
                int isLiteral = isNumber(parsedLine[2]);
                if(isLiteral == 0){ // decimal
                    // convert to hex
                    parsedLine[2] = "0x" + intToHexString(stoi(parsedLine[2]));
                    isLiteral = 1;
                }

                if(isLiteral == 1){ // hex
                    string hex = parsedLine[2].substr(2);
                    if(hex.length() <= 3){
                        while (hex.length() < 3) {
                            hex = "0" + hex;
                        }
                        string code = "8000" + string(regNum, 1) + hex;
                        Section::appendCode(code);

                    }else{
                        LiteralPoolEntry* entry = Section::getActiveSection()->getLiteralPool()->insertEntry(hex);
                        
                        entry->positionsInCode.push_back(currentLocation);
                        string code = "82F0" + string(1, hexDigits[regNum]) + "000";
                        Section::appendCode(code);
                    }

                }else{ //symbol
                    Symbol* symbol = Symbol::getSymbol(parsedLine[2]);
                    if(symbol == nullptr){
                        bool success = addSymbolToTheSymbolList(parsedLine[2], 0, Scope::LOCAL, false, Section::getUndefinedSection(), SymbolType::NOTYP);
                        if(!success) return -1;
                        symbol = Symbol::getSymbol(parsedLine[1]);
                    }

                    LiteralPoolEntry* entry = Section::getActiveSection()->getLiteralPool()->insertEntry(parsedLine[2]);
                    entry->positionsInCode.push_back(currentLocation);

                    // this will later add a reallocation for the location in the literal pool
                    symbol->addFLinkEntry(entry->location, Section::getActiveSection(), FlinkType::LITERAL_POOL);
                    string code = "82F0" + string(1, hexDigits[regNum]) + "000";
                    Section::appendCode(code);
                }
            }

        }else{ //izraz

            regNum = isRegister(parsedLine[1].substr(1));

            if(parsedLine[1][0] != '%' || regNum == -1){
                cout<<"--------------------------------------------------------------"<<endl;
                cerr<<"Error - Assembler: First parameter is not a register or doesn't start with '%'."<<endl;
                cout<<"--------------------------------------------------------------"<<endl;
                return -1;
            }

            int regNum2 = isRegister(parsedLine[2].substr(2));

            if(parsedLine[2][0] != '[' || parsedLine[2][1] != '%' || regNum2 == -1 || parsedLine[3] != "+" || 
            parsedLine[4][parsedLine[4].length()-1] != ']')
            {
                cout<<"--------------------------------------------------------------"<<endl;
                cerr<<"Error - Assembler: Incorrect expression."<<endl;
                cout<<"--------------------------------------------------------------"<<endl;
                return -1;
            }

            int isLiteral = isNumber(parsedLine[4].substr(0, parsedLine[4].length() - 1));
            if(isLiteral == 0){ // decimal
                // convert to hex
                parsedLine[4] = "0x" + intToHexString(stoi(parsedLine[4])) + "]";
                isLiteral = 1;
            }

            if(isLiteral == 1){ // hex
                string hex = parsedLine[4].substr(2, parsedLine[4].length() - 3);
                if(hex.length() <= 3){
                    while (hex.length() < 3) {
                        hex = "0" + hex;
                    }
                    string code = "80" + string(1, hexDigits[regNum2]) + "0" + string(1, hexDigits[regNum]) + hex;
                    Section::appendCode(code);

                }else{
                    cout<<"--------------------------------------------------------------"<<endl;
                    cerr<<"Error - Assembler: Literal can't fit in 12 bits."<<endl;
                    cout<<"--------------------------------------------------------------"<<endl;
                    return -1;
                }
            }else{ //symbol
                Symbol* symbol = Symbol::getSymbol(parsedLine[4].substr(0, parsedLine[4].length() - 1));
                if(symbol != nullptr and symbol->isDefined()){
                    int value = symbol->getValue();
                    string hex = intToHexString(value);

                    if(hex.length() <= 3){
                        while (hex.length() < 3) {
                            hex = "0" + hex;
                        }

                        // TODO proveri da li ovu vrednost treba realocirati
                        if(symbol->getScope() == Scope::LOCAL){
                            Section::getActiveSection()->getReallocationTable()->addReallocation(currentLocation, 
                                Symbol::getSymbol(symbol->getSection()->getName()), ReallocationType::LOCAL_SYM_REALLOC_TWELVE_BIT);
                        }else if(symbol->getScope() == Scope::EXTERN){
                            Section::getActiveSection()->getReallocationTable()->addReallocation(currentLocation, symbol, ReallocationType::EXT_SYM_REALLOC_TWELVE_BIT);
                        }else if(symbol->getScope() == Scope::GLOBAL){
                            Section::getActiveSection()->getReallocationTable()->addReallocation(currentLocation, symbol, ReallocationType::GLOB_SYM_REALLOC_TWELVE_BIT);
                        }                        

                        string code = "80" + string(1, hexDigits[regNum2]) + "0" + string(1, hexDigits[regNum]) + hex;
                        Section::appendCode(code);

                    }else{
                        cout<<"--------------------------------------------------------------"<<endl;
                        cerr<<"Error - Assembler: Symbol value can't fit in 12 bits."<<endl;
                        cout<<"--------------------------------------------------------------"<<endl;
                        return -1;
                    }                    

                }else{
                    cout<<"--------------------------------------------------------------"<<endl;
                    cerr<<"Error - Assembler: Symbol doesn't exist or isn't defined."<<endl;
                    cout<<"--------------------------------------------------------------"<<endl;
                    return -1;
                }
            }
        }
    }
    else if (parsedLine[0] == "csrrd")
    {
        if(parsedLine.size() != 3){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int csrReg = isCSR(parsedLine[1].substr(1));
        int regNum = isRegister(parsedLine[2].substr(1));

        if(parsedLine[1][0] != '%' || parsedLine[2][0] != '%' || csrReg == -1 || regNum == -1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Parameter is not a csr/register or doesn't start with '%'."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        string code = "90" + string(1, hexDigits[regNum]) + string(1, hexDigits[csrReg]) + "0000";

        Section::appendCode(code);
    }
    else if (parsedLine[0] == "csrwr")
    {
        if(parsedLine.size() != 3){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Incorrect syntax."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        int regNum = isRegister(parsedLine[1].substr(1));
        int csrReg = isCSR(parsedLine[2].substr(1));

        if(parsedLine[1][0] != '%' || parsedLine[2][0] != '%' || csrReg == -1 || regNum == -1){
            cout<<"--------------------------------------------------------------"<<endl;
            cerr<<"Error - Assembler: Parameter is not a csr/register or doesn't start with '%'."<<endl;
            cout<<"--------------------------------------------------------------"<<endl;
            return -1;
        }

        string code = "94" + string(1, hexDigits[csrReg]) + string(1, hexDigits[regNum]) + "0000";

        Section::appendCode(code);
    }
    else{
        cout<<"--------------------------------------------------------------"<<endl;
        cerr<<"Error - Assembler: Invalid instruction: " + parsedLine[0] << " " << parsedLine[1]<<endl;
        cout<<"--------------------------------------------------------------"<<endl;
        return -1;
    }

    return 0;
}

bool Assembler::addSymbolToTheSymbolList(string name, int value, Scope scope, bool defined, Section* section, SymbolType type){
    // check if the symbol can be added and is valid (i.e. can't be 343ff55)
    if(isValidIdentifier(name) == false){
        cout<<"-----------------------------------------"<<endl;
        std::cerr<<"Error - Assembler: identifier name is not valid: " + name<<endl;
        cout<<"-----------------------------------------"<<endl;
        return false;
    }


    // check if the symbol already exists
    Symbol* existingSymbol = Symbol::getSymbol(name);
    if(existingSymbol == nullptr){
        new Symbol(name, value, scope, defined, section, type);
        return true;

    }else{

        if(existingSymbol->isDefined()){
            cout<<"-----------------------------------------"<<endl;
            std::cerr<<"Error - Assembler: identifier already defined: " + name<<endl;
            cout<<"-----------------------------------------"<<endl;

            return false;
        }else
        {
            existingSymbol->setValue(value);
            existingSymbol->setDefined(true);
            existingSymbol->setSection(section);
            return true;
        }
    }


    
}

bool Assembler::isValidIdentifier(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    if (!std::isalpha(name[0]) && name[0] != '_') {
        return false;
    }
    for (char ch : name) {
        if (!std::isalnum(ch) && ch != '_') {
            return false;
        }
    }

    return true;
}

int Assembler::isNumber(std::string arg) {
    if (arg.empty()) {
        return -1; // Error - Assembler: empty string
    }

    // Check for hexadecimal format (starts with "0x" or "0X")
    if (arg.size() > 2 && arg[0] == '0' && (arg[1] == 'x' || arg[1] == 'X')) {
        // Check if the rest of the string are valid hex digits
        for (size_t i = 2; i < arg.size(); ++i) {
            if (!isxdigit(arg[i])) {
                return -1; // Error - Assembler: invalid hex number
            }
        }
        return 1; // It's a hexadecimal number
    }

    // Check for decimal format
    for (size_t i = 0; i < arg.size(); ++i) {
        if (!isdigit(arg[i])) {
            return -1; // Error - Assembler: invalid decimal number
        }
    }

    return 0; // It's a decimal number
}

int Assembler::hexToDecimal(string hex){
    int decimal = 0;
    for (size_t i = 2; i < hex.size(); ++i) {
        char digit = toupper(hex[i]); 
        decimal *= 16; 

        if (digit >= '0' && digit <= '9') {
            decimal += digit - '0';
        } else if (digit >= 'A' && digit <= 'F') {
            decimal += digit - 'A' + 10;
        } else {
            cout<<"---------------------------------"<<endl;
            cerr << "Error - Assembler: Invalid hexadecimal digit: " << digit <<endl;
            cout<<"---------------------------------"<<endl;
            return -1; 
        }
    }

    return decimal;
}

string Assembler::decimalToLittleEndianHexString(int num) {
    unsigned int unsignedNum = static_cast<unsigned int>(num);

    std::stringstream ss;

    // Convert the unsigned integer to a hexadecimal string
    ss << std::hex << std::setw(8) << std::setfill('0') << unsignedNum;

    std::string hexStr = ss.str();

    // Convert to little endian
    std::string littleEndianHexStr;
    for (size_t i = 0; i < hexStr.size(); i += 2) {
        littleEndianHexStr.insert(0, hexStr.substr(i, 2));
    }

    return littleEndianHexStr;
}

string Assembler::intToHexString(int num) {
    stringstream ss;
    ss << std::hex << num; // Convert integer to hexadecimal
    return ss.str();       // Return the hexadecimal string
}

int Assembler::isRegister(const std::string& reg) {
    if (reg.empty()) {
        return -1; 
    }

    std::string regName = reg;

    // Check if the string represents "sp" or "pc"
    if (regName == "sp" || regName == "r14") {
        return 14;
    }
    if (regName == "pc" || regName == "r15") {
        return 15;
    }

    // Check if the string represents "r0" to "r13"
    if (regName.length() == 2 && regName[0] == 'r' && isdigit(regName[1])) {
        int regNum = regName[1] - '0'; // Convert the second character to an integer
        if (regNum >= 0 && regNum <= 9) {
            return regNum;
        }
    }

    // Check for "r10" to "r13"
    if (regName.length() == 3 && regName[0] == 'r' && regName[1] == '1' && isdigit(regName[2])) {
        int regNum = 10 + (regName[2] - '0'); // Convert the third character to an integer
        if (regNum >= 10 && regNum <= 13) {
            return regNum;
        }
    }

    return -1; // If no valid register was found
}

int Assembler::isCSR(string csr) {
    // Check for special CSR names
    if (csr == "status" || csr == "crs0") {
        return 0; // csr0
    }
    if (csr == "handler" || csr == "crs1") {
        return 1; // csr1
    }
    if (csr == "cause" || csr == "crs2") {
        return 2; // csr2
    }
    return -1;
}

void Assembler::writeToFile(string inputFileName, string outputFileName){
    std::ofstream outFile;

    outFile.open(outputFileName);

    if(outFile.is_open()){
        outFile << inputFileName << endl;
        outFile << "#.symbol_table\t"<<Symbol::getAllSymbols().size()<<endl;
        outFile << "ID" << "\t" << "Value" << "\t" << "Type" << "\t" << "Scope" << "\t" << "SectionID" << "\t" << "Name" <<endl;
        for(Symbol* symbol : Symbol::getAllSymbols()){
            string hexValue = intToHexString(symbol->getValue());
            while (hexValue.length() < 8){
                hexValue = "0" + hexValue;
            }
            outFile << symbol->getID() << "\t" << hexValue << "\t" << symbol->getTypeString() << "\t" << symbol->getScopeString() << "\t" 
                << Symbol::getSymbol(symbol->getSection()->getName())->getID() << "\t" << symbol->getName() <<endl;
        }

        int cntCode = 0;
        int cntRela = 0;

        for(Section* section : Section::getSectionList()){
            if(section->getId() == 0)continue;
            if(section->getSize() != 0) cntCode++;
            if(section->getReallocationTable()->getReallocations().size() != 0) cntRela++;

        }

        outFile<<"[]\tNumber_of_sections_with_code:\t"<<cntCode<<endl;

        for(Section* section : Section::getSectionList()){

            if(section->getId() == 0 || section->getSize() == 0)continue;
            outFile << "#." << section->getName()<< "\t" << section->getSize() <<endl;
            outFile << section->getCode()<<endl;

        }

        outFile<<"[]\tNumber_of_sections_with_rela_table:\t"<<cntRela<<endl;

        for(Section* section : Section::getSectionList()){
            if(section->getId() == 0 || section->getReallocationTable()->getReallocations().size() == 0)continue;
            outFile << "#.rela." << section->getName() << "\t" << section->getReallocationTable()->getReallocations().size()<<endl;

            outFile << "Offset" << "\t" << "Type" << "\t" << "Symbol" << endl;
            for(Reallocation* realloc : section->getReallocationTable()->getReallocations()){

                string hexOffset = intToHexString(realloc->location);
                while (hexOffset.length()<8){
                    hexOffset = "0" + hexOffset;
                }
                
                outFile << hexOffset << "\t" << ReallocationTable::getTypeString(realloc->type) << "\t" << realloc->symbol->getID() << endl;
            } 
            
        }
    }

    outFile.close();

}

void Assembler::compile(std::string inputFileName, std::string outputFileName){

    Parser *parser = new Parser();  

    std::vector<string> allLines = parser->readLinesFromFile(inputFileName);

    for(string line : allLines){

        line = parser->trimString(line);
        if(line.length() == 0)continue;

        // handle comments
        std::size_t hashtagPosition = line.find('#');
        if(hashtagPosition != std::string::npos){
            line.erase(hashtagPosition);
        }
        if(line.length() == 0)continue;
        // parse a single line
        std::vector<string> parsedLine = parser->splitLine(line, ' ');

        // trim all the words in the line
        for(int i = 0; i < parsedLine.size(); i++){
            parsedLine[i] = parser->trimString(parsedLine[i]);
        }
        while(line.length()>0 && parsedLine[0] == ""){
            parsedLine.erase(parsedLine.begin());
        }
        if(line.length() == 0)continue;

        if(parsedLine.front().find(':') != std::string::npos){
            //label: ...

            // label needs to be in a section
            if(Section::getActiveSection() == nullptr){
                cout<<"------------------------------------------"<<endl;
                std::cerr<<"Error - Assembler: Label needs to be in a section:" + parsedLine[0]<<endl;
                cout<<"------------------------------------------"<<endl;
                return;
            }

            //add a symbol to the symbol table
            parsedLine[0].pop_back();  // remove the ":" at the end of a label
            bool isSymbolAdded = addSymbolToTheSymbolList(parsedLine[0], currentLocation, Scope::LOCAL, true, Section::getActiveSection(), SymbolType::NOTYP);
            if(isSymbolAdded == false)return;

            //delete the first word of the line in case there is an instruction after the label
            parsedLine.erase(parsedLine.begin());
        }
        if(parsedLine.empty())continue;

        if(parsedLine.front().find('.') == 0){
            // directives (.section, .equ itd.)
            int success = handleDirectives(parsedLine);
            if(success !=0)break;
        
        }else{
            // instructions 
            int success = handleInstructions(parsedLine);
            if(success !=0){
                if(success == 1)
                    break;
                else
                    return;
            }
            currentLocation +=4;       
        }

    }

    // Section::determineSectionSizesWithLiteralPools();

    Symbol::resolveSymbolValuesAndFLinks();

    writeToFile(inputFileName, outputFileName);

}