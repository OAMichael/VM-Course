#include "AST.h"


namespace AST {

// ================================================================================

void SimpleIntDeclarationNode::generateCode(CodeGenContext *ctx) {
    auto *currentFrame = ctx->currentFrame;

    uint32_t freeReg = ctx->allocateRegister();
    if (currentFrame) {
        // Inside a function
        auto *currentScope = currentFrame->currentScope;
        currentScope->scopedIDs.insert(m_name);
        currentFrame->IDs_Regs.insert({m_name, freeReg});
    }
    else {
        // Global scope of data
        auto &globalData = ctx->globalData;
        globalData.IDs_Regs.insert({m_name, freeReg});
    }
}

std::string SimpleIntDeclarationNode::getName() const {
    return m_name;
}

SimpleIntDeclarationNode::SimpleIntDeclarationNode(const std::string &name) {
    m_name = name;
}

// ================================================================================

void InitIntDeclarationNode::generateCode(CodeGenContext *ctx) {
    auto *currentFrame = ctx->currentFrame;
    auto &program = ctx->program;

    uint32_t freeReg = ctx->allocateRegister();
    if (currentFrame) {
        // Inside a function
        auto *currentScope = currentFrame->currentScope;
        currentScope->scopedIDs.insert(m_name);
        currentFrame->IDs_Regs.insert({m_name, freeReg});
    }
    else {
        // Global scope of data
        auto &globalData = ctx->globalData;
        globalData.IDs_Regs.insert({m_name, freeReg});
    }

    m_expressionNode->generateCode(ctx);
    
    VM::DecodedInstruction decInstr;
    VM::EncodedInstruction encInstr;

    decInstr.r1 = freeReg;
    decInstr.r2 = m_expressionNode->getRegister();
    decInstr.opcode = VM::InstructionType::MV;

    ctx->encodeInstruction(decInstr, encInstr);
    program.instructions.push_back(encInstr);
}

std::string InitIntDeclarationNode::getName() const {
    return m_name;
}

InitIntDeclarationNode::InitIntDeclarationNode(const std::string &name, ExpressionNode *expressionNode) {
    m_name = name;
    m_expressionNode = expressionNode;
}

// ================================================================================

void ArrayIntDeclarationNode::generateCode(CodeGenContext *ctx) {
    auto *currentFrame = ctx->currentFrame;
    auto &program = ctx->program;

    uint32_t freeReg = ctx->allocateRegister();
    if (currentFrame) {
        // Inside a function
        auto *currentScope = currentFrame->currentScope;
        currentScope->scopedIDs.insert(m_name);
        currentFrame->IDs_Regs.insert({m_name, freeReg});
    }
    else {
        // Global scope of data
        auto &globalData = ctx->globalData;
        globalData.IDs_Regs.insert({m_name, freeReg});
    }

    // Load size of array into accumulator first
    {
        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;

        decInstr.opcode = VM::InstructionType::LOAD_ACCI;

        VM::Immediate constant{};
        constant.type = VM::BasicObjectType::INTEGER;
        constant.i_val = m_size;

        if (auto it = std::find(program.constants.begin(), program.constants.end(), constant); it != program.constants.end()) {
            decInstr.imm = static_cast<VM::ImmediateIndex>(it - program.constants.begin());
        }
        else {
            decInstr.imm = static_cast<VM::ImmediateIndex>(program.constants.size());
            program.constants.push_back(constant);
        }        

        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }
    
    {        
        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;

        decInstr.objType = VM::BasicObjectType::INTEGER;
        decInstr.opcode = VM::InstructionType::NEWARRAY;

        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }

    {        
        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;

        decInstr.r1 = freeReg;
        decInstr.opcode = VM::InstructionType::STORE_ACC;

        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }
}

std::string ArrayIntDeclarationNode::getName() const {
    return m_name;
}

size_t ArrayIntDeclarationNode::getSize() const {
    return m_size;
}

ArrayIntDeclarationNode::ArrayIntDeclarationNode(const std::string &name, const size_t size) {
    m_name = name;
    m_size = size;
}

// ================================================================================

void PrintStatementNode::generateCode(CodeGenContext *ctx) {
    auto &program = ctx->program;

    m_expressionNode->generateCode(ctx);
    {
        VM::DecodedInstruction decInstr;
        decInstr.opcode = VM::InstructionType::LOAD_ACC;
        decInstr.r1 = m_expressionNode->getRegister();

        VM::EncodedInstruction encInstr;
        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }

    {
        VM::DecodedInstruction decInstr;
        decInstr.opcode = VM::InstructionType::CALL_INTRINSIC;
        decInstr.intrCode = VM::IntrinsicType::PRINT;

        VM::EncodedInstruction encInstr;
        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }
}

PrintStatementNode::PrintStatementNode(ExpressionNode *expressionNode) {
    m_expressionNode = expressionNode;
}

// ================================================================================

void ScanStatementNode::generateCode(CodeGenContext *ctx) {
    auto &program = ctx->program;
    {
        VM::DecodedInstruction decInstr;
        decInstr.opcode = VM::InstructionType::CALL_INTRINSIC;
        decInstr.intrCode = VM::IntrinsicType::SCAN;

        VM::EncodedInstruction encInstr;
        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }
}

ScanStatementNode::ScanStatementNode() {

}

// ================================================================================

void ProgramNode::generateCode(CodeGenContext *ctx) {
    for (auto child : m_statements_functions) {
        child->generateCode(ctx);
    }
}

void ProgramNode::insertNode(ASTNode *node) {
    m_statements_functions.push_back(node);
}

ProgramNode::ProgramNode() {

}

// ================================================================================

void IntValueNode::generateCode(CodeGenContext *ctx) {
    auto *currentFrame = ctx->currentFrame;
    auto &globalData = ctx->globalData;
    auto &program = ctx->program;

    if (m_expressionNode) {
        // Array
        this->generateArrayIndex(ctx);
        uint32_t freeReg = ctx->allocateRegister();
        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = freeReg;
            decInstr.opcode = VM::InstructionType::STORE_ACC;  

            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }

        // Now load value from memory
        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = freeReg;
            decInstr.opcode = VM::InstructionType::LOAD_ACC_MEM;

            VM::Immediate constant{};
            constant.type = VM::BasicObjectType::INTEGER;
            constant.i_val = 0;

            if (auto it = std::find(program.constants.begin(), program.constants.end(), constant); it != program.constants.end()) {
                decInstr.imm = static_cast<VM::ImmediateIndex>(it - program.constants.begin());
            }
            else {
                decInstr.imm = static_cast<VM::ImmediateIndex>(program.constants.size());
                program.constants.push_back(constant);
            }        

            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }
        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = freeReg;
            decInstr.opcode = VM::InstructionType::STORE_ACC;  

            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }

        m_registerToStore = freeReg;
    }
    else {
        // Single
        if (currentFrame) {
            // Inside a function
            auto localIt = currentFrame->IDs_Regs.find(m_name);
            if (localIt == currentFrame->IDs_Regs.end()) {
                // Error, undeclared identifier
            }
            m_registerToStore = localIt->second;
        }
        else {
            // Global scope of data
            auto &globalData = ctx->globalData;
            auto globalIt = globalData.IDs_Regs.find(m_name);
            if (globalIt == globalData.IDs_Regs.end()) {
                // Error, undeclared identifier
            }
            m_registerToStore = globalIt->second;
        }
    }
}

void IntValueNode::generateArrayIndex(CodeGenContext *ctx) {
    auto *currentFrame = ctx->currentFrame;
    auto &globalData = ctx->globalData;
    auto &program = ctx->program;

    m_expressionNode->generateCode(ctx);

    uint32_t arrayPtrReg = 0;
    if (currentFrame) {
        auto arrayIt = currentFrame->IDs_Regs.find(m_name);
        if (arrayIt == currentFrame->IDs_Regs.end()) {
            // Error, undeclared identifier
        }
        arrayPtrReg = arrayIt->second;
    }
    else {
        auto arrayIt = globalData.IDs_Regs.find(m_name);
        if (arrayIt == globalData.IDs_Regs.end()) {
            // Error, undeclared identifier
        }
        arrayPtrReg = arrayIt->second;
    }


    // Calculate absolute pointer in memory of array element
    {
        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;

        decInstr.r1 = m_expressionNode->getRegister();
        decInstr.opcode = VM::InstructionType::LOAD_ACC;  

        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }
    {
        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;

        decInstr.opcode = VM::InstructionType::MULI;

        VM::Immediate constant{};
        constant.type = VM::BasicObjectType::INTEGER;
        constant.i_val = sizeof(uint64_t);

        if (auto it = std::find(program.constants.begin(), program.constants.end(), constant); it != program.constants.end()) {
            decInstr.imm = static_cast<VM::ImmediateIndex>(it - program.constants.begin());
        }
        else {
            decInstr.imm = static_cast<VM::ImmediateIndex>(program.constants.size());
            program.constants.push_back(constant);
        }        

        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }
    {
        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;

        decInstr.r1 = arrayPtrReg;
        decInstr.opcode = VM::InstructionType::ADD;  

        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }
}

uint32_t IntValueNode::getRegister() {
    return m_registerToStore;
}

bool IntValueNode::isArray() {
    return m_expressionNode;
}

IntValueNode::IntValueNode(const std::string &name) {
    m_name = name;
}

IntValueNode::IntValueNode(const std::string &name, ExpressionNode *expressionNode) {
    m_name = name;
    m_expressionNode = expressionNode;
}

// ================================================================================

void FunctionCallNode::generateCode(CodeGenContext *ctx) {
    auto funcIt = ctx->globalData.functions_PC.find(m_name);
    if (funcIt == ctx->globalData.functions_PC.end()) {
        // Error. No such function
    }

    auto &program = ctx->program;
    size_t numArgs = m_arguments.size();

    for (int i = 0; i < numArgs; ++i) {
        m_arguments[i]->generateCode(ctx);
    }

    std::vector<std::pair<uint32_t, uint32_t>> swapRegisters;

    for (int i = 0; i < numArgs; ++i) {
        uint32_t freeReg = ctx->allocateRegister();
        swapRegisters.push_back({i, freeReg});
        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = freeReg;
            decInstr.r2 = i;
            decInstr.opcode = VM::InstructionType::MV;

            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }
        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = i;
            decInstr.r2 = m_arguments[i]->getRegister();
            decInstr.opcode = VM::InstructionType::MV;

            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }
    }

    {
        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;

        decInstr.opcode = VM::InstructionType::CALL;
        decInstr.numArgs = numArgs;

        VM::Immediate constant{};
        constant.type = VM::BasicObjectType::INTEGER;
        constant.i_val = funcIt->second - ctx->globalData.pc;

        if (auto it = std::find(program.constants.begin(), program.constants.end(), constant); it != program.constants.end()) {
            decInstr.imm = static_cast<VM::ImmediateIndex>(it - program.constants.begin());
        }
        else {
            decInstr.imm = static_cast<VM::ImmediateIndex>(program.constants.size());
            program.constants.push_back(constant);
        }

        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }

    for (int i = 0; i < numArgs; ++i) {
        
        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;

        decInstr.r1 = i;
        decInstr.r2 = swapRegisters[i].second;
        decInstr.opcode = VM::InstructionType::MV;

        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);

        ctx->freeRegsiter(swapRegisters[i].second);
    }
}

FunctionCallNode::FunctionCallNode(const std::string &name, std::vector<ExpressionNode *> &arguments) {
    m_name = name;
    std::copy(arguments.begin(), arguments.end(), std::back_inserter(m_arguments));
}

// ================================================================================

void PrimaryNode::generateCode(CodeGenContext *ctx) {
    if (m_scanStatementNode) {
        // From stdin
        m_scanStatementNode->generateCode(ctx);

        auto *currentFrame = ctx->currentFrame;
        auto &program = ctx->program;
        
        uint32_t freeReg = ctx->allocateRegister();

        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;

        decInstr.r1 = freeReg;
        decInstr.opcode = VM::InstructionType::STORE_ACC;

        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);

        m_registerToStore = freeReg;
    }
    else if (m_functionCallNode) {
        // From function return
        m_functionCallNode->generateCode(ctx);

        auto *currentFrame = ctx->currentFrame;
        auto &program = ctx->program;
        
        uint32_t freeReg = ctx->allocateRegister();

        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;

        decInstr.r1 = freeReg;
        decInstr.opcode = VM::InstructionType::STORE_ACC;

        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);

        m_registerToStore = freeReg;
    }
    else if (m_intValueNode) {
        // From identifier
        m_intValueNode->generateCode(ctx);
        m_registerToStore = m_intValueNode->getRegister();
    }
    else {
        // From literal
        auto *currentFrame = ctx->currentFrame;
        auto &program = ctx->program;
        
        uint32_t freeReg = ctx->allocateRegister();

        VM::DecodedInstruction decInstr;

        decInstr.r1 = freeReg;
        decInstr.opcode = VM::InstructionType::MVI;

        VM::Immediate constant{};
        constant.type = VM::BasicObjectType::INTEGER;
        constant.i_val = m_value;

        if (auto it = std::find(program.constants.begin(), program.constants.end(), constant); it != program.constants.end()) {
            decInstr.imm = static_cast<VM::ImmediateIndex>(it - program.constants.begin());
        }
        else {
            decInstr.imm = static_cast<VM::ImmediateIndex>(program.constants.size());
            program.constants.push_back(constant);
        }

        VM::EncodedInstruction encInstr;
        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);

        m_registerToStore = freeReg;
    }
}

int PrimaryNode::getValue() {
    return m_value;
}

uint32_t PrimaryNode::getRegister() {
    return m_registerToStore;
}

PrimaryNode::PrimaryNode(const int value) {
    m_value = value;
}

PrimaryNode::PrimaryNode(IntValueNode *intValueNode) {
    m_intValueNode = intValueNode;
}

PrimaryNode::PrimaryNode(FunctionCallNode *functionCallNode) {
    m_functionCallNode = functionCallNode;
}

PrimaryNode::PrimaryNode(ScanStatementNode *scanStatementNode) {
    m_scanStatementNode = scanStatementNode;
}

// ================================================================================

void FactorNode::generateCode(CodeGenContext *ctx) {
    if (m_expressionNode) {
        // Calculate expression first
        m_expressionNode->generateCode(ctx);
        m_registerToStore = m_expressionNode->getRegister();
    }
    else {
        auto &program = ctx->program;
        
        m_primaryNode->generateCode(ctx);
        m_registerToStore = m_primaryNode->getRegister();
        
        if (m_primaryNot) {
            // Negate
            uint32_t freeReg = ctx->allocateRegister();
            {
                VM::DecodedInstruction decInstr;
                VM::EncodedInstruction encInstr;

                decInstr.r1 = m_primaryNode->getRegister();
                decInstr.opcode = VM::InstructionType::LOAD_ACC;

                ctx->encodeInstruction(decInstr, encInstr);
                program.instructions.push_back(encInstr);
            }
            {
                VM::DecodedInstruction decInstr;
                VM::EncodedInstruction encInstr;

                decInstr.opcode = VM::InstructionType::NEG;

                ctx->encodeInstruction(decInstr, encInstr);
                program.instructions.push_back(encInstr);
            }
            {
                VM::DecodedInstruction decInstr;
                VM::EncodedInstruction encInstr;

                decInstr.r1 = freeReg;
                decInstr.opcode = VM::InstructionType::STORE_ACC;

                ctx->encodeInstruction(decInstr, encInstr);
                program.instructions.push_back(encInstr);
            }
            m_registerToStore = freeReg;
        }
    }
}

uint32_t FactorNode::getRegister() {
    return m_registerToStore;
}

FactorNode::FactorNode(PrimaryNode *primaryNode, bool primaryNot) {
    m_primaryNode = primaryNode;
    m_primaryNot = primaryNot;
}

FactorNode::FactorNode(ExpressionNode *expressionNode) {
    m_expressionNode = expressionNode;
}

// ================================================================================

void SummandNode::generateCode(CodeGenContext *ctx) {
    if (m_summandNode) {
        auto &program = ctx->program;
        
        m_summandNode->generateCode(ctx);
        m_factorNode->generateCode(ctx);
        
        uint32_t freeReg = ctx->allocateRegister();

        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = m_summandNode->getRegister();
            decInstr.opcode = VM::InstructionType::LOAD_ACC;

            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }
        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;
            
            decInstr.r1 = m_factorNode->getRegister();
            switch (m_operation) {
                case HighPriorityOperation::HIGH_PRIORITY_OPERATION_MUL: {
                    decInstr.opcode = VM::InstructionType::MUL;
                    break;
                }
                case HighPriorityOperation::HIGH_PRIORITY_OPERATION_DIV: {
                    decInstr.opcode = VM::InstructionType::DIV;
                    break;
                }
                default: {
                    break;
                }

            }
            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }
        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = freeReg;
            decInstr.opcode = VM::InstructionType::STORE_ACC;

            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }
        m_registerToStore = freeReg;
    }
    else {
        m_factorNode->generateCode(ctx);
        m_registerToStore = m_factorNode->getRegister();
    }
}

uint32_t SummandNode::getRegister() {
    return m_registerToStore;
}

SummandNode::SummandNode(FactorNode *factorNode) {
    m_factorNode = factorNode;
}

SummandNode::SummandNode(SummandNode *summandNode, FactorNode *factorNode, HighPriorityOperation operation) {
    m_summandNode = summandNode;
    m_factorNode = factorNode;
    m_operation = operation;
}

// ================================================================================

void SimpleNode::generateCode(CodeGenContext *ctx) {
    if (m_simpleNode) {
        auto &program = ctx->program;

        m_simpleNode->generateCode(ctx);        
        m_summandNode->generateCode(ctx);
        
        uint32_t freeReg = ctx->allocateRegister();

        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = m_simpleNode->getRegister();
            decInstr.opcode = VM::InstructionType::LOAD_ACC;

            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }
        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;
            
            decInstr.r1 = m_summandNode->getRegister();
            switch (m_operation) {
                case LowPriorityOperation::LOW_PRIORITY_OPERATION_ADD: {
                    decInstr.opcode = VM::InstructionType::ADD;
                    break;
                }
                case LowPriorityOperation::LOW_PRIORITY_OPERATION_SUB: {
                    decInstr.opcode = VM::InstructionType::SUB;
                    break;
                }
                case LowPriorityOperation::LOW_PRIORITY_OPERATION_SHIFT_LEFT: {
                    decInstr.opcode = VM::InstructionType::SL;
                    break;
                }
                case LowPriorityOperation::LOW_PRIORITY_OPERATION_SHIFT_RIGHT: {
                    decInstr.opcode = VM::InstructionType::SR;
                    break;
                }
                case LowPriorityOperation::LOW_PRIORITY_OPERATION_BITWISE_AND: {
                    decInstr.opcode = VM::InstructionType::AND;
                    break;
                }
                case LowPriorityOperation::LOW_PRIORITY_OPERATION_BITWISE_OR: {
                    decInstr.opcode = VM::InstructionType::OR;
                    break;
                }
                case LowPriorityOperation::LOW_PRIORITY_OPERATION_BITWISE_XOR: {
                    decInstr.opcode = VM::InstructionType::XOR;
                    break;
                }
                default: {
                    break;
                }

            }
            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }
        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = freeReg;
            decInstr.opcode = VM::InstructionType::STORE_ACC;

            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }
        m_registerToStore = freeReg;
    }
    else {
        m_summandNode->generateCode(ctx);
        m_registerToStore = m_summandNode->getRegister();
    }
}

uint32_t SimpleNode::getRegister() {
    return m_registerToStore;    
}

SimpleNode::SimpleNode(SummandNode *summandNode) {
    m_summandNode = summandNode;
}

SimpleNode::SimpleNode(SimpleNode *simpleNode, SummandNode *summandNode, LowPriorityOperation operation) {
    m_simpleNode = simpleNode;
    m_summandNode = summandNode;
    m_operation = operation;
}

// ================================================================================

void ExpressionNode::generateCode(CodeGenContext *ctx) {
    if (m_expressionNode) {
        auto &program = ctx->program;
    
        m_expressionNode->generateCode(ctx);
        m_simpleNode->generateCode(ctx);

        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = m_expressionNode->getRegister();
            decInstr.opcode = VM::InstructionType::LOAD_ACC;

            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }

        m_registerToStore = m_simpleNode->getRegister();
    }
    else {
        m_simpleNode->generateCode(ctx);
        m_registerToStore = m_simpleNode->getRegister();
    }
}

uint32_t ExpressionNode::getRegister() {
    return m_registerToStore;
}

ExpressionOperation ExpressionNode::getOperation() {
    return m_opeation;
}

uint32_t ExpressionNode::getSimpleNodeRegister() {
    return m_simpleNode->getRegister();
}

ExpressionNode::ExpressionNode(SimpleNode *simpleNode) {
    m_simpleNode = simpleNode;
}

ExpressionNode::ExpressionNode(ExpressionNode *expressionNode, SimpleNode *simpleNode, ExpressionOperation operation) {
    m_expressionNode = expressionNode;
    m_simpleNode = simpleNode;
    m_opeation = operation;
}

// ================================================================================

void AssignmentStatement::generateCode(CodeGenContext *ctx) {
    auto *currentFrame = ctx->currentFrame;
    auto &globalData = ctx->globalData;
    auto &program = ctx->program;

    if (m_intValueNode->isArray()) {
        // Array
        m_intValueNode->generateArrayIndex(ctx);
        m_expressionNode->generateCode(ctx);

        uint32_t freeReg = ctx->allocateRegister();
        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = freeReg;
            decInstr.opcode = VM::InstructionType::STORE_ACC;

            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }
        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = m_expressionNode->getRegister();
            decInstr.opcode = VM::InstructionType::LOAD_ACC;

            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }
        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = freeReg;
            decInstr.opcode = VM::InstructionType::STORE_ACC_MEM;

            VM::Immediate constant{};
            constant.type = VM::BasicObjectType::INTEGER;
            constant.i_val = 0;

            if (auto it = std::find(program.constants.begin(), program.constants.end(), constant); it != program.constants.end()) {
                decInstr.imm = static_cast<VM::ImmediateIndex>(it - program.constants.begin());
            }
            else {
                decInstr.imm = static_cast<VM::ImmediateIndex>(program.constants.size());
                program.constants.push_back(constant);
            }
            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }

        ctx->freeRegsiter(freeReg);
    }
    else {
        m_intValueNode->generateCode(ctx);
        m_expressionNode->generateCode(ctx);

        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = m_intValueNode->getRegister();
            decInstr.r2 = m_expressionNode->getRegister();
            decInstr.opcode = VM::InstructionType::MV;

            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }
    }
}

AssignmentStatement::AssignmentStatement(IntValueNode *intValueNode, ExpressionNode *expressionNode) {
    m_intValueNode = intValueNode;
    m_expressionNode = expressionNode;
}

// ================================================================================

void ReturnStatementNode::generateCode(CodeGenContext *ctx) {
    auto &program = ctx->program;

    if (m_expressionNode) {
        m_expressionNode->generateCode(ctx);

        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;

        decInstr.r1 = m_expressionNode->getRegister();
        decInstr.opcode = VM::InstructionType::LOAD_ACC;

        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }

    {
        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;

        decInstr.opcode = VM::InstructionType::RET;

        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }
}

ReturnStatementNode::ReturnStatementNode(ExpressionNode *expressionNode) {
    m_expressionNode = expressionNode;
}

// ================================================================================

void StatementsScopeNode::generateCode(CodeGenContext *ctx) {
    for (int i = 0; i < m_scopeStatements.size(); ++i) {
        m_scopeStatements[i]->generateCode(ctx);
    }
}

StatementsScopeNode::StatementsScopeNode(std::vector<ASTNode *> &scopeStatements) {
    std::copy(scopeStatements.begin(), scopeStatements.end(), std::back_inserter(m_scopeStatements));
}

// ================================================================================

void FunctionDeclarationNode::generateCode(CodeGenContext *ctx) {
    ctx->globalData.functions_PC.insert({m_name, ctx->globalData.pc});
    if (m_name == "main") {
         ctx->program.entryPoint = ctx->globalData.pc;   
    }

    ctx->currentFrame = new CompilerFrame();
    auto *currentFrame = ctx->currentFrame;

    currentFrame->currentScope = new CompilerScope();

    for (int i = 0; i < m_arguments.size(); ++i) {
        m_arguments[i]->generateCode(ctx);
        currentFrame->currentScope->scopedIDs.insert(m_arguments[i]->getName());
    }

    m_body->generateCode(ctx);

    delete currentFrame->currentScope;
    delete currentFrame;
}

FunctionDeclarationNode::FunctionDeclarationNode(const std::string &name, 
                                                 std::vector<SimpleIntDeclarationNode *> &arguments,
                                                 StatementsScopeNode *body) 
{
    std::copy(arguments.begin(), arguments.end(), std::back_inserter(m_arguments));
    m_body = body;
    m_name = name;
}

// ================================================================================

void IfStatementNode::generateCode(CodeGenContext *ctx) {
    auto &program = ctx->program;
    auto *currentFrame = ctx->currentFrame;
    auto *outerScope = currentFrame->currentScope;

    auto *trueScope = new CompilerScope();
    trueScope->parentScope = currentFrame->currentScope;
    currentFrame->currentScope = trueScope;


    // If-else statement
    m_expressionNode->generateCode(ctx);

    uint64_t branchInstructionIndex = program.instructions.size();
    {
        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;
        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }
    m_trueBody->generateCode(ctx);

    uint64_t jumpInstructionIndex = program.instructions.size();    
    if (m_falseBody) {
        // Jump after branch
        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;
        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }

    {
        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;

        decInstr.r1 = m_expressionNode->getSimpleNodeRegister();

        ExpressionOperation operation = m_expressionNode->getOperation();
        switch (operation) {
            // Inverse operations
            case ExpressionOperation::EXPRESSION_OPERATION_LESS: {
                decInstr.opcode = VM::InstructionType::BGE;
                break;
            }
            case ExpressionOperation::EXPRESSION_OPERATION_LESS_OR_EQ: {
                decInstr.opcode = VM::InstructionType::BGT;
                break;
            }
            case ExpressionOperation::EXPRESSION_OPERATION_GREATER: {
                decInstr.opcode = VM::InstructionType::BLE;
                break;
            }
            case ExpressionOperation::EXPRESSION_OPERATION_GREATER_OR_EQ: {
                decInstr.opcode = VM::InstructionType::BLT;
                break;
            }
            case ExpressionOperation::EXPRESSION_OPERATION_EQUAL: {
                decInstr.opcode = VM::InstructionType::BNE;
                break;
            }
            case ExpressionOperation::EXPRESSION_OPERATION_NOT_EQUAL: {
                decInstr.opcode = VM::InstructionType::BEQ;
                break;
            }
            default: {
                break;
            }
        }

        VM::Immediate constant{};
        constant.type = VM::BasicObjectType::INTEGER;
        constant.i_val = ctx->globalData.pc - branchInstructionIndex * VM::INSTRUCTION_BYTESIZE;

        if (auto it = std::find(program.constants.begin(), program.constants.end(), constant); it != program.constants.end()) {
            decInstr.imm = static_cast<VM::ImmediateIndex>(it - program.constants.begin());
        }
        else {
            decInstr.imm = static_cast<VM::ImmediateIndex>(program.constants.size());
            program.constants.push_back(constant);
        }

        ctx->builder.encodeInstruction(decInstr, encInstr);
        program.instructions[branchInstructionIndex] = encInstr;
    }

    currentFrame->currentScope = outerScope;
    delete trueScope;

    if (m_falseBody) {
        auto *falseScope = new CompilerScope();
        falseScope->parentScope = currentFrame->currentScope;
        currentFrame->currentScope = falseScope;


        m_falseBody->generateCode(ctx);

        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.opcode = VM::InstructionType::JMP;

            VM::Immediate constant{};
            constant.type = VM::BasicObjectType::INTEGER;
            constant.i_val = ctx->globalData.pc - jumpInstructionIndex * VM::INSTRUCTION_BYTESIZE;

            if (auto it = std::find(program.constants.begin(), program.constants.end(), constant); it != program.constants.end()) {
                decInstr.imm = static_cast<VM::ImmediateIndex>(it - program.constants.begin());
            }
            else {
                decInstr.imm = static_cast<VM::ImmediateIndex>(program.constants.size());
                program.constants.push_back(constant);
            }

            ctx->builder.encodeInstruction(decInstr, encInstr);
            program.instructions[jumpInstructionIndex] = encInstr;
        }

        currentFrame->currentScope = outerScope;
        delete falseScope;
    }
}

IfStatementNode::IfStatementNode(ExpressionNode *expressionNode, StatementsScopeNode *trueBody, StatementsScopeNode *falseBody) {
    m_expressionNode = expressionNode;
    m_trueBody = trueBody;
    m_falseBody = falseBody;
}

// ================================================================================

void WhileLoopStatementNode::generateCode(CodeGenContext *ctx)  {
    auto &program = ctx->program;
    auto *currentFrame = ctx->currentFrame;
    auto *outerScope = currentFrame->currentScope;

    auto *bodyScope = new CompilerScope();
    bodyScope->parentScope = currentFrame->currentScope;
    currentFrame->currentScope = bodyScope;


    m_expressionNode->generateCode(ctx);

    uint64_t branchInstructionIndex = program.instructions.size();
    {
        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;
        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }
    m_body->generateCode(ctx);

    // Jump back to beginning of loop
    {
        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;

        decInstr.opcode = VM::InstructionType::JMP;

        VM::Immediate constant{};
        constant.type = VM::BasicObjectType::INTEGER;
        constant.i_val = branchInstructionIndex * VM::INSTRUCTION_BYTESIZE - ctx->globalData.pc;

        if (auto it = std::find(program.constants.begin(), program.constants.end(), constant); it != program.constants.end()) {
            decInstr.imm = static_cast<VM::ImmediateIndex>(it - program.constants.begin());
        }
        else {
            decInstr.imm = static_cast<VM::ImmediateIndex>(program.constants.size());
            program.constants.push_back(constant);
        }

        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }

    {
        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;

        decInstr.r1 = m_expressionNode->getSimpleNodeRegister();

        ExpressionOperation operation = m_expressionNode->getOperation();
        switch (operation) {
            // Inverse operations
            case ExpressionOperation::EXPRESSION_OPERATION_LESS: {
                decInstr.opcode = VM::InstructionType::BGE;
                break;
            }
            case ExpressionOperation::EXPRESSION_OPERATION_LESS_OR_EQ: {
                decInstr.opcode = VM::InstructionType::BGT;
                break;
            }
            case ExpressionOperation::EXPRESSION_OPERATION_GREATER: {
                decInstr.opcode = VM::InstructionType::BLE;
                break;
            }
            case ExpressionOperation::EXPRESSION_OPERATION_GREATER_OR_EQ: {
                decInstr.opcode = VM::InstructionType::BLT;
                break;
            }
            case ExpressionOperation::EXPRESSION_OPERATION_EQUAL: {
                decInstr.opcode = VM::InstructionType::BNE;
                break;
            }
            case ExpressionOperation::EXPRESSION_OPERATION_NOT_EQUAL: {
                decInstr.opcode = VM::InstructionType::BEQ;
                break;
            }
            default: {
                break;
            }
        }

        VM::Immediate constant{};
        constant.type = VM::BasicObjectType::INTEGER;
        constant.i_val = ctx->globalData.pc - branchInstructionIndex * VM::INSTRUCTION_BYTESIZE;

        if (auto it = std::find(program.constants.begin(), program.constants.end(), constant); it != program.constants.end()) {
            decInstr.imm = static_cast<VM::ImmediateIndex>(it - program.constants.begin());
        }
        else {
            decInstr.imm = static_cast<VM::ImmediateIndex>(program.constants.size());
            program.constants.push_back(constant);
        }

        ctx->builder.encodeInstruction(decInstr, encInstr);
        program.instructions[branchInstructionIndex] = encInstr;
    }

    currentFrame->currentScope = outerScope;
    delete bodyScope;
}

WhileLoopStatementNode::WhileLoopStatementNode(ExpressionNode *expressionNode, StatementsScopeNode *body) {
    m_expressionNode = expressionNode;
    m_body = body;
}

// ================================================================================

void ForLoopStatementNode::generateCode(CodeGenContext *ctx) {
    auto &program = ctx->program;
    auto *currentFrame = ctx->currentFrame;
    auto *outerScope = currentFrame->currentScope;

    auto *bodyScope = new CompilerScope();
    bodyScope->parentScope = currentFrame->currentScope;
    currentFrame->currentScope = bodyScope;


    m_preLoopStatement->generateCode(ctx);

    uint64_t expressionInstructionIndex = program.instructions.size();
    m_expressionNode->generateCode(ctx);

    uint64_t branchInstructionIndex = program.instructions.size();
    {
        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;
        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }

    m_body->generateCode(ctx);
    m_postLoopStatement->generateCode(ctx);

    // Jump back to beginning of loop
    {
        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;

        decInstr.opcode = VM::InstructionType::JMP;

        VM::Immediate constant{};
        constant.type = VM::BasicObjectType::INTEGER;
        constant.i_val = expressionInstructionIndex * VM::INSTRUCTION_BYTESIZE - ctx->globalData.pc;

        if (auto it = std::find(program.constants.begin(), program.constants.end(), constant); it != program.constants.end()) {
            decInstr.imm = static_cast<VM::ImmediateIndex>(it - program.constants.begin());
        }
        else {
            decInstr.imm = static_cast<VM::ImmediateIndex>(program.constants.size());
            program.constants.push_back(constant);
        }

        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }

    {
        VM::DecodedInstruction decInstr;
        VM::EncodedInstruction encInstr;

        decInstr.r1 = m_expressionNode->getSimpleNodeRegister();

        ExpressionOperation operation = m_expressionNode->getOperation();
        switch (operation) {
            // Inverse operations
            case ExpressionOperation::EXPRESSION_OPERATION_LESS: {
                decInstr.opcode = VM::InstructionType::BGE;
                break;
            }
            case ExpressionOperation::EXPRESSION_OPERATION_LESS_OR_EQ: {
                decInstr.opcode = VM::InstructionType::BGT;
                break;
            }
            case ExpressionOperation::EXPRESSION_OPERATION_GREATER: {
                decInstr.opcode = VM::InstructionType::BLE;
                break;
            }
            case ExpressionOperation::EXPRESSION_OPERATION_GREATER_OR_EQ: {
                decInstr.opcode = VM::InstructionType::BLT;
                break;
            }
            case ExpressionOperation::EXPRESSION_OPERATION_EQUAL: {
                decInstr.opcode = VM::InstructionType::BNE;
                break;
            }
            case ExpressionOperation::EXPRESSION_OPERATION_NOT_EQUAL: {
                decInstr.opcode = VM::InstructionType::BEQ;
                break;
            }
            default: {
                break;
            }
        }

        VM::Immediate constant{};
        constant.type = VM::BasicObjectType::INTEGER;
        constant.i_val = ctx->globalData.pc - branchInstructionIndex * VM::INSTRUCTION_BYTESIZE;

        if (auto it = std::find(program.constants.begin(), program.constants.end(), constant); it != program.constants.end()) {
            decInstr.imm = static_cast<VM::ImmediateIndex>(it - program.constants.begin());
        }
        else {
            decInstr.imm = static_cast<VM::ImmediateIndex>(program.constants.size());
            program.constants.push_back(constant);
        }

        ctx->builder.encodeInstruction(decInstr, encInstr);
        program.instructions[branchInstructionIndex] = encInstr;
    }

    currentFrame->currentScope = outerScope;
    delete bodyScope;
}

ForLoopStatementNode::ForLoopStatementNode(ASTNode *preLoopStatement,
                                           ExpressionNode *expressionNode,
                                           StatementsScopeNode *body,
                                           ASTNode *postLoopStatement)
{
    m_preLoopStatement = preLoopStatement;
    m_expressionNode = expressionNode;
    m_body = body;
    m_postLoopStatement = postLoopStatement;
}


}
