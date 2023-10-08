#include <iostream>

#include "VirtualMachine.h"


int main(int argc, char* argv[]) {


	Common::Program testProg;
	testProg.entryPoint = 0;
	testProg.instructions.push_back(VM::InstructionType::RET);

	VM::VirtualMachine vm;
	vm.loadProgram(testProg);
	
	bool runResult = vm.run();

	if (runResult) {
		std::cout << "Program has been successfully interpreted" << std::endl;
	}
	else {
		std::cerr << "Program execution has been finished with errors" << std::endl;
	}

	return 0;
}
