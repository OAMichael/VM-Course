/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"


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


#line 128 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  76
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   303

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  106
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  176

#define YYUNDEFTOK  2
#define YYMAXUTOK   306


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   111,   111,   114,   117,   120,   123,   126,   131,   134,
     137,   141,   142,   143,   148,   151,   154,   158,   159,   160,
     165,   169,   174,   175,   177,   178,   179,   181,   184,   191,
     192,   193,   194,   196,   199,   202,   204,   207,   210,   212,
     235,   240,   245,   250,   254,   259,   263,   266,   270,   274,
     278,   282,   286,   290,   294,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   313,   316,
     318,   323,   333,   334,   336,   337,   338,   339,   340,   341,
     342,   344,   345,   346,   347,   348,   349,   350,   351,   354,
     357,   360,   363,   366,   369,   372,   375,   378,   382,   385,
     388,   393,   396,   411,   414,   443,   446
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IntLiteral", "FloatLiteral",
  "StringLiteral", "Identifier", "Mul", "Div", "Add", "Sub", "ShiftLeft",
  "ShiftRight", "Less", "LessOrEq", "Greater", "GreaterOrEq", "Equal",
  "NotEqual", "Assignment", "LogicAnd", "LogicOr", "BitwiseAnd",
  "BitwiseOr", "BitwiseXor", "LeftParent", "RightParent", "LeftBracket",
  "RightBracket", "LeftBrace", "RightBrace", "Semicolon", "Comma",
  "Comment", "Whitespace", "IntType", "FloatType", "StringType",
  "VoidType", "IfKeyword", "ElseKeyword", "ForKeyword", "WhileKeyword",
  "FunctionKeyword", "ReturnKeyword", "PrintKeyword", "PrintfKeyword",
  "PrintsKeyword", "ScanKeyword", "ScanfKeyword", "ScansKeyword",
  "SqrtKeyword", "$accept", "Parse", "Program", "SimpleIntDeclaration",
  "InitIntDeclaration", "ArrayIntDeclaration", "IntVariableDeclaration",
  "SimpleFloatDeclaration", "InitFloatDeclaration",
  "ArrayFloatDeclaration", "FloatVariableDeclaration",
  "InitStringDeclaration", "StringVariableDeclaration",
  "SimpleVariableDeclaration", "VariableDeclaration", "VariableValue",
  "ReturnType", "FunctionArgsDeclarations", "FunctionArgsExpressions",
  "FunctionDeclaration", "IfStatement", "WhileLoopStatement",
  "ForLoopStatement", "FunctionCall", "AssigmentStatement",
  "ReturnStatement", "PrintStatement", "ScanStatement", "PrintfStatement",
  "ScanfStatement", "PrintsStatement", "ScansStatement", "SqrtStatement",
  "Statement", "StatementList", "StatementsScopeBegin", "StatementsScope",
  "HighPriorityOperation", "LowPriorityOperation", "ExpressionOperation",
  "Primary", "Factor", "Summand", "Simple", "Expression", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306
};
# endif

#define YYPACT_NINF (-136)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      58,    36,     6,     7,    10,  -136,    -2,    29,    32,    24,
      40,    41,    42,    43,    44,    45,    77,    58,  -136,  -136,
    -136,  -136,  -136,  -136,  -136,  -136,  -136,  -136,  -136,    59,
      38,  -136,  -136,  -136,  -136,  -136,  -136,  -136,  -136,  -136,
    -136,  -136,  -136,  -136,  -136,    24,    24,   -13,   -12,    60,
      24,   108,    24,  -136,  -136,  -136,    34,    24,    62,  -136,
    -136,  -136,  -136,  -136,  -136,  -136,  -136,    35,   172,   282,
      24,    24,    24,    63,    64,    65,  -136,  -136,  -136,    24,
      86,   -23,   282,   159,    24,    95,    24,   109,    24,   184,
       6,     7,    10,    82,   198,  -136,   212,    24,  -136,  -136,
      24,  -136,  -136,  -136,  -136,  -136,  -136,  -136,    24,  -136,
    -136,  -136,  -136,  -136,  -136,  -136,  -136,    24,   226,   240,
     254,  -136,  -136,  -136,   282,    93,  -136,    24,  -136,   282,
      83,   282,    92,   282,    94,    24,    94,  -136,   268,  -136,
      35,   172,  -136,  -136,  -136,    12,   282,  -136,  -136,  -136,
    -136,    91,   120,  -136,  -136,   126,   133,  -136,  -136,  -136,
       9,    80,    94,   108,  -136,  -136,    94,    12,  -136,  -136,
    -136,   116,  -136,  -136,    94,  -136
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       7,    28,    29,    30,    31,    32,     0,     0,     0,    47,
       0,     0,     0,     0,     0,     0,     0,     2,    13,    11,
      12,    24,    19,    17,    18,    25,    21,    26,    55,     0,
       0,     3,    57,    58,    59,    60,    56,    61,    62,    63,
      64,    65,    66,    67,     4,    38,     0,     8,    14,     0,
       0,     0,     0,    89,    90,    91,     0,     0,     0,    92,
      93,    94,    95,    96,    97,    98,   101,   103,   105,    46,
       0,     0,     0,     0,     0,     0,     1,     5,     6,     0,
       0,     0,    37,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,    72,    73,
       0,    74,    75,    76,    77,    78,    79,    80,     0,    81,
      82,    83,    84,    85,    86,    87,    88,     0,     0,     0,
       0,    49,    51,    53,    45,     0,    44,     0,    27,     9,
       0,    15,     0,    20,     0,     0,     0,   100,     0,   102,
     104,   106,    48,    50,    52,    35,    36,    10,    16,    70,
      69,    41,     0,    42,    54,     0,     0,    22,    23,    34,
       0,     0,     0,     0,     8,    14,     0,     0,    71,    68,
      40,     0,    39,    33,     0,    43
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -136,  -136,  -136,  -135,  -136,  -136,  -136,  -134,  -136,  -136,
    -136,  -136,  -136,    -8,  -136,     0,  -136,  -136,  -136,   143,
    -136,  -136,  -136,     1,  -136,  -136,  -136,     4,  -136,     5,
    -136,     8,  -136,   -15,  -136,  -136,   -86,  -136,  -136,  -136,
     114,    78,    81,    68,   -26
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,   159,    28,    59,    30,   160,    81,    31,
      32,    33,    34,    60,    36,    37,    38,    61,    40,    62,
      42,    63,    64,    44,   161,   150,   151,   100,   108,   117,
      65,    66,    67,    68,    69
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      29,    35,    78,   126,    39,    41,    84,    86,    43,   127,
     157,   158,    47,    48,    85,    87,    49,    29,    35,    82,
      83,    39,    41,    50,    89,    43,    94,    53,    54,    55,
       1,    96,   157,   158,    56,   166,    93,    53,    54,    55,
       1,   167,    98,    99,   118,   119,   120,   155,   156,    57,
     153,    29,    35,   124,    51,    39,    41,    52,   129,    43,
     131,    45,   133,    46,     1,    70,    71,    72,    73,    74,
      75,   138,    13,    14,    15,    58,   170,    76,    79,    88,
     172,    80,    13,    14,    15,    58,     1,    97,   175,   121,
     122,   123,   125,     2,     3,     4,     5,     6,   130,     7,
       8,   146,     9,    10,    11,    12,    13,    14,    15,   152,
     168,   147,   132,   135,     1,    90,    91,    92,   145,     6,
     148,     7,     8,   149,     9,    10,    11,    12,    13,    14,
      15,   162,   164,   109,   110,   111,   112,   113,   114,   165,
     115,   116,   174,    90,    91,    92,   169,     6,   171,     7,
       8,   163,     9,    10,    11,    12,    13,    14,    15,   173,
      77,    29,    35,    29,    35,    39,    41,    39,    41,    43,
      95,    43,   109,   110,   111,   112,   113,   114,   139,   115,
     116,   101,   102,   103,   104,   141,     0,   128,     0,   140,
       0,     0,     0,     0,   105,   106,   107,   109,   110,   111,
     112,   113,   114,     0,   115,   116,     0,     0,     0,     0,
     134,   109,   110,   111,   112,   113,   114,     0,   115,   116,
       0,     0,     0,     0,   136,   109,   110,   111,   112,   113,
     114,     0,   115,   116,     0,     0,     0,     0,   137,   109,
     110,   111,   112,   113,   114,     0,   115,   116,     0,     0,
       0,     0,   142,   109,   110,   111,   112,   113,   114,     0,
     115,   116,     0,     0,     0,     0,   143,   109,   110,   111,
     112,   113,   114,     0,   115,   116,     0,     0,     0,     0,
     144,   109,   110,   111,   112,   113,   114,     0,   115,   116,
       0,     0,     0,     0,   154,   109,   110,   111,   112,   113,
     114,     0,   115,   116
};

static const yytype_int16 yycheck[] =
{
       0,     0,    17,    26,     0,     0,    19,    19,     0,    32,
     145,   145,     6,     6,    27,    27,     6,    17,    17,    45,
      46,    17,    17,    25,    50,    17,    52,     3,     4,     5,
       6,    57,   167,   167,    10,    26,    51,     3,     4,     5,
       6,    32,     7,     8,    70,    71,    72,    35,    36,    25,
     136,    51,    51,    79,    25,    51,    51,    25,    84,    51,
      86,    25,    88,    27,     6,    25,    25,    25,    25,    25,
      25,    97,    48,    49,    50,    51,   162,     0,    19,    19,
     166,    43,    48,    49,    50,    51,     6,    25,   174,    26,
      26,    26,     6,    35,    36,    37,    38,    39,     3,    41,
      42,   127,    44,    45,    46,    47,    48,    49,    50,   135,
      30,    28,     3,    31,     6,    35,    36,    37,    25,    39,
      28,    41,    42,    29,    44,    45,    46,    47,    48,    49,
      50,    40,     6,    13,    14,    15,    16,    17,    18,     6,
      20,    21,    26,    35,    36,    37,   161,    39,   163,    41,
      42,    31,    44,    45,    46,    47,    48,    49,    50,   167,
      17,   161,   161,   163,   163,   161,   161,   163,   163,   161,
      56,   163,    13,    14,    15,    16,    17,    18,   100,    20,
      21,     9,    10,    11,    12,   117,    -1,    28,    -1,   108,
      -1,    -1,    -1,    -1,    22,    23,    24,    13,    14,    15,
      16,    17,    18,    -1,    20,    21,    -1,    -1,    -1,    -1,
      26,    13,    14,    15,    16,    17,    18,    -1,    20,    21,
      -1,    -1,    -1,    -1,    26,    13,    14,    15,    16,    17,
      18,    -1,    20,    21,    -1,    -1,    -1,    -1,    26,    13,
      14,    15,    16,    17,    18,    -1,    20,    21,    -1,    -1,
      -1,    -1,    26,    13,    14,    15,    16,    17,    18,    -1,
      20,    21,    -1,    -1,    -1,    -1,    26,    13,    14,    15,
      16,    17,    18,    -1,    20,    21,    -1,    -1,    -1,    -1,
      26,    13,    14,    15,    16,    17,    18,    -1,    20,    21,
      -1,    -1,    -1,    -1,    26,    13,    14,    15,    16,    17,
      18,    -1,    20,    21
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     6,    35,    36,    37,    38,    39,    41,    42,    44,
      45,    46,    47,    48,    49,    50,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    66,    67,
      68,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    85,    25,    27,     6,     6,     6,
      25,    25,    25,     3,     4,     5,    10,    25,    51,    67,
      75,    79,    81,    83,    84,    92,    93,    94,    95,    96,
      25,    25,    25,    25,    25,    25,     0,    71,    85,    19,
      43,    70,    96,    96,    19,    27,    19,    27,    19,    96,
      35,    36,    37,    85,    96,    92,    96,    25,     7,     8,
      89,     9,    10,    11,    12,    22,    23,    24,    90,    13,
      14,    15,    16,    17,    18,    20,    21,    91,    96,    96,
      96,    26,    26,    26,    96,     6,    26,    32,    28,    96,
       3,    96,     3,    96,    26,    31,    26,    26,    96,    93,
      94,    95,    26,    26,    26,    25,    96,    28,    28,    29,
      87,    88,    96,    88,    26,    35,    36,    55,    59,    65,
      69,    86,    40,    31,     6,     6,    26,    32,    30,    85,
      88,    85,    88,    65,    26,    88
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    52,    53,    54,    54,    54,    54,    54,    55,    56,
      57,    58,    58,    58,    59,    60,    61,    62,    62,    62,
      63,    64,    65,    65,    66,    66,    66,    67,    67,    68,
      68,    68,    68,    69,    69,    69,    70,    70,    70,    71,
      72,    72,    73,    74,    75,    76,    77,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    86,    86,
      87,    88,    89,    89,    90,    90,    90,    90,    90,    90,
      90,    91,    91,    91,    91,    91,    91,    91,    91,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    93,    93,
      93,    94,    94,    95,    95,    96,    96
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     2,     2,     0,     2,     4,
       5,     1,     1,     1,     2,     4,     5,     1,     1,     1,
       4,     1,     1,     1,     1,     1,     1,     4,     1,     1,
       1,     1,     1,     3,     1,     0,     3,     1,     0,     7,
       7,     5,     5,     9,     4,     3,     2,     1,     4,     3,
       4,     3,     4,     3,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     0,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       3,     1,     3,     1,     3,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 111 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
               { YYACCEPT; }
#line 1570 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 3:
#line 114 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                    {
                                    rootNode->insertNode(yyvsp[0]);
                                }
#line 1578 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 4:
#line 117 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                            {
                                    rootNode->insertNode(yyvsp[0]);
                                }
#line 1586 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 5:
#line 120 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                              {
                                    rootNode->insertNode(yyvsp[0]);
                                }
#line 1594 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 6:
#line 123 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                    {
                                    rootNode->insertNode(yyvsp[0]);
                                }
#line 1602 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 7:
#line 126 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                         {}
#line 1608 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 8:
#line 131 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                   {
                                    yyval = new SimpleVariableDeclarationNode((char*)yyvsp[0], VM::BasicObjectType::INTEGER);
                                }
#line 1616 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 9:
#line 134 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                         {
                                    yyval = new InitVariableDeclarationNode((char*)yyvsp[-2], VM::BasicObjectType::INTEGER, dynamic_cast<ExpressionNode *>(yyvsp[0]));
                                }
#line 1624 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 10:
#line 137 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                                       {
                                    yyval = new ArrayVariableDeclarationNode((char*)yyvsp[-3], VM::BasicObjectType::INTEGER, atoi((char*)yyvsp[-1]));
                                }
#line 1632 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 11:
#line 141 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                   {}
#line 1638 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 12:
#line 142 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                      {}
#line 1644 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 13:
#line 143 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                       {}
#line 1650 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 14:
#line 148 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                     {
                                    yyval = new SimpleVariableDeclarationNode((char*)yyvsp[0], VM::BasicObjectType::FLOATING);
                                }
#line 1658 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 15:
#line 151 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                           {
                                    yyval = new InitVariableDeclarationNode((char*)yyvsp[-2], VM::BasicObjectType::FLOATING, dynamic_cast<ExpressionNode *>(yyvsp[0]));
                                }
#line 1666 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 16:
#line 154 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                                         {
                                    yyval = new ArrayVariableDeclarationNode((char*)yyvsp[-3], VM::BasicObjectType::FLOATING, atoi((char*)yyvsp[-1]));
                                }
#line 1674 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 17:
#line 158 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                     {}
#line 1680 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 18:
#line 159 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                        {}
#line 1686 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 19:
#line 160 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                         {}
#line 1692 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 20:
#line 165 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                            {
                                    yyval = new InitVariableDeclarationNode((char*)yyvsp[-2], VM::BasicObjectType::STRING, dynamic_cast<ExpressionNode *>(yyvsp[0]));
                                }
#line 1700 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 21:
#line 169 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                      {}
#line 1706 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 22:
#line 174 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                     {}
#line 1712 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 23:
#line 175 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                         {}
#line 1718 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 24:
#line 177 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                       {}
#line 1724 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 25:
#line 178 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                           {}
#line 1730 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 26:
#line 179 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                            {}
#line 1736 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 27:
#line 181 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                               {
                                    yyval = new VariableValueNode((char*)yyvsp[-3], dynamic_cast<ExpressionNode *>(yyvsp[-1]));
                                }
#line 1744 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 28:
#line 184 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                             {
                                    yyval = new VariableValueNode((char*)yyvsp[0]);
                                }
#line 1752 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 29:
#line 191 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                        {}
#line 1758 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 30:
#line 192 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                            {}
#line 1764 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 31:
#line 193 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                             {}
#line 1770 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 32:
#line 194 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                           {}
#line 1776 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 33:
#line 196 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                                         {
                                    codegenCtx.currentFuncArgs.push_back(dynamic_cast<SimpleVariableDeclarationNode *>(yyvsp[0]));
                                }
#line 1784 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 34:
#line 199 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                            {
                                    codegenCtx.currentFuncArgs.push_back(dynamic_cast<SimpleVariableDeclarationNode *>(yyvsp[0]));
                                }
#line 1792 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 35:
#line 202 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                         {}
#line 1798 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 36:
#line 204 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                         {
                                    codegenCtx.currentCalleeArgs.push_back(dynamic_cast<ExpressionNode *>(yyvsp[0]));
                                }
#line 1806 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 37:
#line 207 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                             {
                                    codegenCtx.currentCalleeArgs.push_back(dynamic_cast<ExpressionNode *>(yyvsp[0]));
                                }
#line 1814 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 38:
#line 210 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                         {}
#line 1820 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 39:
#line 212 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                                                                                      {
                                    ReturnType returnType = ReturnType::RETURN_TYPE_INT;
                                    std::string returnTypeStr = (char*)yyvsp[-6];
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

                                    yyval = new FunctionDeclarationNode(returnType, (char*)yyvsp[-4], codegenCtx.currentFuncArgs, dynamic_cast<StatementsScopeNode *>(yyvsp[0]));
                                    codegenCtx.currentFuncArgs.clear();
                                }
#line 1844 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 40:
#line 235 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                                                                        {
                                    yyval = new IfStatementNode(dynamic_cast<ExpressionNode *>(yyvsp[-4]),
                                                             dynamic_cast<StatementsScopeNode *>(yyvsp[-2]),
                                                             dynamic_cast<StatementsScopeNode *>(yyvsp[0]));
                                }
#line 1854 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 41:
#line 240 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                                              {
                                    yyval = new IfStatementNode(dynamic_cast<ExpressionNode *>(yyvsp[-2]),
                                                             dynamic_cast<StatementsScopeNode *>(yyvsp[0]));
                                }
#line 1863 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 42:
#line 245 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                                               {
                                    yyval = new WhileLoopStatementNode(dynamic_cast<ExpressionNode *>(yyvsp[-2]),
                                                                    dynamic_cast<StatementsScopeNode *>(yyvsp[0]));
                                }
#line 1872 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 43:
#line 250 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                                                                                     {
                                    yyval = new ForLoopStatementNode(yyvsp[-6], dynamic_cast<ExpressionNode *>(yyvsp[-4]), dynamic_cast<StatementsScopeNode *>(yyvsp[0]), yyvsp[-2]);
                                }
#line 1880 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 44:
#line 254 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                                          {
                                    yyval = new FunctionCallNode((char*)yyvsp[-3], codegenCtx.currentCalleeArgs);
                                    codegenCtx.currentCalleeArgs.clear();
                                }
#line 1889 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 45:
#line 259 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                    {
                                    yyval = new AssignmentStatement(dynamic_cast<VariableValueNode *>(yyvsp[-2]), dynamic_cast<ExpressionNode *>(yyvsp[0]));
                                }
#line 1897 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 46:
#line 263 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                         {
                                    yyval = new ReturnStatementNode(dynamic_cast<ExpressionNode *>(yyvsp[0]));
                                }
#line 1905 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 47:
#line 266 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                {
                                    yyval = new ReturnStatementNode();
                                }
#line 1913 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 48:
#line 270 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                               {
                                    yyval = new PrintStatementNode(VM::BasicObjectType::INTEGER, dynamic_cast<ExpressionNode *>(yyvsp[-1]));
                                }
#line 1921 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 49:
#line 274 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                   {
                                    yyval = new ScanStatementNode(VM::BasicObjectType::INTEGER);
                                }
#line 1929 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 50:
#line 278 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                                {
                                    yyval = new PrintStatementNode(VM::BasicObjectType::FLOATING, dynamic_cast<ExpressionNode *>(yyvsp[-1]));
                                }
#line 1937 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 51:
#line 282 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                    {
                                    yyval = new ScanStatementNode(VM::BasicObjectType::FLOATING);
                                }
#line 1945 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 52:
#line 286 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                                {
                                    yyval = new PrintStatementNode(VM::BasicObjectType::STRING, dynamic_cast<ExpressionNode *>(yyvsp[-1]));
                                }
#line 1953 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 53:
#line 290 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                    {
                                    yyval = new ScanStatementNode(VM::BasicObjectType::STRING);
                                }
#line 1961 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 54:
#line 294 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                              {
                                    yyval = new SqrtStatementNode(dynamic_cast<ExpressionNode *>(yyvsp[-1]));
                                }
#line 1969 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 55:
#line 299 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                    {}
#line 1975 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 56:
#line 300 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                     {}
#line 1981 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 57:
#line 301 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                              {}
#line 1987 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 58:
#line 302 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                     {}
#line 1993 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 59:
#line 303 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                   {}
#line 1999 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 60:
#line 304 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                               {}
#line 2005 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 61:
#line 305 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                  {}
#line 2011 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 62:
#line 306 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                 {}
#line 2017 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 63:
#line 307 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                {}
#line 2023 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 64:
#line 308 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                  {}
#line 2029 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 65:
#line 309 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                 {}
#line 2035 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 66:
#line 310 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                  {}
#line 2041 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 67:
#line 311 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                 {}
#line 2047 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 68:
#line 313 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                        {
                                    codegenCtx.scopeStatements.push_back(yyvsp[0]);
                                }
#line 2055 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 69:
#line 316 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                         {}
#line 2061 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 70:
#line 318 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                          {
                                    codegenCtx.scopeStatementsStack.push(codegenCtx.scopeStatements);
                                    codegenCtx.scopeStatements.clear();
                                }
#line 2070 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 71:
#line 323 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                              {
                                    yyval = new StatementsScopeNode(codegenCtx.scopeStatements);
                                    codegenCtx.scopeStatements.clear();
                                    codegenCtx.scopeStatements = codegenCtx.scopeStatementsStack.top();
                                    codegenCtx.scopeStatementsStack.pop();
                                }
#line 2081 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 72:
#line 333 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                    {}
#line 2087 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 73:
#line 334 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                      {}
#line 2093 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 74:
#line 336 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                    {}
#line 2099 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 75:
#line 337 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                      {}
#line 2105 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 76:
#line 338 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                            {}
#line 2111 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 77:
#line 339 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                             {}
#line 2117 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 78:
#line 340 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                             {}
#line 2123 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 79:
#line 341 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                            {}
#line 2129 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 80:
#line 342 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                             {}
#line 2135 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 81:
#line 344 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                     {}
#line 2141 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 82:
#line 345 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                           {}
#line 2147 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 83:
#line 346 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                          {}
#line 2153 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 84:
#line 347 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                              {}
#line 2159 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 85:
#line 348 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                        {}
#line 2165 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 86:
#line 349 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                           {}
#line 2171 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 87:
#line 350 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                           {}
#line 2177 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 88:
#line 351 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                          {}
#line 2183 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 89:
#line 354 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                           {
                                    yyval = new PrimaryNode(atoi((char*)yyvsp[0]));
                                }
#line 2191 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 90:
#line 357 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                               {
                                    yyval = new PrimaryNode(std::stod((char*)yyvsp[0]));
                                }
#line 2199 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 91:
#line 360 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                {
                                    yyval = new PrimaryNode((char*)yyvsp[0]);
                                }
#line 2207 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 92:
#line 363 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                {
                                    yyval = new PrimaryNode(dynamic_cast<VariableValueNode *>(yyvsp[0]));
                                }
#line 2215 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 93:
#line 366 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                               {
                                    yyval = new PrimaryNode(dynamic_cast<FunctionCallNode *>(yyvsp[0]));
                                }
#line 2223 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 94:
#line 369 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                {
                                    yyval = new PrimaryNode(dynamic_cast<ScanStatementNode *>(yyvsp[0]));
                                }
#line 2231 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 95:
#line 372 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                 {
                                    yyval = new PrimaryNode(dynamic_cast<ScanStatementNode *>(yyvsp[0]));
                                }
#line 2239 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 96:
#line 375 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                 {
                                    yyval = new PrimaryNode(dynamic_cast<ScanStatementNode *>(yyvsp[0]));
                                }
#line 2247 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 97:
#line 378 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                               {
                                    yyval = new PrimaryNode(dynamic_cast<SqrtStatementNode *>(yyvsp[0]));
                                }
#line 2255 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 98:
#line 382 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                        {
                                    yyval = new FactorNode(dynamic_cast<PrimaryNode *>(yyvsp[0]));
                                }
#line 2263 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 99:
#line 385 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                              {
                                    yyval = new FactorNode(dynamic_cast<PrimaryNode *>(yyvsp[0]), true);
                                }
#line 2271 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 100:
#line 388 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                    {
                                    yyval = new FactorNode(dynamic_cast<ExpressionNode *>(yyvsp[-1]));
                                }
#line 2279 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 101:
#line 393 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                       {
                                    yyval = new SummandNode(dynamic_cast<FactorNode *>(yyvsp[0]));
                                }
#line 2287 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 102:
#line 396 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                       {
                                    HighPriorityOperation operation = HighPriorityOperation::HIGH_PRIORITY_OPERATION_NONE;

                                    std::string operationStr = (char*)yyvsp[-1];
                                    if (operationStr == "*") {
                                        operation = HighPriorityOperation::HIGH_PRIORITY_OPERATION_MUL;
                                    }
                                    else if (operationStr == "/") {
                                        operation = HighPriorityOperation::HIGH_PRIORITY_OPERATION_DIV;
                                    }

                                    yyval = new SummandNode(dynamic_cast<SummandNode *>(yyvsp[-2]), dynamic_cast<FactorNode *>(yyvsp[0]), operation);
                                }
#line 2305 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 103:
#line 411 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                        {
                                    yyval = new SimpleNode(dynamic_cast<SummandNode *>(yyvsp[0]));
                                }
#line 2313 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 104:
#line 414 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                      {
                                    LowPriorityOperation operation = LowPriorityOperation::LOW_PRIORITY_OPERATION_NONE;

                                    std::string operationStr = (char*)yyvsp[-1];
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
                                    yyval = new SimpleNode(dynamic_cast<SimpleNode *>(yyvsp[-2]), dynamic_cast<SummandNode *>(yyvsp[0]), operation);
                                }
#line 2345 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 105:
#line 443 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                       {
                                    yyval = new ExpressionNode(dynamic_cast<SimpleNode *>(yyvsp[0]));
                                }
#line 2353 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;

  case 106:
#line 446 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
                                                                        {
                                    ExpressionOperation operation = ExpressionOperation::EXPRESSION_OPERATION_NONE;
                                    
                                    std::string operationStr = (char*)yyvsp[-1];
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

                                    yyval = new ExpressionNode(dynamic_cast<ExpressionNode *>(yyvsp[-2]), dynamic_cast<SimpleNode *>(yyvsp[0]), operation);
                                }
#line 2389 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"
    break;


#line 2393 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 478 "/home/michael/Desktop/MIPT/VirtualMachine/frontend/Parser/VMLangParser.y"
