#include <iostream>
#include <fstream>
#include <algorithm>

#include "Parser.h"


namespace Common {

static inline VM::RegisterType getRegisterFromStr(const std::string& str) {
    if (str[0] != 'x')
        throw std::runtime_error("invalid register");

    return static_cast<VM::RegisterType>(std::stoul(str.substr(1, str.npos)));
}

static inline uint32_t getUint32FromStr(const std::string& str) {
    return static_cast<uint32_t>(std::stoul(str));
}

static inline VM::IntrinsicType getIntrinsicFromStr(const std::string& str) {
    auto it = VM::intrinsicsNameType.find("INTRINSIC_" + str); 
    if (it == VM::intrinsicsNameType.cend())
        throw std::runtime_error("invalid intrinsic");

    return it->second;
}


bool Parser::parseAsmProgram(const std::string& filename, std::vector<VM::DecodedInstruction>& decInstructions) const {

    std::ifstream file;
    file.open(filename);

    try {
        if (!file.is_open()) {
            throw std::runtime_error("could not open the file");
        }

        std::string line;
        while (std::getline(file, line)) {
            std::string opcodeStr      = "";
            std::string allOperandsStr = "";


            line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
            if (line.empty())
                continue;

            line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
            if (line.empty())
                continue;


            std::replace(line.begin(), line.end(), '\t', ' ');

            line = line.substr(line.find_first_not_of(' '), line.npos);
            line = line.substr(0, line.find(';'));
            if (line.empty())
                continue;

            size_t spaceIdx = line.find(' ');
            opcodeStr = line.substr(0, spaceIdx);
            if (spaceIdx < line.npos)
                allOperandsStr = line.substr(spaceIdx, line.npos);

            allOperandsStr.erase(std::remove(allOperandsStr.begin(), allOperandsStr.end(), ' '), allOperandsStr.end());

            std::string operandsStr[3];

            size_t comma_0_idx = allOperandsStr.find(',');
            operandsStr[0] = allOperandsStr.substr(0, comma_0_idx);

            if (comma_0_idx < allOperandsStr.npos - 1) {
                std::string remain_opers = allOperandsStr.substr(comma_0_idx + 1, allOperandsStr.npos);

                size_t comma_1_idx = remain_opers.find(',');
                operandsStr[1] = remain_opers.substr(0, comma_1_idx);

                if (comma_1_idx < remain_opers.npos - 1)
                    operandsStr[2] = remain_opers.substr(comma_1_idx + 1, remain_opers.npos);
            }

            auto opcodeIt = VM::instructionsNameOpcode.find(opcodeStr);
            if (opcodeIt == VM::instructionsNameOpcode.cend())
                continue;

            VM::InstructionType opcode = opcodeIt->second;
            VM::DecodedInstruction decInstr;

            switch(opcode) {
                case VM::InstructionType::IADD:
                case VM::InstructionType::ISUB:
                case VM::InstructionType::IMUL:
                case VM::InstructionType::IDIV:
                case VM::InstructionType::FADD:
                case VM::InstructionType::FSUB:
                case VM::InstructionType::FMUL:
                case VM::InstructionType::FDIV:
                case VM::InstructionType::DADD:
                case VM::InstructionType::DSUB:
                case VM::InstructionType::DMUL:
                case VM::InstructionType::DDIV:
                case VM::InstructionType::AND:
                case VM::InstructionType::OR:
                case VM::InstructionType::XOR:
                case VM::InstructionType::SL:
                case VM::InstructionType::SR:
                {
                    decInstr.rd  = getRegisterFromStr(operandsStr[0]);
                    decInstr.rs1 = getRegisterFromStr(operandsStr[1]);
                    decInstr.rs2 = getRegisterFromStr(operandsStr[2]);
                    decInstr.opcode = opcode;
                    break;
                }
                case VM::InstructionType::FSQRT:
                case VM::InstructionType::FSIN:
                case VM::InstructionType::FCOS:
                case VM::InstructionType::FTAN:
                case VM::InstructionType::DSQRT:
                case VM::InstructionType::DSIN:
                case VM::InstructionType::DCOS:
                case VM::InstructionType::DTAN:
                case VM::InstructionType::INEG:
                case VM::InstructionType::IMV:
                case VM::InstructionType::FNEG:
                case VM::InstructionType::FMV:
                case VM::InstructionType::DNEG:
                case VM::InstructionType::DMV:
                {
                    decInstr.rd  = getRegisterFromStr(operandsStr[0]);
                    decInstr.rs1 = getRegisterFromStr(operandsStr[1]);
                    decInstr.opcode = opcode;
                    break;
                }
                case VM::InstructionType::IADDI:
                case VM::InstructionType::ISUBI:
                case VM::InstructionType::IMULI:
                case VM::InstructionType::IDIVI:
                case VM::InstructionType::FADDI:
                case VM::InstructionType::FSUBI:
                case VM::InstructionType::FMULI:
                case VM::InstructionType::FDIVI:
                case VM::InstructionType::DADDI:
                case VM::InstructionType::DSUBI:
                case VM::InstructionType::DMULI:
                case VM::InstructionType::DDIVI:
                case VM::InstructionType::ANDI:
                case VM::InstructionType::ORI:
                case VM::InstructionType::XORI:
                case VM::InstructionType::SLI:
                case VM::InstructionType::SRI:
                {
                    if (opcodeStr[0] == 'F') {
                        decInstr.fpimm = std::stof(operandsStr[2]);
                    }
                    else if (opcodeStr[0] == 'D') {

                    }
                    else {
                        decInstr.imm = getUint32FromStr(operandsStr[2]);
                    }

                    decInstr.rd  = getRegisterFromStr(operandsStr[0]);
                    decInstr.rs1 = getRegisterFromStr(operandsStr[1]);
                    decInstr.opcode = opcode;
                    break;
                }
                case VM::InstructionType::FSQRTI:
                case VM::InstructionType::FSINI:
                case VM::InstructionType::FCOSI:
                case VM::InstructionType::FTANI:
                case VM::InstructionType::DSQRTI:
                case VM::InstructionType::DSINI:
                case VM::InstructionType::DCOSI:
                case VM::InstructionType::DTANI:
                case VM::InstructionType::INEGI:
                case VM::InstructionType::IMVI:
                case VM::InstructionType::FNEGI:
                case VM::InstructionType::FMVI:
                case VM::InstructionType::DNEGI:
                case VM::InstructionType::DMVI:
                {
                    if (opcodeStr[0] == 'F') {
                        decInstr.fpimm = std::stof(operandsStr[1]);
                    }
                    else if (opcodeStr[0] == 'D') {

                    }
                    else {
                        decInstr.imm = getUint32FromStr(operandsStr[1]);
                    }

                    decInstr.rd  = getRegisterFromStr(operandsStr[0]);
                    decInstr.opcode = opcode;
                    break;
                }
                case VM::InstructionType::BEQ:
                case VM::InstructionType::BNE:
                case VM::InstructionType::IBLT:
                case VM::InstructionType::IBGE:
                case VM::InstructionType::FBLT:
                case VM::InstructionType::FBGE:
                case VM::InstructionType::DBLT:
                case VM::InstructionType::DBGE:
                {
                    decInstr.imm = getUint32FromStr(operandsStr[2]);
                    decInstr.rs2 = getRegisterFromStr(operandsStr[1]);
                    decInstr.rs1 = getRegisterFromStr(operandsStr[0]);
                    decInstr.opcode = opcode;
                    break;            
                }
                case VM::InstructionType::LOADB:
                case VM::InstructionType::LOADH:
                case VM::InstructionType::LOADW:
                case VM::InstructionType::LOADD:
                {
                    decInstr.imm = getUint32FromStr(operandsStr[2]);
                    decInstr.rd  = getRegisterFromStr(operandsStr[0]);
                    decInstr.rs1 = getRegisterFromStr(operandsStr[1]);
                    decInstr.opcode = opcode;
                    break;              
                }
                case VM::InstructionType::STOREB:
                case VM::InstructionType::STOREH:
                case VM::InstructionType::STOREW:
                case VM::InstructionType::STORED:
                {
                    decInstr.imm = getUint32FromStr(operandsStr[2]);
                    decInstr.rs2 = getRegisterFromStr(operandsStr[1]);
                    decInstr.rs1 = getRegisterFromStr(operandsStr[0]);
                    decInstr.opcode = opcode;
                    break;              
                }
                case VM::InstructionType::I2F:
                case VM::InstructionType::I2D:
                case VM::InstructionType::F2I:
                case VM::InstructionType::F2D:
                case VM::InstructionType::D2I:
                case VM::InstructionType::D2F:
                {
                    decInstr.opcode = opcode;
                    break;
                }
                case VM::InstructionType::INTRINSIC:
                {
                    decInstr.rs1 = getRegisterFromStr(operandsStr[0]);
                    decInstr.intrinType = getIntrinsicFromStr(operandsStr[1]);
                    decInstr.opcode = opcode;
                    break;
                }
                case VM::InstructionType::RET:
                case VM::InstructionType::IRET:
                case VM::InstructionType::FRET:
                case VM::InstructionType::DRET:
                {
                    decInstr.opcode = opcode;
                    break;
                }
                case VM::InstructionType::JMP:
                {
                    decInstr.imm = getUint32FromStr(operandsStr[0]);
                    decInstr.opcode = opcode;
                    break;
                }
                default: {
                    decInstr.opcode = VM::InstructionType::INSTRUCTION_INVALID;
                    break;
                }
            }

            decInstructions.push_back(decInstr);
        }
    }
    catch(const std::runtime_error& e) {
        std::cerr << "Errors occured during parsing " << filename << ": " << e.what() << std::endl;
        file.close();
        return false;
    }
    file.close();
    return true;
}

}   // Common
