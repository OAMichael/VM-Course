#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>

#include "Common.h"
#include "Instructions.h"


namespace Common {

class Parser {
private:

public:
    bool parseAsmProgram(const std::string& filename, std::vector<VM::DecodedInstruction>& decInstructions) const;
};

}   // Common


#endif  // PARSER_H
