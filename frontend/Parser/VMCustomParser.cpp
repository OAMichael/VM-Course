#include "VMCustomParser.h"

namespace Frontend {

using namespace AST;

bool VMParser::parse(const std::vector<Token> &tokens, AST::ProgramNode *rootNode, AST::CodeGenContext *ctx) {
    for (size_t i = 0; i < tokens.size(); ++i) {
        m_tokens.push_back(tokens[i]);
    }
    m_rootNode = rootNode;
    m_codegenCtx = ctx;

    return getProgram();
}

bool VMParser::getProgram() {
    size_t currentToken_1 = m_currentTokenIdx;
    // if (getProgram()) {
    //     size_t currentToken_2 = m_currentTokenIdx;
    //     ASTNode *tmp = getFunctionDeclaration();
    //     if (tmp) {
    //         m_rootNode->insertNode(tmp);
    //         return true;
    //     }

    //     m_currentTokenIdx = currentToken_2;
    //     tmp = getStatement();
    //     if (tmp) {
    //         m_rootNode->insertNode(tmp);
    //         return true;
    //     }
    // }

    m_currentTokenIdx = currentToken_1;

    ASTNode *tmp = getFunctionDeclaration();
    if (tmp) {
        m_rootNode->insertNode(tmp);
        return true;
    }

    m_currentTokenIdx = currentToken_1;

    tmp = getStatement();
    if (tmp) {
        m_rootNode->insertNode(tmp);
        return true;
    }

    m_currentTokenIdx = currentToken_1;
    return true;
}


ASTNode *VMParser::getSimpleIntDeclaration() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::IntType) {
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::Identifier) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    std::string id = m_tokens[m_currentTokenIdx].value;
    ++m_currentTokenIdx;

    return new SimpleVariableDeclarationNode(id, VM::BasicObjectType::INTEGER);
}


ASTNode *VMParser::getInitIntDeclaration() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::IntType) {
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::Identifier) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    std::string id = m_tokens[m_currentTokenIdx].value;
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::Assignment) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *tmp = getExpression();
    if (!tmp) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;
    
    return new InitVariableDeclarationNode(id, VM::BasicObjectType::INTEGER, dynamic_cast<ExpressionNode *>(tmp));
}


ASTNode *VMParser::getArrayIntDeclaration() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::IntType) {
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::Identifier) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    std::string id = m_tokens[m_currentTokenIdx].value;
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::LeftBracket) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::IntLiteral) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    size_t size = atoi(m_tokens[m_currentTokenIdx].value.c_str());
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::RightBracket) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    return new ArrayVariableDeclarationNode(id, VM::BasicObjectType::INTEGER, size);
}


ASTNode *VMParser::getIntVariableDeclaration() {
    ASTNode *tmp = getInitIntDeclaration();
    if (tmp) {
        return tmp;
    }

    tmp = getArrayIntDeclaration();
    if (tmp) {
        return tmp;
    }

    tmp = getSimpleIntDeclaration();
    if (tmp) {
        return tmp;
    }

    return nullptr;
}


ASTNode *VMParser::getSimpleFloatDeclaration() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::FloatType) {
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::Identifier) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    std::string id = m_tokens[m_currentTokenIdx].value;
    ++m_currentTokenIdx;

    return new SimpleVariableDeclarationNode(id, VM::BasicObjectType::FLOATING);
}


ASTNode *VMParser::getInitFloatDeclaration() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::FloatType) {
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::Identifier) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    std::string id = m_tokens[m_currentTokenIdx].value;
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::Assignment) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *tmp = getExpression();
    if (!tmp) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;
    
    return new InitVariableDeclarationNode(id, VM::BasicObjectType::FLOATING, dynamic_cast<ExpressionNode *>(tmp));
}


ASTNode *VMParser::getArrayFloatDeclaration() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::FloatType) {
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::Identifier) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    std::string id = m_tokens[m_currentTokenIdx].value;
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::LeftBracket) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::IntLiteral) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    size_t size = atoi(m_tokens[m_currentTokenIdx].value.c_str());
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::RightBracket) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    return new ArrayVariableDeclarationNode(id, VM::BasicObjectType::FLOATING, size);
}


ASTNode *VMParser::getFloatVariableDeclaration() {
    ASTNode *tmp = getInitFloatDeclaration();
    if (tmp) {
        return tmp;
    }

    tmp = getArrayFloatDeclaration();
    if (tmp) {
        return tmp;
    }

    tmp = getSimpleFloatDeclaration();
    if (tmp) {
        return tmp;
    }

    return nullptr;
}


ASTNode *VMParser::getInitStringDeclaration() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::StringType) {
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::Identifier) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    std::string id = m_tokens[m_currentTokenIdx].value;
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::Assignment) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *tmp = getExpression();
    if (!tmp) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;
    
    return new InitVariableDeclarationNode(id, VM::BasicObjectType::STRING, dynamic_cast<ExpressionNode *>(tmp));
}


ASTNode *VMParser::getStringVariableDeclaration() {
    ASTNode *tmp = getInitStringDeclaration();
    if (tmp) {
        return tmp;
    }

    return nullptr;
}


ASTNode *VMParser::getSimpleVariableDeclaration() {
    ASTNode *tmp = getSimpleIntDeclaration();
    if (tmp) {
        return tmp;
    }

    tmp = getSimpleFloatDeclaration();
    if (tmp) {
        return tmp;
    }

    return nullptr;
}


ASTNode *VMParser::getVariableDeclaration() {
    ASTNode *tmp = getIntVariableDeclaration();
    if (tmp) {
        return tmp;
    }

    tmp = getFloatVariableDeclaration();
    if (tmp) {
        return tmp;
    }

    tmp = getStringVariableDeclaration();
    if (tmp) {
        return tmp;
    }

    return nullptr;
}


ASTNode *VMParser::getVariableValue() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::Identifier) {
        return nullptr;
    }
    std::string id = m_tokens[m_currentTokenIdx].value;
    ++m_currentTokenIdx;
    
    if (m_tokens[m_currentTokenIdx].type != TokenType::LeftBracket) {
        return new VariableValueNode(id);
    }

    ASTNode *tmp = getExpression();
    if (!tmp) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::RightBracket) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    return new VariableValueNode(id, dynamic_cast<ExpressionNode *>(tmp));
}


std::string VMParser::getReturnType() {
    size_t currentToken_1 = m_currentTokenIdx;
    std::string tmp = "";

    if (m_tokens[m_currentTokenIdx].type == TokenType::IntType) {
        tmp = m_tokens[m_currentTokenIdx].value;
        ++m_currentTokenIdx;
        return tmp;
    }

    if (m_tokens[m_currentTokenIdx].type == TokenType::FloatType) {
        tmp = m_tokens[m_currentTokenIdx].value;
        ++m_currentTokenIdx;
        return tmp;
    }

    if (m_tokens[m_currentTokenIdx].type == TokenType::StringType) {
        tmp = m_tokens[m_currentTokenIdx].value;
        ++m_currentTokenIdx;
        return tmp;
    }

    if (m_tokens[m_currentTokenIdx].type == TokenType::VoidType) {
        tmp = m_tokens[m_currentTokenIdx].value;
        ++m_currentTokenIdx;
        return tmp;
    }

    m_currentTokenIdx = currentToken_1;
    return "";
}


bool VMParser::getFunctionArgsDeclarations() {
    size_t currentToken_1 = m_currentTokenIdx;
    
    ASTNode *tmp = getSimpleVariableDeclaration();
    if (tmp) {
        m_codegenCtx->currentFuncArgs.push_back(dynamic_cast<SimpleVariableDeclarationNode *>(tmp));
        return true;
    }

    // if (getFunctionArgsDeclarations()) {
    //     if (m_tokens[m_currentTokenIdx].type != TokenType::Comma) {
    //         m_currentTokenIdx = currentToken_1;
    //         return false;
    //     }
    //     ++m_currentTokenIdx;

    //     ASTNode *tmp = getSimpleVariableDeclaration();
    //     if (!tmp) {
    //         m_currentTokenIdx = currentToken_1;
    //         return false;
    //     }

    //     m_codegenCtx->currentFuncArgs.push_back(dynamic_cast<SimpleVariableDeclarationNode *>(tmp));
    //     return true;
    // }

    return true;
}


bool VMParser::getFunctionArgsExpressions() {
    size_t currentToken_1 = m_currentTokenIdx;

    ASTNode *tmp = getExpression();
    if (tmp) {
        m_codegenCtx->currentCalleeArgs.push_back(dynamic_cast<ExpressionNode *>(tmp));
        return true;
    }

    // if (getFunctionArgsExpressions()) {
    //     if (m_tokens[m_currentTokenIdx].type != TokenType::Comma) {
    //         m_currentTokenIdx = currentToken_1;
    //         return false;
    //     }
    //     ++m_currentTokenIdx;

    //     ASTNode *tmp = getExpression();
    //     if (!tmp) {
    //         m_currentTokenIdx = currentToken_1;
    //         return false;
    //     }

    //     m_codegenCtx->currentCalleeArgs.push_back(dynamic_cast<ExpressionNode *>(tmp));
    //     return true;
    // }

    return true;
}


ASTNode *VMParser::getFunctionDeclaration() {
    size_t currentToken_1 = m_currentTokenIdx;

    std::string returnTypeStr = getReturnType();
    if (returnTypeStr == "") {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    if (m_tokens[m_currentTokenIdx].type != TokenType::FunctionKeyword) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::Identifier) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    std::string id = m_tokens[m_currentTokenIdx].value;
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::LeftParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (!getFunctionArgsDeclarations()) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    if (m_tokens[m_currentTokenIdx].type != TokenType::RightParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *tmp = getStatementsScope();
    if (!tmp) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }


    ReturnType returnType = ReturnType::RETURN_TYPE_INT;
    if (returnTypeStr == "int") {
        returnType = ReturnType::RETURN_TYPE_INT;
    }
    else if (returnTypeStr == "float") {
        returnType = ReturnType::RETURN_TYPE_FLOAT;
    }
    else if (returnTypeStr == "string") {
        returnType = ReturnType::RETURN_TYPE_STRING;
    }
    else if (returnTypeStr == "void") {
        returnType = ReturnType::RETURN_TYPE_VOID;
    }

    ASTNode *node = new FunctionDeclarationNode(returnType, id, m_codegenCtx->currentFuncArgs, dynamic_cast<StatementsScopeNode *>(tmp));
    m_codegenCtx->currentFuncArgs.clear();
    return node;
}


ASTNode *VMParser::getIfStatement() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::IfKeyword) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::LeftParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *expressionNode = getExpression();
    if (!expressionNode) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    if (m_tokens[m_currentTokenIdx].type != TokenType::RightParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *trueScope = getStatementsScope();
    if (!trueScope) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    if (m_tokens[m_currentTokenIdx].type != TokenType::ElseKeyword) {
        return new IfStatementNode(dynamic_cast<ExpressionNode *>(expressionNode),
                                   dynamic_cast<StatementsScopeNode *>(trueScope));
    }
    ++m_currentTokenIdx;

    ASTNode *falseScope = getStatementsScope();
    if (!falseScope) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    return new IfStatementNode(dynamic_cast<ExpressionNode *>(expressionNode),
                               dynamic_cast<StatementsScopeNode *>(trueScope),
                               dynamic_cast<StatementsScopeNode *>(falseScope));
}


ASTNode *VMParser::getWhileLoopStatement() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::WhileKeyword) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::LeftParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *expressionNode = getExpression();
    if (!expressionNode) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    if (m_tokens[m_currentTokenIdx].type != TokenType::RightParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *bodyScope = getStatementsScope();
    if (!bodyScope) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    return new WhileLoopStatementNode(dynamic_cast<ExpressionNode *>(expressionNode),
                                      dynamic_cast<StatementsScopeNode *>(bodyScope));
}


ASTNode *VMParser::getForLoopStatement() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::ForKeyword) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::LeftParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *preLoopStatementNode = getStatement();
    if (!preLoopStatementNode) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    if (m_tokens[m_currentTokenIdx].type != TokenType::Semicolon) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *expressionNode = getExpression();
    if (!expressionNode) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    if (m_tokens[m_currentTokenIdx].type != TokenType::Semicolon) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *postLoopStatementNode = getStatement();
    if (!postLoopStatementNode) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    if (m_tokens[m_currentTokenIdx].type != TokenType::RightParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *bodyScope = getStatementsScope();
    if (!bodyScope) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    return new ForLoopStatementNode(preLoopStatementNode,
                                    dynamic_cast<ExpressionNode *>(expressionNode),
                                    dynamic_cast<StatementsScopeNode *>(bodyScope),
                                    postLoopStatementNode);
}


ASTNode *VMParser::getFunctionCall() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::Identifier) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    std::string id = m_tokens[m_currentTokenIdx].value;
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::LeftParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (!getFunctionArgsExpressions()) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    if (m_tokens[m_currentTokenIdx].type != TokenType::RightParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *node = new FunctionCallNode(id, m_codegenCtx->currentCalleeArgs);
    m_codegenCtx->currentCalleeArgs.clear();
    return node;
}


ASTNode *VMParser::getAssigmentStatement() {
    size_t currentToken_1 = m_currentTokenIdx;

    ASTNode *valueNode = getVariableValue();
    if (!valueNode) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    if (m_tokens[m_currentTokenIdx].type != TokenType::Assignment) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *expressionNode = getExpression();
    if (!expressionNode) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    return new AssignmentStatement(dynamic_cast<VariableValueNode *>(valueNode), dynamic_cast<ExpressionNode *>(expressionNode));
}


ASTNode *VMParser::getReturnStatement() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::ReturnKeyword) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *expressionNode = getExpression();
    if (expressionNode) {
        return new ReturnStatementNode(dynamic_cast<ExpressionNode *>(expressionNode));
    }
    return new ReturnStatementNode();
}


ASTNode *VMParser::getPrintStatement() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::PrintKeyword) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::LeftParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *expressionNode = getExpression();
    if (!expressionNode) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    if (m_tokens[m_currentTokenIdx].type != TokenType::RightParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    return new PrintStatementNode(VM::BasicObjectType::INTEGER, dynamic_cast<ExpressionNode *>(expressionNode));
}


ASTNode *VMParser::getScanStatement() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::ScanKeyword) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::LeftParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::RightParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    return new ScanStatementNode(VM::BasicObjectType::INTEGER);
}


ASTNode *VMParser::getPrintfStatement() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::PrintfKeyword) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::LeftParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *expressionNode = getExpression();
    if (!expressionNode) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    if (m_tokens[m_currentTokenIdx].type != TokenType::RightParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    return new PrintStatementNode(VM::BasicObjectType::FLOATING, dynamic_cast<ExpressionNode *>(expressionNode));
}


ASTNode *VMParser::getScanfStatement() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::ScanfKeyword) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::LeftParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::RightParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    return new ScanStatementNode(VM::BasicObjectType::FLOATING);
}


ASTNode *VMParser::getPrintsStatement() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::PrintsKeyword) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::LeftParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *expressionNode = getExpression();
    if (!expressionNode) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    if (m_tokens[m_currentTokenIdx].type != TokenType::RightParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    return new PrintStatementNode(VM::BasicObjectType::STRING, dynamic_cast<ExpressionNode *>(expressionNode));
}


ASTNode *VMParser::getScansStatement() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::ScansKeyword) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::LeftParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::RightParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    return new ScanStatementNode(VM::BasicObjectType::STRING);
}


ASTNode *VMParser::getSqrtStatement() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::SqrtKeyword) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::LeftParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *expressionNode = getExpression();
    if (!expressionNode) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    if (m_tokens[m_currentTokenIdx].type != TokenType::RightParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    return new SqrtStatementNode(dynamic_cast<ExpressionNode *>(expressionNode));
}


ASTNode *VMParser::getStatement() {
    ASTNode *tmp = getVariableDeclaration();
    if (tmp) {
        return tmp;
    }

    tmp = getAssigmentStatement();
    if (tmp) {
        return tmp;
    }

    tmp = getIfStatement();
    if (tmp) {
        return tmp;
    }

    tmp = getWhileLoopStatement();
    if (tmp) {
        return tmp;
    }

    tmp = getForLoopStatement();
    if (tmp) {
        return tmp;
    }

    tmp = getFunctionCall();
    if (tmp) {
        return tmp;
    }

    tmp = getReturnStatement();
    if (tmp) {
        return tmp;
    }

    tmp = getPrintStatement();
    if (tmp) {
        return tmp;
    }

    tmp = getScanStatement();
    if (tmp) {
        return tmp;
    }

    tmp = getPrintfStatement();
    if (tmp) {
        return tmp;
    }

    tmp = getScanfStatement();
    if (tmp) {
        return tmp;
    }

    tmp = getPrintsStatement();
    if (tmp) {
        return tmp;
    }

    tmp = getScansStatement();
    if (tmp) {
        return tmp;
    }

    return nullptr;
}


bool VMParser::getStatementList() {
    ASTNode *tmp = getStatement();
    if (tmp) {
        if (!getStatementList()) {
            return false;
        }

        m_codegenCtx->scopeStatements.push_back(tmp);
    }

    return true;
}


bool VMParser::getStatementsScopeBegin() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type != TokenType::LeftBrace) {
        m_currentTokenIdx = currentToken_1;
        return false;
    }
    ++m_currentTokenIdx;

    m_codegenCtx->scopeStatementsStack.push(m_codegenCtx->scopeStatements);
    m_codegenCtx->scopeStatements.clear();

    return true;
}


ASTNode *VMParser::getStatementsScope() {
    size_t currentToken_1 = m_currentTokenIdx;

    if (!getStatementsScopeBegin()) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    if (!getStatementList()) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    if (m_tokens[m_currentTokenIdx].type != TokenType::RightBrace) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *node = new StatementsScopeNode(m_codegenCtx->scopeStatements);
    m_codegenCtx->scopeStatements.clear();
    m_codegenCtx->scopeStatements = m_codegenCtx->scopeStatementsStack.top();
    m_codegenCtx->scopeStatementsStack.pop();

    return node;
}


Token VMParser::getHighPriorityOperation() {
    Token tmp;

    if (m_tokens[m_currentTokenIdx].type == TokenType::Mul) {
        tmp = m_tokens[m_currentTokenIdx];
        ++m_currentTokenIdx;
        return tmp;
    }

    if (m_tokens[m_currentTokenIdx].type == TokenType::Div) {
        tmp = m_tokens[m_currentTokenIdx];
        ++m_currentTokenIdx;
        return tmp;
    }

    Token ret;
    ret.type = Unknown;
    return ret;
}


Token VMParser::getLowPriorityOperation() {
    Token tmp;

    if (m_tokens[m_currentTokenIdx].type == TokenType::Add) {
        tmp = m_tokens[m_currentTokenIdx];
        ++m_currentTokenIdx;
        return tmp;
    }

    if (m_tokens[m_currentTokenIdx].type == TokenType::Sub) {
        tmp = m_tokens[m_currentTokenIdx];
        ++m_currentTokenIdx;
        return tmp;
    }

    if (m_tokens[m_currentTokenIdx].type == TokenType::ShiftLeft) {
        tmp = m_tokens[m_currentTokenIdx];
        ++m_currentTokenIdx;
        return tmp;
    }

    if (m_tokens[m_currentTokenIdx].type == TokenType::ShiftRight) {
        tmp = m_tokens[m_currentTokenIdx];
        ++m_currentTokenIdx;
        return tmp;
    }

    if (m_tokens[m_currentTokenIdx].type == TokenType::BitwiseAnd) {
        tmp = m_tokens[m_currentTokenIdx];
        ++m_currentTokenIdx;
        return tmp;
    }

    if (m_tokens[m_currentTokenIdx].type == TokenType::BitwiseOr) {
        tmp = m_tokens[m_currentTokenIdx];
        ++m_currentTokenIdx;
        return tmp;
    }

    if (m_tokens[m_currentTokenIdx].type == TokenType::BitwiseXor) {
        tmp = m_tokens[m_currentTokenIdx];
        ++m_currentTokenIdx;
        return tmp;
    }

    Token ret;
    ret.type = Unknown;
    return ret;
}


Token VMParser::getExpressionOperation() {
    Token tmp;

    if (m_tokens[m_currentTokenIdx].type == TokenType::Less) {
        tmp = m_tokens[m_currentTokenIdx];
        ++m_currentTokenIdx;
        return tmp;
    }

    if (m_tokens[m_currentTokenIdx].type == TokenType::LessOrEq) {
        tmp = m_tokens[m_currentTokenIdx];
        ++m_currentTokenIdx;
        return tmp;
    }

    if (m_tokens[m_currentTokenIdx].type == TokenType::Greater) {
        tmp = m_tokens[m_currentTokenIdx];
        ++m_currentTokenIdx;
        return tmp;
    }

    if (m_tokens[m_currentTokenIdx].type == TokenType::GreaterOrEq) {
        tmp = m_tokens[m_currentTokenIdx];
        ++m_currentTokenIdx;
        return tmp;
    }

    if (m_tokens[m_currentTokenIdx].type == TokenType::Equal) {
        tmp = m_tokens[m_currentTokenIdx];
        ++m_currentTokenIdx;
        return tmp;
    }

    if (m_tokens[m_currentTokenIdx].type == TokenType::NotEqual) {
        tmp = m_tokens[m_currentTokenIdx];
        ++m_currentTokenIdx;
        return tmp;
    }

    if (m_tokens[m_currentTokenIdx].type == TokenType::LogicAnd) {
        tmp = m_tokens[m_currentTokenIdx];
        ++m_currentTokenIdx;
        return tmp;
    }

    if (m_tokens[m_currentTokenIdx].type == TokenType::LogicOr) {
        tmp = m_tokens[m_currentTokenIdx];
        ++m_currentTokenIdx;
        return tmp;
    }

    Token ret;
    ret.type = Unknown;
    return ret;
}


ASTNode *VMParser::getPrimary() {
    ASTNode *tmp = getVariableValue();
    if (tmp) {
        return new PrimaryNode(dynamic_cast<VariableValueNode *>(tmp));
    }

    tmp = getFunctionCall();
    if (tmp) {
        return new PrimaryNode(dynamic_cast<FunctionCallNode *>(tmp));
    }

    tmp = getScanStatement();
    if (tmp) {
        return new PrimaryNode(dynamic_cast<ScanStatementNode *>(tmp));
    }

    tmp = getScanfStatement();
    if (tmp) {
        return new PrimaryNode(dynamic_cast<ScanStatementNode *>(tmp));
    }

    tmp = getScansStatement();
    if (tmp) {
        return new PrimaryNode(dynamic_cast<ScanStatementNode *>(tmp));
    }

    tmp = getSqrtStatement();
    if (tmp) {
        return new PrimaryNode(dynamic_cast<SqrtStatementNode *>(tmp));
    }

    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type == TokenType::IntLiteral) {
        ASTNode *node = new PrimaryNode(atoi(m_tokens[m_currentTokenIdx].value.c_str()));
        ++m_currentTokenIdx;
        return node;
    }

    if (m_tokens[m_currentTokenIdx].type == TokenType::FloatLiteral) {
        ASTNode *node = new PrimaryNode(std::stod(m_tokens[m_currentTokenIdx].value.c_str()));
        ++m_currentTokenIdx;
        return node;
    }

    if (m_tokens[m_currentTokenIdx].type == TokenType::StringLiteral) {
        ASTNode *node = new PrimaryNode(m_tokens[m_currentTokenIdx].value);
        ++m_currentTokenIdx;
        return node;
    }

    return nullptr;
}


ASTNode *VMParser::getFactor() {
    ASTNode *primary = getPrimary();
    if (primary) {
        return new FactorNode(dynamic_cast<PrimaryNode *>(primary));
    }

    size_t currentToken_1 = m_currentTokenIdx;

    if (m_tokens[m_currentTokenIdx].type == TokenType::Sub) {
        ++m_currentTokenIdx;
        primary = getPrimary();
        if (primary) {
            return new FactorNode(dynamic_cast<PrimaryNode *>(primary), true);
        }

        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    if (m_tokens[m_currentTokenIdx].type != TokenType::LeftParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    ASTNode *expressionNode = getExpression();
    if (!expressionNode) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }

    if (m_tokens[m_currentTokenIdx].type != TokenType::RightParent) {
        m_currentTokenIdx = currentToken_1;
        return nullptr;
    }
    ++m_currentTokenIdx;

    return new FactorNode(dynamic_cast<ExpressionNode *>(expressionNode));
}


ASTNode *VMParser::getSummand() {
    ASTNode *factorNode = getFactor();
    if (factorNode) {
        new SummandNode(dynamic_cast<FactorNode *>(factorNode));
    }

    // TODO : resolve left recursion :(((
    return nullptr;

    ASTNode *summandNode = getSummand();
    if (!summandNode) {
        return nullptr;
    }

    Token tokenOperation = getHighPriorityOperation();
    if (tokenOperation.type = TokenType::Unknown) {
        return nullptr;
    }

    factorNode = getFactor();
    if (!factorNode) {
        return nullptr;
    }

    HighPriorityOperation operation = HighPriorityOperation::HIGH_PRIORITY_OPERATION_NONE;
    switch (tokenOperation.type) {
        case TokenType::Mul: {
            operation = HighPriorityOperation::HIGH_PRIORITY_OPERATION_MUL;
            break;
        }
        case TokenType::Div: {
            operation = HighPriorityOperation::HIGH_PRIORITY_OPERATION_DIV;
            break;
        }
        default: {
            operation = HighPriorityOperation::HIGH_PRIORITY_OPERATION_NONE;
            break;
        }
    }

    return new SummandNode(dynamic_cast<SummandNode *>(summandNode), dynamic_cast<FactorNode *>(factorNode), operation);
}


ASTNode *VMParser::getSimple() {
    ASTNode *summandNode = getSummand();
    if (summandNode) {
        new SimpleNode(dynamic_cast<SummandNode *>(summandNode));
    }

    // TODO : resolve left recursion :(((
    return nullptr;

    ASTNode *simpleNode = getSimple();
    if (!simpleNode) {
        return nullptr;
    }

    Token tokenOperation = getLowPriorityOperation();
    if (tokenOperation.type = TokenType::Unknown) {
        return nullptr;
    }

    LowPriorityOperation operation = LowPriorityOperation::LOW_PRIORITY_OPERATION_NONE;
    switch (tokenOperation.type) {
        case TokenType::Add: {
            operation = LowPriorityOperation::LOW_PRIORITY_OPERATION_ADD;
            break;
        }
        case TokenType::Sub: {
            operation = LowPriorityOperation::LOW_PRIORITY_OPERATION_SUB;
            break;
        }
        case TokenType::ShiftLeft: {
            operation = LowPriorityOperation::LOW_PRIORITY_OPERATION_SHIFT_LEFT;
            break;
        }
        case TokenType::ShiftRight: {
            operation = LowPriorityOperation::LOW_PRIORITY_OPERATION_SHIFT_RIGHT;
            break;
        }
        case TokenType::BitwiseAnd: {
            operation = LowPriorityOperation::LOW_PRIORITY_OPERATION_BITWISE_AND;
            break;
        }
        case TokenType::BitwiseOr: {
            operation = LowPriorityOperation::LOW_PRIORITY_OPERATION_BITWISE_OR;
            break;
        }
        case TokenType::BitwiseXor: {
            operation = LowPriorityOperation::LOW_PRIORITY_OPERATION_BITWISE_XOR;
            break;
        }
        default: {
            operation = LowPriorityOperation::LOW_PRIORITY_OPERATION_NONE;
            break;
        }
    }

    return new SimpleNode(dynamic_cast<SimpleNode *>(simpleNode), dynamic_cast<SummandNode *>(summandNode), operation);
}


ASTNode *VMParser::getExpression() {
    ASTNode *simpleNode = getSimple();
    if (simpleNode) {
        return new ExpressionNode(dynamic_cast<SimpleNode *>(simpleNode));
    }

    // TODO : resolve left recursion :(((
    return nullptr;

    ASTNode *expressionNode = getExpression();
    if (!expressionNode) {
        return nullptr;
    }

    Token tokenOperation = getExpressionOperation();
    if (tokenOperation.type = TokenType::Unknown) {
        return nullptr;
    }

    ExpressionOperation operation = ExpressionOperation::EXPRESSION_OPERATION_NONE;
    switch (tokenOperation.type) {
        case TokenType::Less: {
            operation = ExpressionOperation::EXPRESSION_OPERATION_LESS;
            break;
        }
        case TokenType::LessOrEq: {
            operation = ExpressionOperation::EXPRESSION_OPERATION_LESS_OR_EQ;
            break;
        }
        case TokenType::Greater: {
            operation = ExpressionOperation::EXPRESSION_OPERATION_GREATER;
            break;
        }
        case TokenType::GreaterOrEq: {
            operation = ExpressionOperation::EXPRESSION_OPERATION_GREATER_OR_EQ;
            break;
        }
        case TokenType::Equal: {
            operation = ExpressionOperation::EXPRESSION_OPERATION_EQUAL;
            break;
        }
        case TokenType::NotEqual: {
            operation = ExpressionOperation::EXPRESSION_OPERATION_NOT_EQUAL;
            break;
        }
        case TokenType::LogicAnd: {
            operation = ExpressionOperation::EXPRESSION_OPERATION_LOGIC_AND;
            break;
        }
        case TokenType::LogicOr: {
            operation = ExpressionOperation::EXPRESSION_OPERATION_LOGIC_OR;
            break;
        }
        default: {
            operation = ExpressionOperation::EXPRESSION_OPERATION_NONE;
            break;
        }
    }

    return new ExpressionNode(dynamic_cast<ExpressionNode *>(expressionNode), dynamic_cast<SimpleNode *>(simpleNode), operation);
}

}
