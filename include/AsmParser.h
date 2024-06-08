#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>

#include "Common.h"
#include "Instructions.h"
#include "Decoder.h"


namespace Common {

struct ClassDesc {
    std::string name;

    struct ClassField {
        std::string type;
        std::string name;
    };

    std::vector<ClassField> fields;

    ClassDesc() = default;
    ClassDesc(const std::string& n) : name{n} {};

    size_t getByteSize() const {
        // Since every field is 64 bit value or 64 bit reference
        return 8 * fields.size();
    }

    uint16_t getFieldIndex(const std::string& name) const {
        auto it = std::find_if(fields.begin(), fields.end(), [&name](const ClassField& f) { return f.name == name; });
        if (it == fields.end()) {
            throw std::runtime_error("invalid class field");
        }
        return static_cast<uint16_t>(it - fields.begin());
    }

    std::string getFieldType(const std::string& name) const {
        auto it = std::find_if(fields.begin(), fields.end(), [&name](const ClassField& f) { return f.name == name; });
        if (it == fields.end()) {
            throw std::runtime_error("invalid class field");
        }
        return it->type;
    }
};


class AsmParser {
private:
    VM::Decoder m_coder;
    uint64_t m_currFileline;

    bool removeExtraSpacesAndComments(std::string& line) const;
    bool parseOpcodeAndOperands(const std::string& origLine, std::vector<std::string>& tokens) const;
    bool parseIfSimpleLabel(const std::string& line, std::vector<std::string>& tokens) const;
    bool parseIfFunctionLabel(const std::string& line, std::vector<std::string>& tokens) const;
    bool parseIfClassDeclaration(const std::string& line, std::vector<std::string>& tokens) const;

public:
    bool parseAsmProgram(const std::string& filename, Common::Program& program);
};

}   // Common


#endif  // PARSER_H
