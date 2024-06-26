#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>

#include "Instructions.h"


namespace Common {

static constexpr const char programHeaderMessage[] = "bibaboba";
static constexpr uint64_t DEFAULT_ENTRY_POINT = 0;

struct Program {
    uint64_t entryPoint;
    std::vector<VM::EncodedInstruction> instructions;
    std::vector<VM::Immediate> constants;
    std::vector<uint8_t> strings;
    std::vector<uint16_t> classes;
};


struct ProgramHeader {
    uint8_t  p_message[sizeof(programHeaderMessage) - 1];    // because we don't want \0
    uint64_t p_entry;
    uint64_t p_instructionsSize;
    uint64_t p_constantsSize;
    uint64_t p_stringsBytesize;
    uint64_t p_classesSize;
};


[[maybe_unused]] static bool serializeProgram(const std::string& filename, const Program& program) {
    ProgramHeader header;

    memcpy(header.p_message, programHeaderMessage, sizeof(programHeaderMessage));
    header.p_entry = program.entryPoint;
    header.p_instructionsSize = program.instructions.size();
    header.p_constantsSize = program.constants.size();
    header.p_stringsBytesize = program.strings.size();
    header.p_classesSize = program.classes.size();

    std::ofstream outFile;
    outFile.open(filename, std::ios::binary | std::ios::out | std::ios::trunc);

    outFile.write((const char*)&header, sizeof(ProgramHeader));
    outFile.write((const char*)program.instructions.data(), program.instructions.size() * sizeof(VM::EncodedInstruction));
    outFile.write((const char*)program.constants.data(), program.constants.size() * sizeof(VM::Immediate));
    outFile.write((const char*)program.strings.data(), program.strings.size() * sizeof(uint8_t));
    outFile.write((const char*)program.classes.data(), program.classes.size() * sizeof(uint16_t));

    outFile.close();

    return true;
}


[[maybe_unused]] static bool deserializeProgram(const std::string& filename, Program& program) {
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

    std::string readMessage((char*)header.p_message, sizeof(programHeaderMessage) - 1);
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


    const size_t stringsBytesize = header.p_stringsBytesize * sizeof(uint8_t);
    if (remainSize < stringsBytesize) {
        inFile.close();
        return false;
    }

    program.strings.resize(header.p_stringsBytesize);
    inFile.read((char*)program.strings.data(), stringsBytesize);
    remainSize -= stringsBytesize;


    const size_t classesBytesize = header.p_classesSize * sizeof(uint16_t);
    if (remainSize < classesBytesize) {
        inFile.close();
        return false;
    }

    program.classes.resize(header.p_classesSize);
    inFile.read((char*)program.classes.data(), classesBytesize);
    remainSize -= classesBytesize;


    if (remainSize != 0) {
        inFile.close();
        return false;
    }

    inFile.close();
    return true;
}

[[maybe_unused]] static void replaceEscapeSeq(std::string& str, const std::string& search, const std::string& replace) {
    size_t index = 0;
    while (true) {
        index = str.find(search, index);
        if (index == std::string::npos)
            break;

        str.replace(index, search.length(), replace);
        index += replace.length();
    }
}

[[maybe_unused]] static void replaceAllEscapeSeq(std::string& str) {
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

}   // Common


#endif  // COMMON_H
