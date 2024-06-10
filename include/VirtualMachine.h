#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include <string>

#include "Common.h"
#include "Interpreter.h"
#include "Allocator.h"
#include "GarbageCollector.h"

namespace VM {

class VirtualMachine {
private:

    // Main components of every virtual machine
    Interpreter m_interpreter;
    GarbageCollector m_gc;

    /*
    JITCompiler m_jit;
    */


    // Virtual machine state
    uint64_t m_entry = 0;
    Allocator m_allocator;
    uint64_t m_stringPoolPointer = VM_STRING_POOL_MEMORY_ADDRESS;
    uint8_t* m_memory = nullptr;

public:

    bool loadProgram(const Common::Program& program);
    bool loadProgram(const std::string& filename);
    bool run();

    inline void invokeGC() { m_gc.markAndSweepDeadObjects(); };

    VirtualMachine() : m_interpreter{this}, m_gc{this}, m_allocator{this} {
        m_allocator.allocateVirtualMachineMemory();
    };

    ~VirtualMachine() {
        m_allocator.deallocateVirtualMachineMemory();
    };

    friend class Interpreter;
    friend class Allocator;
    friend class GarbageCollector;
};

}   // VM


#endif  // VIRTUAL_MACHINE_H
