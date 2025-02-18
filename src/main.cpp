#include "VM.h"

int main()
{
    VM vm;

    Function mainFunc;
    mainFunc.returnAddress = 0;
    mainFunc.returnType = Type::Float;
    mainFunc.funcAddress = 0;
    mainFunc.functionScope.resize(3);

    float var1 = 2.0f;
    float var2 = 4.0f;
    float var3 = 0.0f;
    mainFunc.functionScope[0] = *(uint32_t*) &var1;
    mainFunc.functionScope[1] = *(uint32_t*) &var2;
    mainFunc.functionScope[2] = *(uint32_t*) &var3;

    mainFunc.variableTable.push_back({ Type::Float, 0});
    mainFunc.variableTable.push_back({ Type::Float, 1 });
    mainFunc.variableTable.push_back({ Type::Float, 2 });

    mainFunc.code = {
        static_cast<uint8_t>(Inst::DivFloat), 0, 1, 2,
        static_cast<uint8_t>(Inst::Return), 2,
        static_cast<uint8_t>(Inst::Halt)
    };

    vm.LoadFunction(mainFunc);
    vm.RunFunction(mainFunc.funcAddress);

    std::cout << vm.GetDisassembly() << std::endl;

    // For int
    // std::cout << "Result: " << vm.GetValueAt(0) << std::endl;

    // For float
    uint32_t result = vm.GetValueAt(0);
    std::cout << "Result: " << * (float*) &result << std::endl;

    vm.DumpMemory();
    return 0;
}