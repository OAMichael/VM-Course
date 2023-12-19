#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include "VMCustomLexer.h"
#include "VMCustomParser.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 0;
    }

    std::ifstream inFile(argv[1]);
    std::stringstream buffer;
    buffer << inFile.rdbuf();


    Frontend::VMLexer lexer;
    Frontend::VMParser parser;

    AST::ProgramNode *rootNode = new AST::ProgramNode();
    AST::CodeGenContext codegenCtx;
    std::vector<Frontend::Token> tokens;


    if (!lexer.lex(buffer.str(), tokens)) {
        std::cerr << "Error occured while lexing" << std::endl;
        return 1;
    }

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i].type > -1) {
            std::cout << Frontend::tokenNames[tokens[i].type] << " " << tokens[i].value << std::endl;
        }
    }

    if (!parser.parse(tokens, rootNode, &codegenCtx)) {
        std::cerr << "Error occured while parsing" << std::endl;
        return 2;
    }
    
    rootNode->generateCode(&codegenCtx);

    delete rootNode;

    std::cout << "\n\nGenerated program:" << std::endl;
    for (auto instr : codegenCtx.program.instructions) {
        VM::DecodedInstruction decInstr;
        codegenCtx.builder.decodeInstruction(instr, decInstr);
        if (auto it = VM::instructionsOpcodeName.find(decInstr.opcode); it != VM::instructionsOpcodeName.cend()) {
            std::cout << it->second << " " << (int)decInstr.r1 << " " << (int)decInstr.r2 << std::endl;
        }
    }

    std::string outFilename = argv[1];
    size_t lastIndex = outFilename.find_last_of(".");
    if (lastIndex != outFilename.npos)
        outFilename = outFilename.substr(0, lastIndex);

    outFilename += ".prog";

    Common::serializeProgram(outFilename, codegenCtx.program);

    std::cout << "Successfully translated the program: " << argv[1] << std::endl;
    return 0;
}