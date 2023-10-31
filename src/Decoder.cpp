#include "Decoder.h"


namespace VM {

InstructionType Decoder::getOpcode(const EncodedInstruction instr) const {
    return static_cast<InstructionType>(getPartialBitsShifted<7, 0>(instr));
}


IntrinsicType Decoder::getIntrinsicType(const EncodedInstruction instr) const {
    return static_cast<IntrinsicType>(getPartialBitsShifted<15, 8>(instr));
}


void Decoder::decodeInstruction(const EncodedInstruction encInstr, DecodedInstruction& decInstr) const {
    const InstructionType opcode = getOpcode(encInstr);

    switch(opcode) {

        // ================================== Type A ================================== //
        case InstructionType::LOAD_ACC:
        case InstructionType::STORE_ACC:
        case InstructionType::TO_FLOAT_REG:
        case InstructionType::TO_INT_REG:
        case InstructionType::ADD:
        case InstructionType::SUB:
        case InstructionType::MUL:
        case InstructionType::DIV:
        case InstructionType::AND:
        case InstructionType::OR:
        case InstructionType::XOR:
        case InstructionType::SL:
        case InstructionType::SR:
        case InstructionType::ADDF:
        case InstructionType::SUBF:
        case InstructionType::MULF:
        case InstructionType::DIVF:
        {
            decInstr.r1 = static_cast<RegisterType>(getPartialBitsShifted<15, 8>(encInstr));
            decInstr.opcode = opcode;
            break;
        }

        // ================================== Type B ================================== //
        case InstructionType::LOAD_ACC_MEM:
        case InstructionType::STORE_ACC_MEM:
        case InstructionType::BEQ:
        case InstructionType::BNE:
        case InstructionType::BGE:
        case InstructionType::BLT:
        case InstructionType::BGEF:
        case InstructionType::BLTF:
        case InstructionType::MVI:
        case InstructionType::CALL:
        {
            decInstr.r1 = static_cast<RegisterType>(getPartialBitsShifted<15, 8>(encInstr));
            decInstr.immIdx = static_cast<VM::ImmediateIndex>(getPartialBitsShifted<31, 16>(encInstr));
            decInstr.opcode = opcode;
            break;
        }

        // ================================== Type I ================================== //
        case InstructionType::LOAD_ACCI:
        case InstructionType::ADDI:
        case InstructionType::SUBI:
        case InstructionType::MULI:
        case InstructionType::DIVI:
        case InstructionType::ANDI:
        case InstructionType::ORI:
        case InstructionType::XORI:
        case InstructionType::SLI:
        case InstructionType::SRI:
        case InstructionType::JMP:
        {
            decInstr.immIdx = static_cast<VM::ImmediateIndex>(getPartialBitsShifted<31, 16>(encInstr));
            decInstr.opcode = opcode;
            break;
        }

        // ================================== Type C ================================== //
        case InstructionType::TO_FLOAT:
        case InstructionType::TO_INT:
        case InstructionType::NEG:
        case InstructionType::NEGF:
        case InstructionType::SIN:
        case InstructionType::COS:
        case InstructionType::SQRT:
        {
            decInstr.opcode = opcode;
            break;
        }

        // ================================== Type R ================================== //
        case InstructionType::MV:
        {
            decInstr.r1 = static_cast<RegisterType>(getPartialBitsShifted<15, 8>(encInstr));
            decInstr.r2 = static_cast<RegisterType>(getPartialBitsShifted<23, 16>(encInstr));
            decInstr.opcode = opcode;
            break;
        }

        // ================================== Type N ================================== //
        case InstructionType::CALL_INTRINSIC:
        {
            decInstr.intrinType = getIntrinsicType(encInstr);
            decInstr.opcode = opcode;
            break;
        }

        // ================================== Type ? ================================== //
        case InstructionType::RET:
        {
            decInstr.opcode = opcode;
            break;
        }
        default: {
            decInstr.opcode = InstructionType::INSTRUCTION_INVALID;
            break;
        }
    }
}


void Decoder::encodeInstruction(const DecodedInstruction& decInstr, EncodedInstruction& encInstr) const {

    switch(decInstr.opcode) {

        // ================================== Type A ================================== //
        case InstructionType::LOAD_ACC:
        case InstructionType::STORE_ACC:
        case InstructionType::TO_FLOAT_REG:
        case InstructionType::TO_INT_REG:
        case InstructionType::ADD:
        case InstructionType::SUB:
        case InstructionType::MUL:
        case InstructionType::DIV:
        case InstructionType::AND:
        case InstructionType::OR:
        case InstructionType::XOR:
        case InstructionType::SL:
        case InstructionType::SR:
        case InstructionType::ADDF:
        case InstructionType::SUBF:
        case InstructionType::MULF:
        case InstructionType::DIVF:
        {
            encInstr = makePartialBits<15, 8>(decInstr.r1) | makePartialBits<7, 0>(decInstr.opcode);
            break;
        }

        // ================================== Type B ================================== //
        case InstructionType::LOAD_ACC_MEM:
        case InstructionType::STORE_ACC_MEM:
        case InstructionType::BEQ:
        case InstructionType::BNE:
        case InstructionType::BGE:
        case InstructionType::BLT:
        case InstructionType::BGEF:
        case InstructionType::BLTF:
        case InstructionType::MVI:
        case InstructionType::CALL:
        {
            encInstr = makePartialBits<31, 16>(decInstr.immIdx) | makePartialBits<15, 8>(decInstr.r1) | makePartialBits<7, 0>(decInstr.opcode);
            break;
        }

        // ================================== Type I ================================== //
        case InstructionType::LOAD_ACCI:
        case InstructionType::ADDI:
        case InstructionType::SUBI:
        case InstructionType::MULI:
        case InstructionType::DIVI:
        case InstructionType::ANDI:
        case InstructionType::ORI:
        case InstructionType::XORI:
        case InstructionType::SLI:
        case InstructionType::SRI:
        case InstructionType::JMP:
        {
            encInstr = makePartialBits<31, 16>(decInstr.immIdx) | makePartialBits<7, 0>(decInstr.opcode);
            break;
        }

        // ================================== Type C ================================== //
        case InstructionType::TO_FLOAT:
        case InstructionType::TO_INT:
        case InstructionType::NEG:
        case InstructionType::NEGF:
        case InstructionType::SIN:
        case InstructionType::COS:
        case InstructionType::SQRT:
        {
            encInstr = makePartialBits<7, 0>(decInstr.opcode);
            break;
        }

        // ================================== Type R ================================== //
        case InstructionType::MV:
        {
            encInstr = makePartialBits<23, 16>(decInstr.r2) | makePartialBits<15, 8>(decInstr.r1) | makePartialBits<7, 0>(decInstr.opcode);
            break;
        }

        // ================================== Type N ================================== //
        case InstructionType::CALL_INTRINSIC:
        {
            encInstr = makePartialBits<15, 8>(decInstr.intrinType) | makePartialBits<7, 0>(decInstr.opcode);
            break;
        }

        // ================================== Type ? ================================== //
        case InstructionType::RET:
        {
            encInstr = makePartialBits<7, 0>(decInstr.opcode);
            break;
        }
        default: {
            encInstr = makePartialBits<7, 0>(InstructionType::INSTRUCTION_INVALID);
            break;
        }
    }
}


}   // VM
