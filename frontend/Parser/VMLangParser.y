%{

#include <iostream>
#include "AST.h"

#pragma GCC diagnostic ignored "-Wwrite-strings"

using namespace AST;

#define YYSTYPE ASTNode *

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

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 0;
    }

    yyparse();
    rootNode->generateCode(&codegenCtx);

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

%}

%token IntLiteral
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
%token VoidType
%token IfKeyword
%token ElseKeyword
%token ForKeyword
%token WhileKeyword
%token FunctionKeyword
%token ReturnKeyword
%token PrintKeyword
%token ScanKeyword

%%

Parse: Program { YYACCEPT; }


Program:                        FunctionDeclaration {
                                    rootNode->insertNode($1);
                                }
                                | Statement {
                                    rootNode->insertNode($1);
                                }
                                | Program FunctionDeclaration {
                                    rootNode->insertNode($2);
                                }
                                | Program Statement {
                                    rootNode->insertNode($2);
                                }
                                | %empty {}




SimpleIntDeclaration:           IntType Identifier {
                                    $$ = new SimpleIntDeclarationNode((char*)$2);
                                }
InitIntDeclaration:             IntType Identifier Assignment Expression {
                                    $$ = new InitIntDeclarationNode((char*)$2, dynamic_cast<ExpressionNode *>($4));
                                }
ArrayIntDeclaration:            IntType Identifier LeftBracket IntLiteral RightBracket {
                                    $$ = new ArrayIntDeclarationNode((char*)$2, atoi((char*)$4));
                                }


IntVariableDeclaration:         InitIntDeclaration {
                                    $$ = $1;
                                }
                                | ArrayIntDeclaration {
                                    $$ = $1;
                                }
                                | SimpleIntDeclaration {
                                    $$ = $1;
                                }

IntValue:                       Identifier LeftBracket Expression RightBracket {
                                    $$ = new IntValueNode((char*)$1, dynamic_cast<ExpressionNode *>($3));
                                }
                                | Identifier {
                                    $$ = new IntValueNode((char*)$1);
                                }




ReturnType:                     IntType {}
                                | VoidType {}

FunctionArgsDeclarations:       FunctionArgsDeclarations Comma SimpleIntDeclaration {
                                    codegenCtx.currentFuncArgs.push_back(dynamic_cast<SimpleIntDeclarationNode *>($3));
                                }
                                | SimpleIntDeclaration {
                                    codegenCtx.currentFuncArgs.push_back(dynamic_cast<SimpleIntDeclarationNode *>($1));
                                }
                                | %empty {}

FunctionArgsExpressions:        FunctionArgsExpressions Comma Expression {
                                    codegenCtx.currentCalleeArgs.push_back(dynamic_cast<ExpressionNode *>($3));
                                }
                                | Expression {
                                    codegenCtx.currentCalleeArgs.push_back(dynamic_cast<ExpressionNode *>($1));
                                }
                                | %empty {}

FunctionDeclaration:            ReturnType FunctionKeyword Identifier LeftParent FunctionArgsDeclarations RightParent StatementsScope {
                                    $$ = new FunctionDeclarationNode((char*)$3, codegenCtx.currentFuncArgs, dynamic_cast<StatementsScopeNode *>($7));
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

AssigmentStatement:             IntValue Assignment Expression {
                                    $$ = new AssignmentStatement(dynamic_cast<IntValueNode *>($1), dynamic_cast<ExpressionNode *>($3));
                                }

ReturnStatement:                ReturnKeyword Expression {
                                    $$ = new ReturnStatementNode(dynamic_cast<ExpressionNode *>($2));
                                }
                                | ReturnKeyword {
                                    $$ = new ReturnStatementNode();
                                }

PrintStatement:                 PrintKeyword LeftParent Expression RightParent {
                                    $$ = new PrintStatementNode(dynamic_cast<ExpressionNode *>($3));
                                }

ScanStatement:                  ScanKeyword LeftParent RightParent {
                                    $$ = new ScanStatementNode();
                                }


Statement:                      IntVariableDeclaration {
                                    $$ = $1;
                                }
                                | AssigmentStatement {
                                    $$ = $1;
                                }
                                | IfStatement {
                                    $$ = $1;
                                }
                                | WhileLoopStatement {
                                    $$ = $1;
                                }
                                | ForLoopStatement {
                                    $$ = $1;
                                }
                                | PrintStatement {
                                    $$ = $1;
                                }
                                | FunctionCall {
                                    $$ = $1;
                                }
                                | ReturnStatement {
                                    $$ = $1;
                                }
                                | ScanStatement {
                                    $$ = $1;
                                }

StatementList:                  StatementList Statement {
                                    codegenCtx.scopeStatements.push_back($2);
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
                                | IntValue {
                                    $$ = new PrimaryNode(dynamic_cast<IntValueNode *>($1));
                                }
                                | FunctionCall {
                                    $$ = new PrimaryNode(dynamic_cast<FunctionCallNode *>($1));
                                }
                                | ScanStatement {
                                    $$ = new PrimaryNode(dynamic_cast<ScanStatementNode *>($1));
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
                                | Summand HighPriorityOperation Factor {
                                    HighPriorityOperation operation = HighPriorityOperation::HIGH_PRIORITY_OPERATION_NONE;

                                    std::string operationStr = (char*)$2;
                                    if (operationStr == "*") {
                                        operation = HighPriorityOperation::HIGH_PRIORITY_OPERATION_MUL;
                                    }
                                    else if (operationStr == "/") {
                                        operation = HighPriorityOperation::HIGH_PRIORITY_OPERATION_DIV;
                                    }

                                    $$ = new SummandNode(dynamic_cast<SummandNode *>($1), dynamic_cast<FactorNode *>($3), operation);
                                }


Simple:                         Summand {
                                    $$ = new SimpleNode(dynamic_cast<SummandNode *>($1));
                                }
                                | Simple LowPriorityOperation Summand {
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

                                    $$ = new SimpleNode(dynamic_cast<SimpleNode *>($1), dynamic_cast<SummandNode *>($3), operation);
                                }


Expression:                     Simple {
                                    $$ = new ExpressionNode(dynamic_cast<SimpleNode *>($1));
                                }
                                | Expression ExpressionOperation Simple {
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
                                    
                                    $$ = new ExpressionNode(dynamic_cast<ExpressionNode *>($1), dynamic_cast<SimpleNode *>($3), operation);
                                }

%%