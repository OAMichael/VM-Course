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
    memcpy(m_memory + VM_CONSTANT_POOL_MEMORY_ADDRESS, program.constants.data(), program.constants.size() * sizeof(Immediate));
    memcpy(m_memory + VM_STRING_POOL_MEMORY_ADDRESS, program.strings.data(), program.strings.size() * sizeof(uint8_t));
    memcpy(m_memory + VM_CLASS_DESCS_MEMORY_ADDRESS, program.classes.data(), program.classes.size() * sizeof(uint16_t));

    m_stringPoolPointer += program.strings.size() * sizeof(uint8_t);
    m_classesCount = program.classes[0];

    // for (auto v : program.classes) {
    //     std::cout << v << " ";
    // }
    // std::cout << std::endl;
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
