#include "Interpreter.h"
#include <iostream>

namespace VM {

static inline uint8_t getOpcode(const EncodedInstruction& instr) {
    return instr & 0xFF;    // Last 8 bits
}


bool Interpreter::interpret(const Common::Program& program) {
    m_pc = program.entryPoint;
    const auto& instrs = program.instructions; 

    static void* dispatchTable[InstructionType::INSTRUCTION_COUNT] = {
        &&INSTRUCTION_INVALID,

        &&ADD,  &&SUB,  &&AND,  &&OR,  &&XOR,  &&SL,  &&SR,  &&SQRT,  &&SIN,  &&COS,  &&TAN,  &&COT,  &&NEG,

        &&ADDI, &&SUBI, &&ANDI, &&ORI, &&XORI, &&SLI, &&SRI, &&SQRTI, &&SINI, &&COSI, &&TANI, &&COTI, &&NEGI,

        &&BEQ, &&BNE, &&BLT, &&BGE,

        &&LB, &&LH, &&LW, &&LD,

        &&SB, &&SH, &&SW, &&SD,

        &&I2F, &&I2D, &&I2L, &&F2I, &&F2D, &&F2L, &&D2I, &&D2F, &&D2L, &&L2I, &&L2F, &&L2D,

        &&RET,

        &&INITRIN_INPUT, &&INTRIN_OUTPUT,
    };


    #define DISPATCH() goto *dispatchTable[getOpcode(instrs[m_pc++])];


    // Actual main interpretation
    DISPATCH()


INSTRUCTION_INVALID:
    return false;



ADD:


    DISPATCH()

SUB:


    DISPATCH()

AND:


    DISPATCH()

OR:


    DISPATCH()

XOR:


    DISPATCH()

SL:


    DISPATCH()

SR:


    DISPATCH()

SQRT:


    DISPATCH()

SIN:


    DISPATCH()

COS:


    DISPATCH()

TAN:


    DISPATCH()

COT:


    DISPATCH()

NEG:


    DISPATCH()

ADDI:


    DISPATCH()

SUBI:


    DISPATCH()

ANDI:


    DISPATCH()

ORI:


    DISPATCH()

XORI:


    DISPATCH()

SLI:


    DISPATCH()

SRI:


    DISPATCH()

SQRTI:


    DISPATCH()

SINI:


    DISPATCH()

COSI:


    DISPATCH()

TANI:


    DISPATCH()

COTI:


    DISPATCH()

NEGI:


    DISPATCH()

BEQ:


    DISPATCH()

BNE:


    DISPATCH()

BLT:


    DISPATCH()

BGE:


    DISPATCH()

LB:


    DISPATCH()

LH:


    DISPATCH()

LW:


    DISPATCH()

LD:


    DISPATCH()

SB:


    DISPATCH()

SH:


    DISPATCH()

SW:


    DISPATCH()

SD:


    DISPATCH()

I2F:

    
    DISPATCH()

I2D:

    
    DISPATCH()

I2L:

    
    DISPATCH()

F2I:

    
    DISPATCH()

F2D:

    
    DISPATCH()

F2L:

    
    DISPATCH()

D2I:

    
    DISPATCH()

D2F:

    
    DISPATCH()

D2L:

    
    DISPATCH()

L2I:

    
    DISPATCH()

L2F:

    
    DISPATCH()

L2D:

    
    DISPATCH()

RET:

    return true;
    DISPATCH()

INITRIN_INPUT:
    

    DISPATCH()

INTRIN_OUTPUT:


    DISPATCH()

}

}   // VM
