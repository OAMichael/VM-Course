#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <cstdint>
#include <unordered_map>
#include <string>

namespace VM {

static constexpr const uint32_t INSTRUCTION_BYTESIZE = 8;

enum RegisterType : uint8_t {
    ACC = 0,
    X0,
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

    REGISTER_COUNT
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



static const std::unordered_map<std::string, InstructionType> instructionsNameOpcode = {

    { "INSTRUCTION_INVALID",    InstructionType::INSTRUCTION_INVALID },
    { "IADD",                   InstructionType::IADD                },
    { "ISUB",                   InstructionType::ISUB                },
    { "IMUL",                   InstructionType::IMUL                },
    { "IDIV",                   InstructionType::IDIV                },
    { "FADD",                   InstructionType::FADD                },
    { "FSUB",                   InstructionType::FSUB                },
    { "FMUL",                   InstructionType::FMUL                },
    { "FDIV",                   InstructionType::FDIV                },
    { "DADD",                   InstructionType::DADD                },
    { "DSUB",                   InstructionType::DSUB                },
    { "DMUL",                   InstructionType::DMUL                },
    { "DDIV",                   InstructionType::DDIV                },
    { "AND",                    InstructionType::AND                 },
    { "OR",                     InstructionType::OR                  },
    { "XOR",                    InstructionType::XOR                 },
    { "SL",                     InstructionType::SL                  },
    { "SR",                     InstructionType::SR                  },
    { "FSQRT",                  InstructionType::FSQRT               },
    { "FSIN",                   InstructionType::FSIN                },
    { "FCOS",                   InstructionType::FCOS                },
    { "FTAN",                   InstructionType::FTAN                },
    { "DSQRT",                  InstructionType::DSQRT               },
    { "DSIN",                   InstructionType::DSIN                },
    { "DCOS",                   InstructionType::DCOS                },
    { "DTAN",                   InstructionType::DTAN                },
    { "INEG",                   InstructionType::INEG                },
    { "IMV",                    InstructionType::IMV                 },
    { "FNEG",                   InstructionType::FNEG                },
    { "FMV",                    InstructionType::FMV                 },
    { "DNEG",                   InstructionType::DNEG                },
    { "DMV",                    InstructionType::DMV                 },
    { "IADDI",                  InstructionType::IADDI               },
    { "ISUBI",                  InstructionType::ISUBI               },
    { "IMULI",                  InstructionType::IMULI               },
    { "IDIVI",                  InstructionType::IDIVI               },
    { "FADDI",                  InstructionType::FADDI               },
    { "FSUBI",                  InstructionType::FSUBI               },
    { "FMULI",                  InstructionType::FMULI               },
    { "FDIVI",                  InstructionType::FDIVI               },
    { "DADDI",                  InstructionType::DADDI               },
    { "DSUBI",                  InstructionType::DSUBI               },
    { "DMULI",                  InstructionType::DMULI               },
    { "DDIVI",                  InstructionType::DDIVI               },
    { "ANDI",                   InstructionType::ANDI                },
    { "ORI",                    InstructionType::ORI                 },
    { "XORI",                   InstructionType::XORI                },
    { "SLI",                    InstructionType::SLI                 },
    { "SRI",                    InstructionType::SRI                 },
    { "FSQRTI",                 InstructionType::FSQRTI              },
    { "FSINI",                  InstructionType::FSINI               },
    { "FCOSI",                  InstructionType::FCOSI               },
    { "FTANI",                  InstructionType::FTANI               },
    { "DSQRTI",                 InstructionType::DSQRTI              },
    { "DSINI",                  InstructionType::DSINI               },
    { "DCOSI",                  InstructionType::DCOSI               },
    { "DTANI",                  InstructionType::DTANI               },
    { "INEGI",                  InstructionType::INEGI               },
    { "IMVI",                   InstructionType::IMVI                },
    { "FNEGI",                  InstructionType::FNEGI               },
    { "FMVI",                   InstructionType::FMVI                },
    { "DNEGI",                  InstructionType::DNEGI               },
    { "DMVI",                   InstructionType::DMVI                },
    { "BEQ",                    InstructionType::BEQ                 },
    { "BNE",                    InstructionType::BNE                 },
    { "IBLT",                   InstructionType::IBLT                },
    { "IBGE",                   InstructionType::IBGE                },
    { "FBLT",                   InstructionType::FBLT                },
    { "FBGE",                   InstructionType::FBGE                },
    { "DBLT",                   InstructionType::DBLT                },
    { "DBGE",                   InstructionType::DBGE                },
    { "LOADB",                  InstructionType::LOADB               },
    { "LOADH",                  InstructionType::LOADH               },
    { "LOADW",                  InstructionType::LOADW               },
    { "LOADD",                  InstructionType::LOADD               },
    { "STOREB",                 InstructionType::STOREB              },
    { "STOREH",                 InstructionType::STOREH              },
    { "STOREW",                 InstructionType::STOREW              },
    { "STORED",                 InstructionType::STORED              },
    { "I2F",                    InstructionType::I2F                 },
    { "I2D",                    InstructionType::I2D                 },
    { "F2I",                    InstructionType::F2I                 },
    { "F2D",                    InstructionType::F2D                 },
    { "D2I",                    InstructionType::D2I                 },
    { "D2F",                    InstructionType::D2F                 },
    { "RET",                    InstructionType::RET                 },
    { "IRET",                   InstructionType::IRET                },
    { "FRET",                   InstructionType::FRET                },
    { "DRET",                   InstructionType::DRET                },
    { "JMP",                    InstructionType::JMP                 },
    { "INTRINSIC",              InstructionType::INTRINSIC           }
};


enum IntrinsicType : uint8_t {
    INTRINSIC_INVALID = 0,

    INTRINSIC_ISCAN,
    INTRINSIC_IPRINT,
    INTRINSIC_FSCAN,
    INTRINSIC_FPRINT,
    INTRINSIC_DSCAN,
    INTRINSIC_DPRINT,

    INTRINSIC_COUNT
};

static const std::unordered_map<std::string, IntrinsicType> intrinsicsNameType = {
    { "INTRINSIC_INVALID",  INTRINSIC_INVALID },

    { "INTRINSIC_ISCAN",    INTRINSIC_ISCAN   },
    { "INTRINSIC_IPRINT",   INTRINSIC_IPRINT  },
    { "INTRINSIC_FSCAN",    INTRINSIC_FSCAN   },
    { "INTRINSIC_FPRINT",   INTRINSIC_FPRINT  },
    { "INTRINSIC_DSCAN",    INTRINSIC_DSCAN   },
    { "INTRINSIC_DPRINT",   INTRINSIC_DPRINT  }
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

        int32_t s_imm;
        float fpimm;

        IntrinsicType intrinType;
    };
};


}   // VM

#endif  // INSTRUCTIONS_H
