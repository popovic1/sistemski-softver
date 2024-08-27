#include <string>
#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include "../include/emulator.hpp"

Emulator::Emulator() {
    for(int i = 0; i < 14; i++) {
        regs.push_back("00000000");
    }

    for(int i = 0; i < 3; i++) {
        csr.push_back("00000000");
    }
}

Emulator::~Emulator() {}

std::string Emulator::toBigEndian(std::string val) {
    if (val.length() > 8) {
        std::cerr << "Error: Invalid input length for conversion to big endian. Input: " << val << std::endl;
        return "";
    }else if(val.length()<8){
        while(val.length()<8){
            val = "0" + val;
        }
        
    }

    return val.substr(6, 2) + val.substr(4, 2) + val.substr(2, 2) + val.substr(0, 2);
}

void Emulator::setCSRValue(std::string regIndex, std::string value) {

    uint32_t index = std::stoul(regIndex, nullptr, 16);

    if (index < csr.size()) {
        csr[index] = value;
    } else {
        std::cerr << "Error: CSR index out of bounds. Index: " << regIndex << std::endl;
    }
}

void Emulator::setRegisterValue(string reg, string value) {
    if (reg == "F" || reg == "f") {
        pc = value;
    } else if (reg == "E" || reg == "e") {
        sp = value;
    } else {
        uint32_t regNum = std::stoul(reg, nullptr, 16);

        if (regNum < regs.size()) {
            regs[regNum] = value;
        } else {
            std::cerr << "Error: Register index out of bounds. Register: " << reg << std::endl;
        }
    }
}

std::string Emulator::getRegisterValue(string reg) {
    if (reg == "F" || reg == "f") {
        return pc;
    } else if (reg == "E" || reg == "e") {
        return sp;
    } else {
        uint32_t regNum = std::stoul(reg, nullptr, 16);
        if (regNum < regs.size()) {

            std::string value = regs[regNum];
            std::transform(value.begin(), value.end(), value.begin(), ::toupper);
            return value;
        } else {
            std::cerr << "Error: Register index out of bounds. Register: " << reg << std::endl;
            return ""; 
        }
    }
}

std::string Emulator::getCSRValue( std::string regIndex) {
    uint32_t index = std::stoul(regIndex, nullptr, 16);

    if (index < csr.size()) {
        return csr[index];
    } else {
        std::cerr << "Error: CSR index out of bounds. Index: " << regIndex << std::endl;
        return "00000000";
    }
}

string Emulator::calcIndAddr(std::string regA, std::string regB, std::string offset) {
    std::string valueA = getRegisterValue(regA);
    std::string valueB = getRegisterValue(regB);

    uint32_t intA = std::stoul(valueA, nullptr, 16);
    uint32_t intB = std::stoul(valueB, nullptr, 16);
    int32_t offsetValue = 0;
    if (offset.length() == 3 && (offset[0] >= '8')) {
        offsetValue = std::stoul(offset, nullptr, 16) - 4096;
    } else {
        offsetValue = std::stoul(offset, nullptr, 16);
    }

    uint32_t result = intA + intB + offsetValue;


    std::stringstream ss;
    ss << std::hex << result;
    std::string address = ss.str();
    std::transform(address.begin(), address.end(), address.begin(), ::toupper);

    if (address.length() < 8) {

        address.insert(0, 8 - address.length(), '0');
    }

    return address;
}

void Emulator::printRegisterValues() {
    cout << "Emulated processor executed halt instruction" << endl;
    cout <<"Emulated processor state:" << endl;

    for(int i = 0; i < regs.size(); i++) {
        if(i % 4 == 0) 
            cout << endl;
        if(i < 10)
            cout << " r" + to_string(i) + "=0x" + regs[i] + " ";
        else 
            cout << "r" + to_string(i) + "=0x" + regs[i] + " ";
    }

    cout << "r14=0x" + sp + " ";
    cout << "r15=0x" + pc + " " << endl;
}

void Emulator::loadFromFile(std::string fileName) {
    std::ifstream file(fileName);
    std::string line; 

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << fileName << std::endl;
        return;
    }

    while (std::getline(file, line)) {
        if (!line.empty()) {
            std::string address = line.substr(0, 8);
            std::transform(address.begin(), address.end(), address.begin(), ::toupper);

            if (line.length() == 34) {
                std::string firstInstr = line.substr(10, 11);
                firstInstr.erase(std::remove(firstInstr.begin(), firstInstr.end(), ' '), firstInstr.end());
                std::transform(firstInstr.begin(), firstInstr.end(), firstInstr.begin(), ::toupper);
                code[address] = firstInstr;

                uint32_t nextAddr = std::stoul(address, nullptr, 16) + 4;
                std::stringstream ss;
                ss << std::hex << nextAddr;
                std::string nextAddress = ss.str();
                std::transform(nextAddress.begin(), nextAddress.end(), nextAddress.begin(), ::toupper);

                std::string secondInstr = line.substr(22, 11);
                secondInstr.erase(std::remove(secondInstr.begin(), secondInstr.end(), ' '), secondInstr.end());
                std::transform(secondInstr.begin(), secondInstr.end(), secondInstr.begin(), ::toupper);
                code[nextAddress] = secondInstr;

            } else if (line.length() == 22) {
                std::string firstInstr = line.substr(10, 11);
                firstInstr.erase(std::remove(firstInstr.begin(), firstInstr.end(), ' '), firstInstr.end());
                std::transform(firstInstr.begin(), firstInstr.end(), firstInstr.begin(), ::toupper);
                code[address] = firstInstr;
            }
        }
    }
}





void Emulator::updatePC() {
    uint32_t nextPC = std::stoul(pc, nullptr, 16) + 4;
    std::stringstream ss;
    ss << std::hex << nextPC;
    pc = ss.str();
    std::transform(pc.begin(), pc.end(), pc.begin(), ::toupper);
}

bool Emulator::handleOpcode0(const std::string& opcode) {
    if (opcode == "00") {
        printRegisterValues();
        exit(0);
        return true;
    }
    return false;
}

void Emulator::handleOpcode1(const std::string& opcode) {
    if (opcode == "10") {
        std::string newSP = calcIndAddr("E", "00000000", "FFC");
        if (code.find(newSP) == code.end()) {
            code[newSP] = toBigEndian(csr[0]);
            sp = newSP;
        }
        newSP = calcIndAddr("E", "00000000", "FFC");
        if (code.find(newSP) == code.end()) {
            code[newSP] = toBigEndian(pc);
            sp = newSP;
        }
        csr[2] = "00000004";
        csr[0] = "00000000";
        pc = csr[1];
    }
}

void Emulator::handleOpcode2(const std::string& opcode, const std::string& gprA, const std::string& gprB, const std::string& offset) {
    if (opcode == "20") {
        std::string newSP = calcIndAddr("E", "00000000", offset);
        if (code.find(newSP) == code.end()) {
            code[newSP] = toBigEndian(pc);
            sp = newSP;
        }
        pc = calcIndAddr(gprA, gprB, offset);
    } else if (opcode == "21") {
        std::string newSP = calcIndAddr("E", "00000000", "FFC");
        code[newSP] = toBigEndian(pc);
        sp = newSP;
        std::string value = code[calcIndAddr(gprA, gprB, offset)];
        pc = toBigEndian(value);
    }
}

void Emulator::handleBranching(const std::string& opcode, const std::string& gprA, const std::string& gprB, const std::string& gprC, const std::string& offset) {
    uint32_t rgB = std::stoul(getRegisterValue(gprB), nullptr, 16);
    uint32_t rgC = std::stoul(getRegisterValue(gprC), nullptr, 16);

    switch (opcode[1]) {
        case '0':
            pc = calcIndAddr(gprA, "00000000", offset);
            break;
        case '1':
            if (rgB == rgC) {
                pc = calcIndAddr(gprA, "00000000", offset);
            }
            break;
        case '2':
            if (rgB != rgC) {
                pc = calcIndAddr(gprA, "00000000", offset);
            }
            break;
        case '3':
            if (rgB > rgC) {
                pc = calcIndAddr(gprA, "00000000", offset);
            }
            break;
        case '8':
            pc = toBigEndian(code[calcIndAddr(gprA, "00000000", offset)]);
            break;
        case '9':
            if (rgB == rgC) {
                pc = toBigEndian(code[calcIndAddr(gprA, "00000000", offset)]);
            }
            break;
        case 'A':
            if (rgB != rgC) {
                pc = toBigEndian(code[calcIndAddr(gprA, "00000000", offset)]);
            }
            break;
        case 'B':
            if (rgB > rgC) {
                pc = toBigEndian(code[calcIndAddr(gprA, "00000000", offset)]);
            }
            break;
    }
}

void Emulator::handleSwap(const std::string& gprB, const std::string& gprC) {
    std::string regB = getRegisterValue(gprB);
    std::string regC = getRegisterValue(gprC);
    setRegisterValue(gprB, regC);
    setRegisterValue(gprC, regB);
}

void Emulator::handleArithmetic(const std::string& opcode, const std::string& gprA, const std::string& gprB, const std::string& gprC) {
    uint32_t rgB = std::stoul(getRegisterValue(gprB), nullptr, 16);
    uint32_t rgC = std::stoul(getRegisterValue(gprC), nullptr, 16);
    uint32_t rgA = 0;

    switch (opcode[1]) {
        case '0':
            rgA = rgB + rgC;
            break;
        case '1':
            rgA = rgB - rgC;
            break;
        case '2':
            rgA = rgB * rgC;
            break;
        case '3':
            rgA = rgB / rgC;
            break;
    }

    std::stringstream ss;
    ss << std::hex << rgA;
    std::string result = ss.str();
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);

    setRegisterValue(gprA, result);
}

void Emulator::handleLogical(const std::string& opcode, const std::string& gprA, const std::string& gprB, const std::string& gprC) {
    uint32_t rgB = std::stoul(getRegisterValue(gprB), nullptr, 16);
    uint32_t rgC = std::stoul(getRegisterValue(gprC), nullptr, 16);
    uint32_t rgA = 0;

    switch (opcode[1]) {
        case '0':
            rgA = ~rgB;
            break;
        case '1':
            rgA = rgB & rgC;
            break;
        case '2':
            rgA = rgB | rgC;
            break;
        case '3':
            rgA = rgB ^ rgC;
            break;
    }

    std::stringstream ss;
    ss << std::hex << rgA;
    std::string result = ss.str();
    result.insert(0, 8 - result.length(), '0');
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);

    setRegisterValue(gprA, result);
}

void Emulator::handleShift(const std::string& opcode, const std::string& gprA, const std::string& gprB, const std::string& gprC) {
    uint32_t rgB = std::stoul(getRegisterValue(gprB), nullptr, 16);
    uint32_t rgC = std::stoul(getRegisterValue(gprC), nullptr, 16);
    uint32_t rgA = 0;

    switch (opcode[1]) {
        case '0':
            rgA = rgB >> rgC;
            break;
        case '1':
            rgA = rgB << rgC;
            break;
    }

    std::stringstream ss;
    ss << std::hex << rgA;
    std::string result = ss.str();
    result.insert(0, 8 - result.length(), '0');
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);

    setRegisterValue(gprA, result);
}

void Emulator::handleMemory(const std::string& opcode, const std::string& gprA, const std::string& gprB, const std::string& gprC, const std::string& offset) {
    std::string targetAddress;

    switch (opcode[1]) {
        case '0': 
            targetAddress = calcIndAddr(gprA, gprB, offset);
            code[targetAddress] = toBigEndian(getRegisterValue(gprC));
            break;
        case '1':  // Push operation
            targetAddress = calcIndAddr(gprA, "00000000", offset);
            if (code.find(targetAddress) == code.end()) {
                code[targetAddress] = "00000000";
            }
            sp = targetAddress;
            setRegisterValue(gprA, targetAddress);
            code[targetAddress] = toBigEndian(getRegisterValue(gprC));
            break;
        case '2':  // Store indirect operation
            targetAddress = toBigEndian(code[calcIndAddr(gprA, gprB, offset)]);
            targetAddress.insert(0, 8 - targetAddress.length(), '0');
            code[targetAddress] = toBigEndian(getRegisterValue(gprC));
            break;
    }
}

void Emulator::handleCSR(const std::string& opcode, const std::string& gprA, const std::string& gprB, const std::string& gprC, const std::string& offset) {
    switch (opcode[1]) {
        case '0':
            setRegisterValue(gprA, getCSRValue(gprB));
            break;
        case '1':
            setRegisterValue(gprA, calcIndAddr("00000000", gprB, offset));
            break;
        case '2':
            {
                std::string value = toBigEndian(code[calcIndAddr(gprB, gprC, offset)]);
                if (gprA == "E") {
                    if (code.find(value) == code.end()) {
                        code[value] = "00000000";
                    }
                    setRegisterValue(gprA, value);
                } else {
                    setRegisterValue(gprA, value);
                }
            }
            break;
        case '3':  // Pop operation
            {
                if (code.find(sp) == code.end()) {
                    code[sp] = "00000000";
                }
                std::string value = toBigEndian(code[sp]);
                setRegisterValue(gprA, value);

                value = calcIndAddr("00000000", gprB, offset);
                setRegisterValue(gprB, value);
                sp = value;
            }
            break;
        case '4':
            setCSRValue(gprA, getRegisterValue(gprB));
            break;
        case '5':
            {
                uint32_t rgA = std::stoul(getCSRValue(gprA), nullptr, 16);
                uint32_t rgB = std::stoul(getCSRValue(gprB), nullptr, 16);
                uint32_t off = std::stoul(offset, nullptr, 16);
                
                rgA = rgB | off;
                std::stringstream ss;
                ss << std::hex << rgA;
                std::string result = ss.str();
                result.insert(0, 8 - result.length(), '0');
                setCSRValue(gprA, result);
            }
            break;
        case '6':
            setCSRValue(gprA, toBigEndian(code[calcIndAddr(gprB, gprC, offset)]));
            break;
        case '7':
            {
                std::string regBValue = getCSRValue(gprB);
                std::string value = toBigEndian(code[regBValue]);
                setCSRValue(gprA, value);

                value = calcIndAddr("00000000", regBValue, offset);
                value.insert(0, 8 - value.length(), '0');
                std::transform(value.begin(), value.end(), value.begin(), ::toupper);
                setRegisterValue(gprB, value);
            }
            break;
    }
}


void Emulator::emulate() {
    pc = "40000000";
    bool halted = false;
    while (!halted) {
        std::string instruction = code[pc];

        std::string opcode = instruction.substr(6, 2);
        std::string gprA(1, instruction[4]);
        std::string gprB(1, instruction[5]);
        std::string gprC(1, instruction[2]);
        std::string offset = instruction[3] + instruction.substr(0, 2);

        updatePC();

        switch (opcode[0]) {
            case '0':
                halted = handleOpcode0(opcode);
                break;
            case '1':
                handleOpcode1(opcode);
                break;
            case '2':
                handleOpcode2(opcode, gprA, gprB, offset);
                break;
            case '3':
                handleBranching(opcode, gprA, gprB, gprC, offset);
                break;
            case '4':
                handleSwap(gprB, gprC);
                break;
            case '5':
                handleArithmetic(opcode, gprA, gprB, gprC);
                break;
            case '6':
                handleLogical(opcode, gprA, gprB, gprC);
                break;
            case '7':
                handleShift(opcode, gprA, gprB, gprC);
                break;
            case '8':
                handleMemory(opcode, gprA, gprB, gprC, offset);
                break;
            case '9':
                handleCSR(opcode, gprA, gprB, gprC, offset);
                break;
            default:
                std::cerr << "Unknown opcode: " << opcode << std::endl;
                break;
        }
    }
}


int main(int argc, char* argv[]) {
    string inputFile;
    string outputFile;

    if(argc > 2) {
        cout << "ERROR: Too many arguments!" << endl;
        exit(0);
    }
    string programName = argv[1];
    if(programName.find(".hex") == string::npos) {
        cout << "ERROR! Wrong file type!" << endl;
        exit(0);
    }

    Emulator* emulator = new Emulator();
    emulator->loadFromFile(programName);
    emulator->emulate();

   return 0;
} 

