#include <iostream>

#include "VirtualMachine.h"


static bool makeTestProgram_Quadratic(Common::Program& outProg) {
	outProg.entryPoint = 0;

	// mvi x1, 4000
	{
		uint32_t opcode = VM::InstructionType::MVI;
		uint32_t rd = 1 << 8;

		uint32_t imm = 4000 << 13;

		outProg.instructions.push_back(imm | rd | opcode);
	}

	// intrinsic iscan x1
	{
		uint32_t opcode = VM::InstructionType::INTRINSIC;
		uint32_t intrType = VM::IntrinsicType::INTRINSIC_ISCAN << 8;
		uint32_t rs1 = 1 << 16;

		outProg.instructions.push_back(rs1 | intrType | opcode);
	}

	// intrinsic iprint x1
	{
		uint32_t opcode = VM::InstructionType::INTRINSIC;
		uint32_t intrType = VM::IntrinsicType::INTRINSIC_IPRINT << 8;
		uint32_t rs1 = 1 << 16;

		outProg.instructions.push_back(rs1 | intrType | opcode);
	}

	// return
	{
		outProg.instructions.push_back(VM::InstructionType::RET);
	}
	return true;
}


int main(int argc, char* argv[]) {

	Common::Program testQuadratic;
	makeTestProgram_Quadratic(testQuadratic);

	VM::VirtualMachine vm;
	vm.loadProgram(testQuadratic);
	
	bool runResult = vm.run();

	if (runResult) {
		std::cout << "Program has been successfully interpreted" << std::endl;
	}
	else {
		std::cerr << "Program execution has been finished with errors" << std::endl;
	}

	return 0;
}
