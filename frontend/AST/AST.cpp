#include "AST.h"


namespace AST {

// ================================================================================

void SimpleVariableDeclarationNode::generateCode(CodeGenContext *ctx) {
    auto *currentFrame = ctx->currentFrame;

    uint32_t freeReg = ctx->allocateRegister();
    if (currentFrame) {
        // Inside a function
        auto *currentScope = currentFrame->currentScope;
        currentScope->scopedIDs.insert(m_name);
        currentFrame->IDs_Regs.insert({m_name, freeReg});
        currentFrame->identifierTypes.insert({m_name, m_objType});
    }
    else {
        // Global scope of data
        auto &globalData = ctx->globalData;
        globalData.IDs_Regs.insert({m_name, freeReg});
        globalData.identifierTypes.insert({m_name, m_objType});
    }
}

void SimpleVariableDeclarationNode::print(size_t printLevel) {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }
    printf("SimpleVariableDeclarationNode: (name = %s) (type = %s)\n", m_name.c_str(), m_objType == VM::BasicObjectType::INTEGER ? "int" :
                                                                                       m_objType == VM::BasicObjectType::FLOATING ? "float" :
                                                                                       m_objType == VM::BasicObjectType::STRING ? "string" :
                                                                                       "unknown");
}

std::string SimpleVariableDeclarationNode::getName() const {
    return m_name;
}

SimpleVariableDeclarationNode::SimpleVariableDeclarationNode(const std::string &name, VM::BasicObjectType objType) {
    m_name = name;
    m_objType = objType;
}

// ================================================================================

void InitVariableDeclarationNode::generateCode(CodeGenContext *ctx) {
    auto *currentFrame = ctx->currentFrame;
    auto &program = ctx->program;

    uint32_t freeReg = ctx->allocateRegister();
    if (currentFrame) {
        // Inside a function
        auto *currentScope = currentFrame->currentScope;
        currentScope->scopedIDs.insert(m_name);
        currentFrame->IDs_Regs.insert({m_name, freeReg});
        currentFrame->identifierTypes.insert({m_name, m_objType});
    }
    else {
        // Global scope of data
        auto &globalData = ctx->globalData;
        globalData.IDs_Regs.insert({m_name, freeReg});
        globalData.identifierTypes.insert({m_name, m_objType});
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

void InitVariableDeclarationNode::print(size_t printLevel) {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }
    printf("InitVariableDeclarationNode: (name = %s) (type = %s) (ExpressionNode)\n", m_name.c_str(), m_objType == VM::BasicObjectType::INTEGER ? "int" :
                                                                                                      m_objType == VM::BasicObjectType::FLOATING ? "float" :
                                                                                                      m_objType == VM::BasicObjectType::STRING ? "string" :
                                                                                                      "unknown");
    m_expressionNode->print(printLevel + 1);
}

std::string InitVariableDeclarationNode::getName() const {
    return m_name;
}

InitVariableDeclarationNode::InitVariableDeclarationNode(const std::string &name, VM::BasicObjectType objType, ExpressionNode *expressionNode) {
    m_name = name;
    m_objType = objType;
    m_expressionNode = expressionNode;
}

InitVariableDeclarationNode::~InitVariableDeclarationNode() {
    delete m_expressionNode;
}

// ================================================================================

void ArrayVariableDeclarationNode::generateCode(CodeGenContext *ctx) {
    auto *currentFrame = ctx->currentFrame;
    auto &program = ctx->program;

    uint32_t freeReg = ctx->allocateRegister();
    if (currentFrame) {
        // Inside a function
        auto *currentScope = currentFrame->currentScope;
        currentScope->scopedIDs.insert(m_name);
        currentFrame->IDs_Regs.insert({m_name, freeReg});
        currentFrame->identifierTypes.insert({m_name, m_objType});
    }
    else {
        // Global scope of data
        auto &globalData = ctx->globalData;
        globalData.IDs_Regs.insert({m_name, freeReg});
        globalData.identifierTypes.insert({m_name, m_objType});
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

        decInstr.classIdx = m_objType;
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

void ArrayVariableDeclarationNode::print(size_t printLevel) {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }
    printf("ArrayVariableDeclarationNode: (name = %s) (type = %s) (size = %ld)\n", m_name.c_str(), m_objType == VM::BasicObjectType::INTEGER ? "int" :
                                                                                                   m_objType == VM::BasicObjectType::FLOATING ? "float" :
                                                                                                   m_objType == VM::BasicObjectType::STRING ? "string" :
                                                                                                   "unknown", m_size);
}

std::string ArrayVariableDeclarationNode::getName() const {
    return m_name;
}

size_t ArrayVariableDeclarationNode::getSize() const {
    return m_size;
}

ArrayVariableDeclarationNode::ArrayVariableDeclarationNode(const std::string &name, VM::BasicObjectType objType, const size_t size) {
    m_name = name;
    m_objType = objType;
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

        switch (m_objType) {
            case VM::BasicObjectType::INTEGER: {
                decInstr.intrCode = VM::IntrinsicType::PRINT;
                break;
            }
            case VM::BasicObjectType::FLOATING: {
                decInstr.intrCode = VM::IntrinsicType::PRINTF;
                break;
            }
            case VM::BasicObjectType::STRING: {
                decInstr.intrCode = VM::IntrinsicType::PRINTS;
                break;
            }
            default: {
                break;
            }
        }

        VM::EncodedInstruction encInstr;
        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }
}

void PrintStatementNode::print(size_t printLevel) {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }
    printf("PrintStatementNode: (type = %s) (ExpressionNode)\n", m_objType == VM::BasicObjectType::INTEGER ? "int" :
                                                                 m_objType == VM::BasicObjectType::FLOATING ? "float" :
                                                                 m_objType == VM::BasicObjectType::STRING ? "string" :
                                                                 "unknown");
    m_expressionNode->print(printLevel + 1);
}

PrintStatementNode::PrintStatementNode(VM::BasicObjectType objType, ExpressionNode *expressionNode) {
    m_objType = objType;
    m_expressionNode = expressionNode;
}

PrintStatementNode::~PrintStatementNode() {
    delete m_expressionNode;
}

// ================================================================================

void ScanStatementNode::generateCode(CodeGenContext *ctx) {
    auto &program = ctx->program;
    {
        VM::DecodedInstruction decInstr;
        decInstr.opcode = VM::InstructionType::CALL_INTRINSIC;

        switch (m_objType) {
            case VM::BasicObjectType::INTEGER: {
                decInstr.intrCode = VM::IntrinsicType::SCAN;
                break;
            }
            case VM::BasicObjectType::FLOATING: {
                decInstr.intrCode = VM::IntrinsicType::SCANF;
                break;
            }
            case VM::BasicObjectType::STRING: {
                decInstr.intrCode = VM::IntrinsicType::SCANS;
                break;
            }
            default: {
                break;
            }
        }

        VM::EncodedInstruction encInstr;
        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }
}

void ScanStatementNode::print(size_t printLevel) {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }
    printf("ScanStatementNode: (type = %s)\n", m_objType == VM::BasicObjectType::INTEGER ? "int" :
                                               m_objType == VM::BasicObjectType::FLOATING ? "float" :
                                               m_objType == VM::BasicObjectType::STRING ? "string" :
                                               "unknown");
}

VM::BasicObjectType ScanStatementNode::getType() {
    return m_objType;
}

ScanStatementNode::ScanStatementNode(VM::BasicObjectType objType) {
    m_objType = objType;
}

// ================================================================================

void SqrtStatementNode::generateCode(CodeGenContext *ctx) {
    auto &program = ctx->program;

    m_expressionNode->generateCode(ctx);
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

        decInstr.opcode = VM::InstructionType::SQRT;

        ctx->encodeInstruction(decInstr, encInstr);
        program.instructions.push_back(encInstr);
    }
}

void SqrtStatementNode::print(size_t printLevel) {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }
    printf("SqrtStatementNode: (ExpressionNode)\n");
    m_expressionNode->print(printLevel + 1);
}

SqrtStatementNode::SqrtStatementNode(ExpressionNode *expressionNode) {
    m_expressionNode = expressionNode;
}

SqrtStatementNode::~SqrtStatementNode() {
    delete m_expressionNode;
}

// ================================================================================

void ProgramNode::generateCode(CodeGenContext *ctx) {
    for (auto child : m_statements_functions) {
        child->generateCode(ctx);
    }
}

void ProgramNode::print(size_t printLevel) {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }

    printf("ProgramNode: (children)\n");
    for (auto child : m_statements_functions) {
        child->print(printLevel + 1);
    }
}

void ProgramNode::insertNode(ASTNode *node) {
    m_statements_functions.insert(m_statements_functions.begin(), node);
}

ProgramNode::ProgramNode() {

}

ProgramNode::~ProgramNode() {
    for (auto node : m_statements_functions) {
        delete node;
    }
    m_statements_functions.clear();
}

// ================================================================================

void VariableValueNode::generateCode(CodeGenContext *ctx) {
    auto *currentFrame = ctx->currentFrame;
    auto &globalData = ctx->globalData;
    auto &program = ctx->program;

    if (m_expressionNode) {
        // Array
        uint32_t freeReg = ctx->allocateRegister();
        uint32_t arrayPtrReg = 0;
        VM::RegisterType indexReg = generateArrayIndex(ctx);
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

        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = arrayPtrReg;
            decInstr.opcode = VM::InstructionType::LOAD_ACC;

            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }
        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = freeReg;
            decInstr.r2 = indexReg;
            decInstr.opcode = VM::InstructionType::LOAD_ARR_ELEM;

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

void VariableValueNode::print(size_t printLevel) {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }
    printf("VariableValueNode: (name = %s) ", m_name.c_str());
    if (m_expressionNode) {
        printf("(ExpressionNode)\n");
        m_expressionNode->print(printLevel + 1);
    }
    else {
        putchar('\n');
    }
}

VM::RegisterType VariableValueNode::generateArrayIndex(CodeGenContext *ctx) {
    m_expressionNode->generateCode(ctx);
    return m_expressionNode->getRegister();
}

uint32_t VariableValueNode::getRegister() {
    return m_registerToStore;
}

bool VariableValueNode::isArray() {
    return m_expressionNode;
}

VM::BasicObjectType VariableValueNode::getType(CodeGenContext *ctx) {
    auto *currentFrame = ctx->currentFrame;

    if (currentFrame) {
        auto it = ctx->currentFrame->identifierTypes.find(m_name);
        if (it == ctx->currentFrame->identifierTypes.end()) {
            // Error, undeclared identifier
        }
        return it->second;
    }
    else {
        auto it = ctx->globalData.identifierTypes.find(m_name);
        if (it == ctx->globalData.identifierTypes.end()) {
            // Error, undeclared identifier
        }
        return it->second;
    }
}

std::string VariableValueNode::getName() {
    return m_name;
}

VariableValueNode::VariableValueNode(const std::string &name) {
    m_name = name;
}

VariableValueNode::VariableValueNode(const std::string &name, ExpressionNode *expressionNode) {
    m_name = name;
    m_expressionNode = expressionNode;
}

VariableValueNode::~VariableValueNode() {
    delete m_expressionNode;
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

void FunctionCallNode::print(size_t printLevel) {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }
    printf("FunctionCallNode: (name = %s) (args)\n", m_name.c_str());
    for (auto node : m_arguments) {
        node->print(printLevel + 1);
    }
}

VM::BasicObjectType FunctionCallNode::getType(CodeGenContext *ctx) {
    auto it = ctx->globalData.functionReturnTypes.find(m_name);
    if (it == ctx->globalData.functionReturnTypes.end()) {
        // Error, undeclared function
    }
    switch (it->second) {
        case ReturnType::RETURN_TYPE_INT: {
            return VM::BasicObjectType::INTEGER;
        }
        case ReturnType::RETURN_TYPE_FLOAT: {
            return VM::BasicObjectType::FLOATING;
        }
        case ReturnType::RETURN_TYPE_STRING: {
            return VM::BasicObjectType::STRING;
        }
        default: {
            return VM::BasicObjectType::INTEGER;
        }
    }
}

FunctionCallNode::FunctionCallNode(const std::string &name, std::vector<ExpressionNode *> &arguments) {
    m_name = name;
    std::copy(arguments.begin(), arguments.end(), std::back_inserter(m_arguments));
}

FunctionCallNode::~FunctionCallNode() {
    for (auto node : m_arguments) {
        delete node;
    }
    m_arguments.clear();
}

// ================================================================================

void PrimaryNode::generateCode(CodeGenContext *ctx) {
    if (m_sqrtStatementNode) {
        // Sqrt builtin function
        m_sqrtStatementNode->generateCode(ctx);

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
        m_objType = VM::BasicObjectType::FLOATING;
    }
    else if (m_scanStatementNode) {
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
        m_objType = m_scanStatementNode->getType();
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
        m_objType = m_functionCallNode->getType(ctx);
    }
    else if (m_valueNode) {
        // From identifier
        m_valueNode->generateCode(ctx);
        m_registerToStore = m_valueNode->getRegister();
        m_objType = m_valueNode->getType(ctx);
    }
    else {
        // From literal
        auto *currentFrame = ctx->currentFrame;
        auto &program = ctx->program;
        
        uint32_t freeReg = ctx->allocateRegister();

        if (m_objType == VM::BasicObjectType::STRING) {
            {
                VM::DecodedInstruction decInstr;
                VM::EncodedInstruction encInstr;

                decInstr.opcode = VM::InstructionType::LOAD_ACCI;
                decInstr.imm = ctx->insertStringIntoPool(m_stringValue);

                ctx->encodeInstruction(decInstr, encInstr);
                program.instructions.push_back(encInstr);
            }
            {
                VM::DecodedInstruction decInstr;
                VM::EncodedInstruction encInstr;

                decInstr.opcode = VM::InstructionType::STORE_ACC;
                decInstr.r1 = freeReg;

                ctx->encodeInstruction(decInstr, encInstr);
                program.instructions.push_back(encInstr);
            }
        }
        else {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = freeReg;
            decInstr.opcode = VM::InstructionType::MVI;

            VM::Immediate constant{};
            constant.type = m_objType;
            switch (m_objType) {
                case VM::BasicObjectType::INTEGER: {
                    constant.i_val = m_intValue;
                    break;
                }
                case VM::BasicObjectType::FLOATING: {
                    constant.f_val = m_floatValue;
                    break;
                }
                default: {
                    break;
                }
            }

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
        m_registerToStore = freeReg;
    }
}

void PrimaryNode::print(size_t printLevel) {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }

    printf("PrimaryNode: ");
    if (m_valueNode) {
        printf("(VariableValueNode)\n");
        m_valueNode->print(printLevel + 1);
    }
    else if (m_functionCallNode) {
        printf("(FunctionCallNode)\n");
        m_functionCallNode->print(printLevel + 1);
    }
    else if (m_scanStatementNode) {
        printf("(ScanStatementNode)\n");
        m_scanStatementNode->print(printLevel + 1);
    }
    else if (m_sqrtStatementNode) {
        printf("(SqrtStatementNode)\n");
        m_sqrtStatementNode->print(printLevel + 1);
    }
    else if (m_objType == VM::BasicObjectType::INTEGER) {
        printf("(type = int) (value = %d)\n", m_intValue);
    }
    else if (m_objType == VM::BasicObjectType::FLOATING) {
        printf("(type = float) (value = %lf)\n", m_floatValue);
    }
    else if (m_objType == VM::BasicObjectType::STRING) {
        printf("(type = string) (value = \"%s\")\n", m_stringValue.c_str());
    }
}

int PrimaryNode::getIntValue() {
    return m_intValue;
}

double PrimaryNode::getFloatValue() {
    return m_floatValue;
}

std::string PrimaryNode::getStringValue() {
    return m_stringValue;
}

uint32_t PrimaryNode::getRegister() {
    return m_registerToStore;
}

VM::BasicObjectType PrimaryNode::getType() {
    return m_objType;
}

PrimaryNode::PrimaryNode(const int value) {
    m_intValue = value;
    m_objType = VM::BasicObjectType::INTEGER;
}

PrimaryNode::PrimaryNode(const double value) {
    m_floatValue = value;
    m_objType = VM::BasicObjectType::FLOATING;
}

PrimaryNode::PrimaryNode(const std::string &value) {
    m_stringValue = value;
    Common::replaceAllEscapeSeq(m_stringValue);
    m_stringValue = m_stringValue.substr(1, m_stringValue.length() - 2);
    m_objType = VM::BasicObjectType::STRING;
}

PrimaryNode::PrimaryNode(VariableValueNode *valueNode) {
    m_valueNode = valueNode;
    // Initialize integer
    m_objType = VM::BasicObjectType::INTEGER;
}

PrimaryNode::PrimaryNode(FunctionCallNode *functionCallNode) {
    m_functionCallNode = functionCallNode;
    // Initialize integer
    m_objType = VM::BasicObjectType::INTEGER;
}

PrimaryNode::PrimaryNode(ScanStatementNode *scanStatementNode) {
    m_scanStatementNode = scanStatementNode;
    // Initialize integer
    m_objType = VM::BasicObjectType::INTEGER;
}

PrimaryNode::PrimaryNode(SqrtStatementNode *sqrtStatementNode) {
    m_sqrtStatementNode = sqrtStatementNode;
    m_objType = VM::BasicObjectType::FLOATING;
}

PrimaryNode::~PrimaryNode() {
    delete m_valueNode;
    delete m_functionCallNode;
    delete m_scanStatementNode;
    delete m_sqrtStatementNode;
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

                if (m_primaryNode->getType() == VM::BasicObjectType::INTEGER) {
                    decInstr.opcode = VM::InstructionType::NEG;
                }
                if (m_primaryNode->getType() == VM::BasicObjectType::FLOATING) {
                    decInstr.opcode = VM::InstructionType::NEGF;
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
    }
}

void FactorNode::print(size_t printLevel) {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }
    printf("FactorNode: ");
    if (m_primaryNode) {
        if (m_primaryNot) {
            printf("(Not) ");
        }
        printf("(PrimaryNode)\n");
        m_primaryNode->print(printLevel + 1);
    }
    else if (m_expressionNode) {
        printf("(ExpressionNode)\n");
        m_expressionNode->print(printLevel + 1);
    }
}

uint32_t FactorNode::getRegister() {
    return m_registerToStore;
}

VM::BasicObjectType FactorNode::getType() {
    if (m_primaryNode) {
        return m_primaryNode->getType();
    }
    return m_expressionNode->getType();
}

FactorNode::FactorNode(PrimaryNode *primaryNode, bool primaryNot) {
    m_primaryNode = primaryNode;
    m_primaryNot = primaryNot;
}

FactorNode::FactorNode(ExpressionNode *expressionNode) {
    m_expressionNode = expressionNode;
}

FactorNode::~FactorNode() {
    delete m_primaryNode;
    delete m_expressionNode;
}

// ================================================================================

void SummandNode::generateCode(CodeGenContext *ctx) {
    m_factorNode->generateCode(ctx);

    if (m_summandNode) {
        m_summandNode->generateCode(ctx);

        auto &program = ctx->program;
        uint32_t freeReg = ctx->allocateRegister();

        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = m_factorNode->getRegister();
            decInstr.opcode = VM::InstructionType::LOAD_ACC;

            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }
        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;
            
            decInstr.r1 = m_summandNode->getRegister();
            VM::BasicObjectType objType = m_factorNode->getType();
            switch (m_operation) {
                case HighPriorityOperation::HIGH_PRIORITY_OPERATION_MUL: {
                    if (objType == VM::BasicObjectType::INTEGER) {
                        decInstr.opcode = VM::InstructionType::MUL;
                    }
                    if (objType == VM::BasicObjectType::FLOATING) {
                        decInstr.opcode = VM::InstructionType::MULF;
                    }
                    break;
                }
                case HighPriorityOperation::HIGH_PRIORITY_OPERATION_DIV: {
                    if (objType == VM::BasicObjectType::INTEGER) {
                        decInstr.opcode = VM::InstructionType::DIV;
                    }
                    if (objType == VM::BasicObjectType::FLOATING) {
                        decInstr.opcode = VM::InstructionType::DIVF;
                    }
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
        m_registerToStore = m_factorNode->getRegister();
    }
}

void SummandNode::print(size_t printLevel) {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }

    printf("SummandNode: (FactorNode) ");
    if (m_summandNode) {
        printf("(operation = %d) (SummandNode)\n", m_operation);
        m_factorNode->print(printLevel + 1);
        m_summandNode->print(printLevel + 1);
    }
    else {
        putchar('\n');
        m_factorNode->print(printLevel + 1);
    }
}

uint32_t SummandNode::getRegister() {
    return m_registerToStore;
}

VM::BasicObjectType SummandNode::getType() {
    return m_factorNode->getType();
}

SummandNode::SummandNode(FactorNode *factorNode) {
    m_factorNode = factorNode;
}

SummandNode::SummandNode(FactorNode *factorNode, SummandNode *summandNode, HighPriorityOperation operation) {
    m_factorNode = factorNode;
    m_summandNode = summandNode;
    m_operation = operation;
}

SummandNode::~SummandNode() {
    delete m_factorNode;
    delete m_summandNode;
}

// ================================================================================

void SimpleNode::generateCode(CodeGenContext *ctx) {
    m_summandNode->generateCode(ctx);

    if (m_simpleNode) {
        m_simpleNode->generateCode(ctx);        

        auto &program = ctx->program;
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
            
            decInstr.r1 = m_simpleNode->getRegister();
            VM::BasicObjectType objType = m_summandNode->getType();
            switch (m_operation) {
                case LowPriorityOperation::LOW_PRIORITY_OPERATION_ADD: {
                    if (objType == VM::BasicObjectType::INTEGER) {
                        decInstr.opcode = VM::InstructionType::ADD;
                    }
                    if (objType == VM::BasicObjectType::FLOATING) {
                        decInstr.opcode = VM::InstructionType::ADDF;
                    }
                    break;
                }
                case LowPriorityOperation::LOW_PRIORITY_OPERATION_SUB: {
                    if (objType == VM::BasicObjectType::INTEGER) {
                        decInstr.opcode = VM::InstructionType::SUB;
                    }
                    if (objType == VM::BasicObjectType::FLOATING) {
                        decInstr.opcode = VM::InstructionType::SUBF;
                    }
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
        m_registerToStore = m_summandNode->getRegister();
    }
}

void SimpleNode::print(size_t printLevel) {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }

    printf("SimpleNode: (SummandNode) ");
    if (m_simpleNode) {
        printf("(operation = %d) (SimpleNode)\n", m_operation);
        m_summandNode->print(printLevel + 1);
        m_simpleNode->print(printLevel + 1);
    }
    else {
        putchar('\n');
        m_summandNode->print(printLevel + 1);
    }
}

uint32_t SimpleNode::getRegister() {
    return m_registerToStore;    
}

VM::BasicObjectType SimpleNode::getType() {
    return m_summandNode->getType();
}

SimpleNode::SimpleNode(SummandNode *summandNode) {
    m_summandNode = summandNode;
}

SimpleNode::SimpleNode(SummandNode *summandNode, SimpleNode *simpleNode, LowPriorityOperation operation) {
    m_summandNode = summandNode;
    m_simpleNode = simpleNode;
    m_operation = operation;
}

SimpleNode::~SimpleNode() {
    delete m_summandNode;
    delete m_simpleNode;
}

// ================================================================================

void ExpressionNode::generateCode(CodeGenContext *ctx) {
    m_simpleNode->generateCode(ctx);
    m_registerToStore = m_simpleNode->getRegister();

    if (m_expressionNode) {
        m_expressionNode->generateCode(ctx);

        auto &program = ctx->program;
        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = m_simpleNode->getRegister();
            decInstr.opcode = VM::InstructionType::LOAD_ACC;

            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }
    }
}

void ExpressionNode::print(size_t printLevel) {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }

    printf("ExpressionNode: (SimpleNode) ");
    if (m_expressionNode) {
        printf("(operation = %d) (ExpressionNode)\n", m_opeation);
        m_simpleNode->print(printLevel + 1);
        m_expressionNode->print(printLevel + 1);
    }
    else {
        putchar('\n');
        m_simpleNode->print(printLevel + 1);
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

uint32_t ExpressionNode::getExpressionNodeRegister() {
    return m_expressionNode->getRegister();
}

VM::BasicObjectType ExpressionNode::getType() {
    return m_simpleNode->getType();
}

ExpressionNode::ExpressionNode(SimpleNode *simpleNode) {
    m_simpleNode = simpleNode;
}

ExpressionNode::ExpressionNode(SimpleNode *simpleNode, ExpressionNode *expressionNode, ExpressionOperation operation) {
    m_simpleNode = simpleNode;
    m_expressionNode = expressionNode;
    m_opeation = operation;
}

ExpressionNode::~ExpressionNode() {
    delete m_simpleNode;
    delete m_expressionNode;
}

// ================================================================================

void AssignmentStatement::generateCode(CodeGenContext *ctx) {
    auto *currentFrame = ctx->currentFrame;
    auto &globalData = ctx->globalData;
    auto &program = ctx->program;

    if (m_valueNode->isArray()) {
        // Array
        m_expressionNode->generateCode(ctx);

        uint32_t arrayPtrReg = 0;
        VM::RegisterType indexReg = m_valueNode->generateArrayIndex(ctx);
        if (currentFrame) {
            auto arrayIt = currentFrame->IDs_Regs.find(m_valueNode->getName());
            if (arrayIt == currentFrame->IDs_Regs.end()) {
                // Error, undeclared identifier
            }
            arrayPtrReg = arrayIt->second;
        }
        else {
            auto arrayIt = globalData.IDs_Regs.find(m_valueNode->getName());
            if (arrayIt == globalData.IDs_Regs.end()) {
                // Error, undeclared identifier
            }
            arrayPtrReg = arrayIt->second;
        }

        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = arrayPtrReg;
            decInstr.opcode = VM::InstructionType::LOAD_ACC;

            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }
        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = m_expressionNode->getRegister();
            decInstr.r2 = indexReg;
            decInstr.opcode = VM::InstructionType::STORE_ARR_ELEM;

            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }
    }
    else {
        m_valueNode->generateCode(ctx);
        m_expressionNode->generateCode(ctx);

        {
            VM::DecodedInstruction decInstr;
            VM::EncodedInstruction encInstr;

            decInstr.r1 = m_valueNode->getRegister();
            decInstr.r2 = m_expressionNode->getRegister();
            decInstr.opcode = VM::InstructionType::MV;

            ctx->encodeInstruction(decInstr, encInstr);
            program.instructions.push_back(encInstr);
        }
    }
}

void AssignmentStatement::print(size_t printLevel) {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }

    printf("AssignmentStatement: (VariableValueNode) (ExpressionNode)\n");
    m_valueNode->print(printLevel + 1);
    m_expressionNode->print(printLevel + 1);
}

AssignmentStatement::AssignmentStatement(VariableValueNode *valueNode, ExpressionNode *expressionNode) {
    m_valueNode = valueNode;
    m_expressionNode = expressionNode;
}

AssignmentStatement::~AssignmentStatement() {
    delete m_valueNode;
    delete m_expressionNode;
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

void ReturnStatementNode::print(size_t printLevel) {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }

    printf("ReturnStatementNode: ");
    if (m_expressionNode) {
        printf("(ExpressionNode)\n");
        m_expressionNode->print(printLevel + 1);
    }
    else {
        putchar('\n');
    }
}

ReturnStatementNode::ReturnStatementNode(ExpressionNode *expressionNode) {
    m_expressionNode = expressionNode;
}

ReturnStatementNode::~ReturnStatementNode() {
    delete m_expressionNode;
}

// ================================================================================

void StatementsScopeNode::generateCode(CodeGenContext *ctx) {
    for (int i = 0; i < m_scopeStatements.size(); ++i) {
        m_scopeStatements[i]->generateCode(ctx);
    }
}

void StatementsScopeNode::print(size_t printLevel) {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }

    printf("StatementsScopeNode: (statements)\n");
    for (auto node : m_scopeStatements) {
        node->print(printLevel + 1);
    }
}

StatementsScopeNode::StatementsScopeNode(std::vector<ASTNode *> &scopeStatements) {
    std::copy(scopeStatements.begin(), scopeStatements.end(), std::back_inserter(m_scopeStatements));
}

StatementsScopeNode::~StatementsScopeNode() {
    for (auto node : m_scopeStatements) {
        delete node;
    }
    m_scopeStatements.clear();
}

// ================================================================================

void FunctionDeclarationNode::generateCode(CodeGenContext *ctx) {
    ctx->globalData.functions_PC.insert({m_name, ctx->globalData.pc});
    ctx->globalData.functionReturnTypes.insert({m_name, m_returnType});
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

    for (auto &id : currentFrame->currentScope->scopedIDs) {
        auto it = currentFrame->IDs_Regs.find(id);
        if (it != currentFrame->IDs_Regs.end()) {
            currentFrame->identifierTypes.erase(currentFrame->identifierTypes.find(id));
            currentFrame->IDs_Regs.erase(it);
            ctx->freeRegsiter(it->second);
        }
    }

    delete currentFrame->currentScope;
    delete currentFrame;
}

void FunctionDeclarationNode::print(size_t printLevel) {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }

    printf("FunctionDeclarationNode: (returnType = %d) (name = %s) (args) (body)\n", m_returnType, m_name.c_str());
    for (auto node : m_arguments) {
        node->print(printLevel + 1);
    }
    m_body->print(printLevel + 1);
}

FunctionDeclarationNode::FunctionDeclarationNode(ReturnType returnType,
                                                 const std::string &name,
                                                 std::vector<SimpleVariableDeclarationNode *> &arguments,
                                                 StatementsScopeNode *body) 
{
    m_returnType = returnType;
    std::copy(arguments.begin(), arguments.end(), std::back_inserter(m_arguments));
    m_body = body;
    m_name = name;
}

FunctionDeclarationNode::~FunctionDeclarationNode() {
    for (auto node : m_arguments) {
        delete node;
    }
    m_arguments.clear();

    delete m_body;
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

        decInstr.r1 = m_expressionNode->getExpressionNodeRegister();

        ExpressionOperation operation = m_expressionNode->getOperation();
        switch (operation) {
            // Inverse operations
            case ExpressionOperation::EXPRESSION_OPERATION_LESS: {
                if (m_expressionNode->getType() == VM::BasicObjectType::INTEGER) {
                    decInstr.opcode = VM::InstructionType::BGE;
                }
                if (m_expressionNode->getType() == VM::BasicObjectType::FLOATING) {
                    decInstr.opcode = VM::InstructionType::BGEF;
                }
                break;
            }
            case ExpressionOperation::EXPRESSION_OPERATION_LESS_OR_EQ: {
                if (m_expressionNode->getType() == VM::BasicObjectType::INTEGER) {
                    decInstr.opcode = VM::InstructionType::BGT;
                }
                if (m_expressionNode->getType() == VM::BasicObjectType::FLOATING) {
                    decInstr.opcode = VM::InstructionType::BGTF;
                }
                break;
            }
            case ExpressionOperation::EXPRESSION_OPERATION_GREATER: {
                if (m_expressionNode->getType() == VM::BasicObjectType::INTEGER) {
                    decInstr.opcode = VM::InstructionType::BLE;
                }
                if (m_expressionNode->getType() == VM::BasicObjectType::FLOATING) {
                    decInstr.opcode = VM::InstructionType::BLEF;
                }
                break;
            }
            case ExpressionOperation::EXPRESSION_OPERATION_GREATER_OR_EQ: {
                if (m_expressionNode->getType() == VM::BasicObjectType::INTEGER) {
                    decInstr.opcode = VM::InstructionType::BLT;
                }
                if (m_expressionNode->getType() == VM::BasicObjectType::FLOATING) {
                    decInstr.opcode = VM::InstructionType::BLTF;
                }
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

    for (auto &id : currentFrame->currentScope->scopedIDs) {
        auto it = currentFrame->IDs_Regs.find(id);
        if (it != currentFrame->IDs_Regs.end()) {
            currentFrame->identifierTypes.erase(currentFrame->identifierTypes.find(id));
            currentFrame->IDs_Regs.erase(it);
            ctx->freeRegsiter(it->second);
        }
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

        for (auto &id : currentFrame->currentScope->scopedIDs) {
            auto it = currentFrame->IDs_Regs.find(id);
            if (it != currentFrame->IDs_Regs.end()) {
                currentFrame->identifierTypes.erase(currentFrame->identifierTypes.find(id));
                currentFrame->IDs_Regs.erase(it);
                ctx->freeRegsiter(it->second);
            }
        }

        currentFrame->currentScope = outerScope;
        delete falseScope;
    }
}

void IfStatementNode::print(size_t printLevel) {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }

    printf("IfStatementNode: (ExpressionNode) (true StatementsScopeNode) ");
    if (m_falseBody) {
        printf("(false StatementsScopeNode)\n");
        m_expressionNode->print(printLevel + 1);
        m_trueBody->print(printLevel + 1);
        m_falseBody->print(printLevel + 1);
    }
    else {
        putchar('\n');
        m_expressionNode->print(printLevel + 1);
        m_trueBody->print(printLevel + 1);
    }
}

IfStatementNode::IfStatementNode(ExpressionNode *expressionNode, StatementsScopeNode *trueBody, StatementsScopeNode *falseBody) {
    m_expressionNode = expressionNode;
    m_trueBody = trueBody;
    m_falseBody = falseBody;
}

IfStatementNode::~IfStatementNode() {
    delete m_expressionNode;
    delete m_trueBody;
    delete m_falseBody;
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

        decInstr.r1 = m_expressionNode->getExpressionNodeRegister();

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

    for (auto &id : currentFrame->currentScope->scopedIDs) {
        auto it = currentFrame->IDs_Regs.find(id);
        if (it != currentFrame->IDs_Regs.end()) {
            currentFrame->identifierTypes.erase(currentFrame->identifierTypes.find(id));
            currentFrame->IDs_Regs.erase(it);
            ctx->freeRegsiter(it->second);
        }
    }

    currentFrame->currentScope = outerScope;
    delete bodyScope;
}

void WhileLoopStatementNode::print(size_t printLevel)  {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }

    printf("WhileLoopStatementNode: (ExpressionNode) (StatementsScopeNode)\n");
    m_expressionNode->print(printLevel + 1);
    m_body->print(printLevel + 1);
}

WhileLoopStatementNode::WhileLoopStatementNode(ExpressionNode *expressionNode, StatementsScopeNode *body) {
    m_expressionNode = expressionNode;
    m_body = body;
}

WhileLoopStatementNode::~WhileLoopStatementNode() {
    delete m_expressionNode;
    delete m_body;
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

        decInstr.r1 = m_expressionNode->getExpressionNodeRegister();

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

    for (auto &id : currentFrame->currentScope->scopedIDs) {
        auto it = currentFrame->IDs_Regs.find(id);
        if (it != currentFrame->IDs_Regs.end()) {
            currentFrame->identifierTypes.erase(currentFrame->identifierTypes.find(id));
            currentFrame->IDs_Regs.erase(it);
            ctx->freeRegsiter(it->second);
        }
    }

    currentFrame->currentScope = outerScope;
    delete bodyScope;
}

void ForLoopStatementNode::print(size_t printLevel) {
    for (int i = 0; i < printLevel; ++i) {
        printf("  ");
    }

    printf("ForLoopStatementNode: (pre loop statement) (ExpressionNode) (StatementsScopeNode) (post loop statement)\n");
    m_preLoopStatement->print(printLevel + 1);
    m_expressionNode->print(printLevel + 1);
    m_body->print(printLevel + 1);
    m_postLoopStatement->print(printLevel + 1);
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

ForLoopStatementNode::~ForLoopStatementNode() {
    delete m_preLoopStatement;
    delete m_expressionNode;
    delete m_body;
    delete m_postLoopStatement;
}


}
