#include "VM.h"

int main()
{
    VM vm;

    Function mainFunc;
    mainFunc.returnAddress = 0;
    mainFunc.returnType = Type::Int;

    mainFunc.functionScope.resize(2);

    mainFunc.functionScope[0] = 2;
    mainFunc.functionScope[1] = 4;

    mainFunc.variableTable.push_back({ Type::Int, 0 });
    mainFunc.variableTable.push_back({ Type::Int, 1 });

    mainFunc.code = {
        static_cast<uint8_t>(Inst::AddInt),
        0,
        1,
        static_cast<uint8_t>(Inst::Return),
        static_cast<uint8_t>(Inst::Halt)
    };

    vm.LoadFunction(mainFunc);

    vm.RunFunction(0);

    std::cout << vm.GetDisassembly() << std::endl;

    std::cout << "main() result: " << vm.GetValueAt(mainFunc.returnAddress) << std::endl;

    return 0;
}