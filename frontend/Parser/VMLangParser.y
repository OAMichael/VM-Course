%{

#include <iostream>
#include <vector>

#include "AST.h"

#pragma GCC diagnostic ignored "-Wwrite-strings"

using namespace AST;

#define YYSTYPE ASTNode *

extern FILE *yyin;
extern "C" {
    int yyparse();
    int yylex();
    void yyerror(char *s) {
        std::cerr << s << "\n";
    }
    int yywrap(void) { return 1; }
}



ProgramNode *rootNode = new ProgramNode();
CodeGenContext codegenCtx;
std::vector<std::string> tokens;

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <filename> [-v]\n", argv[0]);
        return 0;
    }

    bool verbose = false;
    if (argc > 2 && std::string(argv[2]) == "-v") {
        verbose = true;
    }

    yyin = fopen(argv[1], "r");
    yyparse();

    rootNode->generateCode(&codegenCtx);

    if (verbose) {
        std::cout << "Parsed tokens:" << std::endl;
        for (size_t i = 0; i < tokens.size(); ++i) {
            std::cout << "   " <<  tokens[i] << std::endl;
        }

        std::cout << "\n\nBuilt AST:" << std::endl;
        rootNode->print(0);

        std::cout << "\n\nGenerated program:" << std::endl;
        for (auto instr : codegenCtx.program.instructions) {
            VM::DecodedInstruction decInstr;
            codegenCtx.builder.decodeInstruction(instr, decInstr);
            std::cout << "   ";
            codegenCtx.builder.printInstruction(decInstr);
            std::cout << std::endl;
        }
    }

    delete rootNode;

    std::string outFilename = argv[1];
    size_t lastIndex = outFilename.find_last_of(".");
    if (lastIndex != outFilename.npos)
        outFilename = outFilename.substr(0, lastIndex);

    outFilename += ".prog";

    Common::serializeProgram(outFilename, codegenCtx.program);

    std::cout << "Successfully translated the program: " << argv[1] << std::endl;
    return 0;
}

%}

%token IntLiteral
%token FloatLiteral
%token StringLiteral
%token Identifier
%token Mul
%token Div
%token Add
%token Sub
%token ShiftLeft
%token ShiftRight
%token Less
%token LessOrEq
%token Greater
%token GreaterOrEq
%token Equal
%token NotEqual
%token Assignment
%token LogicAnd
%token LogicOr
%token BitwiseAnd
%token BitwiseOr
%token BitwiseXor
%token LeftParent
%token RightParent
%token LeftBracket
%token RightBracket
%token LeftBrace
%token RightBrace
%token Semicolon
%token Comma
%token Comment
%token Whitespace
%token IntType
%token FloatType
%token StringType
%token VoidType
%token IfKeyword
%token ElseKeyword
%token ForKeyword
%token WhileKeyword
%token FunctionKeyword
%token ReturnKeyword
%token PrintKeyword
%token PrintfKeyword
%token PrintsKeyword
%token ScanKeyword
%token ScanfKeyword
%token ScansKeyword
%token SqrtKeyword

%%

Parse: Program { YYACCEPT; }


Program:                        FunctionDeclaration {
                                    rootNode->insertNode($1);
                                }
                                | Statement {
                                    rootNode->insertNode($1);
                                }
                                | FunctionDeclaration Program {
                                    rootNode->insertNode($1);
                                }
                                | Statement Program {
                                    rootNode->insertNode($1);
                                }




SimpleIntDeclaration:           IntType Identifier {
                                    $$ = new SimpleVariableDeclarationNode((char*)$2, VM::BasicObjectType::INTEGER);
                                }
InitIntDeclaration:             IntType Identifier Assignment Expression {
                                    $$ = new InitVariableDeclarationNode((char*)$2, VM::BasicObjectType::INTEGER, dynamic_cast<ExpressionNode *>($4));
                                }
ArrayIntDeclaration:            IntType Identifier LeftBracket IntLiteral RightBracket {
                                    $$ = new ArrayVariableDeclarationNode((char*)$2, VM::BasicObjectType::INTEGER, atoi((char*)$4));
                                }

IntVariableDeclaration:         InitIntDeclaration {}
                                | ArrayIntDeclaration {}
                                | SimpleIntDeclaration {}




SimpleFloatDeclaration:         FloatType Identifier {
                                    $$ = new SimpleVariableDeclarationNode((char*)$2, VM::BasicObjectType::FLOATING);
                                }
InitFloatDeclaration:           FloatType Identifier Assignment Expression {
                                    $$ = new InitVariableDeclarationNode((char*)$2, VM::BasicObjectType::FLOATING, dynamic_cast<ExpressionNode *>($4));
                                }
ArrayFloatDeclaration:          FloatType Identifier LeftBracket IntLiteral RightBracket {
                                    $$ = new ArrayVariableDeclarationNode((char*)$2, VM::BasicObjectType::FLOATING, atoi((char*)$4));
                                }

FloatVariableDeclaration:       InitFloatDeclaration {}
                                | ArrayFloatDeclaration {}
                                | SimpleFloatDeclaration {}




InitStringDeclaration:          StringType Identifier Assignment Expression {
                                    $$ = new InitVariableDeclarationNode((char*)$2, VM::BasicObjectType::STRING, dynamic_cast<ExpressionNode *>($4));
                                }

StringVariableDeclaration:      InitStringDeclaration {}




SimpleVariableDeclaration:      SimpleIntDeclaration {}
                                | SimpleFloatDeclaration {}

VariableDeclaration:            IntVariableDeclaration {}
                                | FloatVariableDeclaration {}
                                | StringVariableDeclaration {}

VariableValue:                  Identifier LeftBracket Expression RightBracket {
                                    $$ = new VariableValueNode((char*)$1, dynamic_cast<ExpressionNode *>($3));
                                }
                                | Identifier {
                                    $$ = new VariableValueNode((char*)$1);
                                }




ReturnType:                     IntType {}
                                | FloatType {}
                                | StringType {}
                                | VoidType {}

FunctionArgsDeclarations:       SimpleVariableDeclaration Comma FunctionArgsDeclarations {
                                    codegenCtx.currentFuncArgs.insert(codegenCtx.currentFuncArgs.begin(), dynamic_cast<SimpleVariableDeclarationNode *>($1));
                                }
                                | SimpleVariableDeclaration {
                                    codegenCtx.currentFuncArgs.insert(codegenCtx.currentFuncArgs.begin(), dynamic_cast<SimpleVariableDeclarationNode *>($1));
                                }
                                | %empty {}

FunctionArgsExpressions:        Expression Comma FunctionArgsExpressions {
                                    codegenCtx.currentCalleeArgs.insert(codegenCtx.currentCalleeArgs.begin(), dynamic_cast<ExpressionNode *>($1));
                                }
                                | Expression {
                                    codegenCtx.currentCalleeArgs.insert(codegenCtx.currentCalleeArgs.begin(), dynamic_cast<ExpressionNode *>($1));
                                }
                                | %empty {}

FunctionDeclaration:            ReturnType FunctionKeyword Identifier LeftParent FunctionArgsDeclarations RightParent StatementsScope {
                                    ReturnType returnType = ReturnType::RETURN_TYPE_INT;
                                    std::string returnTypeStr = (char*)$1;
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

                                    $$ = new FunctionDeclarationNode(returnType, (char*)$3, codegenCtx.currentFuncArgs, dynamic_cast<StatementsScopeNode *>($7));
                                    codegenCtx.currentFuncArgs.clear();
                                }




IfStatement:                    IfKeyword LeftParent Expression RightParent StatementsScope ElseKeyword StatementsScope {
                                    $$ = new IfStatementNode(dynamic_cast<ExpressionNode *>($3),
                                                             dynamic_cast<StatementsScopeNode *>($5),
                                                             dynamic_cast<StatementsScopeNode *>($7));
                                }
                                | IfKeyword LeftParent Expression RightParent StatementsScope {
                                    $$ = new IfStatementNode(dynamic_cast<ExpressionNode *>($3),
                                                             dynamic_cast<StatementsScopeNode *>($5));
                                }

WhileLoopStatement:             WhileKeyword LeftParent Expression RightParent StatementsScope {
                                    $$ = new WhileLoopStatementNode(dynamic_cast<ExpressionNode *>($3),
                                                                    dynamic_cast<StatementsScopeNode *>($5));
                                }

ForLoopStatement:               ForKeyword LeftParent Statement Semicolon Expression Semicolon Statement RightParent StatementsScope {
                                    $$ = new ForLoopStatementNode($3, dynamic_cast<ExpressionNode *>($5), dynamic_cast<StatementsScopeNode *>($9), $7);
                                }

FunctionCall:                   Identifier LeftParent FunctionArgsExpressions RightParent {
                                    $$ = new FunctionCallNode((char*)$1, codegenCtx.currentCalleeArgs);
                                    codegenCtx.currentCalleeArgs.clear();
                                }

AssigmentStatement:             VariableValue Assignment Expression {
                                    $$ = new AssignmentStatement(dynamic_cast<VariableValueNode *>($1), dynamic_cast<ExpressionNode *>($3));
                                }

ReturnStatement:                ReturnKeyword Expression {
                                    $$ = new ReturnStatementNode(dynamic_cast<ExpressionNode *>($2));
                                }
                                | ReturnKeyword {
                                    $$ = new ReturnStatementNode();
                                }

PrintStatement:                 PrintKeyword LeftParent Expression RightParent {
                                    $$ = new PrintStatementNode(VM::BasicObjectType::INTEGER, dynamic_cast<ExpressionNode *>($3));
                                }

ScanStatement:                  ScanKeyword LeftParent RightParent {
                                    $$ = new ScanStatementNode(VM::BasicObjectType::INTEGER);
                                }

PrintfStatement:                PrintfKeyword LeftParent Expression RightParent {
                                    $$ = new PrintStatementNode(VM::BasicObjectType::FLOATING, dynamic_cast<ExpressionNode *>($3));
                                }

ScanfStatement:                 ScanfKeyword LeftParent RightParent {
                                    $$ = new ScanStatementNode(VM::BasicObjectType::FLOATING);
                                }

PrintsStatement:                PrintsKeyword LeftParent Expression RightParent {
                                    $$ = new PrintStatementNode(VM::BasicObjectType::STRING, dynamic_cast<ExpressionNode *>($3));
                                }

ScansStatement:                 ScansKeyword LeftParent RightParent {
                                    $$ = new ScanStatementNode(VM::BasicObjectType::STRING);
                                }

SqrtStatement:                  SqrtKeyword LeftParent Expression RightParent {
                                    $$ = new SqrtStatementNode(dynamic_cast<ExpressionNode *>($3));
                                }


Statement:                      VariableDeclaration {}
                                | AssigmentStatement {}
                                | IfStatement {}
                                | WhileLoopStatement {}
                                | ForLoopStatement {}
                                | FunctionCall {}
                                | ReturnStatement {}
                                | PrintStatement {}
                                | ScanStatement {}
                                | PrintfStatement {}
                                | ScanfStatement {}
                                | PrintsStatement {}
                                | ScansStatement {}

StatementList:                  Statement StatementList {
                                    codegenCtx.scopeStatements.insert(codegenCtx.scopeStatements.begin(), $1);
                                }
                                | %empty {}

StatementsScopeBegin:           LeftBrace {
                                    codegenCtx.scopeStatementsStack.push(codegenCtx.scopeStatements);
                                    codegenCtx.scopeStatements.clear();
                                }

StatementsScope:                StatementsScopeBegin StatementList RightBrace {
                                    $$ = new StatementsScopeNode(codegenCtx.scopeStatements);
                                    codegenCtx.scopeStatements.clear();
                                    codegenCtx.scopeStatements = codegenCtx.scopeStatementsStack.top();
                                    codegenCtx.scopeStatementsStack.pop();
                                }




HighPriorityOperation:          Mul {}
                                | Div {}

LowPriorityOperation:           Add {}
                                | Sub {}
                                | ShiftLeft {}
                                | ShiftRight {}
                                | BitwiseAnd {}
                                | BitwiseOr {}
                                | BitwiseXor {}

ExpressionOperation:            Less {}
                                | LessOrEq {}
                                | Greater {}
                                | GreaterOrEq {}
                                | Equal {}
                                | NotEqual {}
                                | LogicAnd {}
                                | LogicOr {}


Primary:                        IntLiteral {
                                    $$ = new PrimaryNode(atoi((char*)$1));
                                }
                                | FloatLiteral {
                                    $$ = new PrimaryNode(std::stod((char*)$1));
                                }
                                | StringLiteral {
                                    $$ = new PrimaryNode((char*)$1);
                                }
                                | VariableValue {
                                    $$ = new PrimaryNode(dynamic_cast<VariableValueNode *>($1));
                                }
                                | FunctionCall {
                                    $$ = new PrimaryNode(dynamic_cast<FunctionCallNode *>($1));
                                }
                                | ScanStatement {
                                    $$ = new PrimaryNode(dynamic_cast<ScanStatementNode *>($1));
                                }
                                | ScanfStatement {
                                    $$ = new PrimaryNode(dynamic_cast<ScanStatementNode *>($1));
                                }
                                | ScansStatement {
                                    $$ = new PrimaryNode(dynamic_cast<ScanStatementNode *>($1));
                                }
                                | SqrtStatement{
                                    $$ = new PrimaryNode(dynamic_cast<SqrtStatementNode *>($1));
                                }

Factor:                         Primary {
                                    $$ = new FactorNode(dynamic_cast<PrimaryNode *>($1));
                                }
                                | Sub Primary {
                                    $$ = new FactorNode(dynamic_cast<PrimaryNode *>($2), true);
                                }
                                | LeftParent Expression RightParent {
                                    $$ = new FactorNode(dynamic_cast<ExpressionNode *>($2));
                                }


Summand:                        Factor {
                                    $$ = new SummandNode(dynamic_cast<FactorNode *>($1));
                                }
                                | Factor HighPriorityOperation Summand {
                                    HighPriorityOperation operation = HighPriorityOperation::HIGH_PRIORITY_OPERATION_NONE;

                                    std::string operationStr = (char*)$2;
                                    if (operationStr == "*") {
                                        operation = HighPriorityOperation::HIGH_PRIORITY_OPERATION_MUL;
                                    }
                                    else if (operationStr == "/") {
                                        operation = HighPriorityOperation::HIGH_PRIORITY_OPERATION_DIV;
                                    }

                                    $$ = new SummandNode(dynamic_cast<FactorNode *>($1), dynamic_cast<SummandNode *>($3), operation);
                                }


Simple:                         Summand {
                                    $$ = new SimpleNode(dynamic_cast<SummandNode *>($1));
                                }
                                | Summand LowPriorityOperation Simple {
                                    LowPriorityOperation operation = LowPriorityOperation::LOW_PRIORITY_OPERATION_NONE;

                                    std::string operationStr = (char*)$2;
                                    if (operationStr == "+") {
                                        operation = LowPriorityOperation::LOW_PRIORITY_OPERATION_ADD;
                                    }
                                    else if (operationStr == "-") {
                                        operation = LowPriorityOperation::LOW_PRIORITY_OPERATION_SUB;
                                    }
                                    else if (operationStr == "<<") {
                                        operation = LowPriorityOperation::LOW_PRIORITY_OPERATION_SHIFT_LEFT;
                                    }
                                    else if (operationStr == ">>") {
                                        operation = LowPriorityOperation::LOW_PRIORITY_OPERATION_SHIFT_RIGHT;
                                    }
                                    else if (operationStr == "&") {
                                        operation = LowPriorityOperation::LOW_PRIORITY_OPERATION_BITWISE_AND;
                                    }
                                    else if (operationStr == "|") {
                                        operation = LowPriorityOperation::LOW_PRIORITY_OPERATION_BITWISE_OR;
                                    }
                                    else if (operationStr == "^") {
                                        operation = LowPriorityOperation::LOW_PRIORITY_OPERATION_BITWISE_XOR;
                                    }

                                    $$ = new SimpleNode(dynamic_cast<SummandNode *>($1), dynamic_cast<SimpleNode *>($3), operation);
                                }


Expression:                     Simple {
                                    $$ = new ExpressionNode(dynamic_cast<SimpleNode *>($1));
                                }
                                | Simple ExpressionOperation Expression {
                                    ExpressionOperation operation = ExpressionOperation::EXPRESSION_OPERATION_NONE;
                                    
                                    std::string operationStr = (char*)$2;
                                    if (operationStr == "<") {
                                        operation = ExpressionOperation::EXPRESSION_OPERATION_LESS;
                                    }
                                    else if (operationStr == "<=") {
                                        operation = ExpressionOperation::EXPRESSION_OPERATION_LESS_OR_EQ;
                                    }
                                    else if (operationStr == ">") {
                                        operation = ExpressionOperation::EXPRESSION_OPERATION_GREATER;
                                    }
                                    else if (operationStr == ">=") {
                                        operation = ExpressionOperation::EXPRESSION_OPERATION_GREATER_OR_EQ;
                                    }
                                    else if (operationStr == "==") {
                                        operation = ExpressionOperation::EXPRESSION_OPERATION_EQUAL;
                                    }
                                    else if (operationStr == "!=") {
                                        operation = ExpressionOperation::EXPRESSION_OPERATION_NOT_EQUAL;
                                    }
                                    else if (operationStr == "&&") {
                                        operation = ExpressionOperation::EXPRESSION_OPERATION_LOGIC_AND;
                                    }
                                    else if (operationStr == "||") {
                                        operation = ExpressionOperation::EXPRESSION_OPERATION_LOGIC_OR;
                                    }

                                    $$ = new ExpressionNode(dynamic_cast<SimpleNode *>($1), dynamic_cast<ExpressionNode *>($3), operation);
                                }

%%