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
    RETURN_TYPE_FLOAT,
    RETURN_TYPE_STRING,
    RETURN_TYPE_VOID
};


class ASTNode;
class SimpleVariableDeclarationNode;
class ExpressionNode;


struct CompilerScope {
    std::set<std::string> scopedIDs;
    CompilerScope *parentScope = nullptr;
};

struct CompilerFrame {
    std::array<bool, VM::FRAME_REGISTER_COUNT> occupiedRegs = {};
    std::unordered_map<std::string, uint32_t> IDs_Regs;
    std::unordered_map<std::string, VM::BasicObjectType> identifierTypes;
    CompilerScope *currentScope = nullptr;
};

struct GlobalData {
    std::array<bool, VM::FRAME_REGISTER_COUNT> occupiedRegs = {};
    std::unordered_map<std::string, uint32_t> IDs_Regs;
    std::unordered_map<std::string, VM::BasicObjectType> identifierTypes;
    std::unordered_map<std::string, ReturnType> functionReturnTypes;
    std::unordered_map<std::string, uint64_t> functions_PC;
    uint64_t pc = 0;
};

struct CodeGenContext {
    VM::Decoder builder;
    Common::Program program;
    CompilerFrame *currentFrame = nullptr;
    GlobalData globalData;

    std::vector<SimpleVariableDeclarationNode *> currentFuncArgs;
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

    VM::ImmediateIndex insertStringIntoPool(const std::string &str) {
        static std::unordered_map<std::string, uint64_t> stringsPtrCache;

        VM::Immediate constant{};
        constant.type = VM::BasicObjectType::STRING;

        if (auto it = stringsPtrCache.find(str); it != stringsPtrCache.end()) {
            constant.i_val = it->second;
        }
        else {
            uint64_t strPtr = program.strings.size();
            uint32_t strSize = static_cast<uint32_t>(str.length());
            stringsPtrCache[str] = strPtr;

            program.strings.resize(strPtr + sizeof(uint32_t) + strSize);
            std::memcpy(program.strings.data() + strPtr, &strSize, sizeof(uint32_t));
            std::memcpy(program.strings.data() + strPtr + sizeof(uint32_t), str.data(), strSize);

            constant.p_str = strPtr;
        }

        VM::ImmediateIndex strIdx;
        if (auto it = std::find(program.constants.begin(), program.constants.end(), constant); it != program.constants.end()) {
            strIdx = static_cast<VM::ImmediateIndex>(it - program.constants.begin());
        }
        else {
            strIdx = static_cast<VM::ImmediateIndex>(program.constants.size());
            program.constants.push_back(constant);
        }

        return strIdx;
    }
};


class ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) = 0;
    virtual void print(size_t printLevel) = 0;

private:

};


// ================================================================================

class SimpleVariableDeclarationNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    virtual void print(size_t printLevel) override;
    std::string getName() const;

    SimpleVariableDeclarationNode(const std::string &name, VM::BasicObjectType objType);

private:
    std::string m_name;
    VM::BasicObjectType m_objType;
};

// ================================================================================

class InitVariableDeclarationNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    virtual void print(size_t printLevel) override;
    std::string getName() const;

    InitVariableDeclarationNode(const std::string &name, VM::BasicObjectType objType, ExpressionNode *expressionNode);
    ~InitVariableDeclarationNode();

private:
    std::string m_name;
    VM::BasicObjectType m_objType;
    ExpressionNode *m_expressionNode = nullptr;
};

// ================================================================================

class ArrayVariableDeclarationNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    virtual void print(size_t printLevel) override;
    std::string getName() const;
    size_t getSize() const;

    ArrayVariableDeclarationNode(const std::string &name, VM::BasicObjectType objType, const size_t size);

private:
    std::string m_name;
    VM::BasicObjectType m_objType;
    size_t m_size = 0;
};

// ================================================================================

class PrintStatementNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    virtual void print(size_t printLevel) override;

    PrintStatementNode(VM::BasicObjectType objType, ExpressionNode *expressionNode);
    ~PrintStatementNode();

private:
    std::string m_name;
    VM::BasicObjectType m_objType;
    ExpressionNode *m_expressionNode = nullptr;
};

// ================================================================================

class ScanStatementNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    virtual void print(size_t printLevel) override;

    VM::BasicObjectType getType();

    ScanStatementNode(VM::BasicObjectType objType);

private:
    VM::BasicObjectType m_objType;
};

// ================================================================================

class SqrtStatementNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    virtual void print(size_t printLevel) override;

    SqrtStatementNode(ExpressionNode *expressionNode);
    ~SqrtStatementNode();

private:
    ExpressionNode *m_expressionNode;
};

// ================================================================================

class ProgramNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    virtual void print(size_t printLevel) override;

    void insertNode(ASTNode *node);

    ProgramNode();
    ~ProgramNode();

private:
    std::vector<ASTNode *> m_statements_functions;
};

// ================================================================================

class VariableValueNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    virtual void print(size_t printLevel) override;
    // Save address into accumulator
    void generateArrayIndex(CodeGenContext *ctx);
    uint32_t getRegister();
    bool isArray();
    VM::BasicObjectType getType(CodeGenContext *ctx);

    VariableValueNode(const std::string &name);
    VariableValueNode(const std::string &name, ExpressionNode *expressionNode);
    ~VariableValueNode();

private:
    uint32_t m_registerToStore;
    std::string m_name;
    ExpressionNode *m_expressionNode = nullptr;
};

// ================================================================================

class FunctionCallNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    virtual void print(size_t printLevel) override;
    VM::BasicObjectType getType(CodeGenContext *ctx);

    FunctionCallNode(const std::string &name, std::vector<ExpressionNode *> &arguments);
    ~FunctionCallNode();

private:
    std::vector<ExpressionNode *> m_arguments;
    std::string m_name;
};

// ================================================================================

class PrimaryNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    virtual void print(size_t printLevel) override;
    int getIntValue();
    double getFloatValue();
    std::string getStringValue();
    uint32_t getRegister();
    VM::BasicObjectType getType();

    PrimaryNode(const int value);
    PrimaryNode(const double value);
    PrimaryNode(const std::string &value);
    PrimaryNode(VariableValueNode *valueNode);
    PrimaryNode(FunctionCallNode *functionCallNode);
    PrimaryNode(ScanStatementNode *scanStatementNode);
    PrimaryNode(SqrtStatementNode *sqrtStatementNode);
    ~PrimaryNode();

private:
    int m_intValue;
    double m_floatValue;
    std::string m_stringValue;
    VM::BasicObjectType m_objType;

    uint32_t m_registerToStore;
    VariableValueNode *m_valueNode = nullptr;
    FunctionCallNode *m_functionCallNode = nullptr;
    ScanStatementNode *m_scanStatementNode = nullptr;
    SqrtStatementNode *m_sqrtStatementNode = nullptr;
};

// ================================================================================

class FactorNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    virtual void print(size_t printLevel) override;
    uint32_t getRegister();
    VM::BasicObjectType getType();

    FactorNode(PrimaryNode *primaryNode, bool primaryNot = false);
    FactorNode(ExpressionNode *expressionNode);
    ~FactorNode();

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
    virtual void print(size_t printLevel) override;
    uint32_t getRegister();
    VM::BasicObjectType getType();

    SummandNode(FactorNode *factorNode);
    SummandNode(FactorNode *factorNode, SummandNode *summandNode, HighPriorityOperation operation);
    ~SummandNode();

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
    virtual void print(size_t printLevel) override;
    uint32_t getRegister();
    VM::BasicObjectType getType();

    SimpleNode(SummandNode *summandNode);
    SimpleNode(SummandNode *summandNode, SimpleNode *simpleNode, LowPriorityOperation operation);
    ~SimpleNode();

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
    virtual void print(size_t printLevel) override;
    uint32_t getRegister();
    ExpressionOperation getOperation();
    uint32_t getSimpleNodeRegister();
    uint32_t getExpressionNodeRegister();

    VM::BasicObjectType getType();

    ExpressionNode(SimpleNode *simpleNode);
    ExpressionNode(SimpleNode *simpleNode, ExpressionNode *expressionNode, ExpressionOperation operation);
    ~ExpressionNode();

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
    virtual void print(size_t printLevel) override;

    AssignmentStatement(VariableValueNode *valueNode, ExpressionNode *expressionNode);
    ~AssignmentStatement();

private:
    VariableValueNode *m_valueNode = nullptr;
    ExpressionNode *m_expressionNode = nullptr;
};

// ================================================================================

class ReturnStatementNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    virtual void print(size_t printLevel) override;

    ReturnStatementNode(ExpressionNode *expressionNode = nullptr);
    ~ReturnStatementNode();

private:
    ExpressionNode *m_expressionNode = nullptr;
};

// ================================================================================

class StatementsScopeNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    virtual void print(size_t printLevel) override;

    StatementsScopeNode(std::vector<ASTNode *> &scopeStatements);
    ~StatementsScopeNode();

private:
    std::vector<ASTNode *> m_scopeStatements;
};

// ================================================================================

class FunctionDeclarationNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    virtual void print(size_t printLevel) override;

    FunctionDeclarationNode(ReturnType returnType,
                            const std::string &name,
                            std::vector<SimpleVariableDeclarationNode *> &arguments,
                            StatementsScopeNode *body);

    ~FunctionDeclarationNode();

private:
    ReturnType m_returnType = ReturnType::RETURN_TYPE_INT;
    std::vector<SimpleVariableDeclarationNode *> m_arguments;
    StatementsScopeNode *m_body = nullptr;
    std::string m_name;
};

// ================================================================================

class IfStatementNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    virtual void print(size_t printLevel) override;

    IfStatementNode(ExpressionNode *expressionNode, StatementsScopeNode *trueBody, StatementsScopeNode *falseBody = nullptr);
    ~IfStatementNode();

private:
    ExpressionNode *m_expressionNode = nullptr;
    StatementsScopeNode *m_trueBody = nullptr;
    StatementsScopeNode *m_falseBody = nullptr;
};

// ================================================================================

class WhileLoopStatementNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    virtual void print(size_t printLevel) override;

    WhileLoopStatementNode(ExpressionNode *expressionNode, StatementsScopeNode *body);
    ~WhileLoopStatementNode();

private:
    ExpressionNode *m_expressionNode = nullptr;
    StatementsScopeNode *m_body = nullptr;
};

// ================================================================================

class ForLoopStatementNode : public ASTNode {
public:
    virtual void generateCode(CodeGenContext *ctx) override;
    virtual void print(size_t printLevel) override;

    ForLoopStatementNode(ASTNode *preLoopStatement,
                         ExpressionNode *expressionNode,
                         StatementsScopeNode *body,
                         ASTNode *postLoopStatement);
    
    ~ForLoopStatementNode();

private:
    ASTNode *m_preLoopStatement = nullptr;
    ExpressionNode *m_expressionNode = nullptr;
    StatementsScopeNode *m_body = nullptr;
    ASTNode *m_postLoopStatement = nullptr;
};

}

#endif  // AST_H
