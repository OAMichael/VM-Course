#include <algorithm>

#include "VirtualMachine.h"

namespace VM {

bool VirtualMachine::run() {
    m_interpreter.interpret(m_program);

    return true;
}

bool VirtualMachine::loadProgram(const Common::Program& program) {
    // Perform copying of the program
    m_program.entryPoint = program.entryPoint;
    std::copy(program.instructions.begin(), program.instructions.end(), std::back_inserter(m_program.instructions));

    return true;
}

}   // VM
