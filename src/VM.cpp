#include "VM.h"
#include <unordered_map>

std::unordered_map<Inst, std::string> instToStringMap =
{
    {Inst::AddInt, "AddInt"},
    {Inst::SubInt, "SubInt"},
    {Inst::MulInt, "MulInt"},
    {Inst::DivInt, "DivInt"},
    {Inst::Return, "Return"},
    {Inst::Halt, "Halt"}
};

VM::VM() : pc(0) {}

VM::~VM() {}

size_t VM::LoadFunction(const Function& func)
{
    functions.push_back(func);
    return functions.size() - 1;
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
            PerformArithmetic(static_cast<Inst>(instruction), func);
            return;

        case Inst::SubInt:
            PerformArithmetic(static_cast<Inst>(instruction), func);
            return;

        case Inst::MulInt:
            PerformArithmetic(static_cast<Inst>(instruction), func);
            return;

        case Inst::DivInt:
            PerformArithmetic(static_cast<Inst>(instruction), func);
            return;

        case Inst::Return:
            return;
        case Inst::Halt:
            return;
        default:
            std::cerr << "Unknown instruction!" << __LINE__ << std::endl;
            return;
        }
    }
}

void VM::PerformArithmetic(Inst instruction, const Function& func)
{
    // Get the indices for variables
    size_t varIndexA = func.code[pc++];
    size_t varIndexB = func.code[pc++];

    // Access functionScope directly as uint32_t
    int32_t a = static_cast<int32_t>(func.functionScope[varIndexA]);
    int32_t b = static_cast<int32_t>(func.functionScope[varIndexB]);
    int32_t result = 0;

    switch (instruction)
    {
    case Inst::AddInt:
        result = a + b;
        break;

    case Inst::SubInt:
        result = a - b;
        break;

    case Inst::MulInt:
        result = a * b;
        break;

    case Inst::DivInt:
        result = a / b;
        break;
    default:
        std::cerr << "Unknown instruction!" << __LINE__ << std::endl;
        break;
    }

    // Store result in memory at returnAddress
    memory[func.returnAddress] = static_cast<uint32_t>(result);
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

        ss << "  Variables:\n";
        ss << "    Variable Count: " << function.variableTable.size() << "\n";
        for (size_t varIndex = 0; varIndex < function.variableTable.size(); ++varIndex)
        {
            const Variable& variable = function.variableTable[varIndex];
            uint32_t value = function.functionScope[variable.offset];

            ss << "    Var " << varIndex << ": ";
            ss << (variable.type == Type::Int ? "int" : "float") << "\n";
        }

        ss << "  Code:\n";

        size_t instructionIndex = 0;
        while (instructionIndex < function.code.size())
        {
            uint8_t instruction = function.code[instructionIndex++];
            ss << "    " << instToStringMap[(Inst)instruction];

            switch (static_cast<Inst>(instruction))
            {
            case Inst::AddInt:
                ss << PrintOperands(function, instructionIndex);
                break;

            case Inst::SubInt:
                ss << PrintOperands(function, instructionIndex);
				break;

            case Inst::MulInt:
                ss << PrintOperands(function, instructionIndex);
                break;

            case Inst::DivInt:
                ss << PrintOperands(function, instructionIndex);
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
    }

    ss << "-------------------------------------\n";

    return ss.str();
}

std::string VM::PrintOperands(const Function& func, size_t& instIndex)
{
    std::string operands = "\t\t" + std::to_string(func.code[instIndex]) + ", " + std::to_string(func.code[instIndex + 1]);
    instIndex += 2;
	return operands;
}