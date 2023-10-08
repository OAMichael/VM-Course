#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include <string>

#include "Common.h"
#include "Interpreter.h"

namespace VM {

// 256 KiB for now. TODO: organize memory
static constexpr size_t VM_MEMORY_BYTESIZE = 1 << 18;


class VirtualMachine {
private:

	// Main components of every virtual machine
	Interpreter m_interpreter;

	/*
	GarbageCollector m_gc;
	JITCompiler m_jit;
	*/


	// Virtual machine state
	uint64_t m_pc = 0;
	RegValue m_regfile[RegisterType::REGISTER_COUNT] = {};
	uint8_t m_memory[VM_MEMORY_BYTESIZE] = {};

public:

	bool loadProgram(const Common::Program& program);
	bool loadProgram(const std::string& filename);
	bool run();

	RegValue getRegister(const RegisterType id) const {
		return m_regfile[id];
	}

	void setRegister(const RegisterType id, const RegValue val) {
		m_regfile[id] = val;
	}



	VirtualMachine() : m_interpreter{this} {};

	friend class Interpreter;

};

}	// VM


#endif 	// VIRTUAL_MACHINE_H
