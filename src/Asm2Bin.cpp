#include <iostream>
#include <vector>

#include "Common.h"
#include "Parser.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <asm_filename> [<another_filename> ...]\n", argv[0]);
        return 0;
    }

    Common::Parser parser;

    int result = 0;
    for (int i = 1; i < argc; ++i) {
        Common::Program program;

        if (!parser.parseAsmProgram(argv[i], program)) {
            std::cerr << "Could not parse the file" << std::endl;
            result = -1;
            continue;
        }

        std::string outFilename = argv[i];
        size_t lastIndex = outFilename.find_last_of(".");
        if (lastIndex != outFilename.npos)
            outFilename = outFilename.substr(0, lastIndex);

        outFilename += ".prog";

        serializeProgram(outFilename, program);

        std::cout << "Successfully translated the program: " << argv[i] << std::endl;
    }
    return result;
}
