#include "VM.h"

int main()
{
    VM vm;

    Function main;
    main.returnAddress = 0;
    main.returnType = Type::Int;

    

    main.code = {
        static_cast<uint8_t>(Inst::AddInt), 0, 1,
        static_cast<uint8_t>(Inst::Return),
        static_cast<uint8_t>(Inst::Halt)
    };

    vm.LoadFunction(main);

    // Run main
    vm.RunFunction(0);

    std::cout << vm.GetDisassembly() << std::endl;

    std::cout << "main() result: " << vm.GetValueAt(main.returnAddress) << std::endl;

    return 0;
}