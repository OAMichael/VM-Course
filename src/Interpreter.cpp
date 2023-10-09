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

        &&IADD, &&ISUB, &&IMUL, &&IDIV, &&FADD, &&FSUB, &&FMUL, &&FDIV,
        &&DADD, &&DSUB, &&DMUL, &&DDIV, &&AND, &&OR, &&XOR, &&SL, &&SR,
        &&FSQRT, &&FSIN, &&FCOS, &&FTAN, &&DSQRT, &&DSIN, &&DCOS, &&DTAN,
        &&INEG, &&IMV, &&FNEG, &&FMV, &&DNEG, &&DMV,

        &&IADDI, &&ISUBI, &&IMULI, &&IDIVI, &&FADDI, &&FSUBI, &&FMULI, &&FDIVI,
        &&DADDI, &&DSUBI, &&DMULI, &&DDIVI, &&ANDI, &&ORI, &&XORI, &&SLI, &&SRI,
        &&FSQRTI, &&FSINI, &&FCOSI, &&FTANI, &&DSQRTI, &&DSINI, &&DCOSI, &&DTANI,
        &&INEGI, &&IMVI, &&FNEGI, &&FMVI, &&DNEGI, &&DMVI,

        &&BEQ, &&BNE, &&IBLT, &&IBGE, &&FBLT, &&FBGE, &&DBLT, &&DBGE,

        &&LOADB, &&LOADH, &&LOADW, &&LOADD,

        &&STOREB, &&STOREH, &&STOREW, &&STORED,

        &&I2F, &&I2D, &&F2I, &&F2D, &&D2I, &&D2F,

        &&RET, &&IRET, &&FRET, &&DRET,

        &&JMP,

        &&INITRINSIC,
    };

    #define DISPATCH()                                                      \
    {                                                                       \
        currInstr = (EncodedInstruction*)(memory + pc);                     \
        pc += INSTRUCTION_BYTESIZE;                                         \
        goto *dispatchTable[m_decoder.getOpcode(*currInstr)];               \
    }

    try {

        // Actual main interpretation
        DISPATCH()


    INSTRUCTION_INVALID:
        throw std::runtime_error("got invalid instruction");


    IADD:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].u_val = regfile[decInstr.rs1].u_val + regfile[decInstr.rs2].u_val;

        DISPATCH()

    ISUB:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].u_val = regfile[decInstr.rs1].u_val - regfile[decInstr.rs2].u_val;

        DISPATCH()

    IMUL:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].u_val = regfile[decInstr.rs1].u_val * regfile[decInstr.rs2].u_val;

        DISPATCH()

    IDIV:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs2].u_val == 0) {
            throw std::runtime_error("division by zero");
        }
        regfile[decInstr.rd].u_val = regfile[decInstr.rs1].u_val / regfile[decInstr.rs2].u_val;

        DISPATCH()

    FADD:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].f_val = regfile[decInstr.rs1].f_val + regfile[decInstr.rs2].f_val;

        DISPATCH()

    FSUB:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].f_val = regfile[decInstr.rs1].f_val - regfile[decInstr.rs2].f_val;

        DISPATCH()

    FMUL:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].f_val = regfile[decInstr.rs1].f_val * regfile[decInstr.rs2].f_val;

        DISPATCH()

    FDIV:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs2].f_val == 0) {
            throw std::runtime_error("division by zero");
        }
        regfile[decInstr.rd].f_val = regfile[decInstr.rs1].f_val / regfile[decInstr.rs2].f_val;

        DISPATCH()

    DADD:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].d_val = regfile[decInstr.rs1].d_val + regfile[decInstr.rs2].d_val;

        DISPATCH()

    DSUB:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].d_val = regfile[decInstr.rs1].d_val - regfile[decInstr.rs2].d_val;

        DISPATCH()

    DMUL:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].d_val = regfile[decInstr.rs1].d_val * regfile[decInstr.rs2].d_val;

        DISPATCH()

    DDIV:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs2].d_val == 0) {
            throw std::runtime_error("division by zero");
        }
        regfile[decInstr.rd].d_val = regfile[decInstr.rs1].d_val / regfile[decInstr.rs2].d_val;

        DISPATCH()

    AND:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].u_val = regfile[decInstr.rs1].u_val & regfile[decInstr.rs2].u_val;

        DISPATCH()

    OR:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].u_val = regfile[decInstr.rs1].u_val | regfile[decInstr.rs2].u_val;

        DISPATCH()

    XOR:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].u_val = regfile[decInstr.rs1].u_val ^ regfile[decInstr.rs2].u_val;

        DISPATCH()

    SL:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].u_val = regfile[decInstr.rs1].u_val << regfile[decInstr.rs2].u_val;

        DISPATCH()

    SR:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].u_val = regfile[decInstr.rs1].u_val >> regfile[decInstr.rs2].u_val;

        DISPATCH()

    FSQRT:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs2].f_val < 0) {
            throw std::runtime_error("sqrt of negative number");
        }
        regfile[decInstr.rd].f_val = std::sqrt(regfile[decInstr.rs1].f_val);

        DISPATCH()

    FSIN:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].f_val = std::sin(regfile[decInstr.rs1].f_val);

        DISPATCH()

    FCOS:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].f_val = std::cos(regfile[decInstr.rs1].f_val);

        DISPATCH()

    FTAN:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].f_val = std::tan(regfile[decInstr.rs1].f_val);

        DISPATCH()

    DSQRT:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs2].d_val < 0) {
            throw std::runtime_error("sqrt of negative number");
        }
        regfile[decInstr.rd].d_val = std::sqrt(regfile[decInstr.rs1].d_val);

        DISPATCH()

    DSIN:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].d_val = std::sin(regfile[decInstr.rs1].d_val);

        DISPATCH()

    DCOS:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].d_val = std::cos(regfile[decInstr.rs1].d_val);

        DISPATCH()

    DTAN:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].d_val = std::tan(regfile[decInstr.rs1].d_val);

        DISPATCH()

    INEG:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].s_val = -regfile[decInstr.rs1].s_val;

        DISPATCH()

    IMV:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].s_val = regfile[decInstr.rs1].s_val;

        DISPATCH()

    FNEG:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].f_val = -regfile[decInstr.rs1].f_val;

        DISPATCH()

    FMV:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].f_val = regfile[decInstr.rs1].f_val;

        DISPATCH()

    DNEG:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].d_val = -regfile[decInstr.rs1].d_val;

        DISPATCH()

    DMV:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].d_val = regfile[decInstr.rs1].d_val;

        DISPATCH()


    IADDI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].u_val = regfile[decInstr.rs1].u_val + decInstr.imm;

        DISPATCH()

    ISUBI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].u_val = regfile[decInstr.rs1].u_val - decInstr.imm;

        DISPATCH()

    IMULI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].u_val = regfile[decInstr.rs1].u_val * decInstr.imm;

        DISPATCH()

    IDIVI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (decInstr.imm == 0) {
            throw std::runtime_error("division by zero");
        }
        regfile[decInstr.rd].u_val = regfile[decInstr.rs1].u_val / decInstr.imm;

        DISPATCH()

    FADDI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].f_val = regfile[decInstr.rs1].f_val + decInstr.fpimm;

        DISPATCH()

    FSUBI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].f_val = regfile[decInstr.rs1].f_val - decInstr.fpimm;

        DISPATCH()

    FMULI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].f_val = regfile[decInstr.rs1].f_val * decInstr.fpimm;

        DISPATCH()

    FDIVI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (decInstr.fpimm == 0) {
            throw std::runtime_error("division by zero");
        }
        regfile[decInstr.rd].f_val = regfile[decInstr.rs1].f_val / decInstr.fpimm;

        DISPATCH()

    DADDI:

        DISPATCH()

    DSUBI:

        DISPATCH()

    DMULI:

        DISPATCH()

    DDIVI:

        DISPATCH()

    ANDI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].u_val = regfile[decInstr.rs1].u_val & decInstr.imm;

        DISPATCH()

    ORI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].u_val = regfile[decInstr.rs1].u_val | decInstr.imm;

        DISPATCH()

    XORI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].u_val = regfile[decInstr.rs1].u_val ^ decInstr.imm;

        DISPATCH()

    SLI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].u_val = regfile[decInstr.rs1].u_val << decInstr.imm;

        DISPATCH()

    SRI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].u_val = regfile[decInstr.rs1].u_val >> decInstr.imm;

        DISPATCH()

    FSQRTI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (decInstr.fpimm < 0) {
            throw std::runtime_error("sqrt of negative number");
        }        
        regfile[decInstr.rd].f_val = std::sqrt(decInstr.fpimm);

        DISPATCH()

    FSINI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].f_val = std::sin(decInstr.fpimm);

        DISPATCH()

    FCOSI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].f_val = std::cos(decInstr.fpimm);

        DISPATCH()

    FTANI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].f_val = std::tan(decInstr.fpimm);

        DISPATCH()

    DSQRTI:

        DISPATCH()

    DSINI:

        DISPATCH()

    DCOSI:

        DISPATCH()

    DTANI:

        DISPATCH()

    INEGI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].s_val = -decInstr.imm;

        DISPATCH()

    IMVI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].s_val = decInstr.imm;

        DISPATCH()

    FNEGI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].f_val = -decInstr.fpimm;

        DISPATCH()

    FMVI:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        regfile[decInstr.rd].f_val = decInstr.fpimm;

        DISPATCH()

    DNEGI:

        DISPATCH()

    DMVI:

        DISPATCH()


    BEQ:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs1].s_val == regfile[decInstr.rs2].s_val)
            pc += decInstr.imm - INSTRUCTION_BYTESIZE;

        DISPATCH()

    BNE:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs1].s_val != regfile[decInstr.rs2].s_val)
            pc += decInstr.imm - INSTRUCTION_BYTESIZE;

        DISPATCH()

    IBLT:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs1].s_val < regfile[decInstr.rs2].s_val)
            pc += decInstr.imm - INSTRUCTION_BYTESIZE;

        DISPATCH()

    IBGE:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs1].s_val >= regfile[decInstr.rs2].s_val)
            pc += decInstr.imm - INSTRUCTION_BYTESIZE;

        DISPATCH()

    FBLT:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs1].f_val < regfile[decInstr.rs2].f_val)
            pc += decInstr.imm - INSTRUCTION_BYTESIZE;

        DISPATCH()

    FBGE:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs1].f_val >= regfile[decInstr.rs2].f_val)
            pc += decInstr.imm - INSTRUCTION_BYTESIZE;

        DISPATCH()


    DBLT:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs1].d_val < regfile[decInstr.rs2].d_val)
            pc += decInstr.imm - INSTRUCTION_BYTESIZE;

        DISPATCH()

    DBGE:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs1].d_val >= regfile[decInstr.rs2].d_val)
            pc += decInstr.imm - INSTRUCTION_BYTESIZE;

        DISPATCH()


    LOADB:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs1].u_val + decInstr.imm >= sizeof(m_vm->m_memory) - sizeof(Byte))
            throw std::runtime_error("invalid address");

        memcpy(&regfile[decInstr.rd].u_val, &memory[regfile[decInstr.rs1].u_val + decInstr.imm], sizeof(Byte));

        DISPATCH()

    LOADH:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs1].u_val + decInstr.imm >= sizeof(m_vm->m_memory) - sizeof(Half))
            throw std::runtime_error("invalid address");

        memcpy(&regfile[decInstr.rd].u_val, &memory[regfile[decInstr.rs1].u_val + decInstr.imm], sizeof(Half));

        DISPATCH()

    LOADW:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs1].u_val + decInstr.imm >= sizeof(m_vm->m_memory) - sizeof(Word))
            throw std::runtime_error("invalid address");

        memcpy(&regfile[decInstr.rd].u_val, &memory[regfile[decInstr.rs1].u_val + decInstr.imm], sizeof(Word));

        DISPATCH()

    LOADD:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs1].u_val + decInstr.imm >= sizeof(m_vm->m_memory) - sizeof(DWord))
            throw std::runtime_error("invalid address");

        memcpy(&regfile[decInstr.rd].u_val, &memory[regfile[decInstr.rs1].u_val + decInstr.imm], sizeof(DWord));

        DISPATCH()


    STOREB:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs1].u_val + decInstr.imm >= sizeof(m_vm->m_memory) - sizeof(Byte))
            throw std::runtime_error("invalid address");

        memcpy(&memory[regfile[decInstr.rs1].u_val + decInstr.imm], &regfile[decInstr.rs2].u_val, sizeof(Byte));

        DISPATCH()

    STOREH:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs1].u_val + decInstr.imm >= sizeof(m_vm->m_memory) - sizeof(Half))
            throw std::runtime_error("invalid address");

        memcpy(&memory[regfile[decInstr.rs1].u_val + decInstr.imm], &regfile[decInstr.rs2].u_val, sizeof(Half));

        DISPATCH()

    STOREW:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs1].u_val + decInstr.imm >= sizeof(m_vm->m_memory) - sizeof(Word))
            throw std::runtime_error("invalid address");

        memcpy(&memory[regfile[decInstr.rs1].u_val + decInstr.imm], &regfile[decInstr.rs2].u_val, sizeof(Word));

        DISPATCH()

    STORED:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        if (regfile[decInstr.rs1].u_val + decInstr.imm >= sizeof(m_vm->m_memory) - sizeof(DWord))
            throw std::runtime_error("invalid address");

        memcpy(&memory[regfile[decInstr.rs1].u_val + decInstr.imm], &regfile[decInstr.rs2].u_val, sizeof(DWord));

        DISPATCH()


    I2F:

        
        DISPATCH()

    I2D:

        
        DISPATCH()

    F2I:

        
        DISPATCH()

    F2D:

        
        DISPATCH()

    D2I:

        
        DISPATCH()

    D2F:

        
        DISPATCH()


    RET:

        return true;
        DISPATCH()

    IRET:

        return true;
        DISPATCH()

    FRET:

        return true;
        DISPATCH()

    DRET:

        return true;
        DISPATCH()

    JMP:
        m_decoder.decodeInstruction(*currInstr, decInstr);
        pc += decInstr.imm - INSTRUCTION_BYTESIZE;

        DISPATCH()

    INITRINSIC:

        m_decoder.decodeInstruction(*currInstr, decInstr);
        switch(decInstr.intrinType) {

            case IntrinsicType::INTRINSIC_ISCAN: {

                UnsignedRegValue storeAddress = regfile[decInstr.rs1].u_val;
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

                UnsignedRegValue loadAddress = regfile[decInstr.rs1].u_val;
                if (loadAddress >= sizeof(m_vm->m_memory) - sizeof(int))
                    throw std::runtime_error("invalid address");

                int tmp;
                memcpy(&tmp, m_vm->m_memory + loadAddress, sizeof(int));
                std::cout << tmp << std::endl;

                break;
            }
            case IntrinsicType::INTRINSIC_FSCAN: {

                UnsignedRegValue storeAddress = regfile[decInstr.rs1].u_val;
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

                UnsignedRegValue loadAddress = regfile[decInstr.rs1].u_val;
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
