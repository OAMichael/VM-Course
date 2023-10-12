#include <iostream>
#include <vector>

#include "Common.h"
#include "Parser.h"
#include "Decoder.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <asm_filename> [-o <out_filename>]\n", argv[0]);
        return 0;
    }

    Common::Parser parser;
    VM::Decoder coder;
    Common::Program program;
    std::vector<VM::DecodedInstruction> decInstructions; 

    try {
        parser.parseAsmProgram(argv[1], decInstructions);
    }
    catch(const std::runtime_error& e) {
        std::cerr << "Errors occured during parsing " << argv[1] << ": " << e.what() << std::endl;
        return -1;
    }

    for (size_t i = 0; i < decInstructions.size(); ++i) {
        VM::EncodedInstruction encInstr;
        coder.encodeInstruction(decInstructions[i], encInstr);
        program.instructions.push_back(encInstr);
    }

    program.entryPoint = 0;

    std::string outFilename = argv[1];
    size_t lastIndex = outFilename.find_last_of(".");
    if (lastIndex != outFilename.npos)
        outFilename = outFilename.substr(0, lastIndex);

    outFilename += ".prog";

    if (argc >= 4)
        outFilename = argv[3];

    serializeProgram(outFilename, program);
    return 0;
}
