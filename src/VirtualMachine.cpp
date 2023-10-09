#include <algorithm>
#include <cstring>

#include "VirtualMachine.h"

namespace VM {

bool VirtualMachine::run() {
    bool interpretResult = m_interpreter.interpret(m_pc);

    return interpretResult;
}

bool VirtualMachine::loadProgram(const Common::Program& program) {
    // Perform copying of the program
    m_pc = program.entryPoint;
    memcpy(m_memory, program.instructions.data(), program.instructions.size() * sizeof(EncodedInstruction));

    return true;
}

const Register VirtualMachine::getRegisterValue(RegisterType reg) const {
    return m_regfile[reg];
}

}   // VM
