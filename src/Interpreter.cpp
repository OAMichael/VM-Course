#include <iostream>
#include <cstring>
#include <exception>

#include <cmath>

#include "VirtualMachine.h"
#include "Interpreter.h"


namespace VM {


inline Immediate Interpreter::loadConstant(const ImmediateIndex idx) {
    Immediate* constantPool = (Immediate*)(m_vm->m_memory + VM_CONSTANT_POOL_ADDRESS);
    return constantPool[idx];
}


bool Interpreter::interpret(const uint64_t entry) {

    auto& pc = m_vm->m_pc;
    uint8_t* memory = m_vm->m_memory;
    auto& regfile = m_vm->m_regfile;

    EncodedInstruction* currInstr;
    DecodedInstruction decInstr;
    Immediate imm;

    static void* dispatchTable[InstructionType::INSTRUCTION_COUNT] = {
        &&INSTRUCTION_INVALID,

        &&LOAD_ACC, &&STORE_ACC, &&LOAD_ACCI, &&LOAD_ACC_MEM, &&STORE_ACC_MEM,

        &&TO_FLOAT_REG, &&TO_INT_REG, &&TO_FLOAT, &&TO_INT,

        &&ADD, &&SUB, &&MUL, &&DIV, &&AND, &&OR, &&XOR, &&SL, &&SR, &&NEG,

        &&ADDF, &&SUBF, &&MULF, &&DIVF, &&NEGF,

        &&ADDI, &&SUBI, &&MULI, &&DIVI, &&ANDI, &&ORI, &&XORI, &&SLI, &&SRI,

        &&SIN, &&COS, &&SQRT, &&MV, &&MVI, &&CALL_INTINSIC,

        &&JMP, &&BEQ, &&BNE, &&BGE, &&BLT, &&BGEF, &&BLTF, &&CALL, &&RET
    };


    #define DISPATCH()                                                      \
    {                                                                       \
        currInstr = (EncodedInstruction*)(memory + pc);                     \
        pc += INSTRUCTION_BYTESIZE;                                         \
        m_decoder.decodeInstruction(*currInstr, decInstr);                  \
        goto *dispatchTable[decInstr.opcode];                               \
    }

    try {

        // Actual main interpretation
        DISPATCH()


    INSTRUCTION_INVALID:
        throw std::runtime_error("got invalid instruction");


    LOAD_ACC:

        regfile[RegisterType::ACC] = regfile[decInstr.r1];
        DISPATCH()

    STORE_ACC:

        regfile[decInstr.r1] = regfile[RegisterType::ACC];
        DISPATCH()

    LOAD_ACCI:

        imm = loadConstant(decInstr.immIdx);
        memcpy(&regfile[RegisterType::ACC], &imm.i_val, sizeof(regfile[RegisterType::ACC]));

        DISPATCH()

    LOAD_ACC_MEM:

        imm = loadConstant(decInstr.immIdx);
        memcpy(&regfile[RegisterType::ACC], memory + decInstr.r1 + imm.i_val, sizeof(regfile[RegisterType::ACC]));

        DISPATCH()

    STORE_ACC_MEM:

        imm = loadConstant(decInstr.immIdx);
        memcpy(memory + decInstr.r1 + imm.i_val, &regfile[RegisterType::ACC], sizeof(regfile[RegisterType::ACC]));

        DISPATCH()

    TO_FLOAT_REG:

        regfile[RegisterType::ACC].f_val = static_cast<double>(regfile[decInstr.r1].i_val);
        DISPATCH()

    TO_INT_REG:

        regfile[RegisterType::ACC].i_val = static_cast<int64_t>(regfile[decInstr.r1].f_val);
        DISPATCH()

    TO_FLOAT:

        regfile[RegisterType::ACC].f_val = static_cast<double>(regfile[RegisterType::ACC].i_val);
        DISPATCH()

    TO_INT:

        regfile[RegisterType::ACC].i_val = static_cast<int64_t>(regfile[RegisterType::ACC].f_val);
        DISPATCH()

    ADD:

        regfile[RegisterType::ACC].i_val += regfile[decInstr.r1].i_val;
        DISPATCH()

    SUB:

        regfile[RegisterType::ACC].i_val -= regfile[decInstr.r1].i_val;
        DISPATCH()

    MUL:

        regfile[RegisterType::ACC].i_val *= regfile[decInstr.r1].i_val;
        DISPATCH()

    DIV:
        if (regfile[decInstr.r1].i_val == 0) {
            throw std::runtime_error("division by zero");
        }
        regfile[RegisterType::ACC].i_val /= regfile[decInstr.r1].i_val;
        DISPATCH()

    AND:

        regfile[RegisterType::ACC].i_val &= regfile[decInstr.r1].i_val;
        DISPATCH()

    OR:

        regfile[RegisterType::ACC].i_val |= regfile[decInstr.r1].i_val;
        DISPATCH()

    XOR:

        regfile[RegisterType::ACC].i_val ^= regfile[decInstr.r1].i_val;
        DISPATCH()

    SL:

        regfile[RegisterType::ACC].i_val << regfile[decInstr.r1].i_val;
        DISPATCH()

    SR:

        regfile[RegisterType::ACC].i_val >> regfile[decInstr.r1].i_val;
        DISPATCH()

    NEG:

        regfile[RegisterType::ACC].i_val = -regfile[RegisterType::ACC].i_val;
        DISPATCH()

    ADDF:

        regfile[RegisterType::ACC].f_val += regfile[decInstr.r1].f_val;
        DISPATCH()

    SUBF:

        regfile[RegisterType::ACC].f_val -= regfile[decInstr.r1].f_val;
        DISPATCH()

    MULF:

        regfile[RegisterType::ACC].f_val *= regfile[decInstr.r1].f_val;
        DISPATCH()

    DIVF:
        if (regfile[decInstr.r1].f_val == 0) {
            throw std::runtime_error("division by zero");
        }
        regfile[RegisterType::ACC].f_val /= regfile[decInstr.r1].f_val;
        DISPATCH()

    NEGF:

        regfile[RegisterType::ACC].f_val = -regfile[RegisterType::ACC].f_val;
        DISPATCH()

    ADDI:
        imm = loadConstant(decInstr.immIdx);
        switch (imm.type) {
            case ImmType::INTEGER: {
                regfile[RegisterType::ACC].i_val += imm.i_val;
            }
            case ImmType::FLOATING: {
                regfile[RegisterType::ACC].f_val += imm.f_val;
            }
            default:;
        }

        DISPATCH()

    SUBI:

        imm = loadConstant(decInstr.immIdx);
        switch (imm.type) {
            case ImmType::INTEGER: {
                regfile[RegisterType::ACC].i_val -= imm.i_val;
            }
            case ImmType::FLOATING: {
                regfile[RegisterType::ACC].f_val -= imm.f_val;
            }
            default:;
        }
        DISPATCH()

    MULI:

        imm = loadConstant(decInstr.immIdx);
        switch (imm.type) {
            case ImmType::INTEGER: {
                regfile[RegisterType::ACC].i_val *= imm.i_val;
            }
            case ImmType::FLOATING: {
                regfile[RegisterType::ACC].f_val *= imm.f_val;
            }
            default:;
        }
        DISPATCH()

    DIVI:

        imm = loadConstant(decInstr.immIdx);
        switch (imm.type) {
            case ImmType::INTEGER: {
                if (regfile[decInstr.r1].i_val == 0) {
                    throw std::runtime_error("division by zero");
                }
                regfile[RegisterType::ACC].i_val /= imm.i_val;
            }
            case ImmType::FLOATING: {
                if (regfile[decInstr.r1].f_val == 0) {
                    throw std::runtime_error("division by zero");
                }
                regfile[RegisterType::ACC].f_val /= imm.f_val;
            }
            default:;
        }
        DISPATCH()

    ANDI:

        imm = loadConstant(decInstr.immIdx);
        regfile[RegisterType::ACC].i_val &= imm.i_val;
        DISPATCH()

    ORI:

        imm = loadConstant(decInstr.immIdx);
        regfile[RegisterType::ACC].i_val |= imm.i_val;
        DISPATCH()

    XORI:

        imm = loadConstant(decInstr.immIdx);
        regfile[RegisterType::ACC].i_val ^= imm.i_val;
        DISPATCH()

    SLI:

        imm = loadConstant(decInstr.immIdx);
        regfile[RegisterType::ACC].i_val << imm.i_val;
        DISPATCH()

    SRI:

        imm = loadConstant(decInstr.immIdx);
        regfile[RegisterType::ACC].i_val >> imm.i_val;
        DISPATCH()

    SIN:

        regfile[RegisterType::ACC].f_val = std::sin(regfile[RegisterType::ACC].f_val);
        DISPATCH()

    COS:

        regfile[RegisterType::ACC].f_val = std::cos(regfile[RegisterType::ACC].f_val);
        DISPATCH()

    SQRT:

        if (regfile[RegisterType::ACC].f_val < 0) {
            throw std::runtime_error("sqrt of negative number");
        } 
        regfile[RegisterType::ACC].f_val = std::sqrt(regfile[RegisterType::ACC].f_val);
        DISPATCH()

    MV:

        memcpy(&regfile[decInstr.r1].i_val, &regfile[decInstr.r2].i_val, sizeof(regfile[decInstr.r1].i_val));
        DISPATCH()

    MVI:

        imm = loadConstant(decInstr.immIdx);
        memcpy(&regfile[decInstr.r1].i_val, &imm.i_val, sizeof(regfile[decInstr.r1].i_val));
        DISPATCH()

    CALL_INTINSIC:

        switch(decInstr.intrinType) {

            case IntrinsicType::INTRINSIC_SCAN: {
                std::cin >> regfile[RegisterType::ACC].i_val;
                if (std::cin.fail()) {
                    throw std::runtime_error("invalid input");
                }
                break;
            }
            case IntrinsicType::INTRINSIC_PRINT: {
                std::cout << regfile[RegisterType::ACC].i_val << std::endl;
                break;
            }
            case IntrinsicType::INTRINSIC_SCANF: {
                std::cin >> regfile[RegisterType::ACC].f_val;
                if (std::cin.fail()) {
                    throw std::runtime_error("invalid input");
                }
                break;
            }
            case IntrinsicType::INTRINSIC_PRINTF: {
                std::cout << regfile[RegisterType::ACC].f_val << std::endl;
                break;
            }
            default:
                break;

        }
        DISPATCH()

    JMP:

        imm = loadConstant(decInstr.immIdx);
        pc += imm.i_val - INSTRUCTION_BYTESIZE;
        DISPATCH()

    BEQ:

        if (regfile[RegisterType::ACC].i_val == regfile[decInstr.r1].i_val) {
            imm = loadConstant(decInstr.immIdx);
            pc += imm.i_val - INSTRUCTION_BYTESIZE;
        }
        DISPATCH()

    BNE:

        if (regfile[RegisterType::ACC].i_val != regfile[decInstr.r1].i_val) {
            imm = loadConstant(decInstr.immIdx);
            pc += imm.i_val - INSTRUCTION_BYTESIZE;
        }
        DISPATCH()

    BGE:

        if (regfile[RegisterType::ACC].i_val >= regfile[decInstr.r1].i_val) {
            imm = loadConstant(decInstr.immIdx);
            pc += imm.i_val - INSTRUCTION_BYTESIZE;
        }
        DISPATCH()

    BLT:

        if (regfile[RegisterType::ACC].i_val < regfile[decInstr.r1].i_val) {
            imm = loadConstant(decInstr.immIdx);
            pc += imm.i_val - INSTRUCTION_BYTESIZE;
        }
        DISPATCH()

    BGEF:

        if (regfile[RegisterType::ACC].f_val >= regfile[decInstr.r1].f_val) {
            imm = loadConstant(decInstr.immIdx);
            pc += imm.i_val - INSTRUCTION_BYTESIZE;
        }
        DISPATCH()

    BLTF:

        if (regfile[RegisterType::ACC].f_val < regfile[decInstr.r1].f_val) {
            imm = loadConstant(decInstr.immIdx);
            pc += imm.i_val - INSTRUCTION_BYTESIZE;
        }
        DISPATCH()

    CALL:


        DISPATCH()

    RET:

        return true;
        DISPATCH()

    }
    catch (const std::runtime_error& error) {
        std::cerr << "Error occured during interpreting: " << error.what() << std::endl;
        return false;
    }
}

}   // VM
