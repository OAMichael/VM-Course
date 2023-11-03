#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include <string>

#include "Common.h"
#include "Interpreter.h"

namespace VM {

/*
 *  Whole memory looks like this:
 *  || instructions || constant pool || string pool || arena ||
 */

static constexpr uint64_t VM_PROGRAM_MEMORY_BYTESIZE       = 1 << 15;     // 32 KiB of instructions
static constexpr uint64_t VM_CONSTANT_POOL_MEMORY_BYTESIZE = 1 << 15;     // 32 KiB of constants
static constexpr uint64_t VM_STRING_POOL_MEMORY_BYTESIZE   = 1 << 15;     // 32 KiB of strings
static constexpr uint64_t VM_ARENA_MEMORY_BYTESIZE         = 1 << 18;     // 256 KiB of arena memory

static constexpr uint64_t VM_TOTAL_MEMORY_BYTESIZE = VM_PROGRAM_MEMORY_BYTESIZE +
                                                     VM_CONSTANT_POOL_MEMORY_BYTESIZE +
                                                     VM_STRING_POOL_MEMORY_BYTESIZE +
                                                     VM_ARENA_MEMORY_BYTESIZE;

static constexpr uint64_t VM_PROGRAM_MEMORY_ADDRESS = 0;
static constexpr uint64_t VM_CONSTANT_POOL_MEMORY_ADDRESS = VM_PROGRAM_MEMORY_ADDRESS + VM_PROGRAM_MEMORY_BYTESIZE;
static constexpr uint64_t VM_STRING_POOL_MEMORY_ADDRESS = VM_CONSTANT_POOL_MEMORY_ADDRESS + VM_CONSTANT_POOL_MEMORY_BYTESIZE;
static constexpr uint64_t VM_ARENA_MEMORY_ADDRESS = VM_STRING_POOL_MEMORY_ADDRESS + VM_STRING_POOL_MEMORY_BYTESIZE;


class VirtualMachine {
private:

    // Main components of every virtual machine
    Interpreter m_interpreter;

    /*
    GarbageCollector m_gc;
    JITCompiler m_jit;
    */


    // Virtual machine state
    uint64_t m_entry = 0;
    Register m_accumulator = {};
    uint64_t m_arenaPointer = VM_ARENA_MEMORY_ADDRESS;
    uint64_t m_stringPoolPointer = VM_STRING_POOL_MEMORY_ADDRESS;
    uint8_t  m_memory[VM_TOTAL_MEMORY_BYTESIZE] = {};

public:

    bool loadProgram(const Common::Program& program);
    bool loadProgram(const std::string& filename);
    bool run();

    VirtualMachine() : m_interpreter{this} {};

    friend class Interpreter;

};

}   // VM


#endif  // VIRTUAL_MACHINE_H
