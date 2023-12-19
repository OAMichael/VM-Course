#ifndef VM_CUSTOM_PARSER_H
#define VM_CUSTOM_PARSER_H

#include <vector>

#include "VMCustomLexer.h"
#include "AST.h"


namespace Frontend {

class VMParser {
public:
    bool parse(const std::vector<Token> &tokens, AST::ProgramNode *rootNode, AST::CodeGenContext *ctx);

private:
    bool getProgram();

    AST::ASTNode *getSimpleIntDeclaration();
    AST::ASTNode *getInitIntDeclaration();
    AST::ASTNode *getArrayIntDeclaration();
    AST::ASTNode *getIntVariableDeclaration();

    AST::ASTNode *getSimpleFloatDeclaration();
    AST::ASTNode *getInitFloatDeclaration();
    AST::ASTNode *getArrayFloatDeclaration();
    AST::ASTNode *getFloatVariableDeclaration();

    AST::ASTNode *getInitStringDeclaration();
    AST::ASTNode *getStringVariableDeclaration();

    AST::ASTNode *getSimpleVariableDeclaration();
    AST::ASTNode *getVariableDeclaration();
    AST::ASTNode *getVariableValue();

    std::string getReturnType();

    bool getFunctionArgsDeclarations();
    bool getFunctionArgsExpressions();

    AST::ASTNode *getFunctionDeclaration();

    AST::ASTNode *getIfStatement();
    AST::ASTNode *getWhileLoopStatement();
    AST::ASTNode *getForLoopStatement();
    AST::ASTNode *getFunctionCall();
    AST::ASTNode *getAssigmentStatement();
    AST::ASTNode *getReturnStatement();
    AST::ASTNode *getPrintStatement();
    AST::ASTNode *getScanStatement();
    AST::ASTNode *getPrintfStatement();
    AST::ASTNode *getScanfStatement();
    AST::ASTNode *getPrintsStatement();
    AST::ASTNode *getScansStatement();
    AST::ASTNode *getSqrtStatement();

    AST::ASTNode *getStatement();
    bool getStatementList();
    bool getStatementsScopeBegin();
    AST::ASTNode *getStatementsScope();

    Token getHighPriorityOperation();
    Token getLowPriorityOperation();
    Token getExpressionOperation();

    AST::ASTNode *getPrimary();
    AST::ASTNode *getFactor();
    AST::ASTNode *getSummand();
    AST::ASTNode *getSimple();
    AST::ASTNode *getExpression();

    AST::ProgramNode *m_rootNode = nullptr;
    AST::CodeGenContext *m_codegenCtx = nullptr;
    std::vector<Token> m_tokens;
    size_t m_currentTokenIdx = 0;
};

}

#endif  // VM_CUSTOM_PARSER_H
