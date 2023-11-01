#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <cstdint>
#include <unordered_map>
#include <string>

namespace VM {

static constexpr const uint32_t INSTRUCTION_BYTESIZE = 4;
static constexpr const uint32_t FRAME_REGISTER_COUNT = 256;


enum InstructionType : uint8_t {

    INSTRUCTION_INVALID = 0,

// Load & Store to/from accumulator
    LOAD_ACC,               // acc <- r1
    STORE_ACC,              // r1 <- acc
    LOAD_ACCI,              // acc <- imm

    LOAD_ACC_MEM,           // acc <- mem[r1 + imm]
    STORE_ACC_MEM,          // mem[r1 + imm] <- acc

// Casts
    TO_FLOAT_REG,           // acc.f_val <- (double)r1.i_val
    TO_INT_REG,             // acc.i_val <- (int64_t)r1.f_val
    TO_FLOAT,               // acc.f_val <- (double)acc.i_val
    TO_INT,                 // acc.i_val <- (int64_t)acc.f_val

// Integer register arithmetic
    ADD,                    // acc.i_val <- acc.i_val + r1.i_val
    SUB,                    // acc.i_val <- acc.i_val - r1.i_val
    MUL,                    // acc.i_val <- acc.i_val * r1.i_val
    DIV,                    // acc.i_val <- acc.i_val / r1.i_val

    AND,                    // acc.i_val <- acc.i_val & r1.i_val
    OR,                     // acc.i_val <- acc.i_val | r1.i_val
    XOR,                    // acc.i_val <- acc.i_val ^ r1.i_val
    SL,                     // acc.i_val <- acc.i_val << r1.i_val
    SR,                     // acc.i_val <- acc.i_val >> r1.i_val

    NEG,                    // acc.i_val <- -acc.i_val

// Float register arithmetic
    ADDF,                   // acc.f_val <- acc.f_val + r1.f_val
    SUBF,                   // acc.f_val <- acc.f_val - r1.f_val
    MULF,                   // acc.f_val <- acc.f_val * r1.f_val
    DIVF,                   // acc.f_val <- acc.f_val / r1.f_val

    NEGF,                   // acc.f_val <- -acc.f_val

// Arithmetic with immediate
    ADDI,                   // acc.*_val <- acc.*_val + imm
    SUBI,                   // acc.*_val <- acc.*_val - imm
    MULI,                   // acc.*_val <- acc.*_val * imm
    DIVI,                   // acc.*_val <- acc.*_val / imm

    ANDI,                   // acc.i_val <- acc.i_val & imm (integer imm only)
    ORI,                    // acc.i_val <- acc.i_val | imm (integer imm only)
    XORI,                   // acc.i_val <- acc.i_val ^ imm (integer imm only)
    SLI,                    // acc.i_val <- acc.i_val << imm (integer imm only)
    SRI,                    // acc.i_val <- acc.i_val >> imm (integer imm only)

// Miscellaneous
    SIN,                    // acc.f_val <- sin(acc.f_val)
    COS,                    // acc.f_val <- cos(acc.f_val)
    SQRT,                   // acc.f_val <- sqrt(acc.f_val)

    MV,                     // r1 <- r2
    MVI,                    // r1 <- imm

// Intrinsic
    CALL_INTRINSIC,         // intrinsic #intr

// Jumps & Branches
    JMP,                    // pc <- pc + imm

    BEQ,                    // pc <- acc.i_val == r1.i_val ? pc + imm : pc
    BNE,                    // pc <- acc.i_val != r1.i_val ? pc + imm : pc
    BGE,                    // pc <- acc.i_val >= r1.i_val ? pc + imm : pc
    BLT,                    // pc <- acc.i_val  < r1.i_val ? pc + imm : pc
    BGEF,                   // pc <- acc.f_val >= r1.f_val ? pc + imm : pc
    BLTF,                   // pc <- acc.f_val  < r1.f_val ? pc + imm : pc

    CALL,                   // ???
    RET,                    // ???

    NEW,                    // acc.i_val <- &allocated
    NEWARRAY,               // acc.i_val <- &allocated

    INSTRUCTION_COUNT,
};


enum IntrinsicType : uint8_t {
    INTRINSIC_INVALID = 0,

    SCAN,                   // acc.i_val <- stdin
    PRINT,                  // stdout <- acc.i_val
    SCANF,                  // acc.f_val <- (double)stdin
    PRINTF,                 // stdout <- acc.f_val

    INTRINSIC_COUNT
};


enum BasicObjectType : uint8_t {
    INTEGER = 0,
    FLOATING,
    STRING,

    OBJECT_TYPE_COUNT
};



static const std::unordered_map<std::string, InstructionType> instructionsNameOpcode = {

    { "INSTRUCTION_INVALID", InstructionType::INSTRUCTION_INVALID },

    { "LOAD_ACC",            InstructionType::LOAD_ACC            },
    { "STORE_ACC",           InstructionType::STORE_ACC           },
    { "LOAD_ACCI",           InstructionType::LOAD_ACCI           },

    { "LOAD_ACC_MEM",        InstructionType::LOAD_ACC_MEM        },
    { "STORE_ACC_MEM",       InstructionType::STORE_ACC_MEM       },

    { "TO_FLOAT_REG",        InstructionType::TO_FLOAT_REG        },
    { "TO_INT_REG",          InstructionType::TO_INT_REG          },
    { "TO_FLOAT",            InstructionType::TO_FLOAT            },
    { "TO_INT",              InstructionType::TO_INT              },

    { "ADD",                 InstructionType::ADD                 },
    { "SUB",                 InstructionType::SUB                 },
    { "MUL",                 InstructionType::MUL                 },
    { "DIV",                 InstructionType::DIV                 },

    { "AND",                 InstructionType::AND                 },
    { "OR",                  InstructionType::OR                  },
    { "XOR",                 InstructionType::XOR                 },
    { "SL",                  InstructionType::SL                  },
    { "SR",                  InstructionType::SR                  },

    { "NEG",                 InstructionType::NEG                 },

    { "ADDF",                InstructionType::ADDF                },
    { "SUBF",                InstructionType::SUBF                },
    { "MULF",                InstructionType::MULF                },
    { "DIVF",                InstructionType::DIVF                },

    { "NEGF",                InstructionType::NEGF                },

    { "ADDI",                InstructionType::ADDI                },
    { "SUBI",                InstructionType::SUBI                },
    { "MULI",                InstructionType::MULI                },
    { "DIVI",                InstructionType::DIVI                },

    { "ANDI",                InstructionType::ANDI                },
    { "ORI",                 InstructionType::ORI                 },
    { "XORI",                InstructionType::XORI                },
    { "SLI",                 InstructionType::SLI                 },
    { "SRI",                 InstructionType::SRI                 },

    { "SIN",                 InstructionType::SIN                 },
    { "COS",                 InstructionType::COS                 },
    { "SQRT",                InstructionType::SQRT                },

    { "MV",                  InstructionType::MV                  },
    { "MVI",                  InstructionType::MVI                },

    { "CALL_INTRINSIC",      InstructionType::CALL_INTRINSIC      },

    { "JMP",                 InstructionType::JMP                 },

    { "BEQ",                 InstructionType::BEQ                 },
    { "BNE",                 InstructionType::BNE                 },
    { "BLT",                 InstructionType::BLT                 },
    { "BGE",                 InstructionType::BGE                 },
    { "BLTF",                InstructionType::BLTF                },
    { "BGEF",                InstructionType::BGEF                },

    { "CALL",                InstructionType::CALL                },
    { "RET",                 InstructionType::RET                 },

    { "NEW",                 InstructionType::NEW                 },
    { "NEWARRAY",            InstructionType::NEWARRAY            }
};


static const std::unordered_map<std::string, IntrinsicType> intrinsicsNameType = {
    { "SCAN",   IntrinsicType::SCAN   },
    { "PRINT",  IntrinsicType::PRINT  },
    { "SCANF",  IntrinsicType::SCANF  },
    { "PRINTF", IntrinsicType::PRINTF }
};


static const std::unordered_map<std::string, BasicObjectType> objectsNameType = {
    { "INTEGER",  BasicObjectType::INTEGER  },
    { "FLOATING", BasicObjectType::FLOATING },
    { "STRING",   BasicObjectType::STRING   }
};



union Register {
    int64_t i_val;
    double  f_val;
};

using EncodedInstruction = uint32_t;
using ImmediateIndex = uint16_t;
using RegisterType = uint8_t;

struct Immediate {
    BasicObjectType type;
    union {
        int64_t  i_val;
        double   f_val;
        uint64_t p_str;     // Reserved for later
    };
};

struct DecodedInstruction {
    union {
        RegisterType    r1;         // For registers
        IntrinsicType   intrinType; // For intrinsics
        uint8_t         numArgs;    // For call
        BasicObjectType objType;    // For new and newarray
    };

    RegisterType r2;
    ImmediateIndex immIdx;  // Immediate index in constant pool

    InstructionType opcode = InstructionType::INSTRUCTION_INVALID;
};

struct Frame {
    uint64_t pc;
    Register regfile[FRAME_REGISTER_COUNT];
    Frame* parent;
};

}   // VM

#endif  // INSTRUCTIONS_H
