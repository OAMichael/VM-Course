#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>

#include "Common.h"
#include "Instructions.h"
#include "Decoder.h"
#include "VirtualMachine.h"

namespace VM {

class Interpreter {
private:
    VirtualMachine* m_vm;
    Decoder m_decoder;
    Frame* m_currFrame;

    uint8_t* memory;
    Register& accumulator;
    Register* regfile;
    uint64_t& arenaPointer;

    inline Immediate loadConstant(const ImmediateIndex idx);

    #include "generated/Executors.h"

public:
    bool interpret(const uint64_t entry);

    Interpreter(VirtualMachine* p_vm) : m_vm(p_vm), memory(p_vm->m_memory), accumulator(m_vm->m_accumulator),  arenaPointer(m_vm->m_arenaPointer) {};
};

}   // VM

#endif  // INTERPRETER_H
