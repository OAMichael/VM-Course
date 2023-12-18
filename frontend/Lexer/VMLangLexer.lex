%{

#define YYSTYPE void *

#include "VMLangParser.hpp"

extern "C" int yylex();

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
  printf("IntType %s\n", yytext);
  yylval = strdup(yytext);
  return IntType;
}

{FloatType} {
  printf("FloatType %s\n", yytext);
  yylval = strdup(yytext);
  return FloatType;
}

{StringType} {
  printf("StringType %s\n", yytext);
  yylval = strdup(yytext);
  return StringType;
}

{VoidType} {
  printf("VoidType %s\n", yytext);
  yylval = strdup(yytext);
  return VoidType;
}

{IfKeyword} {
  printf("IfKeyword %s\n", yytext);
  yylval = strdup(yytext);
  return IfKeyword;
}

{ElseKeyword} {
  printf("ElseKeyword %s\n", yytext);
  yylval = strdup(yytext);
  return ElseKeyword;
}

{ForKeyword} {
  printf("ForKeyword %s\n", yytext);
  yylval = strdup(yytext);
  return ForKeyword;
}

{WhileKeyword} {
  printf("WhileKeyword %s\n", yytext);
  yylval = strdup(yytext);
  return WhileKeyword;
}

{ReturnKeyword} {
  printf("ReturnKeyword %s\n", yytext);
  yylval = strdup(yytext);
  return ReturnKeyword;
}

{FunctionKeyword} {
  printf("FunctionKeyword %s\n", yytext);
  yylval = strdup(yytext);
  return FunctionKeyword;
}

{PrintKeyword} {
  printf("PrintKeyword %s\n", yytext);
  yylval = strdup(yytext);
  return PrintKeyword;
}

{PrintfKeyword} {
  printf("PrintfKeyword %s\n", yytext);
  yylval = strdup(yytext);
  return PrintfKeyword;
}

{PrintsKeyword} {
  printf("PrintsKeyword %s\n", yytext);
  yylval = strdup(yytext);
  return PrintsKeyword;
}

{ScanKeyword} {
  printf("ScanKeyword %s\n", yytext);
  yylval = strdup(yytext);
  return ScanKeyword;
}

{ScanfKeyword} {
  printf("ScanfKeyword %s\n", yytext);
  yylval = strdup(yytext);
  return ScanfKeyword;
}

{ScansKeyword} {
  printf("ScansKeyword %s\n", yytext);
  yylval = strdup(yytext);
  return ScansKeyword;
}

{SqrtKeyword} {
  printf("SqrtKeyword %s\n", yytext);
  yylval = strdup(yytext);
  return SqrtKeyword;
}

{IntLiteral} {
  printf("IntLiteral %s\n", yytext);
  yylval = strdup(yytext);
  return IntLiteral;
}

{FloatLiteral} {
  printf("FloatLiteral %s\n", yytext);
  yylval = strdup(yytext);
  return FloatLiteral;
}

{StringLiteral} {
  printf("StringLiteral %s\n", yytext);
  yylval = strdup(yytext);
  return StringLiteral;
}

{Identifier} {
  printf("Identifier %s\n", yytext);
  yylval = strdup(yytext);
  return Identifier;
}

{ShiftLeft} {
  printf("ShiftLeft %s\n", yytext);
  yylval = strdup(yytext);
  return ShiftLeft;
}

{ShiftRight} {
  printf("ShiftRight %s\n", yytext);
  yylval = strdup(yytext);
  return ShiftRight;
}

{LessOrEq} {
  printf("LessOrEq %s\n", yytext);
  yylval = strdup(yytext);
  return LessOrEq;
}

{GreaterOrEq} {
  printf("GreaterOrEq %s\n", yytext);
  yylval = strdup(yytext);
  return GreaterOrEq;
}

{NotEqual} {
  printf("NotEqual %s\n", yytext);
  yylval = strdup(yytext);
  return NotEqual;
}

{LogicAnd} {
  printf("LogicAnd %s\n", yytext);
  yylval = strdup(yytext);
  return LogicAnd;
}

{LogicOr} {
  printf("LogicOr %s\n", yytext);
  yylval = strdup(yytext);
  return LogicOr;
}

{Mul} {
  printf("Mul %s\n", yytext);
  yylval = strdup(yytext);
  return Mul;
}

{Div} {
  printf("Div %s\n", yytext);
  yylval = strdup(yytext);
  return Div;
}

{Add} {
  printf("Add %s\n", yytext);
  yylval = strdup(yytext);
  return Add;
}

{Sub} {
  printf("Sub %s\n", yytext);
  yylval = strdup(yytext);
  return Sub;
}

{Equal} {
  printf("Equal %s\n", yytext);
  yylval = strdup(yytext);
  return Equal;
}

{Less} {
  printf("Less %s\n", yytext);
  yylval = strdup(yytext);
  return Less;
}

{Greater} {
  printf("Greater %s\n", yytext);
  yylval = strdup(yytext);
  return Greater;
}

{Assignment} {
  printf("Assignment %s\n", yytext);
  yylval = strdup(yytext);
  return Assignment;
}

{BitwiseAnd} {
  printf("BitwiseAnd %s\n", yytext);
  yylval = strdup(yytext);
  return BitwiseAnd;
}

{BitwiseOr} {
  printf("BitwiseOr %s\n", yytext);
  yylval = strdup(yytext);
  return BitwiseOr;
}

{BitwiseXor} {
  printf("BitwiseXor %s\n", yytext);
  yylval = strdup(yytext);
  return BitwiseXor;
}

{LeftBracket} {
  printf("LeftBracket %s\n", yytext);
  yylval = strdup(yytext);
  return LeftBracket;
}

{RightBracket} {
  printf("RightBracket %s\n", yytext);
  yylval = strdup(yytext);
  return RightBracket;
}

{LeftParent} {
  printf("LeftParent %s\n", yytext);
  yylval = strdup(yytext);
  return LeftParent;
}

{RightParent} {
  printf("RightParent %s\n", yytext);
  yylval = strdup(yytext);
  return RightParent;
}

{LeftBrace} {
  printf("LeftBrace %s\n", yytext);
  yylval = strdup(yytext);
  return LeftBrace;
}

{RightBrace} {
  printf("RightBrace %s\n", yytext);
  yylval = strdup(yytext);
  return RightBrace;
}

{Semicolon} {
  printf("Semicolon %s\n", yytext);
  yylval = strdup(yytext);
  return Semicolon;
}

{Comma} {
  printf("Comma %s\n", yytext);
  yylval = strdup(yytext);
  return Comma;
}

. {
  return *yytext;
}

%%