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
        case InstructionType::IADD:
        case InstructionType::ISUB:
        case InstructionType::IMUL:
        case InstructionType::IDIV:
        case InstructionType::FADD:
        case InstructionType::FSUB:
        case InstructionType::FMUL:
        case InstructionType::FDIV:
        case InstructionType::DADD:
        case InstructionType::DSUB:
        case InstructionType::DMUL:
        case InstructionType::DDIV:
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
        case InstructionType::FSQRT:
        case InstructionType::FSIN:
        case InstructionType::FCOS:
        case InstructionType::FTAN:
        case InstructionType::DSQRT:
        case InstructionType::DSIN:
        case InstructionType::DCOS:
        case InstructionType::DTAN:
        case InstructionType::INEG:
        case InstructionType::IMV:
        case InstructionType::FNEG:
        case InstructionType::FMV:
        case InstructionType::DNEG:
        case InstructionType::DMV:
        {
            decInstr.rd  = static_cast<RegisterType>(getPartialBitsShifted<17, 13>(encInstr));
            decInstr.rs1 = static_cast<RegisterType>(getPartialBitsShifted<12, 8>(encInstr));
            decInstr.opcode = opcode;
            break;
        }

        // ======== Arithmetic immediate type I ======== //
        // instr rd, rs1, imm
        case InstructionType::IADDI:
        case InstructionType::ISUBI:
        case InstructionType::IMULI:
        case InstructionType::IDIVI:
        case InstructionType::FADDI:
        case InstructionType::FSUBI:
        case InstructionType::FMULI:
        case InstructionType::FDIVI:
        case InstructionType::DADDI:
        case InstructionType::DSUBI:
        case InstructionType::DMULI:
        case InstructionType::DDIVI:
        case InstructionType::ANDI:
        case InstructionType::ORI:
        case InstructionType::XORI:
        case InstructionType::SLI:
        case InstructionType::SRI:
        {
            decInstr.imm = getPartialBitsShifted<63, 32>(encInstr);
            decInstr.rd  = static_cast<RegisterType>(getPartialBitsShifted<17, 13>(encInstr));
            decInstr.rs1 = static_cast<RegisterType>(getPartialBitsShifted<12, 8>(encInstr));
            decInstr.opcode = opcode;
            break;
        }

        // ======== Arithmetic immediate type II ======= //
        // instr rd, imm
        case InstructionType::FSQRTI:
        case InstructionType::FSINI:
        case InstructionType::FCOSI:
        case InstructionType::FTANI:
        case InstructionType::DSQRTI:
        case InstructionType::DSINI:
        case InstructionType::DCOSI:
        case InstructionType::DTANI:
        case InstructionType::INEGI:
        case InstructionType::IMVI:
        case InstructionType::FNEGI:
        case InstructionType::FMVI:
        case InstructionType::DNEGI:
        case InstructionType::DMVI:
        {
            decInstr.imm = getPartialBitsShifted<63, 32>(encInstr);
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
            decInstr.imm = getPartialBitsShifted<63, 32>(encInstr);
            decInstr.rs2 = static_cast<RegisterType>(getPartialBitsShifted<17, 13>(encInstr));
            decInstr.rs1 = static_cast<RegisterType>(getPartialBitsShifted<12, 8>(encInstr));
            decInstr.opcode = opcode;
            break;            
        }

        // ==================== Load =================== //
        // instr rd, rs1, imm
        case InstructionType::LOADB:
        case InstructionType::LOADH:
        case InstructionType::LOADW:
        case InstructionType::LOADD:
        {
            decInstr.imm = getPartialBitsShifted<63, 32>(encInstr);
            decInstr.rd  = static_cast<RegisterType>(getPartialBitsShifted<17, 13>(encInstr));
            decInstr.rs1 = static_cast<RegisterType>(getPartialBitsShifted<12, 8>(encInstr));
            decInstr.opcode = opcode;
            break;              
        }

        // =================== Store =================== //
        // instr rs1, rs2, imm
        case InstructionType::STOREB:
        case InstructionType::STOREH:
        case InstructionType::STOREW:
        case InstructionType::STORED:
        {
            decInstr.imm = getPartialBitsShifted<63, 32>(encInstr);
            decInstr.rs2 = static_cast<RegisterType>(getPartialBitsShifted<17, 13>(encInstr));
            decInstr.rs1 = static_cast<RegisterType>(getPartialBitsShifted<12, 8>(encInstr));
            decInstr.opcode = opcode;
            break;              
        }

        // =================== Casts =================== //
        // instr rs1, rs2, imm
        case InstructionType::I2F:
        case InstructionType::I2D:
        case InstructionType::F2I:
        case InstructionType::F2D:
        case InstructionType::D2I:
        case InstructionType::D2F:
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
        case InstructionType::IRET:
        case InstructionType::FRET:
        case InstructionType::DRET:
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

        // ============= Arithmetic type I ============= //
        // instr rd, rs1, rs2
        case InstructionType::IADD:
        case InstructionType::ISUB:
        case InstructionType::IMUL:
        case InstructionType::IDIV:        
        case InstructionType::FADD:
        case InstructionType::FSUB:
        case InstructionType::FMUL:
        case InstructionType::FDIV:
        case InstructionType::DADD:
        case InstructionType::DSUB:
        case InstructionType::DMUL:
        case InstructionType::DDIV:
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
        case InstructionType::FSQRT:
        case InstructionType::FSIN:
        case InstructionType::FCOS:
        case InstructionType::FTAN:
        case InstructionType::DSQRT:
        case InstructionType::DSIN:
        case InstructionType::DCOS:
        case InstructionType::DTAN:
        case InstructionType::INEG:
        case InstructionType::IMV:
        case InstructionType::FNEG:
        case InstructionType::FMV:
        case InstructionType::DNEG:
        case InstructionType::DMV:
        {
            encInstr =
                makePartialBits<17, 13>(decInstr.rd)
                | makePartialBits<12, 8>(decInstr.rs1)
                | makePartialBits<7, 0>(decInstr.opcode);

            break;
        }

        // ======== Arithmetic immediate type I ======== //
        // instr rd, rs1, imm
        case InstructionType::IADDI:
        case InstructionType::ISUBI:
        case InstructionType::IMULI:
        case InstructionType::IDIVI:
        case InstructionType::FADDI:
        case InstructionType::FSUBI:
        case InstructionType::FMULI:
        case InstructionType::FDIVI:
        case InstructionType::DADDI:
        case InstructionType::DSUBI:
        case InstructionType::DMULI:
        case InstructionType::DDIVI:
        case InstructionType::ANDI:
        case InstructionType::ORI:
        case InstructionType::XORI:
        case InstructionType::SLI:
        case InstructionType::SRI:
        {
            encInstr =
                makePartialBits<63, 32>(decInstr.imm)
                | makePartialBits<17, 13>(decInstr.rd)
                | makePartialBits<12, 8>(decInstr.rs1)
                | makePartialBits<7, 0>(decInstr.opcode);

            break;
        }

        // ======== Arithmetic immediate type II ======= //
        // instr rd, imm
        case InstructionType::FSQRTI:
        case InstructionType::FSINI:
        case InstructionType::FCOSI:
        case InstructionType::FTANI:
        case InstructionType::DSQRTI:
        case InstructionType::DSINI:
        case InstructionType::DCOSI:
        case InstructionType::DTANI:
        case InstructionType::INEGI:
        case InstructionType::IMVI:
        case InstructionType::FNEGI:
        case InstructionType::FMVI:
        case InstructionType::DNEGI:
        case InstructionType::DMVI:
        {
            encInstr =
                makePartialBits<63, 32>(decInstr.imm)
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
                makePartialBits<63, 32>(decInstr.imm)
                | makePartialBits<17, 13>(decInstr.rs2)
                | makePartialBits<12, 8>(decInstr.rs1)
                | makePartialBits<7, 0>(decInstr.opcode);

            break;
        }

        // ==================== Load =================== //
        // instr rd, rs1, imm
        case InstructionType::LOADB:
        case InstructionType::LOADH:
        case InstructionType::LOADW:
        case InstructionType::LOADD:
        {
            encInstr =
                makePartialBits<63, 32>(decInstr.imm)
                | makePartialBits<17, 13>(decInstr.rd)
                | makePartialBits<12, 8>(decInstr.rs1)
                | makePartialBits<7, 0>(decInstr.opcode);

            break;
        }

        // =================== Store =================== //
        // instr rs1, rs2, imm
        case InstructionType::STOREB:
        case InstructionType::STOREH:
        case InstructionType::STOREW:
        case InstructionType::STORED:
        {
            encInstr =
                makePartialBits<63, 32>(decInstr.imm)
                | makePartialBits<17, 13>(decInstr.rs2)
                | makePartialBits<12, 8>(decInstr.rs1)
                | makePartialBits<7, 0>(decInstr.opcode);

            break;
        }

        // =================== Casts =================== //
        // instr rs1, rs2, imm
        case InstructionType::I2F:
        case InstructionType::I2D:
        case InstructionType::F2I:
        case InstructionType::F2D:
        case InstructionType::D2I:
        case InstructionType::D2F:
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
        case InstructionType::IRET:
        case InstructionType::FRET:
        case InstructionType::DRET:
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
