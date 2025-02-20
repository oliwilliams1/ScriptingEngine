#include "VM.h"
#include "Compiler.h"
/*
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
        (uint8_t)Inst::AddInt, 0, 1, 0,
        (uint8_t)Inst::Return, 0
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
        (uint8_t)Inst::CallFuncInt, 1, 0,
        (uint8_t)Inst::AddInt, 0, 1, 0,
        (uint8_t)Inst::Return, 0,
        (uint8_t)Inst::Halt
    };

    vm.LoadFunction(mainFunc);
    vm.RunFunction(mainFunc.funcAddress);

    std::cout << vm.GetDisassembly() << std::endl;

    uint32_t result = vm.GetValueAt(mainFunc.funcAddress);
    std::cout << "main() result: " << * (int*) &result << std::endl;

    vm.DumpMemory();
    return 0;
}


void addOtherFunc(VM& vm)
{
    Function func;
    func.returnAddress = 1;
    func.returnType = Type::Float;
    func.funcAddress = 1;

    func.functionScope.resize(2);
    float var1 = 2;
    float var2 = 3;

    func.functionScope[0] = *(uint32_t*) &var1;
    func.functionScope[1] = *(uint32_t*) &var2;

    func.code = {
        (uint8_t)Inst::AddFloat, 0, 1, 0,
        (uint8_t)Inst::Return, 0
    };

    vm.LoadFunction(func);
}

int main()
{
    VM vm;

    addOtherFunc(vm);

    Function mainFunc;
    mainFunc.returnAddress = 0;
    mainFunc.returnType = Type::Float;
    mainFunc.funcAddress = 0;

    mainFunc.functionScope.resize(2); // remembner to resize

    float var1 = 0.0f;
    float var2 = 2.0f;
    mainFunc.functionScope[0] = *(uint32_t*) &var1;
    mainFunc.functionScope[1] = *(uint32_t*) &var2;

    mainFunc.code = {
        (uint8_t)Inst::CallFuncFloat, 1, 0,
        (uint8_t)Inst::DivFloat, 0, 1, 0,
        (uint8_t)Inst::Return, 0,
        (uint8_t)Inst::Halt
    };

    vm.LoadFunction(mainFunc);
    vm.RunFunction(mainFunc.funcAddress);

    std::cout << vm.GetDisassembly() << std::endl;

    uint32_t result = vm.GetValueAt(mainFunc.funcAddress);
    std::cout << "main() result: " << *(float*)&result << std::endl;

    vm.DumpMemory();
    return 0;
}
*/

int main()
{
    Parser parser;

    parser.CleanUpFile("../playtime/script.sbscript");

    parser.Tokenize();

	parser.Compile();

    return 0;
}