#include <iostream>
#include <cstring>
#include <exception>

#include <cmath>

#include "VirtualMachine.h"
#include "Interpreter.h"


namespace VM {

bool Interpreter::interpret(const uint64_t entry) {
    auto& pc = m_vm->m_pc;
    uint8_t* memory = m_vm->m_memory;
    auto& regfile = m_vm->m_regfile;

    EncodedInstruction* currInstr;
    DecodedInstruction decInstr;

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
        goto *dispatchTable[m_decoder.getOpcode(*currInstr)];               \
    }

    try {

        // Actual main interpretation
        DISPATCH()


    INSTRUCTION_INVALID:
        return false;



    ADD:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd] = regfile[decInstr.rs1] + regfile[decInstr.rs2];

        DISPATCH()

    SUB:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd] = regfile[decInstr.rs1] - regfile[decInstr.rs2];

        DISPATCH()

    AND:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd] = regfile[decInstr.rs1] & regfile[decInstr.rs2];

        DISPATCH()

    OR:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd] = regfile[decInstr.rs1] | regfile[decInstr.rs2];

        DISPATCH()

    XOR:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd] = regfile[decInstr.rs1] ^ regfile[decInstr.rs2];

        DISPATCH()

    SL:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd] = regfile[decInstr.rs1] << regfile[decInstr.rs2];

        DISPATCH()

    SR:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd] = regfile[decInstr.rs1] >> regfile[decInstr.rs2];

        DISPATCH()

    SQRT:
        /*
        m_decoder.decodeInstruction(*currInstr, decInstr);
        float tmp = std::bit_cast<float>(regfile[decInstr.rs1]);
        regfile[decInstr.rd] = std::bit_cast<RegValue>(std::sqrt(tmp));
        */

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
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd] = static_cast<RegValue>(-static_cast<SignedRegValue>(regfile[decInstr.rs1]));

        DISPATCH()

    MV:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd] = regfile[decInstr.rs1];

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
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd] = decInstr.imm;

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
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs1] + decInstr.imm >= sizeof(m_vm->m_memory) - sizeof(DWord))
            throw std::runtime_error("invalid address");

        memcpy(&regfile[decInstr.rd], memory + regfile[decInstr.rs1] + decInstr.imm, sizeof(DWord));

        DISPATCH()

    SB:


        DISPATCH()

    SH:


        DISPATCH()

    SW:


        DISPATCH()

    SD:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs1] + decInstr.imm >= sizeof(m_vm->m_memory) - sizeof(DWord))
            throw std::runtime_error("invalid address");

        memcpy(memory + regfile[decInstr.rs1] + decInstr.imm, &regfile[decInstr.rs2], sizeof(DWord));

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

        m_decoder.decodeInstruction(*currInstr, decInstr);
        switch(decInstr.intrinType) {

            case IntrinsicType::INTRINSIC_ISCAN: {

                RegValue storeAddress = regfile[decInstr.rs1];
                if (storeAddress >= sizeof(m_vm->m_memory) - sizeof(int))
                    throw std::runtime_error("invalid address");

                int tmp;
                std::cin >> tmp;
                if (std::cin.fail())
                    throw std::runtime_error("invalid input");

                memcpy(m_vm->m_memory + storeAddress, &tmp, sizeof(int));

                break;
            }
            case IntrinsicType::INTRINSIC_IPRINT: {

                RegValue loadAddress = regfile[decInstr.rs1];
                if (loadAddress >= sizeof(m_vm->m_memory) - sizeof(int))
                    throw std::runtime_error("invalid address");

                int tmp;
                memcpy(&tmp, m_vm->m_memory + loadAddress, sizeof(int));
                std::cout << tmp << std::endl;

                break;
            }
            case IntrinsicType::INTRINSIC_FSCAN: {

                RegValue storeAddress = regfile[decInstr.rs1];
                if (storeAddress >= sizeof(m_vm->m_memory) - sizeof(float))
                    throw std::runtime_error("invalid address");

                float tmp;
                std::cin >> tmp;
                if (std::cin.fail())
                    throw std::runtime_error("invalid input");

                memcpy(m_vm->m_memory + storeAddress, &tmp, sizeof(float));

                break;
            }
            case IntrinsicType::INTRINSIC_FPRINT: {

                RegValue loadAddress = regfile[decInstr.rs1];
                if (loadAddress >= sizeof(m_vm->m_memory) - sizeof(float))
                    throw std::runtime_error("invalid address");

                float tmp;
                memcpy(&tmp, m_vm->m_memory + loadAddress, sizeof(float));
                std::cout << tmp << std::endl;

                break;
            }
            default:
                break;

        }

        DISPATCH()
    }
    catch (const std::runtime_error& error) {
        std::cerr << "Error occured during interpreting: " << error.what() << std::endl;
        return false;
    }
}

}   // VM
