#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>

#include "Common.h"
#include "Instructions.h"
#include "Decoder.h"


namespace Common {

class Parser {
private:
    VM::Decoder m_coder;

    bool removeExtraSpacesAndComments(std::string& line) const;
    bool parseOpcodeAndOperands(const std::string& origLine, std::string& opcode, std::string (&operands)[2]) const;
    std::pair<std::string, uint8_t> parseFunctionLabel(const std::string& line) const;

public:
    bool parseAsmProgram(const std::string& filename, Common::Program& program) const;
};

}   // Common


#endif  // PARSER_H
