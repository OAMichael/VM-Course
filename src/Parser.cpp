#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "Parser.h"


namespace Common {

static constexpr const char* FUNCTION_KEYWORD = "FUNC";


static inline void replaceEscapeSeq(std::string& str, const std::string& search, const std::string& replace) {
    size_t index = 0;
    while (true) {
        index = str.find(search, index);
        if (index == std::string::npos)
            break;

        str.replace(index, search.length(), replace);
        index += replace.length();
    }
}

static inline void replaceAllEscapeSeq(std::string& str) {
    replaceEscapeSeq(str, "\\'",  "\'");
    replaceEscapeSeq(str, "\\\"", "\"");
    replaceEscapeSeq(str, "\\?",  "\?");
    replaceEscapeSeq(str, "\\\\", "\\");
    replaceEscapeSeq(str, "\\a",  "\a");
    replaceEscapeSeq(str, "\\b",  "\b");
    replaceEscapeSeq(str, "\\f",  "\f");
    replaceEscapeSeq(str, "\\n",  "\n");
    replaceEscapeSeq(str, "\\r",  "\r");
    replaceEscapeSeq(str, "\\t",  "\t");
    replaceEscapeSeq(str, "\\v",  "\v");
}

static inline void tokenizeString(const std::string& str, const std::string& delimeter, std::vector<std::string>& tokens) {
    if (str.empty()) {
        return;
    }

    char* tmp = new char[str.size() + 1];
    std::memcpy(tmp, str.c_str(), str.size());
    tmp[str.size()] = '\0';
    char* token = std::strtok(tmp, delimeter.c_str());
    while (token != NULL) {
        tokens.push_back(std::string(token));
        token = std::strtok(NULL, delimeter.c_str());
    }
    delete[] tmp;
}

static inline VM::RegisterType getRegisterFromStr(const std::string& str) {
    if (str[0] != 'x' && str[0] != 'r') {
        throw std::runtime_error("invalid register");
    }

    std::string substr = str.substr(1, str.npos);
    if (!std::all_of(substr.begin(), substr.end(), ::isdigit)) {
        throw std::runtime_error("invalid register");
    }

    return static_cast<VM::RegisterType>(std::stoul(substr));
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
    auto it = VM::intrinsicsNameOpcode.find(str); 
    if (it == VM::intrinsicsNameOpcode.cend())
        throw std::runtime_error("invalid intrinsic");

    return it->second;
}

static inline VM::BasicObjectType getBasicTypeFromStr(const std::string& str) {
    auto it = VM::objectsNameType.find(str);
    if (it == VM::objectsNameType.cend())
        throw std::runtime_error("invalid basic type");

    return it->second;
}

static inline VM::BasicObjectType getOperandBasicType(const std::string& operand) {

    // Try integer
    {
        std::istringstream iss(operand);
        uint64_t numi;
        iss >> std::noskipws >> numi;

        // OK
        if (iss.eof() && !iss.fail()) {
            return VM::BasicObjectType::INTEGER;
        }
    }

    // Try floating point
    {
        std::string tmp = operand;
        if (tmp.back() == 'f')
            tmp.erase(tmp.length() - 1);

        std::istringstream iss(tmp);
        double numd;
        iss >> std::noskipws >> numd;

        // OK
        if (iss.eof() && !iss.fail()) {
            return VM::BasicObjectType::FLOATING;
        }
    }

    // Try string
    if (operand.front() == '\"' && operand.back() == '\"') {
        return VM::BasicObjectType::STRING;
    }

    return VM::BasicObjectType::OBJECT_TYPE_INVALID;
}



bool Parser::removeExtraSpacesAndComments(std::string& line) const {
    line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
    if (line.empty())
        return false;

    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    if (line.empty())
        return false;

    auto spaceIdx = line.find_first_not_of(" \t");
    if (spaceIdx == line.npos)
        return false;

    line = line.substr(spaceIdx);
    if (line.empty())
        return false;

    line = line.substr(0, line.find(';'));
    if (line.empty())
        return false;

    line = line.substr(0, line.find_last_not_of(" \t") + 1);

    return true;
}


bool Parser::parseOpcodeAndOperands(const std::string& origLine, std::vector<std::string>& tokens) const {

    size_t quotIdx = origLine.find_first_of('\"');
    // There is possibly a line
    if (quotIdx != origLine.npos) {
        std::string immediateLine = origLine.substr(quotIdx);
        replaceAllEscapeSeq(immediateLine);
        if (immediateLine.length() < 2 || immediateLine.front() != '\"' || immediateLine.back() != '\"') {
            return false;
        }

        std::string opcodeStr = origLine.substr(0, quotIdx);
        tokenizeString(opcodeStr, " \t", tokens);
        if (tokens.size() != 1) {
            return false;
        }

        tokens.push_back(immediateLine);
        return true;
    }

    tokenizeString(origLine, " ,\t", tokens);
    return true;
}



bool Parser::parseIfSimpleLabel(const std::string& line, std::vector<std::string>& tokens) const {

    tokenizeString(line, " \t", tokens);

    /* Simple labels look like this:
     *
     * labelName:
     *
     */

    if (tokens.size() != 1) {
        return false;
    }
    if (tokens[0].back() != ':') {
        return false;
    }

    // Finally, we know this is a label, remove ':' from name token
    tokens[0].erase(tokens[0].length() - 1);
    return true;
}


bool Parser::parseIfFunctionLabel(const std::string& line, std::vector<std::string>& tokens) const {

    tokenizeString(line, " \t", tokens);

    /* Functions look like this:
     *
     * FUNC functionName: N
     *
     * where N is the number of arguments, it can be omitted if N = 0
     */

    if (tokens.size() < 2 || tokens.size() > 3) {
        return false;
    }
    if (tokens[0] != FUNCTION_KEYWORD) {
        return false;
    }
    if (tokens[1].back() != ':') {
        return false;
    }
    // Has arguments
    if (tokens.size() == 3) {
        if (!std::all_of(tokens[2].begin(), tokens[2].end(), ::isdigit)) {
            return false;
        }
    }

    // Finally, we know this is a function, remove ':' from name token
    tokens[1].erase(tokens[1].length() - 1);
    return true;
}


bool Parser::parseAsmProgram(const std::string& filename, Common::Program& program) {

    m_currFileline = 0;

    std::ifstream file;
    file.open(filename);

    try {
        if (!file.is_open()) {
            throw std::runtime_error("could not open the file");
        }

        std::vector<std::pair<std::string, uint64_t>> instructionLines;
        std::unordered_map<std::string, std::pair<uint64_t, uint8_t>> functions_pc_args;
        std::unordered_map<std::string, uint64_t> labels_pc;
        std::string line;

        std::unordered_map<std::string, uint64_t> stringsPtrCache;

        // Read all file information at first
        while (std::getline(file, line)) {
            ++m_currFileline;

            // Comments, empty lines and spaces before and after all tokens
            if (!removeExtraSpacesAndComments(line)) {
                continue;
            }

            std::vector<std::string> tokens;
            // This is simple label
            if (parseIfSimpleLabel(line, tokens)) {
                labels_pc[tokens[0]] = instructionLines.size();
                continue;
            }

            tokens.clear();
            // This is function label
            if (parseIfFunctionLabel(line, tokens)) {
                uint8_t numArgs = 0;
                if (tokens.size() == 3) {
                    numArgs = static_cast<uint8_t>(std::stoi(tokens[2]));
                }
                functions_pc_args[tokens[1]] = std::pair<uint64_t, uint8_t>(instructionLines.size(), numArgs);
                continue;
            }

            instructionLines.push_back(std::pair<std::string, uint64_t>(line, m_currFileline));
        }
        file.close();

        for (uint64_t i = 0; i < instructionLines.size(); ++i) {
            m_currFileline = instructionLines[i].second;

            std::vector<std::string> tokens;
            if (!parseOpcodeAndOperands(instructionLines[i].first, tokens) || tokens.empty()) {
                throw std::runtime_error("could not parse line: " + instructionLines[i].first);
            }

            auto opcodeIt = VM::instructionsNameOpcode.find(tokens[0]);
            if (opcodeIt == VM::instructionsNameOpcode.cend()) {
                throw std::runtime_error("unknown instruction: " + tokens[0]);
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
                    if (tokens.size() != 2) {
                        throw std::runtime_error("invalid instruction: " + tokens[0]);
                    }
                    decInstr.r1 = getRegisterFromStr(tokens[1]);
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
                case VM::InstructionType::BGT:
                case VM::InstructionType::BLE:
                case VM::InstructionType::BGEF:
                case VM::InstructionType::BLTF:
                case VM::InstructionType::BGTF:
                case VM::InstructionType::BLEF:
                case VM::InstructionType::MVI:
                {
                    if (tokens.size() != 3) {
                        throw std::runtime_error("invalid instruction: " + tokens[0]);
                    }
                    decInstr.r1 = getRegisterFromStr(tokens[1]);
                    decInstr.opcode = opcode;

                    VM::Immediate constant{};
                    constant.type = getOperandBasicType(tokens[2]);
                    switch (constant.type) {

                        case VM::BasicObjectType::INTEGER: {
                            constant.i_val = getInt64FromStr(tokens[2]);
                            break;
                        }
                        case VM::BasicObjectType::FLOATING: {
                            constant.f_val = getDoubleFromStr(tokens[2]);
                            break;
                        }
                        case VM::BasicObjectType::STRING: {
                            throw std::runtime_error("invalid immediate");
                        }
                        default: {
                            // Try labels
                            if (auto it = labels_pc.find(tokens[2]); it != labels_pc.cend()) {
                                constant.type = VM::BasicObjectType::INTEGER;
                                constant.i_val = VM::INSTRUCTION_BYTESIZE * ((int64_t)it->second - i);
                                break;
                            }

                            throw std::runtime_error("unknown basic type for " + tokens[2]);
                        }
                    }
                    if (auto it = std::find(program.constants.begin(), program.constants.end(), constant); it != program.constants.end()) {
                        decInstr.imm = static_cast<VM::ImmediateIndex>(it - program.constants.begin());
                    }
                    else {
                        decInstr.imm = static_cast<VM::ImmediateIndex>(program.constants.size());
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
                    if (tokens.size() != 2) {
                        throw std::runtime_error("invalid instruction: " + tokens[0]);
                    }
                    decInstr.opcode = opcode;

                    VM::Immediate constant{};
                    constant.type = getOperandBasicType(tokens[1]);
                    switch (constant.type) {

                        case VM::BasicObjectType::INTEGER: {
                            constant.i_val = getInt64FromStr(tokens[1]);
                            break;
                        }
                        case VM::BasicObjectType::FLOATING: {
                            constant.f_val = getDoubleFromStr(tokens[1]);
                            break;
                        }
                        case VM::BasicObjectType::STRING: {
                            if (opcode != VM::InstructionType::LOAD_ACCI) {
                                throw std::runtime_error("invalid immediate");
                            }

                            tokens[1] = tokens[1].substr(1, tokens[1].length() - 2);        // Remove double quots

                            if (auto it = stringsPtrCache.find(tokens[1]); it != stringsPtrCache.end()) {
                                constant.i_val = it->second;
                            }
                            else {
                                uint64_t strPtr = program.strings.size();
                                uint32_t strSize = static_cast<uint32_t>(tokens[1].length());
                                stringsPtrCache[tokens[1]] = strPtr;

                                program.strings.resize(strPtr + sizeof(uint32_t) + strSize);
                                std::memcpy(program.strings.data() + strPtr, &strSize, sizeof(uint32_t));                      // Place string size before string itself
                                std::memcpy(program.strings.data() + strPtr + sizeof(uint32_t), tokens[1].data(), strSize);    // And place string itself

                                constant.p_str = strPtr;
                            }
                            break;
                        }
                        default: {
                            // Try labels
                            if (auto it = labels_pc.find(tokens[1]); it != labels_pc.cend()) {
                                constant.type = VM::BasicObjectType::INTEGER;
                                constant.i_val = VM::INSTRUCTION_BYTESIZE * ((int64_t)it->second - i);
                                break;
                            }

                            throw std::runtime_error("unknown basic type for " + tokens[1]);
                        }
                    }
                    if (auto it = std::find(program.constants.begin(), program.constants.end(), constant); it != program.constants.end()) {
                        decInstr.imm = static_cast<VM::ImmediateIndex>(it - program.constants.begin());
                    }
                    else {
                        decInstr.imm = static_cast<VM::ImmediateIndex>(program.constants.size());
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
                    if (tokens.size() != 1) {
                        throw std::runtime_error("invalid instruction: " + tokens[0]);
                    }
                    decInstr.opcode = opcode;
                    break;
                }

                // ================================== Type R ================================== //
                case VM::InstructionType::MV:
                {
                    if (tokens.size() != 3) {
                        throw std::runtime_error("invalid instruction: " + tokens[0]);
                    }
                    decInstr.r1 = getRegisterFromStr(tokens[1]);
                    decInstr.r2 = getRegisterFromStr(tokens[2]);
                    decInstr.opcode = opcode;
                    break;
                }

                // ================================== Type N ================================== //
                case VM::InstructionType::CALL_INTRINSIC:
                {
                    if (tokens.size() != 2) {
                        throw std::runtime_error("invalid instruction: " + tokens[0]);
                    }
                    decInstr.intrCode = getIntrinsicFromStr(tokens[1]);
                    decInstr.opcode = opcode;
                    break;
                }

                // ================================== Type ? ================================== //
                case VM::InstructionType::CALL:
                {
                    if (tokens.size() != 2) {
                        throw std::runtime_error("invalid instruction: " + tokens[0]);
                    }

                    auto funcIt = functions_pc_args.find(tokens[1]);
                    if (funcIt == functions_pc_args.end()) {
                        throw std::runtime_error("invalid function: " + tokens[1]);
                    }
                    decInstr.numArgs = funcIt->second.second;
                    decInstr.opcode = opcode;

                    VM::Immediate constant{};
                    constant.type = VM::BasicObjectType::INTEGER;
                    constant.i_val = VM::INSTRUCTION_BYTESIZE * ((int64_t)funcIt->second.first - i);

                    if (auto it = std::find(program.constants.begin(), program.constants.end(), constant); it != program.constants.end()) {
                        decInstr.imm = static_cast<VM::ImmediateIndex>(it - program.constants.begin());
                    }
                    else {
                        decInstr.imm = static_cast<VM::ImmediateIndex>(program.constants.size());
                        program.constants.push_back(constant);
                    }

                    break;
                }
                case VM::InstructionType::RET:
                {
                    if (tokens.size() != 1) {
                        throw std::runtime_error("invalid instruction: " + tokens[0]);
                    }
                    decInstr.opcode = opcode;
                    break;
                }
                case VM::InstructionType::NEW:
                case VM::InstructionType::NEWARRAY:
                {
                    if (tokens.size() != 2) {
                        throw std::runtime_error("invalid instruction: " + tokens[0]);
                    }
                    decInstr.objType = getBasicTypeFromStr(tokens[1]);
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
        std::cerr << "Errors occured during parsing " << filename << "." << m_currFileline << ": " << e.what() << std::endl;
        if (file.is_open()) {
            file.close();
        }
        return false;
    }
    return true;
}

}   // Common
