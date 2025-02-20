#include "VM.h"
#include <unordered_map>
#include <iomanip>

std::unordered_map<Inst, std::string> instToStringMap = {
    {Inst::AddInt, "AddInt"},
    {Inst::SubInt, "SubInt"},
    {Inst::MulInt, "MulInt"},
    {Inst::DivInt, "DivInt"},

    {Inst::AddFloat, "AddFloat"},
    {Inst::SubFloat, "SubFloat"},
    {Inst::MulFloat, "MulFloat"},
    {Inst::DivFloat, "DivFloat"},

    {Inst::CallFuncInt, "CallFuncInt"},
	{Inst::CallFuncFloat, "CallFuncFloat"},

    {Inst::Return, "Return"},
    {Inst::Halt, "Halt"}
};

VM::VM() : pc(0) {}

VM::~VM() {}

void VM::LoadFunction(const Function& func)
{
    if (func.funcAddress >= functions.size())
    {
        functions.resize(func.funcAddress + 1);
    }

    functions[func.funcAddress] = func;

    switch (func.returnType)
    {
    case Type::Int:
        memUsed++;
        break;

    case Type::Float:
        memUsed++;
        break;

    default:
        std::cerr << "nah bro you can use whatever that is for function return type line:" << __LINE__ << std::endl;
    }
}

void VM::RunFunction(size_t funcIndex)
{
    Function& func = functions[funcIndex];
    const auto& code = func.code;
    pc = 0;

    while (pc < code.size())
    {
        uint8_t instruction = code[pc++];
        switch (static_cast<Inst>(instruction))
        {
        case Inst::AddInt:
            PerformIntArithmetic(static_cast<Inst>(instruction), func);
            break;

        case Inst::SubInt:
            PerformIntArithmetic(static_cast<Inst>(instruction), func);
            break;

        case Inst::MulInt:
            PerformIntArithmetic(static_cast<Inst>(instruction), func);
            break;

        case Inst::DivInt:
            PerformIntArithmetic(static_cast<Inst>(instruction), func);
            break;

        case Inst::AddFloat:
            PerformFloatArithmetic(static_cast<Inst>(instruction), func);
            break;

        case Inst::SubFloat:
            PerformFloatArithmetic(static_cast<Inst>(instruction), func);
            break;

        case Inst::MulFloat:
            PerformFloatArithmetic(static_cast<Inst>(instruction), func);
            break;

        case Inst::DivFloat:
            PerformFloatArithmetic(static_cast<Inst>(instruction), func);
            break;

        case Inst::CallFuncInt:
        {
            size_t funcIndex = func.code[pc++];
            size_t returnIndex = func.code[pc++];
            size_t currPc = pc;

            RunFunction(funcIndex);
            
            // Continue where we left off from, SO HARD TO FIND
            pc = currPc;
            func.functionScope[returnIndex] = memory[functions[funcIndex].returnAddress];
            break;
        }

        case Inst::CallFuncFloat:
        {
            size_t funcIndex = func.code[pc++];
            size_t returnIndex = func.code[pc++];
            size_t currPc = pc;

            RunFunction(funcIndex);

            // Continue where we left off from, SO HARD TO FIND
            pc = currPc;
            func.functionScope[returnIndex] = memory[functions[funcIndex].returnAddress];
            break;
        }

        case Inst::Return:
        {
            memory[func.returnAddress] = func.functionScope[func.code[pc++]];
            break;
        }

        case Inst::Halt:
            return;

        default:
            std::cerr << "Unknown instruction! line: " << __LINE__ << std::endl;
            return;
        }
    }
}

void VM::PerformIntArithmetic(Inst instruction, Function& func)
{
    uint8_t varIndexA = func.code[pc++];
    uint8_t varIndexB = func.code[pc++];
    uint8_t resultIndex = func.code[pc++];

    int32_t a = static_cast<uint32_t>(func.functionScope[varIndexA]);
    int32_t b = static_cast<uint32_t>(func.functionScope[varIndexB]);
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
        if (b != 0)
            result = a / b;
        else
            std::cerr << "Division by zero at line " << __LINE__ << std::endl;
        break;

    default:
        std::cerr << "Unknown instruction! line: " << __LINE__ << std::endl;
        break;
    }

    // Store result to the specified variable in functionScope
    func.functionScope[resultIndex] = static_cast<uint32_t>(result);
}

void VM::PerformFloatArithmetic(Inst instruction, Function& func) 
{
    size_t varIndexA = func.code[pc++];
    size_t varIndexB = func.code[pc++];
    size_t resultIndex = func.code[pc++];

    float a = * (float*) &func.functionScope[varIndexA];
    float b = * (float*) &func.functionScope[varIndexB];
    float result = 0;

    switch (instruction)
    {
    case Inst::AddFloat:
        result = a + b;
        break;
    case Inst::SubFloat:
        result = a - b;
        break;
    case Inst::MulFloat:
        result = a * b;
        break;
    case Inst::DivFloat:
        if (b != 0)
            result = a / b;
        else
            std::cerr << "Division by zero at line " << __LINE__ << std::endl;
        break;

    default:
        std::cerr << "Unknown instruction! line: " << __LINE__ << std::endl;
        break;
    }

    // Store result to the specified variable in functionScope
    func.functionScope[resultIndex] = * (uint32_t*) &result;
}

uint32_t VM::GetValueAt(size_t offset) const
{
    return static_cast<uint32_t>(memory[offset]);
}

std::string VM::GetDisassembly()
{
    std::stringstream ss;

    ss << "------------ Disassembly ------------\n";

    for (size_t funcIndex = 0; funcIndex < functions.size(); ++funcIndex)
    {
        const Function& function = functions[funcIndex];
        ss << "  ----------------------------------  \nFunction " << funcIndex << ":\n";
        ss << "  Return Address: " << function.returnAddress << "\n";
        ss << "  Return Type: " << (function.returnType == Type::Int ? "int" : "float") << "\n";

        ss << "\n  Variables:\n";
        ss << "    Variable Count: " << function.functionScope.size() << "\n";
        for (size_t varIndex = 0; varIndex < function.functionScope.size(); ++varIndex)
        {
            uint32_t value = function.functionScope[varIndex];

            ss << "    Var " << varIndex << ": ";
            ss << value << std::endl;
        }

        ss << "\n  Code:\n";

        size_t instructionIndex = 0;
        while (instructionIndex < function.code.size())
        {
            uint8_t instruction = function.code[instructionIndex++];
            ss << "    " << instToStringMap[(Inst)instruction];

            switch (static_cast<Inst>(instruction))
            {
            case Inst::AddInt:
                ss << PrintOperands(function, instructionIndex, 3);
                break;

            case Inst::SubInt:
                ss << PrintOperands(function, instructionIndex, 3);
                break;

            case Inst::MulInt:
                ss << PrintOperands(function, instructionIndex, 3);
                break;

            case Inst::DivInt:
                ss << PrintOperands(function, instructionIndex, 3);
                break;

            case Inst::AddFloat:
                ss << PrintOperands(function, instructionIndex, 3);
                break;

            case Inst::SubFloat:
                ss << PrintOperands(function, instructionIndex, 3);
                break;

            case Inst::MulFloat:
                ss << PrintOperands(function, instructionIndex, 3);
                break;

            case Inst::DivFloat:
                ss << PrintOperands(function, instructionIndex, 3);
                break;

            case Inst::Return:
                ss << "\t\t" << function.returnAddress;
                instructionIndex += 1;
                break;

            case Inst::CallFuncInt:
                ss << PrintOperands(function, instructionIndex, 2);
                break;

            case Inst::CallFuncFloat:
                ss << PrintOperands(function, instructionIndex, 2);
                break;

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

std::string VM::PrintOperands(const Function& func, size_t& instIndex, size_t operandsCount)
{
    std::string operands = "\t\t";
    for (int i = 0; i < operandsCount; i++)
    {
        operands += (i == 0 ? "" : ", ") + std::to_string(func.code[instIndex + i]);
    }

    instIndex += operandsCount;
    return operands;
}

void VM::DumpMemory()
{
    std::cout << "\nMem dump\n";
    std::cout << "Loc  | hex      | int        | float\n";
    std::cout << "---- | -------- | ---------- | ---------------\n";
    for (size_t i = 0; i < memory.size(); i ++)
    {
        std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0')
            << i << " | ";

        std::cout << std::hex << std::setw(8) << std::setfill('0')
            << memory[i] << " | ";

        std::cout << std::dec << std::setw(10) << std::setfill(' ')
            << *(int *) & memory[i] << " | ";

        std::cout << std::dec << std::setw(15) << std::setfill(' ')
            << *(float *)&memory[i] << std::endl;
    }

    std::cout << std::dec;
}