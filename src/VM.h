    #pragma once

#include <vector>
#include <cstdint>
#include <array>
#include <iostream>
#include <string>
#include <sstream>

enum class Type
{
    Int = 0,
    Float = 1
};

enum class Inst
{
    AddInt = 0x0000,
    SubInt = 0x0001,
    MulInt = 0x0002,
    DivInt = 0x0003,

    AddFloat = 0x0004,
    SubFloat = 0x0005,
    MulFloat = 0x0006,
    DivFloat = 0x0007,

    CallFuncInt = 0x0008,
    CallFuncFloat = 0x0009,

    SetRegister = 0x000A,

    Return = 0xFFFE,
    Halt = 0xFFFF
};

struct Variable
{
    Type type;
    size_t offset;
};

struct Function
{
    size_t returnAddress;
    size_t funcAddress;
    Type returnType;
    std::vector<uint16_t> code;
    std::vector<uint32_t> functionScope;
};

class VM
{
public:
    VM();
    ~VM();

    void LoadFunction(const Function& func);
    void RunFunction(size_t funcIndex);
    uint32_t GetValueAt(size_t offset) const;

    std::string GetDisassembly();
    void DumpMemory();

    std::vector<Function> functions;

private:
    size_t memUsed = 0;
    std::array<uint32_t, 32> memory = { 0 };
    size_t pc;

    const static size_t regSize = 32;
    std::array<uint32_t, regSize> registers = { 0 };

    inline void PerformIntArithmetic(Inst instruction, Function& func);
    inline void PerformFloatArithmetic(Inst instruction, Function& func);
    std::string PrintOperands(const Function& func, size_t& instIndex, size_t operandsCount);
};