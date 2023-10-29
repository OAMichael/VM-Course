#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>

#include "Instructions.h"


namespace Common {

static constexpr const char programHeaderMessage[] = "bibaboba";

struct Program {
    uint64_t entryPoint;
    std::vector<VM::EncodedInstruction> instructions;
    std::vector<VM::Immediate> constants;
};


struct ProgramHeader {
    uint8_t  p_message[sizeof(programHeaderMessage) - 1];    // because we don't want \0
    uint64_t p_entry;
    uint64_t p_instructionsSize;
    uint64_t p_constantsSize;
};


static bool serializeProgram(const std::string& filename, const Program& program) {
    ProgramHeader header;

    memcpy(header.p_message, programHeaderMessage, sizeof(programHeaderMessage));
    header.p_entry = program.entryPoint;
    header.p_instructionsSize = program.instructions.size();
    header.p_constantsSize = program.constants.size();

    std::ofstream outFile;
    outFile.open(filename, std::ios::binary | std::ios::out | std::ios::trunc);

    outFile.write((char*)&header, sizeof(ProgramHeader));
    outFile.write((char*)program.instructions.data(), program.instructions.size() * sizeof(VM::EncodedInstruction));
    outFile.write((char*)program.constants.data(), program.constants.size() * sizeof(VM::Immediate));

    outFile.close();

    return true;
}


static bool deserializeProgram(const std::string& filename, Program& program) {
    std::ifstream inFile;
    inFile.open(filename, std::ios::binary | std::ios::in);
    if (!inFile) {
        std::cerr << "File does not exist: " << filename << std::endl;
        return false;
    }

    inFile.seekg(0, std::ios::end);
    size_t remainSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    if (remainSize < sizeof(ProgramHeader)) {
        inFile.close();
        std::cerr << "File header size does not match: " << filename << std::endl;
        return false;
    }

    ProgramHeader header;
    inFile.read((char*)&header, sizeof(ProgramHeader));
    remainSize -= sizeof(ProgramHeader);

    std::string readMessage((char*)header.p_message);
    if (readMessage != programHeaderMessage) {
        inFile.close();
        return false;
    }
    program.entryPoint = header.p_entry;

    const size_t instructionsBytesize = header.p_instructionsSize * sizeof(VM::EncodedInstruction);
    if (remainSize < instructionsBytesize) {
        inFile.close();
        return false;
    }

    program.instructions.resize(header.p_instructionsSize);
    inFile.read((char*)program.instructions.data(), instructionsBytesize);
    remainSize -= instructionsBytesize;

    const size_t constantsBytesize = header.p_constantsSize * sizeof(VM::Immediate);
    if (remainSize < constantsBytesize) {
        inFile.close();
        return false;
    }

    program.constants.resize(header.p_constantsSize);
    inFile.read((char*)program.constants.data(), constantsBytesize);
    remainSize -= constantsBytesize;

    if (remainSize != 0) {
        inFile.close();
        return false;
    }

    inFile.close();
    return true;
}

}   // Common

#endif  // COMMON_H
