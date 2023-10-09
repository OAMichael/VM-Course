#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <cstdint>


namespace VM {

static constexpr const uint32_t INSTRUCTION_BYTESIZE = 8;

enum RegisterType : uint8_t {
    X0 = 0,
    X1,
    X2,
    X3,
    X4,
    X5,
    X6,
    X7,
    X8,
    X9,
    X10,
    X11,
    X12,
    X13,
    X14,
    X15,
    X16,
    X17,
    X18,
    X19,
    X20,
    X21,
    X22,
    X23,
    X24,
    X25,
    X26,
    X27,
    X28,
    X29,
    X30,
    X31,

    REGISTER_COUNT,

    ACCU = X11
};


enum InstructionType : uint8_t {

    INSTRUCTION_INVALID = 0,

// Arithmetic
    IADD,
    ISUB,
    IMUL,
    IDIV,

    FADD,
    FSUB,
    FMUL,
    FDIV,

    DADD,
    DSUB,
    DMUL,
    DDIV,

    AND,
    OR,
    XOR,
    SL,
    SR,
    
    FSQRT,
    FSIN,
    FCOS,
    FTAN,

    DSQRT,
    DSIN,
    DCOS,
    DTAN,
    
    INEG,
    IMV,

    FNEG,
    FMV,

    DNEG,
    DMV,

// Arithmetic immediate
    IADDI,
    ISUBI,
    IMULI,
    IDIVI,

    FADDI,
    FSUBI,
    FMULI,
    FDIVI,

    DADDI,
    DSUBI,
    DMULI,
    DDIVI,

    ANDI,
    ORI,
    XORI,
    SLI,
    SRI,

    FSQRTI,
    FSINI,
    FCOSI,
    FTANI,

    DSQRTI,
    DSINI,
    DCOSI,
    DTANI,
    
    INEGI,
    IMVI,

    FNEGI,
    FMVI,

    DNEGI,
    DMVI,

// Branching
    BEQ,
    BNE,

    IBLT,
    IBGE,

    FBLT,
    FBGE,

    DBLT,
    DBGE,

// Load
    LOADB,
    LOADH,
    LOADW,
    LOADD,

// Store
    STOREB,
    STOREH,
    STOREW,
    STORED,

// Casts
    I2F,
    I2D,
    F2I,
    F2D,
    D2I,
    D2F,

// Return
    RET,
    IRET,
    FRET,
    DRET,

// Jumps
    JMP,

// Intrinsics
    INTRINSIC,

// Miscellaneous
    INSTRUCTION_COUNT,
};


enum IntrinsicType : uint8_t {
    INTRINSIC_INVALID = 0,

    INTRINSIC_ISCAN,
    INTRINSIC_IPRINT,
    INTRINSIC_FSCAN,
    INTRINSIC_FPRINT,
    INTRINSIC_DSCAN,
    INTRINSIC_DPRINT,
    INTRINSIC_LSCAN,
    INTRINSIC_LPRINT,

    INTRINSIC_COUNT
};

using Byte  = uint8_t;
using Half  = uint16_t;
using Word  = uint32_t;
using DWord = uint64_t;

using UnsignedRegValue = uint64_t;
using SignedRegValue   = int64_t;

union Register {
    uint64_t u_val;
    int64_t  s_val;
    double   d_val;
    float    f_val;
};

using EncodedInstruction = uint64_t;

struct DecodedInstruction {
    RegisterType rd, rs1, rs2;

    InstructionType opcode = InstructionType::INSTRUCTION_INVALID;

    union {
        uint32_t imm = 0;
        uint32_t shamt;

        float fpimm;

        IntrinsicType intrinType;
    };
};


}   // VM

#endif  // INSTRUCTIONS_H
