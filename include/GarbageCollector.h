#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

#include "Instructions.h"

namespace VM {

class VirtualMachine;
class Allocator;
class Interpreter;

class GarbageCollector {
private:
    VirtualMachine* const m_vm;

    // Just shortcuts for m_vm->m_memory, m_vm->m_allocator and m_vm->m_interpreter
    uint8_t* memory;
    Allocator* const allocator;
    Interpreter* const interpreter;

    void visitAliveObject(const uint64_t addr);
    void markAndSweepDeadObjectsInternal(const Frame* frame);

public:
    void markAndSweepDeadObjects();

    GarbageCollector(VirtualMachine* p_vm);
};

}   // VM

#endif  // GARBAGE_COLLECTOR_H
