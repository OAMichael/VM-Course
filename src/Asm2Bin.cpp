#include <iostream>
#include <vector>

#include "Common.h"
#include "Parser.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <asm_filename> [-o <out_filename>]\n", argv[0]);
        return 0;
    }

    Common::Parser parser;
    Common::Program program;

    if (!parser.parseAsmProgram(argv[1], program)) {
        std::cerr << "Could not parse the file" << std::endl;
        return -1;
    }

    std::string outFilename = argv[1];
    size_t lastIndex = outFilename.find_last_of(".");
    if (lastIndex != outFilename.npos)
        outFilename = outFilename.substr(0, lastIndex);

    outFilename += ".prog";

    if (argc >= 4)
        outFilename = argv[3];

    serializeProgram(outFilename, program);

    std::cout << "Successfully translated the program: " << argv[1] << std::endl;
    return 0;
}
