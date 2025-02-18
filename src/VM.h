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
    Return = 0x01,
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

    void LoadFunction(const Function& func);
    void RunFunction(size_t funcIndex);
    int32_t GetValueAt(size_t offset) const;

    std::string GetDisassembly();

private:
    std::array<uint32_t, 128> memory = { 0 };
    std::vector<Function> functions;
    size_t pc;
};