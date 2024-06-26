#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <cstdint>
#include <unordered_map>
#include <string>
#include "generated/Instruction_enum.h"

namespace VM {

/*
 *  Whole memory looks like this:
 *  || instructions || constant pool || string pool || class descs || arena ||
 */

static constexpr uint64_t VM_PROGRAM_MEMORY_BYTESIZE       = 1 << 15;     // 32 KiB of instructions
static constexpr uint64_t VM_CONSTANT_POOL_MEMORY_BYTESIZE = 1 << 15;     // 32 KiB of constants
static constexpr uint64_t VM_STRING_POOL_MEMORY_BYTESIZE   = 1 << 15;     // 32 KiB of strings
static constexpr uint64_t VM_CLASS_DESCS_MEMORY_BYTESIZE   = 1 << 15;     // 32 KiB of class table
static constexpr uint64_t VM_ARENA_MEMORY_BYTESIZE         = 1 << 25;     // 32 MiB of arena memory

static constexpr uint64_t VM_TOTAL_MEMORY_BYTESIZE = VM_PROGRAM_MEMORY_BYTESIZE +
                                                     VM_CONSTANT_POOL_MEMORY_BYTESIZE +
                                                     VM_STRING_POOL_MEMORY_BYTESIZE +
                                                     VM_CLASS_DESCS_MEMORY_BYTESIZE +
                                                     VM_ARENA_MEMORY_BYTESIZE;

static constexpr uint64_t VM_PROGRAM_MEMORY_ADDRESS = 0;
static constexpr uint64_t VM_CONSTANT_POOL_MEMORY_ADDRESS = VM_PROGRAM_MEMORY_ADDRESS + VM_PROGRAM_MEMORY_BYTESIZE;
static constexpr uint64_t VM_STRING_POOL_MEMORY_ADDRESS = VM_CONSTANT_POOL_MEMORY_ADDRESS + VM_CONSTANT_POOL_MEMORY_BYTESIZE;
static constexpr uint64_t VM_CLASS_DESCS_MEMORY_ADDRESS = VM_STRING_POOL_MEMORY_ADDRESS + VM_STRING_POOL_MEMORY_BYTESIZE;
static constexpr uint64_t VM_ARENA_MEMORY_ADDRESS = VM_CLASS_DESCS_MEMORY_ADDRESS + VM_CLASS_DESCS_MEMORY_BYTESIZE;


enum MemoryType {
    Program = 0,
    ConstantPool = 1,
    StringPool = 2,
    ClassDescs = 3,
    Arena = 4
};


static constexpr const uint32_t FRAME_REGISTER_COUNT = 256;
static constexpr const uint64_t GARBAGE_COLLECTOR_INSTRUCTION_STEP = 1000;

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

struct Register {
    union {
        int64_t i_val;
        double  f_val;
    };
    bool isRef = false;
};

using EncodedInstruction = uint32_t;
using ImmediateIndex = uint16_t;
using NumArgsType = uint8_t;
using RegisterType = uint8_t;
using ClassIndex = uint16_t;
using FieldIndex = uint16_t;

struct ObjectHeader {
    uint16_t classIdx;
    uint16_t size;      // For arrays and strings
};

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
        NumArgsType     numArgs;    // For call
        BasicObjectType objType;    // For new and newarray (deprecated)
    };

    RegisterType r2;

    union {
        ImmediateIndex imm;         // Immediate index in constant pool
        ClassIndex classIdx;        // Class index in class table
        FieldIndex fieldIdx;        // Field index in class
    };

    InstructionType opcode = InstructionType::INSTRUCTION_INVALID;
};

struct Frame {
    uint64_t pc;
    Register regfile[FRAME_REGISTER_COUNT];
    Frame* parent;
};

}   // VM

#endif  // INSTRUCTIONS_H
