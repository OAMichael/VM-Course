#include <iostream>
#include <cstring>
#include <exception>

#include "VirtualMachine.h"
#include "Interpreter.h"


namespace VM {

static inline uint8_t getOpcode(const EncodedInstruction& instr) {
    return instr & 0xFF;    // Last 8 bits
}

static inline uint8_t getIntrinsicType(const EncodedInstruction& instr) {
    uint32_t tmp = (instr & 0xFF00) >> 8;
    return tmp;
}

static inline uint32_t getPartialBits(const uint32_t val,
                                      const uint32_t first,
                                      const uint32_t second) {
    const uint32_t mask = ((1 << (first - second + 1)) - 1) << second;
    return (val & mask);
}


bool Interpreter::interpret(const uint64_t entry) {
    auto& pc = m_vm->m_pc;
    uint8_t* memory = m_vm->m_memory;

    EncodedInstruction* currInstr;

    static void* dispatchTable[InstructionType::INSTRUCTION_COUNT] = {
        &&INSTRUCTION_INVALID,

        &&ADD,  &&SUB,  &&AND,  &&OR,  &&XOR,  &&SL,  &&SR,  &&SQRT,  &&SIN,  &&COS,  &&TAN,  &&COT,  &&NEG,  &&MV,

        &&ADDI, &&SUBI, &&ANDI, &&ORI, &&XORI, &&SLI, &&SRI, &&SQRTI, &&SINI, &&COSI, &&TANI, &&COTI, &&NEGI, &&MVI,

        &&BEQ, &&BNE, &&BLT, &&BGE,

        &&LB, &&LH, &&LW, &&LD,

        &&SB, &&SH, &&SW, &&SD,

        &&I2F, &&I2D, &&I2L, &&F2I, &&F2D, &&F2L, &&D2I, &&D2F, &&D2L, &&L2I, &&L2F, &&L2D,

        &&RET,

        &&INITRINSIC,
    };

    #define DISPATCH()                                                      \
    {                                                                       \
        currInstr = (EncodedInstruction*)(memory + pc);                     \
        pc += 4;                                                            \
        goto *dispatchTable[getOpcode(*currInstr)];                         \
    }

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

MV:


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

MVI:
    RegisterType rd = static_cast<RegisterType>(getPartialBits(*currInstr, 12, 8) >> 8);
    uint32_t imm = getPartialBits(*currInstr, 31, 13) >> 13;

    m_vm->m_regfile[rd] = imm;

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

INITRINSIC:
    
    IntrinsicType intrType = static_cast<IntrinsicType>(getIntrinsicType(*currInstr));

    switch(intrType) {

        case IntrinsicType::INTRINSIC_ISCAN: {

            int tmp;
            std::cin >> tmp;

            RegisterType rs1 = static_cast<RegisterType>(getPartialBits(*currInstr, 20, 16) >> 16);

            uint64_t storeAddress = m_vm->m_regfile[rs1];
            if (storeAddress < 0 || storeAddress >= sizeof(m_vm->m_memory) - sizeof(int))
                throw std::runtime_error("invalid address");

            memcpy(m_vm->m_memory + storeAddress, &tmp, sizeof(int));

            break;
        }
        case IntrinsicType::INTRINSIC_IPRINT: {

            int tmp;
            RegisterType rs1 = static_cast<RegisterType>(getPartialBits(*currInstr, 20, 16) >> 16);

            uint64_t loadAddress = m_vm->m_regfile[rs1];
            if (loadAddress < 0 || loadAddress >= sizeof(m_vm->m_memory) - sizeof(int))
                throw std::runtime_error("invalid address");

            memcpy(&tmp, m_vm->m_memory + loadAddress, sizeof(int));

            std::cout << tmp << std::endl;

            break;
        }
        case IntrinsicType::INTRINSIC_FSCAN: {

            float tmp;
            std::cin >> tmp;

            RegisterType rs1 = static_cast<RegisterType>(getPartialBits(*currInstr, 20, 16) >> 16);

            uint64_t storeAddress = m_vm->m_regfile[rs1];
            if (storeAddress < 0 || storeAddress >= sizeof(m_vm->m_memory) - sizeof(float))
                throw std::runtime_error("invalid address");

            memcpy(m_vm->m_memory + storeAddress, &tmp, sizeof(float));

            break;
        }
        case IntrinsicType::INTRINSIC_FPRINT: {

            float tmp;
            RegisterType rs1 = static_cast<RegisterType>(getPartialBits(*currInstr, 20, 16) >> 16);

            uint64_t loadAddress = m_vm->m_regfile[rs1];
            if (loadAddress < 0 || loadAddress >= sizeof(m_vm->m_memory) - sizeof(float))
                throw std::runtime_error("invalid address");

            memcpy(&tmp, m_vm->m_memory + loadAddress, sizeof(float));

            std::cout << tmp << std::endl;

            break;
        }
        default:
            break;

    }

    DISPATCH()

}

}   // VM
