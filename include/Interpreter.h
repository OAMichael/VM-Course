#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>

#include "Common.h"
#include "Instructions.h"


namespace VM {

class Interpreter {
private:
	uint64_t m_pc;

public:
	bool interpret(const Common::Program& program);
};

}	// VM

#endif	// INTERPRETER_H
