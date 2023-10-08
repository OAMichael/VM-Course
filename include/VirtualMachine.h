#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include <string>

#include "Common.h"
#include "Interpreter.h"

namespace VM {

class VirtualMachine {
private:

	// Main components of every virtual machine
	Interpreter m_interpreter;

	/*
	GarbageCollector m_gc;
	JITCompiler m_jit;
	*/


	// Virtual machine state
	Common::Program m_program;
	RegValue m_regfile[RegisterType::REGISTER_COUNT];

public:

	bool loadProgram(const Common::Program& program);
	bool loadProgram(const std::string& filename);
	bool run();

};

}	// VM


#endif 	// VIRTUAL_MACHINE_H
