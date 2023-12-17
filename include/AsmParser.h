#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>

#include "Common.h"
#include "Instructions.h"
#include "Decoder.h"


namespace Common {

class AsmParser {
private:
    VM::Decoder m_coder;
    uint64_t m_currFileline;

    bool removeExtraSpacesAndComments(std::string& line) const;
    bool parseOpcodeAndOperands(const std::string& origLine, std::vector<std::string>& tokens) const;
    bool parseIfSimpleLabel(const std::string& line, std::vector<std::string>& tokens) const;
    bool parseIfFunctionLabel(const std::string& line, std::vector<std::string>& tokens) const;

public:
    bool parseAsmProgram(const std::string& filename, Common::Program& program);
};

}   // Common


#endif  // PARSER_H
