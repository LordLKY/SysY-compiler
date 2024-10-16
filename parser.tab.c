/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 1 "parser.y"

#include<iostream>
#include<string>
#include "AST.h"
using namespace std;

int yylex();
void yyerror(astNode *&ast, const char *s); // changed by %parse-param

/* Line 371 of yacc.c  */
#line 78 "parser.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.tab.h".  */
#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INTEGER = 258,
     DECIMAL = 259,
     IDENTIFIER = 260,
     WHILE = 261,
     IF = 262,
     ELSE = 263,
     BREAK = 264,
     CONTINUE = 265,
     RETURN = 266,
     INT = 267,
     FLOAT = 268,
     VOID = 269,
     CONST = 270,
     OR = 271,
     AND = 272,
     GE = 273,
     LE = 274,
     EQ = 275,
     NE = 276,
     PRINT = 277
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 13 "parser.y"

    int iVal;
    float fVal;
    string *Ident;
    astNode *pNode;


/* Line 387 of yacc.c  */
#line 151 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (astNode *&ast);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 179 "parser.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  20
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   276

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  106
/* YYNRULES -- Number of states.  */
#define YYNSTATES  207

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   277

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    34,     2,     2,     2,    37,     2,     2,
      30,    31,    35,    32,    27,    33,     2,    36,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    25,
      38,    26,    39,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    23,     2,    24,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    28,     2,    29,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    16,    21,
      26,    31,    36,    40,    44,    46,    50,    52,    55,    59,
      61,    65,    69,    73,    75,    79,    81,    85,    87,    91,
      93,    96,   100,   102,   106,   112,   119,   125,   132,   138,
     145,   147,   151,   154,   157,   160,   163,   167,   172,   174,
     176,   179,   183,   185,   187,   189,   192,   197,   199,   202,
     204,   212,   218,   224,   227,   230,   233,   237,   243,   245,
     250,   255,   257,   259,   261,   263,   265,   269,   271,   273,
     275,   279,   284,   287,   290,   293,   295,   299,   301,   305,
     309,   313,   315,   319,   323,   325,   329,   333,   337,   341,
     343,   347,   351,   353,   357,   359,   363
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      41,     0,    -1,    42,    -1,    41,    42,    -1,    43,    -1,
      55,    -1,    45,    -1,    50,    -1,     5,    23,    79,    24,
      -1,    44,    23,    79,    24,    -1,    15,    12,    49,    25,
      -1,    15,    13,    49,    25,    -1,     5,    26,    48,    -1,
      44,    26,    48,    -1,    48,    -1,    47,    27,    48,    -1,
      79,    -1,    28,    29,    -1,    28,    47,    29,    -1,    46,
      -1,    49,    27,    46,    -1,    12,    54,    25,    -1,    13,
      54,    25,    -1,     5,    -1,     5,    26,    53,    -1,    44,
      -1,    44,    26,    53,    -1,    53,    -1,    52,    27,    53,
      -1,    65,    -1,    28,    29,    -1,    28,    52,    29,    -1,
      51,    -1,    54,    27,    51,    -1,    12,     5,    30,    31,
      61,    -1,    12,     5,    30,    56,    31,    61,    -1,    13,
       5,    30,    31,    61,    -1,    13,     5,    30,    56,    31,
      61,    -1,    14,     5,    30,    31,    61,    -1,    14,     5,
      30,    56,    31,    61,    -1,    57,    -1,    56,    27,    57,
      -1,    12,     5,    -1,    13,     5,    -1,    12,    58,    -1,
      13,    58,    -1,     5,    23,    24,    -1,    58,    23,    65,
      24,    -1,    28,    -1,    29,    -1,    59,    60,    -1,    59,
      63,    60,    -1,    43,    -1,    64,    -1,    62,    -1,    63,
      62,    -1,    68,    26,    65,    25,    -1,    25,    -1,    65,
      25,    -1,    61,    -1,     7,    30,    67,    31,    64,     8,
      64,    -1,     7,    30,    67,    31,    64,    -1,     6,    30,
      67,    31,    64,    -1,     9,    25,    -1,    10,    25,    -1,
      11,    25,    -1,    11,    65,    25,    -1,    22,    30,    65,
      31,    25,    -1,    74,    -1,     5,    23,    65,    24,    -1,
      66,    23,    65,    24,    -1,    78,    -1,     5,    -1,    66,
      -1,    70,    -1,    68,    -1,    30,    65,    31,    -1,     3,
      -1,     4,    -1,    69,    -1,     5,    30,    31,    -1,     5,
      30,    72,    31,    -1,    32,    71,    -1,    33,    71,    -1,
      34,    71,    -1,    65,    -1,    72,    27,    65,    -1,    71,
      -1,    73,    35,    71,    -1,    73,    36,    71,    -1,    73,
      37,    71,    -1,    73,    -1,    74,    32,    73,    -1,    74,
      33,    73,    -1,    74,    -1,    75,    38,    74,    -1,    75,
      39,    74,    -1,    75,    19,    74,    -1,    75,    18,    74,
      -1,    75,    -1,    76,    20,    75,    -1,    76,    21,    75,
      -1,    76,    -1,    77,    17,    76,    -1,    77,    -1,    78,
      16,    77,    -1,    74,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    43,    43,    47,    53,    54,    57,    58,    61,    62,
      65,    69,    75,    76,    79,    80,    83,    84,    85,    88,
      89,    92,    96,   102,   103,   104,   105,   108,   109,   112,
     113,   114,   117,   118,   121,   126,   131,   136,   141,   146,
     153,   154,   157,   158,   159,   160,   163,   164,   167,   170,
     173,   174,   177,   178,   181,   182,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   199,   202,
     203,   206,   209,   210,   213,   214,   215,   218,   219,   222,
     223,   224,   225,   226,   227,   230,   231,   234,   235,   236,
     237,   240,   241,   242,   245,   246,   247,   248,   249,   252,
     253,   254,   257,   258,   261,   262,   265
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTEGER", "DECIMAL", "IDENTIFIER",
  "WHILE", "IF", "ELSE", "BREAK", "CONTINUE", "RETURN", "INT", "FLOAT",
  "VOID", "CONST", "OR", "AND", "GE", "LE", "EQ", "NE", "PRINT", "'['",
  "']'", "';'", "'='", "','", "'{'", "'}'", "'('", "')'", "'+'", "'-'",
  "'!'", "'*'", "'/'", "'%'", "'<'", "'>'", "$accept", "CompUnitList",
  "CompUnit", "Decl", "ArrayDef", "ConstDecl", "ConstDef",
  "ConstArrayInitVal", "ConstInitVal", "ConstDefList", "VarDecl", "VarDef",
  "VarArrayInitVal", "VarInitVal", "VarDefList", "FuncDef", "FuncFParams",
  "FuncFParam", "FuncFArrayParam", "ScopeStart", "ScopeEnd", "Block",
  "BlockItem", "BlockList", "Stmt", "Exp", "ArrayExp", "Cond", "LVal",
  "PrimaryExp", "Number", "UnaryExp", "FuncRParams", "MulExp", "AddExp",
  "RelExp", "EqExp", "LAndExp", "LOrExp", "ConstExp", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,    91,    93,    59,    61,    44,   123,   125,
      40,    41,    43,    45,    33,    42,    47,    37,    60,    62
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    40,    41,    41,    42,    42,    43,    43,    44,    44,
      45,    45,    46,    46,    47,    47,    48,    48,    48,    49,
      49,    50,    50,    51,    51,    51,    51,    52,    52,    53,
      53,    53,    54,    54,    55,    55,    55,    55,    55,    55,
      56,    56,    57,    57,    57,    57,    58,    58,    59,    60,
      61,    61,    62,    62,    63,    63,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    65,    66,
      66,    67,    68,    68,    69,    69,    69,    70,    70,    71,
      71,    71,    71,    71,    71,    72,    72,    73,    73,    73,
      73,    74,    74,    74,    75,    75,    75,    75,    75,    76,
      76,    76,    77,    77,    78,    78,    79
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     4,     4,
       4,     4,     3,     3,     1,     3,     1,     2,     3,     1,
       3,     3,     3,     1,     3,     1,     3,     1,     3,     1,
       2,     3,     1,     3,     5,     6,     5,     6,     5,     6,
       1,     3,     2,     2,     2,     2,     3,     4,     1,     1,
       2,     3,     1,     1,     1,     2,     4,     1,     2,     1,
       7,     5,     5,     2,     2,     2,     3,     5,     1,     4,
       4,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       3,     4,     2,     2,     2,     1,     3,     1,     3,     3,
       3,     1,     3,     3,     1,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     1,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     2,     4,     6,     7,
       5,    23,    25,    32,     0,    23,     0,     0,     0,     0,
       1,     3,     0,     0,     0,     0,     0,    21,     0,     0,
      22,     0,     0,     0,    19,     0,     0,    77,    78,    72,
       0,     0,     0,     0,    73,    75,    79,    74,    87,    91,
     106,     0,     0,    24,    29,    68,     0,     0,     0,     0,
      40,     0,    26,    23,    33,     0,     0,     0,     0,     0,
       0,    10,     0,    11,     0,     0,     0,    82,    83,    84,
       0,     0,     0,     0,     0,     0,     8,    30,     0,    27,
      42,    44,    43,    45,    48,     0,    34,     0,     0,     9,
      36,     0,    38,     0,     0,    12,    16,    13,    20,     0,
      80,    85,     0,    76,     0,    88,    89,    90,    92,    93,
       0,    31,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    49,    52,    50,    59,    54,     0,    53,
       0,    75,    41,    35,    37,    39,    17,     0,    14,    69,
       0,    81,    70,    28,    46,     0,     0,     0,    63,    64,
      65,     0,     0,    51,    55,    58,     0,     0,    18,    86,
      47,     0,    94,    99,   102,   104,    71,     0,    66,     0,
       0,    15,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,    62,    98,    97,    95,    96,   100,
     101,   103,   105,    61,    67,     0,    60
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,     7,    12,     8,    34,   147,   105,    35,
       9,    13,    88,    53,    14,    10,    59,    60,    91,    95,
     135,   136,   137,   138,   139,   140,    44,   171,    45,    46,
      47,    48,   112,    49,    55,   173,   174,   175,   176,   106
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -157
static const yytype_int16 yypact[] =
{
     142,     9,    17,    49,    18,   103,  -157,  -157,  -157,  -157,
    -157,   107,     2,  -157,   159,   149,   168,    16,    85,    85,
    -157,  -157,    47,   155,     3,    47,   155,  -157,    89,    74,
    -157,   101,   181,   182,  -157,   188,   189,  -157,  -157,    41,
      47,    47,    47,    47,    76,  -157,  -157,  -157,  -157,   175,
      51,    84,     8,  -157,  -157,    51,   192,   229,   210,    79,
    -157,   217,  -157,   183,  -157,   210,    80,   210,    97,   166,
     166,  -157,    85,  -157,    47,   187,   211,  -157,  -157,  -157,
      47,    47,    47,    47,    47,    47,  -157,  -157,   195,  -157,
     220,   221,   220,   221,  -157,    63,  -157,   161,   210,  -157,
    -157,   210,  -157,   210,   148,  -157,  -157,  -157,  -157,   222,
    -157,  -157,   109,  -157,   223,  -157,  -157,  -157,   175,   175,
     155,  -157,   224,    47,   215,   219,   225,   226,   198,    89,
      89,   227,  -157,  -157,  -157,  -157,  -157,  -157,    63,  -157,
     228,   230,  -157,  -157,  -157,  -157,  -157,   200,  -157,  -157,
      47,  -157,  -157,  -157,  -157,   231,    47,    47,  -157,  -157,
    -157,   233,    47,  -157,  -157,  -157,    47,   166,  -157,  -157,
    -157,   232,    51,     5,   205,   235,   238,   234,  -157,   236,
     237,  -157,   116,    47,    47,    47,    47,    47,    47,    47,
      47,   116,   239,  -157,  -157,    51,    51,    51,    51,     5,
       5,   205,   235,   251,  -157,   116,  -157
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -157,  -157,   255,   -85,   -10,  -157,   194,  -157,   -65,   242,
    -157,   240,  -157,   -20,    -1,  -157,   204,   172,   213,  -157,
     133,   -38,   134,  -157,  -156,   -19,  -157,   118,   -93,  -157,
    -157,   -24,  -157,   152,   -22,    52,    87,    83,  -157,   117
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      50,    16,   141,    50,    54,   107,    62,    54,    33,    33,
     134,    37,    38,    39,    11,    56,    57,    77,    78,    79,
      96,    76,    15,   183,   184,    25,   194,   100,    26,   102,
      18,    19,    89,    54,    58,   203,    52,    87,    40,   148,
      41,    42,    43,   185,   186,   141,    31,    50,    50,   206,
      37,    38,    39,   134,    17,   109,   111,   115,   116,   117,
     143,   114,    33,   144,    74,   145,    37,    38,    39,   124,
     125,    75,   126,   127,   128,   129,   130,    40,     4,    41,
      42,    43,    50,    84,    85,   131,    56,    57,   132,   141,
      32,    94,   133,    40,    63,    41,    42,    43,   141,    80,
     153,    54,   181,    20,   155,    65,    97,    97,    86,   161,
      98,   101,   141,    56,    57,     1,     2,     3,     4,    37,
      38,    39,   124,   125,    97,   126,   127,   128,   103,    16,
      22,   169,    67,    23,   172,   172,   150,    24,   131,    51,
     151,   132,    61,   179,    94,    50,    40,   180,    41,    42,
      43,    37,    38,    39,     1,     2,     3,     4,    37,    38,
      39,   195,   196,   197,   198,   172,   172,   172,   172,    37,
      38,    39,    22,    56,    57,    23,   104,   146,    40,    29,
      41,    42,    43,    52,    27,    40,    28,    41,    42,    43,
      37,    38,    39,    30,   104,    28,    40,    90,    41,    42,
      43,    37,    38,    39,    22,    25,    22,    69,    70,    23,
      81,    82,    83,    71,    73,    72,    72,    40,   110,    41,
      42,    43,   120,   160,   121,   187,   188,   167,    40,   168,
      41,    42,    43,    66,    92,    68,   118,   119,    94,   199,
     200,    99,   113,   122,   123,   156,   149,   152,   154,   157,
     158,   159,   189,   165,   190,   170,   166,   162,   178,   205,
      21,    36,   193,   182,   204,   191,   108,   192,    64,   142,
      93,   163,   164,   202,     0,   177,   201
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-157)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      22,     2,    95,    25,    23,    70,    26,    26,    18,    19,
      95,     3,     4,     5,     5,    12,    13,    41,    42,    43,
      58,    40,     5,    18,    19,    23,   182,    65,    26,    67,
      12,    13,    52,    52,    31,   191,    28,    29,    30,   104,
      32,    33,    34,    38,    39,   138,    30,    69,    70,   205,
       3,     4,     5,   138,     5,    74,    75,    81,    82,    83,
      98,    80,    72,   101,    23,   103,     3,     4,     5,     6,
       7,    30,     9,    10,    11,    12,    13,    30,    15,    32,
      33,    34,   104,    32,    33,    22,    12,    13,    25,   182,
       5,    28,    29,    30,     5,    32,    33,    34,   191,    23,
     120,   120,   167,     0,   123,    31,    27,    27,    24,   128,
      31,    31,   205,    12,    13,    12,    13,    14,    15,     3,
       4,     5,     6,     7,    27,     9,    10,    11,    31,   130,
      23,   150,    31,    26,   156,   157,    27,    30,    22,    22,
      31,    25,    25,   162,    28,   167,    30,   166,    32,    33,
      34,     3,     4,     5,    12,    13,    14,    15,     3,     4,
       5,   183,   184,   185,   186,   187,   188,   189,   190,     3,
       4,     5,    23,    12,    13,    26,    28,    29,    30,    30,
      32,    33,    34,    28,    25,    30,    27,    32,    33,    34,
       3,     4,     5,    25,    28,    27,    30,     5,    32,    33,
      34,     3,     4,     5,    23,    23,    23,    26,    26,    26,
      35,    36,    37,    25,    25,    27,    27,    30,    31,    32,
      33,    34,    27,    25,    29,    20,    21,    27,    30,    29,
      32,    33,    34,    29,     5,    31,    84,    85,    28,   187,
     188,    24,    31,    23,    23,    30,    24,    24,    24,    30,
      25,    25,    17,    25,    16,    24,    26,    30,    25,     8,
       5,    19,    25,    31,    25,    31,    72,    31,    28,    97,
      57,   138,   138,   190,    -1,   157,   189
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    12,    13,    14,    15,    41,    42,    43,    45,    50,
      55,     5,    44,    51,    54,     5,    54,     5,    12,    13,
       0,    42,    23,    26,    30,    23,    26,    25,    27,    30,
      25,    30,     5,    44,    46,    49,    49,     3,     4,     5,
      30,    32,    33,    34,    66,    68,    69,    70,    71,    73,
      74,    79,    28,    53,    65,    74,    12,    13,    31,    56,
      57,    79,    53,     5,    51,    31,    56,    31,    56,    26,
      26,    25,    27,    25,    23,    30,    65,    71,    71,    71,
      23,    35,    36,    37,    32,    33,    24,    29,    52,    53,
       5,    58,     5,    58,    28,    59,    61,    27,    31,    24,
      61,    31,    61,    31,    28,    48,    79,    48,    46,    65,
      31,    65,    72,    31,    65,    71,    71,    71,    73,    73,
      27,    29,    23,    23,     6,     7,     9,    10,    11,    12,
      13,    22,    25,    29,    43,    60,    61,    62,    63,    64,
      65,    68,    57,    61,    61,    61,    29,    47,    48,    24,
      27,    31,    24,    53,    24,    65,    30,    30,    25,    25,
      25,    65,    30,    60,    62,    25,    26,    27,    29,    65,
      24,    67,    74,    75,    76,    77,    78,    67,    25,    65,
      65,    48,    31,    18,    19,    38,    39,    20,    21,    17,
      16,    31,    31,    25,    64,    74,    74,    74,    74,    75,
      75,    76,    77,    64,    25,     8,    64
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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
      yyerror (ast, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, ast); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, astNode *&ast)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, ast)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    astNode *&ast;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
  YYUSE (ast);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, astNode *&ast)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, ast)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    astNode *&ast;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, ast);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, astNode *&ast)
#else
static void
yy_reduce_print (yyvsp, yyrule, ast)
    YYSTYPE *yyvsp;
    int yyrule;
    astNode *&ast;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       , ast);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, ast); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
	    /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
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
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
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
      int yyn = yypact[*yyssp];
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
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
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
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
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, astNode *&ast)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, ast)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    astNode *&ast;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (ast);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (astNode *&ast)
#else
int
yyparse (ast)
    astNode *&ast;
#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

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
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
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
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
      yychar = YYLEX;
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

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
/* Line 1792 of yacc.c  */
#line 43 "parser.y"
    {
        (yyval.pNode) = newExp(_CompUnits, 1, (yyvsp[(1) - (1)].pNode)); /* wrap as CompUnitList */
        ast = (yyval.pNode);
    }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 47 "parser.y"
    {
        (yyval.pNode) = mergeNode((yyvsp[(1) - (2)].pNode), (yyvsp[(2) - (2)].pNode)); /* merge the nodes */
        ast = (yyval.pNode);
    }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 53 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 54 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 57 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 58 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 61 "parser.y"
    {(yyval.pNode) = newExp(_ArrayDef, 2, newId(Array, (yyvsp[(1) - (4)].Ident)), (yyvsp[(3) - (4)].pNode));}
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 62 "parser.y"
    {(yyval.pNode) = mergeNode((yyvsp[(1) - (4)].pNode), (yyvsp[(3) - (4)].pNode));}
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 65 "parser.y"
    {
        (yyval.pNode) = (yyvsp[(3) - (4)].pNode);
        sym_preprocess_datadef(CONST_I32, (yyvsp[(3) - (4)].pNode));
    }
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 69 "parser.y"
    {
        (yyval.pNode) = (yyvsp[(3) - (4)].pNode);
        sym_preprocess_datadef(CONST_F32, (yyvsp[(3) - (4)].pNode));
    }
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 75 "parser.y"
    {(yyval.pNode) = newExp(_ConstDef, 2, newId(Simple, (yyvsp[(1) - (3)].Ident)), (yyvsp[(3) - (3)].pNode));}
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 76 "parser.y"
    {(yyval.pNode) = newExp(_ConstDef, 2, (yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode));}
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 79 "parser.y"
    {(yyval.pNode) = newExp(_ConstArrayInitVal, 1, (yyvsp[(1) - (1)].pNode));}
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 80 "parser.y"
    {(yyval.pNode) = mergeNode((yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode));}
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 83 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 84 "parser.y"
    {(yyval.pNode) = newExp(_ConstArrayInitVal, 0);}
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 85 "parser.y"
    {(yyval.pNode) = (yyvsp[(2) - (3)].pNode);}
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 88 "parser.y"
    {(yyval.pNode) = newExp(_ConstDefs, 1, (yyvsp[(1) - (1)].pNode)); /* wrap as ConstDefList */}
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 89 "parser.y"
    {(yyval.pNode) = mergeNode((yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode)); /* merge the nodes*/}
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 92 "parser.y"
    {
        (yyval.pNode) = (yyvsp[(2) - (3)].pNode);
        sym_preprocess_datadef(I32, (yyvsp[(2) - (3)].pNode));
    }
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 96 "parser.y"
    {
        (yyval.pNode) = (yyvsp[(2) - (3)].pNode);
        sym_preprocess_datadef(F32, (yyvsp[(2) - (3)].pNode));
    }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 102 "parser.y"
    {(yyval.pNode) = newExp(_VarDef, 1, newId(Simple, (yyvsp[(1) - (1)].Ident)));}
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 103 "parser.y"
    {(yyval.pNode) = newExp(_VarDef, 2, newId(Simple, (yyvsp[(1) - (3)].Ident)), (yyvsp[(3) - (3)].pNode));}
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 104 "parser.y"
    {(yyval.pNode) = newExp(_VarDef, 1, (yyvsp[(1) - (1)].pNode));}
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 105 "parser.y"
    {(yyval.pNode) = newExp(_VarDef, 2, (yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode));}
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 108 "parser.y"
    {(yyval.pNode) = newExp(_VarArrayInitVal, 1, (yyvsp[(1) - (1)].pNode));}
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 109 "parser.y"
    {(yyval.pNode) = mergeNode((yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode));}
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 112 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 113 "parser.y"
    {(yyval.pNode) = newExp(_VarArrayInitVal, 0);}
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 114 "parser.y"
    {(yyval.pNode) = (yyvsp[(2) - (3)].pNode);}
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 117 "parser.y"
    {(yyval.pNode) = newExp(_VarDefs, 1, (yyvsp[(1) - (1)].pNode)); /* wrap as VarDefList */}
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 118 "parser.y"
    {(yyval.pNode) = mergeNode((yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode)); /* merge the nodes */}
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 121 "parser.y"
    {
        astNode *p = newId(Func, (yyvsp[(2) - (5)].Ident));
        (yyval.pNode) = newExp(_FuncDef, 2, p, (yyvsp[(5) - (5)].pNode));
        sym_preprocess_func(I32, p, newExp(_FuncFParams, 0));
    }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 126 "parser.y"
    {
        astNode *p = newId(Func, (yyvsp[(2) - (6)].Ident));
        (yyval.pNode) = newExp(_FuncDef, 3, p, (yyvsp[(4) - (6)].pNode), (yyvsp[(6) - (6)].pNode));
        sym_preprocess_func(I32, p, (yyvsp[(4) - (6)].pNode));
    }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 131 "parser.y"
    {
        astNode *p = newId(Func, (yyvsp[(2) - (5)].Ident));
        (yyval.pNode) = newExp(_FuncDef, 2, p, (yyvsp[(5) - (5)].pNode));
        sym_preprocess_func(F32, p, newExp(_FuncFParams, 0));
    }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 136 "parser.y"
    {
        astNode *p = newId(Func, (yyvsp[(2) - (6)].Ident));
        (yyval.pNode) = newExp(_FuncDef, 3, p, (yyvsp[(4) - (6)].pNode), (yyvsp[(6) - (6)].pNode));
        sym_preprocess_func(F32, p, (yyvsp[(4) - (6)].pNode));
    }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 141 "parser.y"
    {
        astNode *p = newId(Func, (yyvsp[(2) - (5)].Ident));
        (yyval.pNode) = newExp(_FuncDef, 2, p, (yyvsp[(5) - (5)].pNode));
        sym_preprocess_func(VOID_TYPE, p, newExp(_FuncFParams, 0));
    }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 146 "parser.y"
    {
        astNode *p = newId(Func, (yyvsp[(2) - (6)].Ident));
        (yyval.pNode) = newExp(_FuncDef, 3, p, (yyvsp[(4) - (6)].pNode), (yyvsp[(6) - (6)].pNode));
        sym_preprocess_func(VOID_TYPE, p, (yyvsp[(4) - (6)].pNode));
    }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 153 "parser.y"
    {(yyval.pNode) = newExp(_FuncFParams, 1, (yyvsp[(1) - (1)].pNode));}
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 154 "parser.y"
    {(yyval.pNode) = mergeNode((yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode));}
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 157 "parser.y"
    {(yyval.pNode) = newExp(_FuncFParam, 2, newI(_INT), newId(Simple, (yyvsp[(2) - (2)].Ident)));}
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 158 "parser.y"
    {(yyval.pNode) = newExp(_FuncFParam, 2, newI(_FLOAT), newId(Simple, (yyvsp[(2) - (2)].Ident)));}
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 159 "parser.y"
    {(yyval.pNode) = newExp(_FuncFParam, 2, newI(_INT), (yyvsp[(2) - (2)].pNode));}
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 160 "parser.y"
    {(yyval.pNode) = newExp(_FuncFParam, 2, newI(_FLOAT), (yyvsp[(2) - (2)].pNode));}
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 163 "parser.y"
    {(yyval.pNode) = newExp(_ArrayDef, 1, newId(Array, (yyvsp[(1) - (3)].Ident)));}
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 164 "parser.y"
    {(yyval.pNode) = mergeNode((yyvsp[(1) - (4)].pNode), (yyvsp[(3) - (4)].pNode));}
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 167 "parser.y"
    {scope_start(sym);}
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 170 "parser.y"
    {scope_end(sym);}
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 173 "parser.y"
    {(yyval.pNode) = newExp(_BlockItems, 0);}
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 174 "parser.y"
    {(yyval.pNode) = (yyvsp[(2) - (3)].pNode);}
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 177 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 178 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 181 "parser.y"
    {(yyval.pNode) = newExp(_BlockItems, 1, (yyvsp[(1) - (1)].pNode)); /* wrap as BlockList */}
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 182 "parser.y"
    {(yyval.pNode) = mergeNode((yyvsp[(1) - (2)].pNode), (yyvsp[(2) - (2)].pNode)); /* merge the nodes */}
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 185 "parser.y"
    {(yyval.pNode) = newExp('=', 2, (yyvsp[(1) - (4)].pNode), (yyvsp[(3) - (4)].pNode));}
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 186 "parser.y"
    {(yyval.pNode) = newExp(';', 0);}
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 187 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (2)].pNode);}
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 188 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 189 "parser.y"
    {(yyval.pNode) = newExp(IF, 3, (yyvsp[(3) - (7)].pNode), (yyvsp[(5) - (7)].pNode), (yyvsp[(7) - (7)].pNode));}
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 190 "parser.y"
    {(yyval.pNode) = newExp(IF, 2, (yyvsp[(3) - (5)].pNode), (yyvsp[(5) - (5)].pNode));}
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 191 "parser.y"
    {(yyval.pNode) = newExp(WHILE, 2, (yyvsp[(3) - (5)].pNode), (yyvsp[(5) - (5)].pNode));}
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 192 "parser.y"
    {(yyval.pNode) = newExp(BREAK, 0);}
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 193 "parser.y"
    {(yyval.pNode) = newExp(CONTINUE, 0);}
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 194 "parser.y"
    {(yyval.pNode) = newExp(RETURN, 0);}
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 195 "parser.y"
    {(yyval.pNode) = newExp(RETURN, 1, (yyvsp[(2) - (3)].pNode));}
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 196 "parser.y"
    {(yyval.pNode) = newExp(_PRINT, 1, (yyvsp[(3) - (5)].pNode));}
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 199 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 202 "parser.y"
    {(yyval.pNode) = newExp(_ArrayExp, 2, newId(Array, (yyvsp[(1) - (4)].Ident)), (yyvsp[(3) - (4)].pNode));}
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 203 "parser.y"
    {(yyval.pNode) = mergeNode((yyvsp[(1) - (4)].pNode), (yyvsp[(3) - (4)].pNode));}
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 206 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 209 "parser.y"
    {(yyval.pNode) = newId(Simple, (yyvsp[(1) - (1)].Ident));}
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 210 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 213 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 214 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 215 "parser.y"
    {(yyval.pNode) = (yyvsp[(2) - (3)].pNode);}
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 218 "parser.y"
    {(yyval.pNode) = newI((yyvsp[(1) - (1)].iVal));}
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 219 "parser.y"
    {(yyval.pNode) = newF((yyvsp[(1) - (1)].fVal));}
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 222 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 223 "parser.y"
    {(yyval.pNode) = newExp(_FuncCall, 1, newId(Func, (yyvsp[(1) - (3)].Ident)));}
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 224 "parser.y"
    {(yyval.pNode) = newExp(_FuncCall, 2, newId(Func, (yyvsp[(1) - (4)].Ident)), (yyvsp[(3) - (4)].pNode));}
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 225 "parser.y"
    {(yyval.pNode) = newExp('+', 2, newI(0), (yyvsp[(2) - (2)].pNode));}
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 226 "parser.y"
    {(yyval.pNode) = newExp('-', 2, newI(0), (yyvsp[(2) - (2)].pNode));}
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 227 "parser.y"
    {(yyval.pNode) = newExp(EQ, 2, newI(0), (yyvsp[(2) - (2)].pNode));}
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 230 "parser.y"
    {(yyval.pNode) = newExp(_FuncRParams, 1, (yyvsp[(1) - (1)].pNode));}
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 231 "parser.y"
    {(yyval.pNode) = mergeNode((yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode));}
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 234 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 235 "parser.y"
    {(yyval.pNode) = newExp('*', 2, (yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode));}
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 236 "parser.y"
    {(yyval.pNode) = newExp('/', 2, (yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode));}
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 237 "parser.y"
    {(yyval.pNode) = newExp('%', 2, (yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode));}
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 240 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 241 "parser.y"
    {(yyval.pNode) = newExp('+', 2, (yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode));}
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 242 "parser.y"
    {(yyval.pNode) = newExp('-', 2, (yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode));}
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 245 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 246 "parser.y"
    {(yyval.pNode) = newExp('<', 2, (yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode));}
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 247 "parser.y"
    {(yyval.pNode) = newExp('>', 2, (yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode));}
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 248 "parser.y"
    {(yyval.pNode) = newExp(LE, 2, (yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode));}
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 249 "parser.y"
    {(yyval.pNode) = newExp(GE, 2, (yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode));}
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 252 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 253 "parser.y"
    {(yyval.pNode) = newExp(EQ, 2, (yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode));}
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 254 "parser.y"
    {(yyval.pNode) = newExp(NE, 2, (yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode));}
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 257 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 258 "parser.y"
    {(yyval.pNode) = newExp(AND, 2, (yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode));}
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 261 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 262 "parser.y"
    {(yyval.pNode) = newExp(OR, 2, (yyvsp[(1) - (3)].pNode), (yyvsp[(3) - (3)].pNode));}
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 265 "parser.y"
    {(yyval.pNode) = (yyvsp[(1) - (1)].pNode);}
    break;


/* Line 1792 of yacc.c  */
#line 2243 "parser.tab.c"
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

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (ast, YY_("syntax error"));
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
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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
        yyerror (ast, yymsgp);
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
		      yytoken, &yylval, ast);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

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
		  yystos[yystate], yyvsp, ast);
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
  yyerror (ast, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, ast);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, ast);
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 267 "parser.y"


void yyerror(astNode *&ast, const char *s){
    extern int yylineno; // defined and maintained in lex
    extern char *yytext; // defined and maintained in lex
    cout << "Error: " << string(s) << " at symbol '";
    cout << string(yytext) << "' on line " << yylineno << endl;
    exit(-1);
}