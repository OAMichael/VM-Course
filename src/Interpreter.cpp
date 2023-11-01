#include <iostream>
#include <cstring>
#include <exception>

#include <cmath>

#include "VirtualMachine.h"
#include "Interpreter.h"


namespace VM {


inline Immediate Interpreter::loadConstant(const ImmediateIndex idx) {
    Immediate* constantPool = (Immediate*)(m_vm->m_memory + VM_CONSTANT_POOL_MEMORY_ADDRESS);
    return constantPool[idx];
}


bool Interpreter::interpret(const uint64_t entry) {

    // Initialize root frame
    m_currFrame = new Frame();
    m_currFrame->pc = entry;
    m_currFrame->parent = nullptr;

    static uint8_t*  memory = m_vm->m_memory;
    static Register* regfile = m_currFrame->regfile;
    static Register& accumulator = m_vm->m_accumulator;
    static uint64_t& arenaPointer = m_vm->m_arenaPointer;

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

        &&JMP, &&BEQ, &&BNE, &&BGE, &&BLT, &&BGEF, &&BLTF, &&CALL, &&RET,

        &&NEW, &&NEWARRAY
    };


    #define DISPATCH()                                                      \
    {                                                                       \
        currInstr = (EncodedInstruction*)(memory + m_currFrame->pc);        \
        m_currFrame->pc += INSTRUCTION_BYTESIZE;                            \
        m_decoder.decodeInstruction(*currInstr, decInstr);                  \
        goto *dispatchTable[decInstr.opcode];                               \
    }

    try {

        // Actual main interpretation
        DISPATCH()


    INSTRUCTION_INVALID:
        throw std::runtime_error("got invalid instruction");


    LOAD_ACC:

        accumulator = regfile[decInstr.r1];
        DISPATCH()

    STORE_ACC:

        regfile[decInstr.r1] = accumulator;
        DISPATCH()

    LOAD_ACCI:

        imm = loadConstant(decInstr.imm);
        memcpy(&accumulator, &imm.i_val, sizeof(accumulator));

        DISPATCH()

    LOAD_ACC_MEM:

        imm = loadConstant(decInstr.imm);
        memcpy(&accumulator, memory + decInstr.r1 + imm.i_val, sizeof(accumulator));

        DISPATCH()

    STORE_ACC_MEM:

        imm = loadConstant(decInstr.imm);
        memcpy(memory + decInstr.r1 + imm.i_val, &accumulator, sizeof(accumulator));

        DISPATCH()

    TO_FLOAT_REG:

        accumulator.f_val = static_cast<double>(regfile[decInstr.r1].i_val);
        DISPATCH()

    TO_INT_REG:

        accumulator.i_val = static_cast<int64_t>(regfile[decInstr.r1].f_val);
        DISPATCH()

    TO_FLOAT:

        accumulator.f_val = static_cast<double>(accumulator.i_val);
        DISPATCH()

    TO_INT:

        accumulator.i_val = static_cast<int64_t>(accumulator.f_val);
        DISPATCH()

    ADD:

        accumulator.i_val += regfile[decInstr.r1].i_val;
        DISPATCH()

    SUB:

        accumulator.i_val -= regfile[decInstr.r1].i_val;
        DISPATCH()

    MUL:

        accumulator.i_val *= regfile[decInstr.r1].i_val;
        DISPATCH()

    DIV:
        if (regfile[decInstr.r1].i_val == 0) {
            throw std::runtime_error("division by zero");
        }
        accumulator.i_val /= regfile[decInstr.r1].i_val;
        DISPATCH()

    AND:

        accumulator.i_val &= regfile[decInstr.r1].i_val;
        DISPATCH()

    OR:

        accumulator.i_val |= regfile[decInstr.r1].i_val;
        DISPATCH()

    XOR:

        accumulator.i_val ^= regfile[decInstr.r1].i_val;
        DISPATCH()

    SL:

        accumulator.i_val = accumulator.i_val << regfile[decInstr.r1].i_val;
        DISPATCH()

    SR:

        accumulator.i_val = accumulator.i_val >> regfile[decInstr.r1].i_val;
        DISPATCH()

    NEG:

        accumulator.i_val = -accumulator.i_val;
        DISPATCH()

    ADDF:

        accumulator.f_val += regfile[decInstr.r1].f_val;
        DISPATCH()

    SUBF:

        accumulator.f_val -= regfile[decInstr.r1].f_val;
        DISPATCH()

    MULF:

        accumulator.f_val *= regfile[decInstr.r1].f_val;
        DISPATCH()

    DIVF:
        if (regfile[decInstr.r1].f_val == 0) {
            throw std::runtime_error("division by zero");
        }
        accumulator.f_val /= regfile[decInstr.r1].f_val;
        DISPATCH()

    NEGF:

        accumulator.f_val = -accumulator.f_val;
        DISPATCH()

    ADDI:
        imm = loadConstant(decInstr.imm);
        switch (imm.type) {
            case BasicObjectType::INTEGER: {
                accumulator.i_val += imm.i_val;
                break;
            }
            case BasicObjectType::FLOATING: {
                accumulator.f_val += imm.f_val;
                break;
            }
            default: {
                throw std::runtime_error("unknown basic object type");
            }
        }

        DISPATCH()

    SUBI:

        imm = loadConstant(decInstr.imm);
        switch (imm.type) {
            case BasicObjectType::INTEGER: {
                accumulator.i_val -= imm.i_val;
                break;
            }
            case BasicObjectType::FLOATING: {
                accumulator.f_val -= imm.f_val;
                break;
            }
            default: {
                throw std::runtime_error("unknown basic object type");
            }
        }
        DISPATCH()

    MULI:

        imm = loadConstant(decInstr.imm);
        switch (imm.type) {
            case BasicObjectType::INTEGER: {
                accumulator.i_val *= imm.i_val;
                break;
            }
            case BasicObjectType::FLOATING: {
                accumulator.f_val *= imm.f_val;
                break;
            }
            default: {
                throw std::runtime_error("unknown basic object type");
            }
        }
        DISPATCH()

    DIVI:

        imm = loadConstant(decInstr.imm);
        switch (imm.type) {
            case BasicObjectType::INTEGER: {
                if (regfile[decInstr.r1].i_val == 0) {
                    throw std::runtime_error("division by zero");
                }
                accumulator.i_val /= imm.i_val;
                break;
            }
            case BasicObjectType::FLOATING: {
                if (regfile[decInstr.r1].f_val == 0) {
                    throw std::runtime_error("division by zero");
                }
                accumulator.f_val /= imm.f_val;
                break;
            }
            default: {
                throw std::runtime_error("unknown basic object type");
            }
        }
        DISPATCH()

    ANDI:

        imm = loadConstant(decInstr.imm);
        accumulator.i_val &= imm.i_val;
        DISPATCH()

    ORI:

        imm = loadConstant(decInstr.imm);
        accumulator.i_val |= imm.i_val;
        DISPATCH()

    XORI:

        imm = loadConstant(decInstr.imm);
        accumulator.i_val ^= imm.i_val;
        DISPATCH()

    SLI:

        imm = loadConstant(decInstr.imm);
        accumulator.i_val = accumulator.i_val << imm.i_val;
        DISPATCH()

    SRI:

        imm = loadConstant(decInstr.imm);
        accumulator.i_val = accumulator.i_val >> imm.i_val;
        DISPATCH()

    SIN:

        accumulator.f_val = std::sin(accumulator.f_val);
        DISPATCH()

    COS:

        accumulator.f_val = std::cos(accumulator.f_val);
        DISPATCH()

    SQRT:

        if (accumulator.f_val < 0) {
            throw std::runtime_error("sqrt of negative number");
        } 
        accumulator.f_val = std::sqrt(accumulator.f_val);
        DISPATCH()

    MV:

        memcpy(&regfile[decInstr.r1].i_val, &regfile[decInstr.r2].i_val, sizeof(regfile[decInstr.r1].i_val));
        DISPATCH()

    MVI:

        imm = loadConstant(decInstr.imm);
        memcpy(&regfile[decInstr.r1].i_val, &imm.i_val, sizeof(regfile[decInstr.r1].i_val));
        DISPATCH()

    CALL_INTINSIC:

        switch(decInstr.intrCode) {

            case IntrinsicType::SCAN: {
                std::cin >> accumulator.i_val;
                if (std::cin.fail()) {
                    throw std::runtime_error("invalid input");
                }
                break;
            }
            case IntrinsicType::PRINT: {
                std::cout << accumulator.i_val << std::endl;
                break;
            }
            case IntrinsicType::SCANF: {
                std::cin >> accumulator.f_val;
                if (std::cin.fail()) {
                    throw std::runtime_error("invalid input");
                }
                break;
            }
            case IntrinsicType::PRINTF: {
                std::cout << accumulator.f_val << std::endl;
                break;
            }
            case IntrinsicType::SCANS: {
                uint32_t strSize = static_cast<uint32_t>(accumulator.i_val);

                std::string str;
                str.resize(strSize);
                std::cin >> str;

                accumulator.i_val = m_vm->m_stringPoolPointer - VM_STRING_POOL_MEMORY_ADDRESS;
                std::memcpy(memory + m_vm->m_stringPoolPointer, &strSize, sizeof(uint32_t));
                std::memcpy(memory + m_vm->m_stringPoolPointer + sizeof(uint32_t), str.data(), strSize);

                m_vm->m_stringPoolPointer += sizeof(uint32_t) + strSize;
                break;
            }
            case IntrinsicType::PRINTS: {
                uint32_t strSize = 0;
                std::memcpy(&strSize, memory + VM_STRING_POOL_MEMORY_ADDRESS + accumulator.i_val, sizeof(uint32_t));
                std::string str;
                str.resize(strSize);
                std::memcpy(str.data(), memory + VM_STRING_POOL_MEMORY_ADDRESS + accumulator.i_val + sizeof(uint32_t), strSize);

                std::cout << str.c_str();
                break;
            }
            default: {
                throw std::runtime_error("unknown intrinsic type");
            }

        }
        DISPATCH()

    JMP:

        imm = loadConstant(decInstr.imm);
        m_currFrame->pc += imm.i_val - INSTRUCTION_BYTESIZE;
        DISPATCH()

    BEQ:

        if (accumulator.i_val == regfile[decInstr.r1].i_val) {
            imm = loadConstant(decInstr.imm);
            m_currFrame->pc += imm.i_val - INSTRUCTION_BYTESIZE;
        }
        DISPATCH()

    BNE:

        if (accumulator.i_val != regfile[decInstr.r1].i_val) {
            imm = loadConstant(decInstr.imm);
            m_currFrame->pc += imm.i_val - INSTRUCTION_BYTESIZE;
        }
        DISPATCH()

    BGE:

        if (accumulator.i_val >= regfile[decInstr.r1].i_val) {
            imm = loadConstant(decInstr.imm);
            m_currFrame->pc += imm.i_val - INSTRUCTION_BYTESIZE;
        }
        DISPATCH()

    BLT:

        if (accumulator.i_val < regfile[decInstr.r1].i_val) {
            imm = loadConstant(decInstr.imm);
            m_currFrame->pc += imm.i_val - INSTRUCTION_BYTESIZE;
        }
        DISPATCH()

    BGEF:

        if (accumulator.f_val >= regfile[decInstr.r1].f_val) {
            imm = loadConstant(decInstr.imm);
            m_currFrame->pc += imm.i_val - INSTRUCTION_BYTESIZE;
        }
        DISPATCH()

    BLTF:

        if (accumulator.f_val < regfile[decInstr.r1].f_val) {
            imm = loadConstant(decInstr.imm);
            m_currFrame->pc += imm.i_val - INSTRUCTION_BYTESIZE;
        }
        DISPATCH()

    CALL:

        imm = loadConstant(decInstr.imm);

        size_t numArgs = decInstr.numArgs;
        int64_t offset = imm.i_val;

        // Create new frame
        Frame* newFrame = m_currFrame;
        m_currFrame = new Frame();
        m_currFrame->parent = newFrame;
        m_currFrame->pc = newFrame->pc - INSTRUCTION_BYTESIZE + offset;
        regfile = m_currFrame->regfile;
        memcpy(m_currFrame->regfile, m_currFrame->parent->regfile, numArgs * sizeof(Register));

        DISPATCH()

    RET:

        if (m_currFrame->parent == nullptr) {
            delete m_currFrame;
            return true;
        }

        Frame* parent = m_currFrame->parent;
        delete m_currFrame;
        m_currFrame = parent;
        regfile = m_currFrame->regfile;

        DISPATCH()

    NEW:

        accumulator.i_val = arenaPointer;
        switch (decInstr.objType) {
            case BasicObjectType::INTEGER: {
                arenaPointer += sizeof(uint64_t);
                break;
            }
            case BasicObjectType::FLOATING: {
                arenaPointer += sizeof(double);
                break;
            }
            default: {
                throw std::runtime_error("unknown basic object type");
            }
        }
        DISPATCH()

    NEWARRAY:

        auto allocSize = accumulator.i_val;
        accumulator.i_val = arenaPointer;
        switch (decInstr.objType) {
            case BasicObjectType::INTEGER: {
                arenaPointer += sizeof(uint64_t) * allocSize;
                break;
            }
            case BasicObjectType::FLOATING: {
                arenaPointer += sizeof(double) * allocSize;
                break;
            }
            default: {
                throw std::runtime_error("unknown basic object type");
            }
        }
        DISPATCH()

    }
    catch (const std::runtime_error& error) {
        std::cerr << "Error occured during interpreting: " << error.what() << std::endl;
        return false;
    }
}

}   // VM
