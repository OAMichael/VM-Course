%{

#define YYSTYPE void *

#include <vector>
#include <string>

#include "VMLangParser.hpp"

extern "C" int yylex();
extern std::vector<std::string> tokens;

%}

%option yylineno
%option noyywrap

IntLiteral                  [-]?[1-9]+[0-9]*|[0]
FloatLiteral                [-]?[0-9]+.[0-9]+
StringLiteral               \"(\\.|[^"\\])*\"

Identifier                  [A-Za-z_]+[A-Za-z_0-9]*

Mul                         [*]
Div                         [/]
Add                         [+]
Sub                         [-]
ShiftLeft                   [<][<]
ShiftRight                  [>][>]

Less                        [<]
LessOrEq                    [<][=]
Greater                     [>]
GreaterOrEq                 [>][=]
Equal                       [=][=]
NotEqual                    [!][=]
Assignment                  [=]

LogicAnd                    [&][&]
LogicOr                     [|][|]
BitwiseAnd                  [&]
BitwiseOr                   [|]
BitwiseXor                  "^"

LeftParent                  "("
RightParent                 ")"
LeftBracket                 "["
RightBracket                "]"
LeftBrace                   "{"
RightBrace                  "}"
Semicolon                   [;]
Comma                       [,]
Comment                     [/][/].*\n
Whitespace                  [ \t\r\n]

IntType                     [i][n][t]
FloatType                   [f][l][o][a][t]
StringType                  [s][t][r][i][n][g]
VoidType                    [v][o][i][d]

IfKeyword                   [i][f]
ElseKeyword                 [e][l][s][e]
ForKeyword                  [f][o][r]
WhileKeyword                [w][h][i][l][e]
FunctionKeyword             [f][u][n][c][t][i][o][n]
ReturnKeyword               [r][e][t][u][r][n]
PrintKeyword                [p][r][i][n][t]
ScanKeyword                 [s][c][a][n]
PrintfKeyword               [p][r][i][n][t][f]
ScanfKeyword                [s][c][a][n][f]
PrintsKeyword               [p][r][i][n][t][s]
ScansKeyword                [s][c][a][n][s]
SqrtKeyword                 [s][q][r][t]

%%

{Whitespace}
{Comment}


{IntType} {
  tokens.push_back(std::string("IntType ") + yytext);
  yylval = strdup(yytext);
  return IntType;
}

{FloatType} {
  tokens.push_back(std::string("FloatType ") + yytext);
  yylval = strdup(yytext);
  return FloatType;
}

{StringType} {
  tokens.push_back(std::string("StringType ") + yytext);
  yylval = strdup(yytext);
  return StringType;
}

{VoidType} {
  tokens.push_back(std::string("VoidType ") + yytext);
  yylval = strdup(yytext);
  return VoidType;
}

{IfKeyword} {
  tokens.push_back(std::string("IfKeyword ") + yytext);
  yylval = strdup(yytext);
  return IfKeyword;
}

{ElseKeyword} {
  tokens.push_back(std::string("ElseKeyword ") + yytext);
  yylval = strdup(yytext);
  return ElseKeyword;
}

{ForKeyword} {
  tokens.push_back(std::string("ForKeyword ") + yytext);
  yylval = strdup(yytext);
  return ForKeyword;
}

{WhileKeyword} {
  tokens.push_back(std::string("WhileKeyword ") + yytext);
  yylval = strdup(yytext);
  return WhileKeyword;
}

{ReturnKeyword} {
  tokens.push_back(std::string("ReturnKeyword ") + yytext);
  yylval = strdup(yytext);
  return ReturnKeyword;
}

{FunctionKeyword} {
  tokens.push_back(std::string("FunctionKeyword ") + yytext);
  yylval = strdup(yytext);
  return FunctionKeyword;
}

{PrintKeyword} {
  tokens.push_back(std::string("PrintKeyword ") + yytext);
  yylval = strdup(yytext);
  return PrintKeyword;
}

{PrintfKeyword} {
  tokens.push_back(std::string("PrintfKeyword ") + yytext);
  yylval = strdup(yytext);
  return PrintfKeyword;
}

{PrintsKeyword} {
  tokens.push_back(std::string("PrintsKeyword ") + yytext);
  yylval = strdup(yytext);
  return PrintsKeyword;
}

{ScanKeyword} {
  tokens.push_back(std::string("ScanKeyword ") + yytext);
  yylval = strdup(yytext);
  return ScanKeyword;
}

{ScanfKeyword} {
  tokens.push_back(std::string("ScanfKeyword ") + yytext);
  yylval = strdup(yytext);
  return ScanfKeyword;
}

{ScansKeyword} {
  tokens.push_back(std::string("ScansKeyword ") + yytext);
  yylval = strdup(yytext);
  return ScansKeyword;
}

{SqrtKeyword} {
  tokens.push_back(std::string("SqrtKeyword ") + yytext);
  yylval = strdup(yytext);
  return SqrtKeyword;
}

{IntLiteral} {
  tokens.push_back(std::string("IntLiteral ") + yytext);
  yylval = strdup(yytext);
  return IntLiteral;
}

{FloatLiteral} {
  tokens.push_back(std::string("FloatLiteral ") + yytext);
  yylval = strdup(yytext);
  return FloatLiteral;
}

{StringLiteral} {
  tokens.push_back(std::string("StringLiteral ") + yytext);
  yylval = strdup(yytext);
  return StringLiteral;
}

{Identifier} {
  tokens.push_back(std::string("Identifier ") + yytext);
  yylval = strdup(yytext);
  return Identifier;
}

{ShiftLeft} {
  tokens.push_back(std::string("ShiftLeft ") + yytext);
  yylval = strdup(yytext);
  return ShiftLeft;
}

{ShiftRight} {
  tokens.push_back(std::string("ShiftRight ") + yytext);
  yylval = strdup(yytext);
  return ShiftRight;
}

{LessOrEq} {
  tokens.push_back(std::string("LessOrEq ") + yytext);
  yylval = strdup(yytext);
  return LessOrEq;
}

{GreaterOrEq} {
  tokens.push_back(std::string("GreaterOrEq ") + yytext);
  yylval = strdup(yytext);
  return GreaterOrEq;
}

{NotEqual} {
  tokens.push_back(std::string("NotEqual ") + yytext);
  yylval = strdup(yytext);
  return NotEqual;
}

{LogicAnd} {
  tokens.push_back(std::string("LogicAnd ") + yytext);
  yylval = strdup(yytext);
  return LogicAnd;
}

{LogicOr} {
  tokens.push_back(std::string("LogicOr ") + yytext);
  yylval = strdup(yytext);
  return LogicOr;
}

{Mul} {
  tokens.push_back(std::string("Mul ") + yytext);
  yylval = strdup(yytext);
  return Mul;
}

{Div} {
  tokens.push_back(std::string("Div ") + yytext);
  yylval = strdup(yytext);
  return Div;
}

{Add} {
  tokens.push_back(std::string("Add ") + yytext);
  yylval = strdup(yytext);
  return Add;
}

{Sub} {
  tokens.push_back(std::string("Sub ") + yytext);
  yylval = strdup(yytext);
  return Sub;
}

{Equal} {
  tokens.push_back(std::string("Equal ") + yytext);
  yylval = strdup(yytext);
  return Equal;
}

{Less} {
  tokens.push_back(std::string("Less ") + yytext);
  yylval = strdup(yytext);
  return Less;
}

{Greater} {
  tokens.push_back(std::string("Greater ") + yytext);
  yylval = strdup(yytext);
  return Greater;
}

{Assignment} {
  tokens.push_back(std::string("Assignment ") + yytext);
  yylval = strdup(yytext);
  return Assignment;
}

{BitwiseAnd} {
  tokens.push_back(std::string("BitwiseAnd ") + yytext);
  yylval = strdup(yytext);
  return BitwiseAnd;
}

{BitwiseOr} {
  tokens.push_back(std::string("BitwiseOr ") + yytext);
  yylval = strdup(yytext);
  return BitwiseOr;
}

{BitwiseXor} {
  tokens.push_back(std::string("BitwiseXor ") + yytext);
  yylval = strdup(yytext);
  return BitwiseXor;
}

{LeftBracket} {
  tokens.push_back(std::string("LeftBracket ") + yytext);
  yylval = strdup(yytext);
  return LeftBracket;
}

{RightBracket} {
  tokens.push_back(std::string("RightBracket ") + yytext);
  yylval = strdup(yytext);
  return RightBracket;
}

{LeftParent} {
  tokens.push_back(std::string("LeftParent ") + yytext);
  yylval = strdup(yytext);
  return LeftParent;
}

{RightParent} {
  tokens.push_back(std::string("RightParent ") + yytext);
  yylval = strdup(yytext);
  return RightParent;
}

{LeftBrace} {
  tokens.push_back(std::string("LeftBrace ") + yytext);
  yylval = strdup(yytext);
  return LeftBrace;
}

{RightBrace} {
  tokens.push_back(std::string("RightBrace ") + yytext);
  yylval = strdup(yytext);
  return RightBrace;
}

{Semicolon} {
  tokens.push_back(std::string("Semicolon ") + yytext);
  yylval = strdup(yytext);
  return Semicolon;
}

{Comma} {
  tokens.push_back(std::string("Comma ") + yytext);
  yylval = strdup(yytext);
  return Comma;
}

. {
  return *yytext;
}

%%