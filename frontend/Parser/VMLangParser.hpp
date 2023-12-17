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
    Identifier = 259,
    Mul = 260,
    Div = 261,
    Add = 262,
    Sub = 263,
    ShiftLeft = 264,
    ShiftRight = 265,
    Less = 266,
    LessOrEq = 267,
    Greater = 268,
    GreaterOrEq = 269,
    Equal = 270,
    NotEqual = 271,
    Assignment = 272,
    LogicAnd = 273,
    LogicOr = 274,
    BitwiseAnd = 275,
    BitwiseOr = 276,
    BitwiseXor = 277,
    LeftParent = 278,
    RightParent = 279,
    LeftBracket = 280,
    RightBracket = 281,
    LeftBrace = 282,
    RightBrace = 283,
    Semicolon = 284,
    Comma = 285,
    Comment = 286,
    Whitespace = 287,
    IntType = 288,
    VoidType = 289,
    IfKeyword = 290,
    ElseKeyword = 291,
    ForKeyword = 292,
    WhileKeyword = 293,
    FunctionKeyword = 294,
    ReturnKeyword = 295,
    PrintKeyword = 296,
    ScanKeyword = 297
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
