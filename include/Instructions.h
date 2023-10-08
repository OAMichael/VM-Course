#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <cstdint>


namespace VM {

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

	REGISTER_COUNT
};


enum InstructionType : uint8_t {

	INSTRUCTION_INVALID = 0,

// Arithmetic
	ADD,
	SUB,
	AND,
	OR,
	XOR,
	SL,
	SR,
	SQRT,
	SIN,
	COS,
	TAN,
	COT,
	NEG,

// Arithmetic immediate
	ADDI,
	SUBI,
	ANDI,
	ORI,
	XORI,
	SLI,
	SRI,
	SQRTI,
	SINI,
	COSI,
	TANI,
	COTI,
	NEGI,

// Branching
	BEQ,
	BNE,
	BLT,
	BGE,

// Load
	LB,
	LH,
	LW,
	LD,

// Store
	SB,
	SH,
	SW,
	SD,

// Casts
	I2F,
	I2D,
	I2L,
	F2I,
	F2D,
	F2L,
	D2I,
	D2F,
	D2L,
	L2I,
	L2F,
	L2D,

// Return
	RET,

// Intrinsics
	INTRIN_INPUT,
	INTRIN_OUTPUT,

// Miscellaneous
	INSTRUCTION_COUNT,
};


using RegValue = uint64_t;
using SignedRegValue = int64_t;

using EncodedInstruction = uint32_t;

struct DecodedInstruction {
	RegisterType rd, rs1, rs2;

	InstructionType opcode = InstructionType::INSTRUCTION_INVALID;

	union {
		uint32_t imm = 0;
		uint32_t shamt;
	};
};


}	// VM

#endif 	// INSTRUCTIONS_H
