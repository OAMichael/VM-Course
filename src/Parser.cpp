#include <iostream>
#include <fstream>
#include <algorithm>

#include "Parser.h"


namespace Common {

static inline VM::RegisterType getRegisterFromStr(const std::string& str) {
    if (str[0] != 'x' && str[0] != 'r')
        throw std::runtime_error("invalid register");

    return static_cast<VM::RegisterType>(std::stoul(str.substr(1, str.npos)));
}

static inline uint16_t getUint16FromStr(const std::string& str) {
    return static_cast<uint16_t>(std::stoi(str));
}

static inline int64_t getInt64FromStr(const std::string& str) {
    return static_cast<int64_t>(std::stoll(str));
}

static inline double getDoubleFromStr(const std::string& str) {
    return static_cast<double>(std::stod(str));
}

static inline VM::IntrinsicType getIntrinsicFromStr(const std::string& str) {
    auto it = VM::intrinsicsNameType.find(str);
    if (it == VM::intrinsicsNameType.cend())
        throw std::runtime_error("invalid intrinsic");

    return it->second;
}

static inline VM::BasicObjectType getBasicTypeFromStr(const std::string& str) {
    auto it = VM::objectsNameType.find(str);
    if (it == VM::objectsNameType.cend())
        throw std::runtime_error("invalid basic type");

    return it->second;
}

bool Parser::removeExtraSpacesAndComments(std::string& line) const {
    line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
    if (line.empty())
        return false;

    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    if (line.empty())
        return false;

    std::replace(line.begin(), line.end(), '\t', ' ');

    auto spaceIdx = line.find_first_not_of(' ');
    if (spaceIdx == line.npos)
        return false;

    line = line.substr(spaceIdx);
    if (line.empty())
        return false;

    line = line.substr(0, line.find(';'));
    if (line.empty())
        return false;

    line = line.substr(0, line.find_last_not_of(' ') + 1);

    return true;
}


bool Parser::parseOpcodeAndOperands(const std::string& origLine, std::string& opcode, std::string (&operands)[2]) const {
    std::string line = origLine;
    std::string allOperandsStr = "";

    size_t spaceIdx = line.find(' ');
    opcode = line.substr(0, spaceIdx);
    if (spaceIdx < line.npos)
        allOperandsStr = line.substr(spaceIdx, line.npos);

    allOperandsStr.erase(std::remove(allOperandsStr.begin(), allOperandsStr.end(), ' '), allOperandsStr.end());

    size_t comma_0_idx = allOperandsStr.find(',');
    operands[0] = allOperandsStr.substr(0, comma_0_idx);

    if (comma_0_idx < allOperandsStr.npos - 1) {
        std::string remain_opers = allOperandsStr.substr(comma_0_idx + 1, allOperandsStr.npos);

        size_t comma_1_idx = remain_opers.find(',');
        operands[1] = remain_opers.substr(0, comma_1_idx);
    }

    return true;
}


bool Parser::checkIfLineIsFunctionLabel(const std::string& line) const {
    static const size_t FUNC_LEN = std::strlen("FUNC ");

    // Function with no name is invalid
    if (line.length() <= FUNC_LEN) {
        return false;
    }
    return line.substr(0, FUNC_LEN) == "FUNC ";
}

std::pair<std::string, uint8_t> Parser::parseFunctionLabel(const std::string& line) const {
    uint8_t numArgs = 0;
    std::string funcLabel = "";

    size_t colonIdx = line.find_last_of(":");

    static const size_t FUNC_LEN = std::strlen("FUNC ");
    funcLabel = line.substr(FUNC_LEN, colonIdx - FUNC_LEN);

    if (colonIdx < line.npos - 1) {
        std::string argsStr = line.substr(colonIdx + 1);
        const char* digits = "0123456789";
        std::size_t n = argsStr.find_first_of(digits);
        if (n != argsStr.npos) {
            std::string numStr = argsStr.substr(n, argsStr.find_first_not_of(digits, n));
            numArgs = static_cast<uint8_t>(std::stoi(numStr));
        }
    }

    return std::pair<std::string, uint8_t>(funcLabel, numArgs);
}


bool Parser::checkIfLineIsSimpleLabel(const std::string& line) const {
    // Label always end with colon
    if (line.back() != ':')
        return false;

    // There must be at least one symbol != ':'
    if (line.length() < 2)
        return false;

    // Label always is only token
    if (line.find(' ') != line.npos)
        return false;

    return true;
}

std::string Parser::parseSimpleLabel(const std::string& line) const {
    return line.substr(0, line.length() - 1);
}


bool Parser::parseAsmProgram(const std::string& filename, Common::Program& program) const {

    std::ifstream file;
    file.open(filename);

    try {
        if (!file.is_open()) {
            throw std::runtime_error("could not open the file");
        }

        std::vector<std::string> instructionLines;
        std::unordered_map<std::string, std::pair<uint64_t, uint8_t>> functions_pc_args;
        std::unordered_map<std::string, uint64_t> labels_pc;
        std::string line;

        // Read all file information at first
        while (std::getline(file, line)) {

            // Comments, empty lines and spaces before and after all tokens
            if (!removeExtraSpacesAndComments(line)) {
                continue;
            }

            // This is function label
            if (checkIfLineIsFunctionLabel(line)) {
                auto label_args = parseFunctionLabel(line);
                functions_pc_args[label_args.first] = std::pair<uint64_t, uint8_t>(instructionLines.size(), label_args.second);
                continue;
            }

            // This is simple label
            if (checkIfLineIsSimpleLabel(line)) {
                std::string label = parseSimpleLabel(line);
                labels_pc[label] = instructionLines.size();
                continue;
            }

            instructionLines.push_back(line);
        }
        file.close();

        for (uint64_t i = 0; i < instructionLines.size(); ++i) {

            std::string opcodeStr = "";
            std::string operandsStr[2];

            parseOpcodeAndOperands(instructionLines[i], opcodeStr, operandsStr);

            auto opcodeIt = VM::instructionsNameOpcode.find(opcodeStr);
            if (opcodeIt == VM::instructionsNameOpcode.cend()) {
                std::string er = "unknown instruction: " + opcodeStr;
                throw std::runtime_error(er);
            }

            VM::InstructionType opcode = opcodeIt->second;
            VM::DecodedInstruction decInstr;

            switch(opcode) {

                // ================================== Type A ================================== //
                case VM::InstructionType::LOAD_ACC:
                case VM::InstructionType::STORE_ACC:
                case VM::InstructionType::TO_FLOAT_REG:
                case VM::InstructionType::TO_INT_REG:
                case VM::InstructionType::ADD:
                case VM::InstructionType::SUB:
                case VM::InstructionType::MUL:
                case VM::InstructionType::DIV:
                case VM::InstructionType::AND:
                case VM::InstructionType::OR:
                case VM::InstructionType::XOR:
                case VM::InstructionType::SL:
                case VM::InstructionType::SR:
                case VM::InstructionType::ADDF:
                case VM::InstructionType::SUBF:
                case VM::InstructionType::MULF:
                case VM::InstructionType::DIVF:
                {
                    decInstr.r1 = getRegisterFromStr(operandsStr[0]);
                    decInstr.opcode = opcode;
                    break;
                }

                // ================================== Type B ================================== //
                case VM::InstructionType::LOAD_ACC_MEM:
                case VM::InstructionType::STORE_ACC_MEM:
                case VM::InstructionType::BEQ:
                case VM::InstructionType::BNE:
                case VM::InstructionType::BGE:
                case VM::InstructionType::BLT:
                case VM::InstructionType::BGEF:
                case VM::InstructionType::BLTF:
                case VM::InstructionType::MVI:
                {
                    decInstr.r1 = getRegisterFromStr(operandsStr[0]);
                    decInstr.opcode = opcode;

                    VM::Immediate constant{};
                    if (operandsStr[1].find(".") != operandsStr[1].npos) {
                        constant.type = VM::BasicObjectType::FLOATING;
                        constant.f_val = getDoubleFromStr(operandsStr[1]);
                    }
                    else {
                        constant.type = VM::BasicObjectType::INTEGER;
                        if (auto it = labels_pc.find(operandsStr[1]); it != labels_pc.cend()) {
                            constant.i_val = VM::INSTRUCTION_BYTESIZE * ((int64_t)it->second - i);
                        }
                        else {
                            constant.i_val = getInt64FromStr(operandsStr[1]);
                        }
                    }

                    if (auto it = std::find(program.constants.begin(), program.constants.end(), constant); it != program.constants.end()) {
                        decInstr.immIdx = static_cast<VM::ImmediateIndex>(it - program.constants.begin());
                    }
                    else {
                        decInstr.immIdx = static_cast<VM::ImmediateIndex>(program.constants.size());
                        program.constants.push_back(constant);
                    }

                    break;
                }

                // ================================== Type I ================================== //
                case VM::InstructionType::LOAD_ACCI:
                case VM::InstructionType::ADDI:
                case VM::InstructionType::SUBI:
                case VM::InstructionType::MULI:
                case VM::InstructionType::DIVI:
                case VM::InstructionType::ANDI:
                case VM::InstructionType::ORI:
                case VM::InstructionType::XORI:
                case VM::InstructionType::SLI:
                case VM::InstructionType::SRI:
                case VM::InstructionType::JMP:
                {
                    decInstr.opcode = opcode;

                    VM::Immediate constant{};
                    if (operandsStr[0].find(".") != operandsStr[0].npos) {
                        constant.type = VM::BasicObjectType::FLOATING;
                        constant.f_val = getDoubleFromStr(operandsStr[0]);
                    }
                    else {
                        constant.type = VM::BasicObjectType::INTEGER;
                        if (auto it = labels_pc.find(operandsStr[0]); it != labels_pc.cend()) {
                            constant.i_val = VM::INSTRUCTION_BYTESIZE * ((int64_t)it->second - i);
                        }
                        else {
                            constant.i_val = getInt64FromStr(operandsStr[0]);
                        }
                    }

                    if (auto it = std::find(program.constants.begin(), program.constants.end(), constant); it != program.constants.end()) {
                        decInstr.immIdx = static_cast<VM::ImmediateIndex>(it - program.constants.begin());
                    }
                    else {
                        decInstr.immIdx = static_cast<VM::ImmediateIndex>(program.constants.size());
                        program.constants.push_back(constant);
                    }

                    break;
                }

                // ================================== Type C ================================== //
                case VM::InstructionType::TO_FLOAT:
                case VM::InstructionType::TO_INT:
                case VM::InstructionType::NEG:
                case VM::InstructionType::NEGF:
                case VM::InstructionType::SIN:
                case VM::InstructionType::COS:
                case VM::InstructionType::SQRT:
                {
                    decInstr.opcode = opcode;
                    break;
                }

                // ================================== Type R ================================== //
                case VM::InstructionType::MV:
                {
                    decInstr.r1 = getRegisterFromStr(operandsStr[0]);
                    decInstr.r2 = getRegisterFromStr(operandsStr[1]);
                    decInstr.opcode = opcode;
                    break;
                }

                // ================================== Type N ================================== //
                case VM::InstructionType::CALL_INTRINSIC:
                {
                    decInstr.intrinType = getIntrinsicFromStr(operandsStr[0]);
                    decInstr.opcode = opcode;
                    break;
                }

                // ================================== Type ? ================================== //
                case VM::InstructionType::CALL:
                {
                    decInstr.numArgs = functions_pc_args[operandsStr[0]].second;
                    decInstr.opcode = opcode;

                    VM::Immediate constant{};
                    constant.type = VM::BasicObjectType::INTEGER;
                    constant.i_val = VM::INSTRUCTION_BYTESIZE * ((int64_t)functions_pc_args[operandsStr[0]].first - i);

                    if (auto it = std::find(program.constants.begin(), program.constants.end(), constant); it != program.constants.end()) {
                        decInstr.immIdx = static_cast<VM::ImmediateIndex>(it - program.constants.begin());
                    }
                    else {
                        decInstr.immIdx = static_cast<VM::ImmediateIndex>(program.constants.size());
                        program.constants.push_back(constant);
                    }

                    break;
                }
                case VM::InstructionType::RET:
                {
                    decInstr.opcode = opcode;
                    break;
                }
                case VM::InstructionType::NEW:
                case VM::InstructionType::NEWARRAY:
                {
                    decInstr.objType = getBasicTypeFromStr(operandsStr[0]);
                    decInstr.opcode = opcode;
                    break;
                }
                default: {
                    decInstr.opcode = VM::InstructionType::INSTRUCTION_INVALID;
                    break;
                }
            }

            VM::EncodedInstruction encInstr;
            m_coder.encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }

        program.entryPoint = DEFAULT_ENTRY_POINT;
        if (auto it = functions_pc_args.find("MAIN"); it != functions_pc_args.end()) {
            program.entryPoint = VM::INSTRUCTION_BYTESIZE * it->second.first;
        }
    }
    catch(const std::runtime_error& e) {
        std::cerr << "Errors occured during parsing " << filename << ": " << e.what() << std::endl;
        if (file.is_open()) {
            file.close();
        }
        return false;
    }
    return true;
}

}   // Common
