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

        // ============= Arithmetic type I ============= //
        // instr rd, rs1, rs2
        case InstructionType::ADD:
        case InstructionType::SUB:
        case InstructionType::AND:
        case InstructionType::OR:
        case InstructionType::XOR:
        case InstructionType::SL:
        case InstructionType::SR:
        {
            decInstr.rd  = static_cast<RegisterType>(getPartialBitsShifted<22, 18>(encInstr));
            decInstr.rs1 = static_cast<RegisterType>(getPartialBitsShifted<17, 13>(encInstr));
            decInstr.rs2 = static_cast<RegisterType>(getPartialBitsShifted<12, 8>(encInstr));
            decInstr.opcode = opcode;
            break;
        }

        // ============= Arithmetic type II ============ //
        // instr rd, rs1
        case InstructionType::SQRT:
        case InstructionType::SIN:
        case InstructionType::COS:
        case InstructionType::TAN:
        case InstructionType::COT:
        case InstructionType::NEG:
        case InstructionType::MV:
        {
            decInstr.rd  = static_cast<RegisterType>(getPartialBitsShifted<17, 13>(encInstr));
            decInstr.rs1 = static_cast<RegisterType>(getPartialBitsShifted<12, 8>(encInstr));
            decInstr.opcode = opcode;
            break;
        }

        // ======== Arithmetic immediate type I ======== //
        // instr rd, rs1, imm
        case InstructionType::ADDI:
        case InstructionType::SUBI:
        case InstructionType::ANDI:
        case InstructionType::ORI:
        case InstructionType::XORI:
        case InstructionType::SLI:
        case InstructionType::SRI:
        {
            decInstr.imm = getPartialBitsShifted<31, 18>(encInstr);
            decInstr.rd  = static_cast<RegisterType>(getPartialBitsShifted<17, 13>(encInstr));
            decInstr.rs1 = static_cast<RegisterType>(getPartialBitsShifted<12, 8>(encInstr));
            decInstr.opcode = opcode;
            break;
        }

        // ======== Arithmetic immediate type II ======= //
        // instr rd, imm
        case InstructionType::SQRTI:
        case InstructionType::SINI:
        case InstructionType::COSI:
        case InstructionType::TANI:
        case InstructionType::COTI:
        case InstructionType::NEGI:
        case InstructionType::MVI:
        {
            decInstr.imm = getPartialBitsShifted<31, 13>(encInstr);
            decInstr.rd  = static_cast<RegisterType>(getPartialBitsShifted<12, 8>(encInstr));
            decInstr.opcode = opcode;
            break;
        }

        // ================= Branching ================= //
        // instr rs1, rs2, imm
        case InstructionType::BEQ:
        case InstructionType::BNE:
        case InstructionType::BLT:
        case InstructionType::BGE:
        {
            decInstr.imm = getPartialBitsShifted<31, 18>(encInstr);
            decInstr.rs2 = static_cast<RegisterType>(getPartialBitsShifted<17, 13>(encInstr));
            decInstr.rs1 = static_cast<RegisterType>(getPartialBitsShifted<12, 8>(encInstr));
            decInstr.opcode = opcode;
            break;            
        }

        // ==================== Load =================== //
        // instr rd, rs1, imm
        case InstructionType::LB:
        case InstructionType::LH:
        case InstructionType::LW:
        case InstructionType::LD:
        {
            decInstr.imm = getPartialBitsShifted<31, 18>(encInstr);
            decInstr.rd  = static_cast<RegisterType>(getPartialBitsShifted<17, 13>(encInstr));
            decInstr.rs1 = static_cast<RegisterType>(getPartialBitsShifted<12, 8>(encInstr));
            decInstr.opcode = opcode;
            break;              
        }

        // =================== Store =================== //
        // instr rs1, rs2, imm
        case InstructionType::SB:
        case InstructionType::SH:
        case InstructionType::SW:
        case InstructionType::SD:
        {
            decInstr.imm = getPartialBitsShifted<31, 18>(encInstr);
            decInstr.rs2 = static_cast<RegisterType>(getPartialBitsShifted<17, 13>(encInstr));
            decInstr.rs1 = static_cast<RegisterType>(getPartialBitsShifted<12, 8>(encInstr));
            decInstr.opcode = opcode;
            break;              
        }

        // =================== Casts =================== //
        // instr rs1, rs2, imm
        case InstructionType::I2F:
        case InstructionType::I2D:
        case InstructionType::I2L:
        case InstructionType::F2I:
        case InstructionType::F2D:
        case InstructionType::F2L:
        case InstructionType::D2I:
        case InstructionType::D2F:
        case InstructionType::D2L:
        case InstructionType::L2I:
        case InstructionType::L2F:
        case InstructionType::L2D:
        {
            decInstr.opcode = opcode;
            break;
        }

        // ================= Intrinsics ================ //
        // intrinsic intr_num
        case InstructionType::INTRINSIC:
        {
            decInstr.rs1 = static_cast<RegisterType>(getPartialBitsShifted<20, 16>(encInstr));
            decInstr.intrinType = getIntrinsicType(encInstr);
            decInstr.opcode = InstructionType::INTRINSIC;
            break;
        }

        // =================== Return ================== //
        // instr
        case InstructionType::RET:
        {
            decInstr.opcode = opcode;
            break;
        }
    }
}


void Decoder::encodeInstruction(const DecodedInstruction& decInstr, EncodedInstruction& encInstr) const {

    switch(decInstr.opcode) {

        // ============= Arithmetic type I ============= //
        // instr rd, rs1, rs2
        case InstructionType::ADD:
        case InstructionType::SUB:
        case InstructionType::AND:
        case InstructionType::OR:
        case InstructionType::XOR:
        case InstructionType::SL:
        case InstructionType::SR:
        {
            encInstr =
                makePartialBits<22, 18>(decInstr.rd)
                | makePartialBits<17, 13>(decInstr.rs1)
                | makePartialBits<12, 8>(decInstr.rs2)
                | makePartialBits<7, 0>(decInstr.opcode);

            break;
        }

        // ============= Arithmetic type II ============ //
        // instr rd, rs1
        case InstructionType::SQRT:
        case InstructionType::SIN:
        case InstructionType::COS:
        case InstructionType::TAN:
        case InstructionType::COT:
        case InstructionType::NEG:
        case InstructionType::MV:
        {
            encInstr =
                makePartialBits<17, 13>(decInstr.rd)
                | makePartialBits<12, 8>(decInstr.rs1)
                | makePartialBits<7, 0>(decInstr.opcode);

            break;
        }

        // ======== Arithmetic immediate type I ======== //
        // instr rd, rs1, imm
        case InstructionType::ADDI:
        case InstructionType::SUBI:
        case InstructionType::ANDI:
        case InstructionType::ORI:
        case InstructionType::XORI:
        case InstructionType::SLI:
        case InstructionType::SRI:
        {
            encInstr =
                makePartialBits<31, 18>(decInstr.imm)
                | makePartialBits<17, 13>(decInstr.rd)
                | makePartialBits<12, 8>(decInstr.rs1)
                | makePartialBits<7, 0>(decInstr.opcode);

            break;
        }

        // ======== Arithmetic immediate type II ======= //
        // instr rd, imm
        case InstructionType::SQRTI:
        case InstructionType::SINI:
        case InstructionType::COSI:
        case InstructionType::TANI:
        case InstructionType::COTI:
        case InstructionType::NEGI:
        case InstructionType::MVI:
        {
            encInstr =
                makePartialBits<31, 13>(decInstr.imm)
                | makePartialBits<12, 8>(decInstr.rd)
                | makePartialBits<7, 0>(decInstr.opcode);

            break;
        }

        // ================= Branching ================= //
        // instr rs1, rs2, imm
        case InstructionType::BEQ:
        case InstructionType::BNE:
        case InstructionType::BLT:
        case InstructionType::BGE:
        {
            encInstr =
                makePartialBits<31, 18>(decInstr.imm)
                | makePartialBits<17, 13>(decInstr.rs2)
                | makePartialBits<12, 8>(decInstr.rs1)
                | makePartialBits<7, 0>(decInstr.opcode);

            break;
        }

        // ==================== Load =================== //
        // instr rd, rs1, imm
        case InstructionType::LB:
        case InstructionType::LH:
        case InstructionType::LW:
        case InstructionType::LD:
        {
            encInstr =
                makePartialBits<31, 18>(decInstr.imm)
                | makePartialBits<17, 13>(decInstr.rd)
                | makePartialBits<12, 8>(decInstr.rs1)
                | makePartialBits<7, 0>(decInstr.opcode);

            break;
        }

        // =================== Store =================== //
        // instr rs1, rs2, imm
        case InstructionType::SB:
        case InstructionType::SH:
        case InstructionType::SW:
        case InstructionType::SD:
        {
            encInstr =
                makePartialBits<31, 18>(decInstr.imm)
                | makePartialBits<17, 13>(decInstr.rs2)
                | makePartialBits<12, 8>(decInstr.rs1)
                | makePartialBits<7, 0>(decInstr.opcode);

            break;
        }

        // =================== Casts =================== //
        // instr rs1, rs2, imm
        case InstructionType::I2F:
        case InstructionType::I2D:
        case InstructionType::I2L:
        case InstructionType::F2I:
        case InstructionType::F2D:
        case InstructionType::F2L:
        case InstructionType::D2I:
        case InstructionType::D2F:
        case InstructionType::D2L:
        case InstructionType::L2I:
        case InstructionType::L2F:
        case InstructionType::L2D:
        {
            encInstr = makePartialBits<7, 0>(decInstr.opcode);
            break;
        }

        // ================= Intrinsics ================ //
        // intrinsic intr_num
        case InstructionType::INTRINSIC:
        {
            encInstr =
                makePartialBits<20, 16>(decInstr.rs1)
                | makePartialBits<15, 8>(decInstr.intrinType)
                | makePartialBits<7, 0>(decInstr.opcode);

            break;
        }

        // =================== Return ================== //
        // instr
        case InstructionType::RET:
        {
            encInstr = makePartialBits<7, 0>(decInstr.opcode);
            break;
        }
    }
}


}   // VM
