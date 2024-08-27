#ifndef EMULATOR_H_
#define EMULATOR_H_

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>

using namespace std;

class Emulator
{
private:
    string pc;
    string sp;
    vector<string> regs;
    vector<string> csr;
    vector<string> stack;
    map<string, string> code;
public:
    Emulator();
    ~Emulator();

    void loadFromFile(string fileName);

    void updatePC();
    bool handleOpcode0(const std::string& opcode);
    void handleOpcode1(const std::string& opcode);
    void handleOpcode2(const std::string& opcode, const std::string& gprA, const std::string& gprB, const std::string& offset);
    void handleBranching(const std::string& opcode, const std::string& gprA, const std::string& gprB, const std::string& gprC, const std::string& offset);
    void handleSwap(const std::string& gprB, const std::string& gprC);
    void handleArithmetic(const std::string& opcode, const std::string& gprA, const std::string& gprB, const std::string& gprC);
    void handleLogical(const std::string& opcode, const std::string& gprA, const std::string& gprB, const std::string& gprC);
    void handleShift(const std::string& opcode, const std::string& gprA, const std::string& gprB, const std::string& gprC);
    void handleMemory(const std::string& opcode, const std::string& gprA, const std::string& gprB, const std::string& gprC, const std::string& offset);
    void handleCSR(const std::string& opcode, const std::string& gprA, const std::string& gprB, const std::string& gprC, const std::string& offset);


    void emulate();

    string calcIndAddr(string gprA, string gprB, string offset);
    string getRegisterValue(string reg);
    string toBigEndian(string val);
    void setRegisterValue(string reg, string value);
    string getCSRValue(string num);
    void setCSRValue(string num, string value);
    void printRegisterValues();

};

#endif