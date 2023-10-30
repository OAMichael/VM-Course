#include <algorithm>
#include <cstring>

#include "VirtualMachine.h"

namespace VM {

bool VirtualMachine::run() {
    bool interpretResult = m_interpreter.interpret();

    return interpretResult;
}

bool VirtualMachine::loadProgram(const Common::Program& program) {
    // Perform copying of the program
    m_pc = program.entryPoint;
    memcpy(m_memory, program.instructions.data(), program.instructions.size() * sizeof(EncodedInstruction));
    memcpy(m_memory + VM_CONSTANT_POOL_ADDRESS, program.constants.data(), program.constants.size() * sizeof(Immediate));

    return true;
}

bool VirtualMachine::loadProgram(const std::string& filename) {
    Common::Program program;
    if (!deserializeProgram(filename, program)) {
        return false;
    }
    return loadProgram(program);
}

const Register VirtualMachine::getRegisterValue(RegisterType reg) const {
    return m_regfile[reg];
}

}   // VM
