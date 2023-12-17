#ifndef AST_H
#define AST_H

#include <string>
#include <unordered_map>
#include <set>
#include <stack>
#include <array>

#include "Decoder.h"
#include "Common.h"

namespace AST {

enum HighPriorityOperation {
    HIGH_PRIORITY_OPERATION_NONE = 0,
    HIGH_PRIORITY_OPERATION_MUL,
    HIGH_PRIORITY_OPERATION_DIV
};

enum LowPriorityOperation {
    LOW_PRIORITY_OPERATION_NONE = 0,
    LOW_PRIORITY_OPERATION_ADD,
    LOW_PRIORITY_OPERATION_SUB,
    LOW_PRIORITY_OPERATION_SHIFT_LEFT,
    LOW_PRIORITY_OPERATION_SHIFT_RIGHT,
    LOW_PRIORITY_OPERATION_BITWISE_AND,
    LOW_PRIORITY_OPERATION_BITWISE_OR,
    LOW_PRIORITY_OPERATION_BITWISE_XOR
};

enum ExpressionOperation {
    EXPRESSION_OPERATION_NONE = 0,
    EXPRESSION_OPERATION_LESS,
    EXPRESSION_OPERATION_LESS_OR_EQ,
    EXPRESSION_OPERATION_GREATER,
    EXPRESSION_OPERATION_GREATER_OR_EQ,
    EXPRESSION_OPERATION_EQUAL,
    EXPRESSION_OPERATION_NOT_EQUAL,
    EXPRESSION_OPERATION_LOGIC_AND,
    EXPRESSION_OPERATION_LOGIC_OR
};

enum ReturnType {
    RETURN_TYPE_INT,
    RETURN_TYPE_VOID
};


class ASTNode;
class SimpleIntDeclarationNode;
class ExpressionNode;


struct CompilerScope {
    std::set<std::string> scopedIDs;
    CompilerScope *parentScope = nullptr;
};

struct CompilerFrame {
    std::array<bool, VM::FRAME_REGISTER_COUNT> occupiedRegs = {};
    std::unordered_map<std::string, uint32_t> IDs_Regs;
    CompilerScope *currentScope = nullptr;
};

struct GlobalData {
    std::array<bool, VM::FRAME_REGISTER_COUNT> occupiedRegs = {};
    std::unordered_map<std::string, uint32_t> IDs_Regs;
    std::unordered_map<std::string, uint64_t> functions_PC;
    uint64_t pc = 0;
};

struct CodeGenContext {
    VM::Decoder builder;
    Common::Program program;
    CompilerFrame *currentFrame = nullptr;
    GlobalData globalData;

    std::vector<SimpleIntDeclarationNode *> currentFuncArgs;
    std::vector<ExpressionNode *> currentCalleeArgs;
    std::vector<ASTNode *> scopeStatements;
    std::stack<std::vector<ASTNode *>> scopeStatementsStack;

    void encodeInstruction(const VM::DecodedInstruction &decInstr, VM::EncodedInstruction &encInstr) {
        builder.encodeInstruction(decInstr, encInstr);
        globalData.pc += VM::INSTRUCTION_BYTESIZE;
    }

    uint32_t getFreeRegister() {
        if (currentFrame) {
            auto it = std::find(currentFrame->occupiedRegs.begin(), currentFrame->occupiedRegs.end(), false);
            if (it == currentFrame->occupiedRegs.end()) {
                // Out of registers
            }

            return it - currentFrame->occupiedRegs.begin();
        }
        else {
            auto it = std::find(globalData.occupiedRegs.begin(), globalData.occupiedRegs.end(), false);
            if (it == globalData.occupiedRegs.end()) {
                // Out of registers
            }

            return it - globalData.occupiedRegs.begin();
        }
    }

    uint32_t allocateRegister() {
        uint32_t freeReg = getFreeRegister();
        if (currentFrame) {
            currentFrame->occupiedRegs[freeReg] = true;
        }
        else {
            globalData.occupiedRegs[freeReg] = true;
        }
        return freeReg;
    }

    void freeRegsiter(uint32_t reg) {
        if (currentFrame) {
            currentFrame->occupiedRegs[reg] = false;
        }
        else {
            globalData.occupiedRegs[reg] = false;
        }
    }
};


class ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) = 0;

private:

};


// ================================================================================

class SimpleIntDeclarationNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    std::string getName() const;

    SimpleIntDeclarationNode(const std::string &name);

private:
    std::string m_name;
};

// ================================================================================

class InitIntDeclarationNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    std::string getName() const;

    InitIntDeclarationNode(const std::string &name, ExpressionNode *expressionNode);

private:
    std::string m_name;
    ExpressionNode *m_expressionNode = nullptr;
};

// ================================================================================

class ArrayIntDeclarationNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    std::string getName() const;
    size_t getSize() const;

    ArrayIntDeclarationNode(const std::string &name, const size_t size);

private:
    std::string m_name;
    size_t m_size = 0;
};

// ================================================================================

class PrintStatementNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    PrintStatementNode(ExpressionNode *expressionNode);

private:
    std::string m_name;
    ExpressionNode *m_expressionNode = nullptr;
};

// ================================================================================

class ScanStatementNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    ScanStatementNode();

private:

};

// ================================================================================

class ProgramNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    void insertNode(ASTNode *node);

    ProgramNode();

private:
    std::vector<ASTNode *> m_statements_functions;
};

// ================================================================================

class IntValueNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    // Save address into accumulator
    void generateArrayIndex(CodeGenContext *ctx);
    uint32_t getRegister();
    bool isArray();

    IntValueNode(const std::string &name);
    IntValueNode(const std::string &name, ExpressionNode *expressionNode);

private:
    uint32_t m_registerToStore;
    std::string m_name;
    ExpressionNode *m_expressionNode = nullptr;
};

// ================================================================================

class FunctionCallNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;

    FunctionCallNode(const std::string &name, std::vector<ExpressionNode *> &arguments);

private:
    std::vector<ExpressionNode *> m_arguments;
    std::string m_name;
};

// ================================================================================

class PrimaryNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    int getValue();
    uint32_t getRegister();

    PrimaryNode(const int value);
    PrimaryNode(IntValueNode *intValueNode);
    PrimaryNode(FunctionCallNode *functionCallNode);
    PrimaryNode(ScanStatementNode *scanStatementNode);

private:
    int m_value;
    uint32_t m_registerToStore;
    IntValueNode *m_intValueNode = nullptr;
    FunctionCallNode *m_functionCallNode = nullptr;
    ScanStatementNode *m_scanStatementNode = nullptr;
};

// ================================================================================

class FactorNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    uint32_t getRegister();

    FactorNode(PrimaryNode *primaryNode, bool primaryNot = false);
    FactorNode(ExpressionNode *expressionNode);

private:
    uint32_t m_registerToStore;
    bool m_primaryNot = false;
    PrimaryNode *m_primaryNode = nullptr;
    ExpressionNode *m_expressionNode = nullptr;
};

// ================================================================================

class SummandNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    uint32_t getRegister();

    SummandNode(FactorNode *factorNode);
    SummandNode(SummandNode *summandNode, FactorNode *factorNode, HighPriorityOperation operation);

private:
    uint32_t m_registerToStore;
    FactorNode *m_factorNode = nullptr;
    SummandNode *m_summandNode = nullptr;
    HighPriorityOperation m_operation = HighPriorityOperation::HIGH_PRIORITY_OPERATION_NONE;
};

// ================================================================================

class SimpleNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    uint32_t getRegister();

    SimpleNode(SummandNode *summandNode);
    SimpleNode(SimpleNode *simpleNode, SummandNode *summandNode, LowPriorityOperation operation);

private:
    uint32_t m_registerToStore;
    SummandNode *m_summandNode = nullptr;
    SimpleNode *m_simpleNode = nullptr;
    LowPriorityOperation m_operation = LowPriorityOperation::LOW_PRIORITY_OPERATION_NONE;
};

// ================================================================================

class ExpressionNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    uint32_t getRegister();
    ExpressionOperation getOperation();
    uint32_t getSimpleNodeRegister();

    ExpressionNode(SimpleNode *simpleNode);
    ExpressionNode(ExpressionNode *expressionNode, SimpleNode *simpleNode, ExpressionOperation operation);

private:
    uint32_t m_registerToStore;
    SimpleNode *m_simpleNode = nullptr;
    ExpressionNode *m_expressionNode = nullptr;
    ExpressionOperation m_opeation = ExpressionOperation::EXPRESSION_OPERATION_NONE;
};

// ================================================================================

class AssignmentStatement : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;

    AssignmentStatement(IntValueNode *intValueNode, ExpressionNode *expressionNode);

private:
    IntValueNode *m_intValueNode = nullptr;
    ExpressionNode *m_expressionNode = nullptr;
};

// ================================================================================

class ReturnStatementNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;

    ReturnStatementNode(ExpressionNode *expressionNode = nullptr);

private:
    ExpressionNode *m_expressionNode = nullptr;
};

// ================================================================================

class StatementsScopeNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;

    StatementsScopeNode(std::vector<ASTNode *> &scopeStatements);

private:
    std::vector<ASTNode *> m_scopeStatements;
};

// ================================================================================

class FunctionDeclarationNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;

    FunctionDeclarationNode(const std::string &name,
                            std::vector<SimpleIntDeclarationNode *> &arguments,
                            StatementsScopeNode *body);

private:
    std::vector<SimpleIntDeclarationNode *> m_arguments;
    StatementsScopeNode *m_body = nullptr;
    std::string m_name;
};

// ================================================================================

class IfStatementNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;

    IfStatementNode(ExpressionNode *expressionNode, StatementsScopeNode *trueBody, StatementsScopeNode *falseBody = nullptr);

private:
    ExpressionNode *m_expressionNode = nullptr;
    StatementsScopeNode *m_trueBody = nullptr;
    StatementsScopeNode *m_falseBody = nullptr;
};

// ================================================================================

class WhileLoopStatementNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;

    WhileLoopStatementNode(ExpressionNode *expressionNode, StatementsScopeNode *body);

private:
    ExpressionNode *m_expressionNode = nullptr;
    StatementsScopeNode *m_body = nullptr;
};

// ================================================================================

class ForLoopStatementNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;

    ForLoopStatementNode(ASTNode *preLoopStatement,
                         ExpressionNode *expressionNode,
                         StatementsScopeNode *body,
                         ASTNode *postLoopStatement);

private:
    ASTNode *m_preLoopStatement = nullptr;
    ExpressionNode *m_expressionNode = nullptr;
    StatementsScopeNode *m_body = nullptr;
    ASTNode *m_postLoopStatement = nullptr;
};

}

#endif  // AST_H
