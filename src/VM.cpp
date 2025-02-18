#include "VM.h"
#include <unordered_map>

VM::VM() : pc(0) {}

VM::~VM() = default;

std::unordered_map<uint8_t, std::string> instructionMap = {
        {static_cast<uint8_t>(Inst::AddInt), "AddInt"},
        {static_cast<uint8_t>(Inst::Return), "Return"},
        {static_cast<uint8_t>(Inst::Halt), "Halt"}
};

void VM::LoadFunction(const Function& func)
{
    functions.push_back(func);
}

void VM::RunFunction(size_t funcIndex)
{
    const auto& func = functions[funcIndex];
    const auto& code = func.code;
    pc = 0;

    while (pc < code.size())
    {
        uint8_t instruction = code[pc++];
        switch (static_cast<Inst>(instruction))
        {
        case Inst::AddInt:
        {
            int32_t a = code[pc++];
            int32_t b = code[pc++];
            int32_t result = a + b;

            memory[func.returnAddress] = result;
            
            break;
        }
        case Inst::Return:
            return;
        case Inst::Halt:
            return;
        default:
            std::cerr << "Unknown instruction!" << std::endl;
            return;
        }
    }
}

int32_t VM::GetValueAt(size_t loc) const
{
    return static_cast<int32_t>(memory[loc]);
}

std::string VM::GetDisassembly()
{
    std::stringstream ss;

    for (size_t funcIndex = 0; funcIndex < functions.size(); ++funcIndex)
    {
        const Function& function = functions[funcIndex];
        ss << "Function " << funcIndex << ":\n";
        ss << "  Return Address: " << function.returnAddress << "\n";
        ss << "  Return Type: " << (function.returnType == Type::Int ? "int" : "float") << "\n";

        size_t instructionIndex = 0;
        while (instructionIndex < function.code.size())
        {
            uint8_t instruction = function.code[instructionIndex++];
            ss << "    " << instructionMap[instruction];

            switch (static_cast<Inst>(instruction))
            {
            case Inst::AddInt:
            {
                int32_t a = function.code[instructionIndex++];
                int32_t b = function.code[instructionIndex++];
                ss << ": \t\t" << a << ", " << b;
                break;
            }
            case Inst::Return:
            case Inst::Halt:
                break;
            default:
                ss << " (Unknown opcode: " << static_cast<int>(instruction) << ")";
                break;
            }
            ss << "\n";
        }
    }

    return ss.str();
}