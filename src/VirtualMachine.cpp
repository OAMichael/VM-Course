#include <algorithm>
#include <cstring>

#include "VirtualMachine.h"

namespace VM {

bool VirtualMachine::run() {
    bool interpretResult = m_interpreter.interpret(m_entry);

    return interpretResult;
}

bool VirtualMachine::loadProgram(const Common::Program& program) {
    // Perform copying of the program
    m_entry = program.entryPoint;
    memcpy(m_memory + VM_PROGRAM_MEMORY_ADDRESS, program.instructions.data(), program.instructions.size() * sizeof(EncodedInstruction));
    memcpy(m_memory + VM_CONSTANT_MEMORY_ADDRESS, program.constants.data(), program.constants.size() * sizeof(Immediate));

    return true;
}

bool VirtualMachine::loadProgram(const std::string& filename) {
    Common::Program program;
    if (!deserializeProgram(filename, program)) {
        return false;
    }
    return loadProgram(program);
}

}   // VM
