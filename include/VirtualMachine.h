#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include <string>

#include "Common.h"
#include "Interpreter.h"

namespace VM {

static constexpr size_t VM_MEMORY_BYTESIZE       = 1 << 18;     // 256 KiB for now. TODO: organize memory
static constexpr size_t VM_CONSTANT_POOL_SIZE    = 1 << 12;     // 4 KiB of constants
static constexpr size_t VM_CONSTANT_POOL_ADDRESS = VM_MEMORY_BYTESIZE - VM_CONSTANT_POOL_SIZE;  // Put the pool at the end of memory


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
    Register m_regfile[RegisterType::REGISTER_COUNT] = {};
    uint8_t m_memory[VM_MEMORY_BYTESIZE] = {};

public:

    bool loadProgram(const Common::Program& program);
    bool loadProgram(const std::string& filename);
    bool run();
    const Register getRegisterValue(RegisterType reg) const; // for check only not use

    VirtualMachine() : m_interpreter{this} {};

    friend class Interpreter;

};

}   // VM


#endif  // VIRTUAL_MACHINE_H
