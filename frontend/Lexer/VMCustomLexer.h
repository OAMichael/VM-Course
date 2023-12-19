#ifndef VM_CUSTOM_LEXEL_H
#define VM_CUSTOM_LEXEL_H

#include <string>
#include <vector>

namespace Frontend {

enum TokenType {
    Unknown = -1,

    IntLiteral,
    FloatLiteral,
    StringLiteral,

    Identifier,

    Mul,
    Div,
    Add,
    Sub,
    ShiftLeft,
    ShiftRight,

    Less,
    LessOrEq,
    Greater,
    GreaterOrEq,
    Equal,
    NotEqual,
    Assignment,

    LogicAnd,
    LogicOr,
    BitwiseAnd,
    BitwiseOr,
    BitwiseXor,

    LeftParent,
    RightParent,
    LeftBracket,
    RightBracket,
    LeftBrace,
    RightBrace,
    Semicolon,
    Comma,
    Comment,
    Whitespace,

    IntType,
    FloatType,
    StringType,
    VoidType,

    IfKeyword,
    ElseKeyword,
    ForKeyword,
    WhileKeyword,
    FunctionKeyword,
    ReturnKeyword,
    PrintKeyword,
    ScanKeyword,
    PrintfKeyword,
    ScanfKeyword,
    PrintsKeyword,
    ScansKeyword,
    SqrtKeyword
};


static const char *tokenNames[] = {
    "IntLiteral",
    "FloatLiteral",
    "StringLiteral",

    "Identifier",

    "Mul",
    "Div",
    "Add",
    "Sub",
    "ShiftLeft",
    "ShiftRight",

    "Less",
    "LessOrEq",
    "Greater",
    "GreaterOrEq",
    "Equal",
    "NotEqual",
    "Assignment",

    "LogicAnd",
    "LogicOr",
    "BitwiseAnd",
    "BitwiseOr",
    "BitwiseXor",

    "LeftParent",
    "RightParent",
    "LeftBracket",
    "RightBracket",
    "LeftBrace",
    "RightBrace",
    "Semicolon",
    "Comma",
    "Comment",
    "Whitespace",

    "IntType",
    "FloatType",
    "StringType",
    "VoidType",

    "IfKeyword",
    "ElseKeyword",
    "ForKeyword",
    "WhileKeyword",
    "FunctionKeyword",
    "ReturnKeyword",
    "PrintKeyword",
    "ScanKeyword",
    "PrintfKeyword",
    "ScanfKeyword",
    "PrintsKeyword",
    "ScansKeyword",
    "SqrtKeyword"
};


struct Token {
    TokenType type = TokenType::Unknown;
    std::string value;
};


class VMLexer {
public:
    bool lex(const std::string &input, std::vector<Token> &outTokens);

private:
    char *m_program = nullptr;
    char *m_programEnd = nullptr;
    char *m_currentChar = nullptr;

    bool parseIfPositiveFloat(std::string &outStr);
    bool parseIfPositiveInt(std::string &outStr);
    bool parseIfIdentifier(std::string &outStr);
    bool parseIfString(std::string &outStr);
};

}

#endif  // VM_CUSTOM_LEXEL_H
