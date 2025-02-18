#include "VM.h"

VM::VM() : pc(0) {}

VM::~VM() {}

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
            // Get the indices for variables
            size_t varIndexA = code[pc++];
            size_t varIndexB = code[pc++];

            // Access functionScope directly as uint32_t
            int32_t a = static_cast<int32_t>(func.functionScope[varIndexA]);
            int32_t b = static_cast<int32_t>(func.functionScope[varIndexB]);
            int32_t result = a + b;

            // Store result in memory at returnAddress
            memory[func.returnAddress] = static_cast<uint32_t>(result);
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

int32_t VM::GetValueAt(size_t offset) const
{
    return static_cast<int32_t>(memory[offset]);
}

std::string VM::GetDisassembly()
{
    std::stringstream ss;

    ss << "------------ Disassembly ------------\n";

    for (size_t funcIndex = 0; funcIndex < functions.size(); ++funcIndex)
    {
        const Function& function = functions[funcIndex];
        ss << "Function " << funcIndex << ":\n";
        ss << "  Return Address: " << function.returnAddress << "\n";
        ss << "  Return Type: " << (function.returnType == Type::Int ? "int" : "float") << "\n";
        ss << "  Code:\n";

        size_t instructionIndex = 0;
        while (instructionIndex < function.code.size())
        {
            uint8_t instruction = function.code[instructionIndex++];
            ss << "    Instruction: " << static_cast<int>(instruction);

            switch (static_cast<Inst>(instruction))
            {
            case Inst::AddInt:
                ss << " (Operands: " << static_cast<int>(function.code[instructionIndex]) << ", "
                    << static_cast<int>(function.code[instructionIndex + 1]) << ")";
                instructionIndex += 2; // Move past operands
                break;
            case Inst::Return:
            case Inst::Halt:
                break;
            default:
                ss << " (Unknown opcode)";
                break;
            }
            ss << "\n";
        }

        // Print the variables in function scope
        ss << "  Variables:\n";
        ss << "    Variable Count: " << function.variableTable.size() << "\n";
        for (size_t varIndex = 0; varIndex < function.variableTable.size(); ++varIndex)
        {
            const Variable& variable = function.variableTable[varIndex];
            uint32_t value = function.functionScope[variable.offset];

            ss << "    Variable " << varIndex << ": ";
            ss << (variable.type == Type::Int ? "int" : "float") << " = " << value << "\n";
        }
    }

    ss << "-------------------------------------\n";

    return ss.str();
}