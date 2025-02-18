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
    AddInt = 0x00,
    SubInt = 0x01,
    MulInt = 0x02,
    DivInt = 0x03,

    AddFloat = 0x04,
    SubFloat = 0x05,
    MulFloat = 0x06,
    DivFloat = 0x07,

    Return = 0xFE,
    Halt = 0xFF
};

struct Variable
{
    Type type;
    size_t offset;
};

struct Function
{
    size_t returnAddress;
    Type returnType;
    std::vector<uint8_t> code;
    std::vector<uint32_t> functionScope;
    std::vector<Variable> variableTable;
};

class VM
{
public:
    VM();
    ~VM();

    size_t LoadFunction(const Function& func);
    void RunFunction(size_t funcIndex);
    uint32_t GetValueAt(size_t offset) const;

    std::string GetDisassembly();

private:
    std::array<uint32_t, 128> memory = { 0 };
    std::vector<Function> functions;
    size_t pc;

    void PerformIntArithmetic(Inst instruction, Function& func);
    void PerformFloatArithmetic(Inst instruction, Function& func);
    std::string PrintOperands(const Function& func, size_t& instIndex);
};