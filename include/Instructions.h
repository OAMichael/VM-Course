#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <cstdint>
#include <unordered_map>
#include <string>
#include "generated/Instruction_enum.h"

namespace VM {

static constexpr const uint32_t FRAME_REGISTER_COUNT = 256;

enum BasicObjectType : uint8_t {
    OBJECT_TYPE_INVALID = 0,

    INTEGER,
    FLOATING,
    STRING,

    OBJECT_TYPE_COUNT
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
using numArgsType = uint8_t;
using RegisterType = uint8_t;

struct Immediate {
    BasicObjectType type;
    union {
        int64_t  i_val;
        double   f_val;
        uint64_t p_str;     // Reserved for later
    };

    inline bool operator==(const Immediate& other) {
        return type == other.type && i_val == other.i_val;
    }
};

struct DecodedInstruction {
    union {
        RegisterType    r1;         // For registers
        IntrinsicType   intrCode;   // For intrinsics
        numArgsType     numArgs;    // For call
        BasicObjectType objType;    // For new and newarray
    };

    RegisterType r2;
    ImmediateIndex imm;  // Immediate index in constant pool

    InstructionType opcode = InstructionType::INSTRUCTION_INVALID;
};

struct Frame {
    uint64_t pc;
    Register regfile[FRAME_REGISTER_COUNT];
    Frame* parent;
};

}   // VM

#endif  // INSTRUCTIONS_H
