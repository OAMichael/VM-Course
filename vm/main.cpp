#include <iostream>

#include "VirtualMachine.h"


int main(int argc, char* argv[]) {

    std::string programFile = "../asm/main.prog";
    Common::Program program;

    if (argc > 1) {
        programFile = argv[1];
    }
    else {
        std::cout << "Using default program: " << programFile << std::endl;
    }

    if (!deserializeProgram(programFile, program)) {
        std::cerr << "Could not load program: " << programFile << std::endl;
        return -1;
    }
    std::cout << "Successfully load program: " << programFile << std::endl;

    VM::VirtualMachine vm;
    vm.loadProgram(program);
    bool runResult = vm.run();

    if (runResult) {
        std::cout << "Program has been successfully interpreted" << std::endl;
    }
    else {
        std::cerr << "Program execution has been finished with errors" << std::endl;
        return -1;
    }

    return 0;
}
