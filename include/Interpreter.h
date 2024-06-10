#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>

#include "Common.h"
#include "Instructions.h"
#include "Decoder.h"

namespace VM {

class VirtualMachine;
class Allocator;
class GarbageCollector;

class Interpreter {
private:
    VirtualMachine* const m_vm;
    Decoder m_decoder;
    Frame* m_currFrame;

    // Just shortcuts for m_vm->m_memory and m_vm->m_allocator
    uint8_t* memory;
    Allocator* const allocator;

    Register accumulator{};
    Register* regfile;

    inline Immediate loadConstant(const ImmediateIndex idx);

    #include "generated/Executors.h"

public:
    bool interpret(const uint64_t entry);

    // Should be const
    inline const Register getAccumulatorValue() const { return accumulator; }
    inline const Frame* getCurrentFrame() const { return m_currFrame; }

    Interpreter(VirtualMachine* p_vm);
};

}   // VM

#endif  // INTERPRETER_H
