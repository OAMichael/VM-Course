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
};


struct ProgramHeader {
    uint8_t p_message[sizeof(programHeaderMessage) - 1];    // because we don't want \0
    uint64_t p_entry;
};


static bool serializeProgram(const std::string& filename, const Program& program) {
    ProgramHeader header;

    memcpy(header.p_message, programHeaderMessage, sizeof(programHeaderMessage));
    header.p_entry = program.entryPoint;

    std::ofstream outFile;
    outFile.open(filename, std::ios::binary | std::ios::out | std::ios::trunc);
    outFile.write((char*)&header, sizeof(ProgramHeader));
    outFile.write((char*)program.instructions.data(), program.instructions.size() * sizeof(VM::EncodedInstruction));
    outFile.close();

    return true;
}


static bool deserializeProgram(const std::string& filename, Program& program) {
    std::ifstream outFile;
    outFile.open(filename, std::ios::binary | std::ios::in);
    if (!outFile) {
        std::cerr << "File does not exist: " << filename << std::endl;
        return false;
    }

    outFile.seekg(0, std::ios::end);
    size_t remainSize = outFile.tellg();
    outFile.seekg(0, std::ios::beg);

    if (remainSize < sizeof(ProgramHeader)) {
        std::cerr << "File header size does not match: " << filename << std::endl;
        return false;
    }

    ProgramHeader header;
    outFile.read((char*)&header, sizeof(ProgramHeader));

    std::string readMessage((char*)header.p_message);
    if (readMessage != programHeaderMessage) {
        return false;
    }
    program.entryPoint = header.p_entry;

    remainSize -= sizeof(ProgramHeader);

    program.instructions.resize(remainSize / sizeof(VM::EncodedInstruction));
    outFile.read((char*)program.instructions.data(), remainSize);
    outFile.close();

    return true;
}

}   // Common

#endif  // COMMON_H
