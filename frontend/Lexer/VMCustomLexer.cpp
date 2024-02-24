#include <cstring>
#include <iostream>

#include "VMCustomLexer.h"


namespace Frontend {

bool VMLexer::parseIfPositiveFloat(std::string &outStr) {
    std::string localStr = "";
    char *tmp = m_currentChar;
    
    if (!isdigit(*tmp)) {
        return false;
    }

    while ((tmp != m_programEnd) && isdigit(*tmp)) {
        localStr += *tmp;
        ++tmp;
    }

    if (*tmp != '.') {
        return false;
    }

    localStr += *tmp;
    ++tmp;

    if (!isdigit(*tmp)) {
        return false;
    }

    while ((tmp != m_programEnd) && isdigit(*tmp)) {
        localStr += *tmp;
        ++tmp;
    }

    outStr = localStr;
    return true;
}

bool VMLexer::parseIfPositiveInt(std::string &outStr) {
    std::string localStr = "";
    char *tmp = m_currentChar;

    if (!isdigit(*tmp) || (*tmp == '0')) {
        return false;
    }

    while ((tmp != m_programEnd) && isdigit(*tmp)) {
        localStr += *tmp;
        ++tmp;
    }

    outStr = localStr;
    return true;
}

bool VMLexer::parseIfIdentifier(std::string &outStr) {
    std::string localStr = "";
    char *tmp = m_currentChar;

    if (!isalpha(*tmp) && (*tmp != '_')) {
        return false;
    }

    while ((tmp != m_programEnd) && (isalnum(*tmp) || (*tmp == '_'))) {
        localStr += *tmp;
        ++tmp;
    }

    outStr = localStr;
    return true;
}

bool VMLexer::parseIfString(std::string &outStr) {
    std::string localStr = "";
    char *tmp = m_currentChar;

    if (*tmp != '\"') {
        return false;
    }

    localStr += *tmp;
    ++tmp;

    while ((tmp != m_programEnd) && (*tmp != '\"')) {
        localStr += *tmp;
        ++tmp;
    }

    outStr = localStr + "\"";
    return true;
}

bool VMLexer::lex(const std::string &input, std::vector<Token> &outTokens) {
    m_program = new char[input.size()];
    std::memcpy(m_program, input.c_str(), input.size());
    m_programEnd = m_program + input.size();
    m_currentChar = m_program;

    while (m_currentChar != m_programEnd) {

        if (isspace(*m_currentChar)) {
            ++m_currentChar;
            continue;
        }

        if ((*m_currentChar == '/') && (*(m_currentChar + 1) == '/')) {
            while ((m_currentChar != m_programEnd) && (*m_currentChar != '\n')) {
                ++m_currentChar;
            }
            continue;
        }

        switch (*m_currentChar) {
            case '*': {
                outTokens.push_back({TokenType::Mul, "*"});
                ++m_currentChar;
                continue;
            }
            case '/': {
                outTokens.push_back({TokenType::Div, "/"});
                ++m_currentChar;
                continue;
            }
            case '+': {
                outTokens.push_back({TokenType::Add, "+"});
                ++m_currentChar;
                continue;
            }
            // Skip minus as it can be just minus or negative number
            case '<': {
                if (*(m_currentChar + 1) == '<') {
                    outTokens.push_back({TokenType::ShiftLeft, "<<"});
                    m_currentChar += 2;
                }
                else if (*(m_currentChar + 1) == '=') {
                    outTokens.push_back({TokenType::LessOrEq, "<="});
                    m_currentChar += 2;
                }
                else {
                    outTokens.push_back({TokenType::Less, "<"});
                    m_currentChar += 1;
                }
                continue;
            }
            case '>': {
                if (*(m_currentChar + 1) == '>') {
                    outTokens.push_back({TokenType::ShiftRight, ">>"});
                    m_currentChar += 2;
                }
                else if (*(m_currentChar + 1) == '=') {
                    outTokens.push_back({TokenType::GreaterOrEq, ">="});
                    m_currentChar += 2;
                }
                else {
                    outTokens.push_back({TokenType::Greater, ">"});
                    m_currentChar += 1;
                }
                continue;
            }
            case '=': {
                if (*(m_currentChar + 1) == '=') {
                    outTokens.push_back({TokenType::Equal, "=="});
                    m_currentChar += 2;
                }
                else {
                    outTokens.push_back({TokenType::Assignment, "="});
                    m_currentChar += 1;
                }
                continue;
            }
            case '&': {
                if (*(m_currentChar + 1) == '&') {
                    outTokens.push_back({TokenType::LogicAnd, "&&"});
                    m_currentChar += 2;
                }
                else {
                    outTokens.push_back({TokenType::BitwiseAnd, "&"});
                    m_currentChar += 1;
                }
                continue;
            }
            case '|': {
                if (*(m_currentChar + 1) == '|') {
                    outTokens.push_back({TokenType::LogicOr, "||"});
                    m_currentChar += 2;
                }
                else {
                    outTokens.push_back({TokenType::BitwiseOr, "|"});
                    m_currentChar += 1;
                }
                continue;
            }
            case '^': {
                outTokens.push_back({TokenType::BitwiseXor, "^"});
                ++m_currentChar;
                continue;
            }
            case '(': {
                outTokens.push_back({TokenType::LeftParent, "("});
                ++m_currentChar;
                continue;
            }
            case ')': {
                outTokens.push_back({TokenType::RightParent, ")"});
                ++m_currentChar;
                continue;
            }
            case '[': {
                outTokens.push_back({TokenType::LeftBracket, "["});
                ++m_currentChar;
                continue;
            }
            case ']': {
                outTokens.push_back({TokenType::RightBracket, "]"});
                ++m_currentChar;
                continue;
            }
            case '{': {
                outTokens.push_back({TokenType::LeftBrace, "{"});
                ++m_currentChar;
                continue;
            }
            case '}': {
                outTokens.push_back({TokenType::RightBrace, "}"});
                ++m_currentChar;
                continue;
            }
            case ';': {
                outTokens.push_back({TokenType::Semicolon, ";"});
                ++m_currentChar;
                continue;
            }
            case ',': {
                outTokens.push_back({TokenType::Comma, ","});
                ++m_currentChar;
                continue;
            }
            default: {
                if (isalpha(*m_currentChar)) {
                    std::string currentWord = "";

                    char *tmp = m_currentChar;
                    while ((tmp != m_programEnd) && isalpha(*tmp)) {
                        currentWord += *tmp;
                        ++tmp;
                    }

                    if (currentWord == "int") {
                        outTokens.push_back({TokenType::IntType, currentWord});
                        m_currentChar = tmp;
                        continue;
                    }
                    else if (currentWord == "float") {
                        outTokens.push_back({TokenType::FloatType, currentWord});
                        m_currentChar = tmp;
                        continue;
                    }
                    else if (currentWord == "string") {
                        outTokens.push_back({TokenType::StringType, currentWord});
                        m_currentChar = tmp;
                        continue;
                    }
                    else if (currentWord == "void") {
                        outTokens.push_back({TokenType::VoidType, currentWord});
                        m_currentChar = tmp;
                        continue;
                    }
                    else if (currentWord == "if") {
                        outTokens.push_back({TokenType::IfKeyword, currentWord});
                        m_currentChar = tmp;
                        continue;
                    }
                    else if (currentWord == "else") {
                        outTokens.push_back({TokenType::ElseKeyword, currentWord});
                        m_currentChar = tmp;
                        continue;
                    }
                    else if (currentWord == "for") {
                        outTokens.push_back({TokenType::ForKeyword, currentWord});
                        m_currentChar = tmp;
                        continue;
                    }
                    else if (currentWord == "while") {
                        outTokens.push_back({TokenType::WhileKeyword, currentWord});
                        m_currentChar = tmp;
                        continue;
                    }
                    else if (currentWord == "function") {
                        outTokens.push_back({TokenType::FunctionKeyword, currentWord});
                        m_currentChar = tmp;
                        continue;
                    }
                    else if (currentWord == "return") {
                        outTokens.push_back({TokenType::ReturnKeyword, currentWord});
                        m_currentChar = tmp;
                        continue;
                    }
                    else if (currentWord == "print") {
                        outTokens.push_back({TokenType::PrintKeyword, currentWord});
                        m_currentChar = tmp;
                        continue;
                    }
                    else if (currentWord == "scan") {
                        outTokens.push_back({TokenType::ScanKeyword, currentWord});
                        m_currentChar = tmp;
                        continue;
                    }
                    else if (currentWord == "printf") {
                        outTokens.push_back({TokenType::PrintfKeyword, currentWord});
                        m_currentChar = tmp;
                        continue;
                    }
                    else if (currentWord == "scanf") {
                        outTokens.push_back({TokenType::ScanfKeyword, currentWord});
                        m_currentChar = tmp;
                        continue;
                    }
                    else if (currentWord == "prints") {
                        outTokens.push_back({TokenType::PrintsKeyword, currentWord});
                        m_currentChar = tmp;
                        continue;
                    }
                    else if (currentWord == "scans") {
                        outTokens.push_back({TokenType::ScansKeyword, currentWord});
                        m_currentChar = tmp;
                        continue;
                    }
                    else if (currentWord == "sqrt") {
                        outTokens.push_back({TokenType::SqrtKeyword, currentWord});
                        m_currentChar = tmp;
                        continue;
                    }
                                        
                    std::string str = "";
                    if (parseIfIdentifier(str)) {
                        outTokens.push_back({TokenType::Identifier, str});
                        m_currentChar += str.size();
                        continue;
                    }
                    return false;
                }
                else if (isdigit(*m_currentChar)) {
                    std::string str = "";
                    if (parseIfPositiveFloat(str)) {
                        outTokens.push_back({TokenType::FloatLiteral, str});
                        m_currentChar += str.size();
                        continue;
                    }
                    else if (parseIfPositiveInt(str)) {
                        outTokens.push_back({TokenType::IntLiteral, str});
                        m_currentChar += str.size();
                        continue;
                    }
                    else if (*m_currentChar == '0') {
                        outTokens.push_back({TokenType::IntLiteral, "0"});
                        m_currentChar += 1;
                        continue;
                    }
                    return false;
                }
                else if (*m_currentChar == '-') {
                    std::string minus = "-";
                    ++m_currentChar;

                    std::string str = "";
                    if (parseIfPositiveFloat(str)) {
                        outTokens.push_back({TokenType::FloatLiteral, minus + str});
                        m_currentChar += str.size();
                        continue;
                    }
                    else if (parseIfPositiveInt(str)) {
                        outTokens.push_back({TokenType::IntLiteral, minus + str});
                        m_currentChar += str.size();
                        continue;
                    }
                    else {
                        outTokens.push_back({TokenType::Sub, minus});
                        continue;                        
                    }
                }
                else if (*m_currentChar == '\"') {
                    std::string str = "";
                    if (parseIfString(str)) {
                        outTokens.push_back({TokenType::StringLiteral, str});
                        m_currentChar += str.size();
                        continue;
                    }

                    outTokens.push_back({TokenType::Unknown, ""});
                    return false;
                }
                else {
                    std::string str = "";
                    if (parseIfIdentifier(str)) {
                        outTokens.push_back({TokenType::Identifier, str});
                        m_currentChar += str.size();
                        continue;
                    }

                    outTokens.push_back({TokenType::Unknown, ""});
                    return false;
                }
            }
        }
        return false;
    }
    return true;
}

}
