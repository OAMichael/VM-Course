/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_HOME_MICHAEL_DESKTOP_MIPT_VIRTUALMACHINE_FRONTEND_PARSER_VMLANGPARSER_HPP_INCLUDED
# define YY_YY_HOME_MICHAEL_DESKTOP_MIPT_VIRTUALMACHINE_FRONTEND_PARSER_VMLANGPARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IntLiteral = 258,
    FloatLiteral = 259,
    StringLiteral = 260,
    Identifier = 261,
    Mul = 262,
    Div = 263,
    Add = 264,
    Sub = 265,
    ShiftLeft = 266,
    ShiftRight = 267,
    Less = 268,
    LessOrEq = 269,
    Greater = 270,
    GreaterOrEq = 271,
    Equal = 272,
    NotEqual = 273,
    Assignment = 274,
    LogicAnd = 275,
    LogicOr = 276,
    BitwiseAnd = 277,
    BitwiseOr = 278,
    BitwiseXor = 279,
    LeftParent = 280,
    RightParent = 281,
    LeftBracket = 282,
    RightBracket = 283,
    LeftBrace = 284,
    RightBrace = 285,
    Semicolon = 286,
    Comma = 287,
    Comment = 288,
    Whitespace = 289,
    IntType = 290,
    FloatType = 291,
    StringType = 292,
    VoidType = 293,
    IfKeyword = 294,
    ElseKeyword = 295,
    ForKeyword = 296,
    WhileKeyword = 297,
    FunctionKeyword = 298,
    ReturnKeyword = 299,
    PrintKeyword = 300,
    PrintfKeyword = 301,
    PrintsKeyword = 302,
    ScanKeyword = 303,
    ScanfKeyword = 304,
    ScansKeyword = 305,
    SqrtKeyword = 306
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_HOME_MICHAEL_DESKTOP_MIPT_VIRTUALMACHINE_FRONTEND_PARSER_VMLANGPARSER_HPP_INCLUDED  */
