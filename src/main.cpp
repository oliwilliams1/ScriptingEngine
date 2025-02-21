#include "VM.h"
#include "Compiler.h"
void addOtherFunc(VM& vm)
{
    Function func;
    func.returnAddress = 1;
    func.returnType = Type::Int;
    func.funcAddress = 1;

	func.functionScope.resize(2);
    int var1 = 2;
    int var2 = 3;

    func.functionScope[0] = *(uint32_t*) &var1;
	func.functionScope[1] = *(uint32_t*) &var2;

    func.code = {
        (uint16_t)Inst::AddInt, 0, 1, 0,
        (uint16_t)Inst::Return, 0
    };

    vm.LoadFunction(func);
}

int main()
{
    VM vm;

    addOtherFunc(vm);

    Function mainFunc;
    mainFunc.returnAddress = 0;
    mainFunc.returnType = Type::Int;
    mainFunc.funcAddress = 0;

    mainFunc.functionScope.resize(2); // remembner to resize

    int var1 = 1;
    int var2 = 6;
    mainFunc.functionScope[0] = *(uint32_t*) &var1;
    mainFunc.functionScope[1] = *(uint32_t*) &var2;

    mainFunc.code = {
        (uint16_t)Inst::CallFuncInt, 1, 0,
        (uint16_t)Inst::AddInt, 0, 1, 0,
        (uint16_t)Inst::SetRegister, 0, 0x0000, 0x0123,
        (uint16_t)Inst::Return, 0,
        (uint16_t)Inst::Halt
    };

    vm.LoadFunction(mainFunc);
    vm.RunFunction(mainFunc.funcAddress);

    std::cout << vm.GetDisassembly() << std::endl;

    uint32_t result = vm.GetValueAt(mainFunc.funcAddress);
    std::cout << "main() result: " << * (int*) &result << std::endl;

    vm.DumpMemory();
    return 0;
}

/*
int main()
{
    Parser parser;

    parser.CleanUpFile("../playtime/script.sbscript");
    parser.Tokenize();
	parser.Compile();

    VM vm;

    for (const Function& func : parser.functions)
    {
        vm.LoadFunction(func);
    }
    
    std::cout << vm.GetDisassembly() << std::endl;
    vm.RunFunction(0);

    uint32_t result = vm.GetValueAt(0);
    if (vm.functions[0].returnType == Type::Int)
    {
        std::cout << "main() result: " << *(int*)&result << std::endl;
    }
    else
    {
        std::cout << "main() result: " << *(float*)&result << std::endl;
    }


    return 0;
}

*/