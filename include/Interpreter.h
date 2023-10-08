#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>

#include "Common.h"
#include "Instructions.h"


namespace VM {

class VirtualMachine;

class Interpreter {
private:
	VirtualMachine* m_vm;

public:
	bool interpret(const uint64_t entry);

	Interpreter(VirtualMachine* p_vm) : m_vm{p_vm} {};
};

}	// VM

#endif	// INTERPRETER_H
