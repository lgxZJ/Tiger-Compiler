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
#line 1 "myTiger.y"


  #include "abstractSyntaxMaker.h"
  #include "myReport.h"
  #include "y.tab.h"
  #include <stdio.h>
  #include <stdlib.h>

  extern FILE* yyin;
  extern char* yytext;
  extern PosInfo g_posInfo;

  //used to hold global abstract syntax tree
  myExp		g_AbstractSyntaxTree;
  
  extern int yylex();//	prototype for lex scanning


/* Line 371 of yacc.c  */
#line 87 "y.tab.c"

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
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
     INTEGER_LITERAL = 258,
     ID = 259,
     STRING_LITERAL = 260,
     INT = 261,
     STRING = 262,
     TYPE = 263,
     ARRAY = 264,
     OF = 265,
     VAR = 266,
     FUNCTION = 267,
     NIL = 268,
     LET = 269,
     IN = 270,
     END = 271,
     IF = 272,
     THEN = 273,
     ELSE = 274,
     WHILE = 275,
     DO = 276,
     FOR = 277,
     TO = 278,
     BREAK = 279,
     LEFT_CURLY_BRACE = 280,
     RIGHT_CURLY_BRACE = 281,
     COLON = 282,
     COMMA = 283,
     LEFT_PARENTHESIS = 284,
     RIGHT_PARENTHESIS = 285,
     DOT = 286,
     SEMICOLON = 287,
     LEFT_BRACKET = 288,
     RIGHT_BRACKET = 289,
     ASSIGNMENT_SIGN = 290,
     LOGICAL_OR = 291,
     LOGICAL_AND = 292,
     LESS_SIGN = 293,
     NOT_EQUAL_SIGN = 294,
     EQUAL_SIGN = 295,
     GREATER_EQUAL_SIGN = 296,
     LESS_EQUAL_SIGN = 297,
     GREATER_SIGN = 298,
     MINUS_SIGN = 299,
     ADD_SIGN = 300,
     DIVIDE_SIGN = 301,
     MULTIPLY_SIGN = 302
   };
#endif
/* Tokens.  */
#define INTEGER_LITERAL 258
#define ID 259
#define STRING_LITERAL 260
#define INT 261
#define STRING 262
#define TYPE 263
#define ARRAY 264
#define OF 265
#define VAR 266
#define FUNCTION 267
#define NIL 268
#define LET 269
#define IN 270
#define END 271
#define IF 272
#define THEN 273
#define ELSE 274
#define WHILE 275
#define DO 276
#define FOR 277
#define TO 278
#define BREAK 279
#define LEFT_CURLY_BRACE 280
#define RIGHT_CURLY_BRACE 281
#define COLON 282
#define COMMA 283
#define LEFT_PARENTHESIS 284
#define RIGHT_PARENTHESIS 285
#define DOT 286
#define SEMICOLON 287
#define LEFT_BRACKET 288
#define RIGHT_BRACKET 289
#define ASSIGNMENT_SIGN 290
#define LOGICAL_OR 291
#define LOGICAL_AND 292
#define LESS_SIGN 293
#define NOT_EQUAL_SIGN 294
#define EQUAL_SIGN 295
#define GREATER_EQUAL_SIGN 296
#define LESS_EQUAL_SIGN 297
#define GREATER_SIGN 298
#define MINUS_SIGN 299
#define ADD_SIGN 300
#define DIVIDE_SIGN 301
#define MULTIPLY_SIGN 302



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 20 "myTiger.y"

	int				int_val;
	myString			myString_val;
	myExp				myExp_val;
	myDec				myDec_val;
	myDecList			myDecList_val;
	myTypeDec			myTypeDec_val;
	myVarDec			myVarDec_val;
	myFuncDec			myFuncDec_val;
	myTy				myTy_val;
	myTyFieldList			myTyFieldList_val;
	myLValueExp			myLValue_val;
	myLValueAux			myLValueAux_val;
	myFunctionCallExp		myFunctionCallExp_val;
	myExpList			myExpList_val;
	myNilExp			myNilExp_val;
	myIntegerLiteralExp		myIntegerLiteralExp_val;
	myStringLiteralExp		myStringLiteralExp_val;
	myArrayCreationExp		myArrayCreationExp_val;
	myRecordCreationExp		myRecordCreationExp_val;
	myRecordFieldList		myRecordFieldList_val;
	myArithmeticExp			myArithmeticExp_val;
	myParenthesesExp		myParenthesesExp_val;
	myNoValueExp			myNoValueExp_val;
	mySequencingExp			mySequencingExp_val;
	myForExp			myForExp_val;
	myIfThenElseExp			myIfThenElseExp_val;
	myIfThenExp			myIfThenExp_val;
	myComparisonExp			myComparisonExp_val;
	myBooleanOperateExp		myBooleanOperateExp_val;
	myAssignmentExp			myAssignmentExp_val;
	myLetExp			myLetExp_val;
	myWhileExp			myWhileExp_val;
	myNegationExp			myNegationExp_val;
	myBreakExp			myBreakExp_val;


/* Line 387 of yacc.c  */
#line 262 "y.tab.c"
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
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 290 "y.tab.c"

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
#define YYFINAL  72
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   423

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  39
/* YYNRULES -- Number of rules.  */
#define YYNRULES  98
/* YYNRULES -- Number of states.  */
#define YYNSTATES  205

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   302

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
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
      45,    46,    47
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     7,     9,    11,    13,    18,    23,
      28,    33,    35,    37,    39,    43,    47,    51,    56,    61,
      62,    68,    74,    75,    80,    87,    94,   102,   112,   122,
     125,   129,   134,   135,   139,   145,   149,   150,   152,   154,
     156,   163,   170,   178,   186,   190,   194,   200,   201,   205,
     209,   213,   217,   221,   224,   231,   235,   236,   245,   252,
     257,   261,   265,   269,   273,   277,   281,   285,   289,   293,
     299,   302,   303,   307,   308,   313,   316,   318,   320,   322,
     324,   326,   328,   330,   332,   334,   336,   338,   340,   342,
     344,   346,   348,   350,   352,   354,   356,   358,   360
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      86,     0,    -1,    50,    49,    -1,    -1,    51,    -1,    56,
      -1,    57,    -1,     8,    52,    40,     4,    -1,     8,     4,
      40,     4,    -1,     8,    52,    40,    53,    -1,     8,     4,
      40,    53,    -1,     6,    -1,     7,    -1,    52,    -1,    25,
      54,    26,    -1,     9,    10,    52,    -1,     9,    10,     4,
      -1,     4,    27,    52,    55,    -1,     4,    27,     4,    55,
      -1,    -1,    28,     4,    27,    52,    55,    -1,    28,     4,
      27,     4,    55,    -1,    -1,    11,     4,    35,    85,    -1,
      11,     4,    27,    52,    35,    85,    -1,    11,     4,    27,
       4,    35,    85,    -1,    12,     4,    29,    54,    30,    40,
      85,    -1,    12,     4,    29,    54,    30,    27,    52,    40,
      85,    -1,    12,     4,    29,    54,    30,    27,     4,    40,
      85,    -1,     4,    59,    -1,    31,     4,    59,    -1,    33,
      85,    34,    59,    -1,    -1,     4,    29,    30,    -1,     4,
      29,    85,    61,    30,    -1,    28,    85,    61,    -1,    -1,
      13,    -1,     3,    -1,     5,    -1,    52,    33,    85,    34,
      10,    85,    -1,     4,    33,    85,    34,    10,    85,    -1,
      52,    25,     4,    40,    85,    67,    26,    -1,     4,    25,
       4,    40,    85,    67,    26,    -1,    52,    25,    26,    -1,
       4,    25,    26,    -1,    28,     4,    40,    85,    67,    -1,
      -1,    85,    45,    85,    -1,    85,    44,    85,    -1,    85,
      47,    85,    -1,    85,    46,    85,    -1,    29,    85,    30,
      -1,    29,    30,    -1,    29,    85,    32,    85,    72,    30,
      -1,    32,    85,    72,    -1,    -1,    22,     4,    35,    85,
      23,    85,    21,    85,    -1,    17,    85,    18,    85,    19,
      85,    -1,    17,    85,    18,    85,    -1,    85,    40,    85,
      -1,    85,    39,    85,    -1,    85,    38,    85,    -1,    85,
      42,    85,    -1,    85,    43,    85,    -1,    85,    41,    85,
      -1,    85,    37,    85,    -1,    85,    36,    85,    -1,    58,
      35,    85,    -1,    14,    49,    15,    80,    16,    -1,    85,
      81,    -1,    -1,    32,    85,    81,    -1,    -1,    20,    85,
      21,    85,    -1,    44,    85,    -1,    24,    -1,    58,    -1,
      60,    -1,    62,    -1,    63,    -1,    64,    -1,    65,    -1,
      66,    -1,    68,    -1,    69,    -1,    70,    -1,    71,    -1,
      73,    -1,    74,    -1,    75,    -1,    76,    -1,    77,    -1,
      78,    -1,    79,    -1,    82,    -1,    83,    -1,    84,    -1,
      85,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   148,   148,   149,   152,   153,   154,   157,   164,   171,
     176,   183,   184,   191,   196,   201,   206,   212,   219,   226,
     228,   235,   242,   245,   251,   258,   267,   274,   282,   297,
     304,   310,   316,   319,   324,   332,   334,   337,   340,   342,
     345,   351,   359,   369,   379,   384,   390,   396,   399,   400,
     401,   402,   405,   409,   413,   416,   417,   420,   429,   432,
     435,   437,   439,   441,   443,   445,   449,   451,   455,   459,
     461,   462,   464,   465,   468,   471,   475,   478,   480,   482,
     484,   486,   488,   490,   492,   494,   496,   498,   500,   502,
     504,   506,   508,   510,   512,   514,   516,   518,   523
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTEGER_LITERAL", "ID",
  "STRING_LITERAL", "INT", "STRING", "TYPE", "ARRAY", "OF", "VAR",
  "FUNCTION", "NIL", "LET", "IN", "END", "IF", "THEN", "ELSE", "WHILE",
  "DO", "FOR", "TO", "BREAK", "LEFT_CURLY_BRACE", "RIGHT_CURLY_BRACE",
  "COLON", "COMMA", "LEFT_PARENTHESIS", "RIGHT_PARENTHESIS", "DOT",
  "SEMICOLON", "LEFT_BRACKET", "RIGHT_BRACKET", "ASSIGNMENT_SIGN",
  "LOGICAL_OR", "LOGICAL_AND", "LESS_SIGN", "NOT_EQUAL_SIGN", "EQUAL_SIGN",
  "GREATER_EQUAL_SIGN", "LESS_EQUAL_SIGN", "GREATER_SIGN", "MINUS_SIGN",
  "ADD_SIGN", "DIVIDE_SIGN", "MULTIPLY_SIGN", "$accept", "decs", "dec",
  "tydec", "build_in_type", "ty", "tyfields", "tyfields_aux", "vardec",
  "fundec", "l_value", "l_value_aux", "function_call", "function_call_aux",
  "nil", "integer_literal", "string_literal", "array_creation",
  "record_creation", "record_creation_aux", "arithmetic", "parentheses",
  "no_value", "sequencing", "sequencing_aux", "for", "if_then_else",
  "if_then", "comparison", "boolean_operate", "assignment", "let",
  "expseq", "let_aux", "while", "negation", "break", "exp", "program", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    48,    49,    49,    50,    50,    50,    51,    51,    51,
      51,    52,    52,    53,    53,    53,    53,    54,    54,    54,
      55,    55,    55,    56,    56,    56,    57,    57,    57,    58,
      59,    59,    59,    60,    60,    61,    61,    62,    63,    64,
      65,    65,    66,    66,    66,    66,    67,    67,    68,    68,
      68,    68,    69,    70,    71,    72,    72,    73,    74,    75,
      76,    76,    76,    76,    76,    76,    77,    77,    78,    79,
      80,    80,    81,    81,    82,    83,    84,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    86
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     0,     1,     1,     1,     4,     4,     4,
       4,     1,     1,     1,     3,     3,     3,     4,     4,     0,
       5,     5,     0,     4,     6,     6,     7,     9,     9,     2,
       3,     4,     0,     3,     5,     3,     0,     1,     1,     1,
       6,     6,     7,     7,     3,     3,     5,     0,     3,     3,
       3,     3,     3,     2,     6,     3,     0,     8,     6,     4,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     5,
       2,     0,     3,     0,     4,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    38,    32,    39,    11,    12,    37,     3,     0,     0,
       0,    76,     0,     0,     0,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,     0,     0,     0,
       0,     0,    29,     0,     0,     0,     0,     3,     4,     5,
       6,     0,     0,     0,    53,     0,    75,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     1,     0,    45,    33,    36,    32,     0,     0,
       0,     0,     0,    71,     2,     0,     0,     0,    52,     0,
       0,    44,     0,    68,    67,    66,    62,    61,    60,    65,
      63,    64,    49,    48,    51,    50,     0,     0,     0,     0,
      30,    32,     0,     0,     0,     0,    19,     0,    73,    59,
      74,     0,    56,     0,     0,    47,    36,    34,     0,     0,
      31,     8,     0,    19,    13,    10,     7,     9,     0,     0,
      23,     0,     0,    69,     0,    70,     0,     0,     0,     0,
      47,     0,     0,     0,    35,    32,    41,     0,     0,     0,
       0,     0,     0,    73,    58,     0,    56,    54,     0,    40,
       0,    43,    16,    15,    14,    25,    24,    22,    22,     0,
       0,    72,     0,    55,    42,     0,     0,    18,    17,     0,
       0,    26,    57,    47,     0,     0,     0,    46,     0,    28,
      27,    22,    22,    21,    20
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    46,    47,    48,    14,   135,   142,   187,    49,    50,
      15,   130,    16,   108,    17,    18,    19,    20,    21,   153,
      22,    23,    24,    25,   149,    26,    27,    28,    29,    30,
      31,    32,   117,   145,    33,    34,    35,    36,    37
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -168
static const yytype_int16 yypact[] =
{
     176,  -168,    79,  -168,  -168,  -168,  -168,    30,   176,   176,
      35,  -168,    89,   176,   -17,   -23,  -168,  -168,  -168,  -168,
    -168,  -168,  -168,  -168,  -168,  -168,  -168,  -168,  -168,  -168,
    -168,  -168,  -168,  -168,  -168,  -168,   198,    17,    10,   141,
      60,   176,  -168,    84,    63,    70,    28,    30,  -168,  -168,
    -168,   185,   -18,    41,  -168,   282,  -168,    11,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,  -168,    49,  -168,  -168,   244,    39,   328,    57,
      65,    13,    51,   176,  -168,   176,   176,   176,  -168,   176,
      85,  -168,   342,   198,   366,   366,   376,   376,   376,   232,
     232,   232,   -40,   -40,  -168,  -168,   176,   176,    94,   176,
    -168,    -1,    40,    62,   110,   176,    96,   114,   298,   172,
     198,   224,   314,   176,   127,   264,   244,  -168,   356,   176,
    -168,  -168,   131,    96,  -168,  -168,  -168,  -168,   107,   115,
     198,   126,   132,  -168,   176,  -168,   176,   176,   176,   134,
     264,   176,   152,   140,  -168,    39,   198,   116,   143,   176,
     176,   122,    46,   298,   198,   212,   314,  -168,   147,   198,
     135,  -168,  -168,  -168,  -168,   198,   198,   139,   139,   128,
     176,  -168,   176,  -168,  -168,   176,   174,  -168,  -168,   136,
     144,   198,   198,   264,   159,   176,   176,  -168,   153,   198,
     198,   139,   139,  -168,  -168
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -168,   145,  -168,  -168,   -30,    81,    64,  -167,  -168,  -168,
    -168,     8,  -168,    69,  -168,  -168,  -168,  -168,  -168,  -148,
    -168,  -168,  -168,  -168,    33,  -168,  -168,  -168,  -168,  -168,
    -168,  -168,  -168,    38,  -168,  -168,  -168,    -8,  -168
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_int16 yytable[] =
{
      51,    52,   168,    86,    55,    56,    70,    71,    57,   129,
      42,   188,    59,    80,    73,    90,    58,    72,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      40,    76,   109,    78,   203,   204,    74,    91,    43,    53,
     114,    44,    45,    83,   131,   197,     4,     5,   115,   132,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,    77,   133,   136,    81,     4,     5,
      40,   132,   109,   179,    82,   118,    87,   119,   120,   121,
     116,   122,   134,   134,   139,   110,   180,   133,    79,   106,
       4,     5,     1,     2,     3,     4,     5,   112,   125,   126,
     141,   128,     6,     7,    38,   113,     8,   140,    39,     9,
      40,    10,    41,    11,   138,   150,     4,     5,    12,    54,
     172,   156,     4,     5,   127,   123,   177,   173,     4,     5,
     143,   178,   189,    13,     4,     5,   163,   151,   164,   165,
     166,   157,   159,   169,     1,     2,     3,     4,     5,   190,
     160,   175,   176,   161,     6,     7,   170,   201,     8,     4,
       5,     9,   162,    10,   167,    11,   171,   186,   202,   174,
      12,    75,   191,   184,   192,   185,   195,   193,   194,     1,
       2,     3,     4,     5,   196,    13,   198,   199,   200,     6,
       7,   146,    84,     8,   137,   154,     9,   158,    10,   183,
      11,   181,     0,    85,     0,    12,     0,     0,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      13,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,   182,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,     0,   147,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,   107,    -1,    -1,    -1,    68,    69,    70,    71,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,   152,     0,     0,     0,     0,     0,     0,     0,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    88,     0,    89,     0,     0,     0,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
     144,     0,     0,     0,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,   148,     0,     0,     0,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,   111,     0,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,   124,     0,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
     155,     0,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    -1,    -1,    -1,    65,    66,    67,
      68,    69,    70,    71
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-168)))

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-1)))

static const yytype_int16 yycheck[] =
{
       8,     9,   150,    21,    12,    13,    46,    47,    25,    10,
       2,   178,    35,    43,     4,     4,    33,     0,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      31,    39,    33,    41,   201,   202,    26,    26,     8,     4,
      27,    11,    12,    15,     4,   193,     6,     7,    35,     9,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,     4,    25,     4,     4,     6,     7,
      31,     9,    33,    27,     4,    83,    35,    85,    86,    87,
      29,    89,   112,   113,   114,    77,    40,    25,     4,    40,
       6,     7,     3,     4,     5,     6,     7,    40,   106,   107,
       4,   109,    13,    14,    25,    40,    17,   115,    29,    20,
      31,    22,    33,    24,     4,   123,     6,     7,    29,    30,
       4,   129,     6,     7,    30,    40,     4,   157,     6,     7,
      16,   161,     4,    44,     6,     7,   144,    10,   146,   147,
     148,    10,    35,   151,     3,     4,     5,     6,     7,   179,
      35,   159,   160,    27,    13,    14,     4,     4,    17,     6,
       7,    20,    30,    22,    30,    24,    26,    28,   198,    26,
      29,    30,   180,    26,   182,    40,    40,   185,     4,     3,
       4,     5,     6,     7,    40,    44,    27,   195,   196,    13,
      14,    19,    47,    17,   113,   126,    20,   133,    22,   166,
      24,   163,    -1,    18,    -1,    29,    -1,    -1,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      44,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    21,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    23,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    28,    41,    42,    43,    44,    45,    46,    47,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    30,    -1,    32,    -1,    -1,    -1,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      32,    -1,    -1,    -1,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    32,    -1,    -1,    -1,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    34,    -1,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    34,    -1,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      34,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,    13,    14,    17,    20,
      22,    24,    29,    44,    52,    58,    60,    62,    63,    64,
      65,    66,    68,    69,    70,    71,    73,    74,    75,    76,
      77,    78,    79,    82,    83,    84,    85,    86,    25,    29,
      31,    33,    59,     8,    11,    12,    49,    50,    51,    56,
      57,    85,    85,     4,    30,    85,    85,    25,    33,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,     0,     4,    26,    30,    85,     4,    85,     4,
      52,     4,     4,    15,    49,    18,    21,    35,    30,    32,
       4,    26,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    40,    28,    61,    33,
      59,    34,    40,    40,    27,    35,    29,    80,    85,    85,
      85,    85,    85,    40,    34,    85,    85,    30,    85,    10,
      59,     4,     9,    25,    52,    53,     4,    53,     4,    52,
      85,     4,    54,    16,    32,    81,    19,    23,    32,    72,
      85,    10,    28,    67,    61,    34,    85,    10,    54,    35,
      35,    27,    30,    85,    85,    85,    85,    30,    67,    85,
       4,    26,     4,    52,    26,    85,    85,     4,    52,    27,
      40,    81,    21,    72,    26,    40,    28,    55,    55,     4,
      52,    85,    85,    85,     4,    40,    40,    67,    27,    85,
      85,     4,    52,    55,    55
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
      yyerror (YY_("syntax error: cannot back up")); \
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
		  Type, Value); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
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
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
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
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

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
yyparse (void)
#else
int
yyparse ()

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
#line 148 "myTiger.y"
    { (yyval.myDecList_val) = makeMyDecList((yyvsp[(1) - (2)].myDec_val), (yyvsp[(2) - (2)].myDecList_val)); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 149 "myTiger.y"
    { (yyval.myDecList_val) = (myDecList)NULL; }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 152 "myTiger.y"
    { (yyval.myDec_val) = makeMyDec_Type(getCurrentPosInfo(), (yyvsp[(1) - (1)].myTypeDec_val)); }
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 153 "myTiger.y"
    { (yyval.myDec_val) = makeMyDec_Var(getCurrentPosInfo(), (yyvsp[(1) - (1)].myVarDec_val)); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 154 "myTiger.y"
    { (yyval.myDec_val) = makeMyDec_Func(getCurrentPosInfo(), (yyvsp[(1) - (1)].myFuncDec_val)); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 158 "myTiger.y"
    { (yyval.myTypeDec_val) = makeMyTyDec(
			MySymbol_MakeSymbol((yyvsp[(2) - (4)].myString_val)),
			makeMyTy_Named(
				getCurrentPosInfo(),
				MySymbol_MakeSymbol((yyvsp[(4) - (4)].myString_val))));
		}
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 165 "myTiger.y"
    { (yyval.myTypeDec_val) = makeMyTyDec(
			MySymbol_MakeSymbol((yyvsp[(2) - (4)].myString_val)),
			    makeMyTy_Named(
				getCurrentPosInfo(),
				MySymbol_MakeSymbol((yyvsp[(4) - (4)].myString_val))));
		}
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 172 "myTiger.y"
    { (yyval.myTypeDec_val) = makeMyTyDec(
			MySymbol_MakeSymbol((yyvsp[(2) - (4)].myString_val)),
			(yyvsp[(4) - (4)].myTy_val));
		}
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 177 "myTiger.y"
    { (yyval.myTypeDec_val) = makeMyTyDec(
			MySymbol_MakeSymbol((yyvsp[(2) - (4)].myString_val)),
			(yyvsp[(4) - (4)].myTy_val));
		}
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 183 "myTiger.y"
    { (yyval.myString_val) = "int"; }
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 184 "myTiger.y"
    { (yyval.myString_val) = "string"; }
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 192 "myTiger.y"
    { (yyval.myTy_val) = makeMyTy_Named(
			getCurrentPosInfo(),
		    	MySymbol_MakeSymbol((yyvsp[(1) - (1)].myString_val)));
		}
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 197 "myTiger.y"
    { (yyval.myTy_val) = makeMyTy_Record(
			getCurrentPosInfo(),
			(yyvsp[(2) - (3)].myTyFieldList_val));
		}
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 202 "myTiger.y"
    { (yyval.myTy_val) = makeMyTy_Array(
			getCurrentPosInfo(),
			MySymbol_MakeSymbol((yyvsp[(3) - (3)].myString_val)));
		}
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 207 "myTiger.y"
    { (yyval.myTy_val) = makeMyTy_Array(
			getCurrentPosInfo(),
			MySymbol_MakeSymbol((yyvsp[(3) - (3)].myString_val)));
		}
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 213 "myTiger.y"
    { (yyval.myTyFieldList_val) = makeMyTyFieldList(
				makeMyTyField(
					MySymbol_MakeSymbol((yyvsp[(1) - (4)].myString_val)),
					MySymbol_MakeSymbol((yyvsp[(3) - (4)].myString_val))),
				(yyvsp[(4) - (4)].myTyFieldList_val));
			}
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 220 "myTiger.y"
    { (yyval.myTyFieldList_val) = makeMyTyFieldList(
				makeMyTyField(
					MySymbol_MakeSymbol((yyvsp[(1) - (4)].myString_val)),
					MySymbol_MakeSymbol((yyvsp[(3) - (4)].myString_val))),
				(yyvsp[(4) - (4)].myTyFieldList_val));
			}
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 226 "myTiger.y"
    { (yyval.myTyFieldList_val) = (myTyFieldList)NULL; }
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 229 "myTiger.y"
    { (yyval.myTyFieldList_val) = makeMyTyFieldList(
				makeMyTyField(
					MySymbol_MakeSymbol((yyvsp[(2) - (5)].myString_val)),
					MySymbol_MakeSymbol((yyvsp[(4) - (5)].myString_val))),
				(yyvsp[(5) - (5)].myTyFieldList_val));
			}
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 236 "myTiger.y"
    { (yyval.myTyFieldList_val) = makeMyTyFieldList(
				makeMyTyField(
					MySymbol_MakeSymbol((yyvsp[(2) - (5)].myString_val)),
					MySymbol_MakeSymbol((yyvsp[(4) - (5)].myString_val))),
				(yyvsp[(5) - (5)].myTyFieldList_val));
			}
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 242 "myTiger.y"
    { (yyval.myTyFieldList_val) = (myTyFieldList)NULL; }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 246 "myTiger.y"
    { (yyval.myVarDec_val) = makeMyVarDec_ShortForm(
			makeMyShortFormVar(
				MySymbol_MakeSymbol((yyvsp[(2) - (4)].myString_val)),
				(yyvsp[(4) - (4)].myExp_val)));
		}
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 252 "myTiger.y"
    { (yyval.myVarDec_val) = makeMyVarDec_LongForm(
			makeMyLongFormVar(
				MySymbol_MakeSymbol((yyvsp[(2) - (6)].myString_val)),
				MySymbol_MakeSymbol((yyvsp[(4) - (6)].myString_val)),
				(yyvsp[(6) - (6)].myExp_val)));
		}
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 259 "myTiger.y"
    { (yyval.myVarDec_val) = makeMyVarDec_LongForm(
			makeMyLongFormVar(
				MySymbol_MakeSymbol((yyvsp[(2) - (6)].myString_val)),
				MySymbol_MakeSymbol((yyvsp[(4) - (6)].myString_val)),
				(yyvsp[(6) - (6)].myExp_val)));
		}
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 268 "myTiger.y"
    { (yyval.myFuncDec_val) = makeMyFuncDec_Procedure(
			makeMyProcedureDec(
				MySymbol_MakeSymbol((yyvsp[(2) - (7)].myString_val)),
				(yyvsp[(4) - (7)].myTyFieldList_val),
				(yyvsp[(7) - (7)].myExp_val)));
		}
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 275 "myTiger.y"
    { (yyval.myFuncDec_val) = makeMyFuncDec_Function(
			makeMyFunctionDec(
				MySymbol_MakeSymbol((yyvsp[(2) - (9)].myString_val)),
				(yyvsp[(4) - (9)].myTyFieldList_val),
				MySymbol_MakeSymbol((yyvsp[(7) - (9)].myString_val)),
				(yyvsp[(9) - (9)].myExp_val)));
		}
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 283 "myTiger.y"
    { (yyval.myFuncDec_val) = makeMyFuncDec_Function(
			makeMyFunctionDec(
				MySymbol_MakeSymbol((yyvsp[(2) - (9)].myString_val)),
				(yyvsp[(4) - (9)].myTyFieldList_val),
				MySymbol_MakeSymbol((yyvsp[(7) - (9)].myString_val)),
				(yyvsp[(9) - (9)].myExp_val)));
		}
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 298 "myTiger.y"
    { (yyval.myLValue_val) = makeMyLValue(
				getCurrentPosInfo(),
				MySymbol_MakeSymbol((yyvsp[(1) - (2)].myString_val)),
				(yyvsp[(2) - (2)].myLValueAux_val));
			}
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 305 "myTiger.y"
    { (yyval.myLValueAux_val) = makeMyLValueAux(
				MySymbol_MakeSymbol((yyvsp[(2) - (3)].myString_val)),
				(myExp)NULL,
				(yyvsp[(3) - (3)].myLValueAux_val));
			}
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 311 "myTiger.y"
    { (yyval.myLValueAux_val) = makeMyLValueAux(
				(mySymbol)NULL,
				(yyvsp[(2) - (4)].myExp_val),
				(yyvsp[(4) - (4)].myLValueAux_val));
			}
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 316 "myTiger.y"
    { (yyval.myLValueAux_val) = (myLValueAux)NULL; }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 320 "myTiger.y"
    { (yyval.myFunctionCallExp_val) = makeMyFunctionCallExp_NoParam(
				makeMyNoParamFunctionCallExp(
					MySymbol_MakeSymbol((yyvsp[(1) - (3)].myString_val))));
			}
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 325 "myTiger.y"
    { (yyval.myFunctionCallExp_val) = makeMyFunctionCallExp_Param(
				makeMyParamFunctionCallExp(
					MySymbol_MakeSymbol((yyvsp[(1) - (5)].myString_val)),
					makeMyExpList(
						(yyvsp[(3) - (5)].myExp_val), (yyvsp[(4) - (5)].myExpList_val))));
			}
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 333 "myTiger.y"
    { (yyval.myExpList_val) = makeMyExpList((yyvsp[(2) - (3)].myExp_val), (yyvsp[(3) - (3)].myExpList_val)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 334 "myTiger.y"
    { (yyval.myExpList_val) = (myExpList)NULL; }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 337 "myTiger.y"
    { (yyval.myNilExp_val) = makeMyNilExp(); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 340 "myTiger.y"
    { (yyval.myIntegerLiteralExp_val) = makeMyIntegerLiteralExp((yyvsp[(1) - (1)].int_val)); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 342 "myTiger.y"
    { (yyval.myStringLiteralExp_val) = makeMyStringLiteralExp((yyvsp[(1) - (1)].myString_val)); }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 346 "myTiger.y"
    { (yyval.myArrayCreationExp_val) = makeMyArrayCreationExp(
				MySymbol_MakeSymbol((yyvsp[(1) - (6)].myString_val)),
				(yyvsp[(3) - (6)].myExp_val),
				(yyvsp[(6) - (6)].myExp_val));
			}
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 352 "myTiger.y"
    { (yyval.myArrayCreationExp_val) = makeMyArrayCreationExp(
				MySymbol_MakeSymbol((yyvsp[(1) - (6)].myString_val)),
				(yyvsp[(3) - (6)].myExp_val),
				(yyvsp[(6) - (6)].myExp_val));
			}
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 360 "myTiger.y"
    { (yyval.myRecordCreationExp_val) = makeMyRecordCreationExp_Field(
				makeMyFieldRecordCreationExp(
					MySymbol_MakeSymbol((yyvsp[(1) - (7)].myString_val)),
					makeMyRecordFieldList(
						makeMyRecordField(
							MySymbol_MakeSymbol((yyvsp[(3) - (7)].myString_val)),
							(yyvsp[(5) - (7)].myExp_val)),
							(yyvsp[(6) - (7)].myRecordFieldList_val))));
			}
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 370 "myTiger.y"
    { (yyval.myRecordCreationExp_val) = makeMyRecordCreationExp_Field(
				makeMyFieldRecordCreationExp(
					MySymbol_MakeSymbol((yyvsp[(1) - (7)].myString_val)),
					makeMyRecordFieldList(
						makeMyRecordField(
							MySymbol_MakeSymbol((yyvsp[(3) - (7)].myString_val)),
							(yyvsp[(5) - (7)].myExp_val)),
							(yyvsp[(6) - (7)].myRecordFieldList_val))));
			}
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 380 "myTiger.y"
    { (yyval.myRecordCreationExp_val) = makeMyRecordCreationExp_NoField(
				makeMyNoFieldRecordCreationExp(
					MySymbol_MakeSymbol((yyvsp[(1) - (3)].myString_val))));
			}
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 385 "myTiger.y"
    { (yyval.myRecordCreationExp_val) = makeMyRecordCreationExp_NoField(
				makeMyNoFieldRecordCreationExp(
					MySymbol_MakeSymbol((yyvsp[(1) - (3)].myString_val))));
			}
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 391 "myTiger.y"
    { (yyval.myRecordFieldList_val) = makeMyRecordFieldList(
					makeMyRecordField(
						MySymbol_MakeSymbol((yyvsp[(2) - (5)].myString_val)),
						(yyvsp[(4) - (5)].myExp_val)),
					(yyvsp[(5) - (5)].myRecordFieldList_val)); }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 396 "myTiger.y"
    { (yyval.myRecordFieldList_val) = (myRecordFieldList)NULL; }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 399 "myTiger.y"
    { (yyval.myArithmeticExp_val) = makeMyArithmeticExp_Plus((yyvsp[(1) - (3)].myExp_val), (yyvsp[(3) - (3)].myExp_val)); }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 400 "myTiger.y"
    { (yyval.myArithmeticExp_val) = makeMyArithmeticExp_Minus((yyvsp[(1) - (3)].myExp_val), (yyvsp[(3) - (3)].myExp_val)); }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 401 "myTiger.y"
    { (yyval.myArithmeticExp_val) = makeMyArithmeticExp_Multiply((yyvsp[(1) - (3)].myExp_val), (yyvsp[(3) - (3)].myExp_val)); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 402 "myTiger.y"
    { (yyval.myArithmeticExp_val) = makeMyArithmeticExp_Divide((yyvsp[(1) - (3)].myExp_val), (yyvsp[(3) - (3)].myExp_val)); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 406 "myTiger.y"
    { (yyval.myParenthesesExp_val) = makeMyParenthesesExp((yyvsp[(2) - (3)].myExp_val)); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 410 "myTiger.y"
    { (yyval.myNoValueExp_val) = makeMyNoValueExp(); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 414 "myTiger.y"
    { (yyval.mySequencingExp_val) = makeMySequencingExp((yyvsp[(2) - (6)].myExp_val), (yyvsp[(4) - (6)].myExp_val), (yyvsp[(5) - (6)].myExpList_val)); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 416 "myTiger.y"
    { (yyval.myExpList_val) = makeMyExpList((yyvsp[(2) - (3)].myExp_val), (yyvsp[(3) - (3)].myExpList_val)); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 417 "myTiger.y"
    { (yyval.myExpList_val) = (myExpList)NULL; }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 421 "myTiger.y"
    { (yyval.myForExp_val) = makeMyForExp(
				MySymbol_MakeSymbol((yyvsp[(2) - (8)].myString_val)),
				(yyvsp[(4) - (8)].myExp_val),
				(yyvsp[(6) - (8)].myExp_val),
				(yyvsp[(8) - (8)].myExp_val));
			}
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 429 "myTiger.y"
    { (yyval.myIfThenElseExp_val) = makeMyIfThenElseExp((yyvsp[(2) - (6)].myExp_val), (yyvsp[(4) - (6)].myExp_val), (yyvsp[(6) - (6)].myExp_val)); }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 432 "myTiger.y"
    { (yyval.myIfThenExp_val) = makeMyIfThenExp((yyvsp[(2) - (4)].myExp_val), (yyvsp[(4) - (4)].myExp_val)); }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 436 "myTiger.y"
    { (yyval.myComparisonExp_val) = makeMyComparisonExp_Equal((yyvsp[(1) - (3)].myExp_val), (yyvsp[(3) - (3)].myExp_val)); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 438 "myTiger.y"
    { (yyval.myComparisonExp_val) = makeMyComparisonExp_NotEqual((yyvsp[(1) - (3)].myExp_val), (yyvsp[(3) - (3)].myExp_val)); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 440 "myTiger.y"
    { (yyval.myComparisonExp_val) = makeMyComparisonExp_SmallerThan((yyvsp[(1) - (3)].myExp_val), (yyvsp[(3) - (3)].myExp_val)); }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 442 "myTiger.y"
    { (yyval.myComparisonExp_val) = makeMyComparisonExp_SmallerEqualThan((yyvsp[(1) - (3)].myExp_val), (yyvsp[(3) - (3)].myExp_val)); }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 444 "myTiger.y"
    { (yyval.myComparisonExp_val) = makeMyComparisonExp_LargerThan((yyvsp[(1) - (3)].myExp_val), (yyvsp[(3) - (3)].myExp_val)); }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 446 "myTiger.y"
    { (yyval.myComparisonExp_val) = makeMyComparisonExp_LargerEqualThan((yyvsp[(1) - (3)].myExp_val), (yyvsp[(3) - (3)].myExp_val)); }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 450 "myTiger.y"
    { (yyval.myBooleanOperateExp_val) = makeMyBooleanOperateExp_And((yyvsp[(1) - (3)].myExp_val), (yyvsp[(3) - (3)].myExp_val)); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 452 "myTiger.y"
    { (yyval.myBooleanOperateExp_val) = makeMyBooleanOperateExp_Or((yyvsp[(1) - (3)].myExp_val), (yyvsp[(3) - (3)].myExp_val)); }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 456 "myTiger.y"
    { (yyval.myAssignmentExp_val) = makeMyAssignmentExp((yyvsp[(1) - (3)].myLValue_val), (yyvsp[(3) - (3)].myExp_val)); }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 459 "myTiger.y"
    { (yyval.myLetExp_val) = makeMyLetExp((yyvsp[(2) - (5)].myDecList_val), (yyvsp[(4) - (5)].myExpList_val)); }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 461 "myTiger.y"
    { (yyval.myExpList_val) = makeMyExpList((yyvsp[(1) - (2)].myExp_val), (yyvsp[(2) - (2)].myExpList_val)); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 462 "myTiger.y"
    { (yyval.myExpList_val) = (myExpList)NULL; }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 464 "myTiger.y"
    { (yyval.myExpList_val) = makeMyExpList((yyvsp[(2) - (3)].myExp_val), (yyvsp[(3) - (3)].myExpList_val)); }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 465 "myTiger.y"
    { (yyval.myExpList_val) = (myExpList)NULL; }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 468 "myTiger.y"
    { (yyval.myWhileExp_val) = makeMyWhileExp((yyvsp[(2) - (4)].myExp_val), (yyvsp[(4) - (4)].myExp_val)); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 472 "myTiger.y"
    { (yyval.myNegationExp_val) = makeMyNegationExp((yyvsp[(2) - (2)].myExp_val)); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 475 "myTiger.y"
    { (yyval.myBreakExp_val) = makeMyBreakExp(); }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 479 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_LValue(getCurrentPosInfo(), (yyvsp[(1) - (1)].myLValue_val)); }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 481 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_FunctionCall(getCurrentPosInfo(), (yyvsp[(1) - (1)].myFunctionCallExp_val)); }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 483 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_Nil(getCurrentPosInfo(), (yyvsp[(1) - (1)].myNilExp_val)); }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 485 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_IntegerLiteral(getCurrentPosInfo(), (yyvsp[(1) - (1)].myIntegerLiteralExp_val)); }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 487 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_StringLiteral(getCurrentPosInfo(), (yyvsp[(1) - (1)].myStringLiteralExp_val)); }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 489 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_ArrayCreation(getCurrentPosInfo(), (yyvsp[(1) - (1)].myArrayCreationExp_val)); }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 491 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_RecordCreation(getCurrentPosInfo(), (yyvsp[(1) - (1)].myRecordCreationExp_val)); }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 493 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_Arithmetic(getCurrentPosInfo(), (yyvsp[(1) - (1)].myArithmeticExp_val)); }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 495 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_Parentheses(getCurrentPosInfo(), (yyvsp[(1) - (1)].myParenthesesExp_val)); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 497 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_NoValue(getCurrentPosInfo(), (yyvsp[(1) - (1)].myNoValueExp_val)); }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 499 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_Sequencing(getCurrentPosInfo(), (yyvsp[(1) - (1)].mySequencingExp_val)); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 501 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_For(getCurrentPosInfo(), (yyvsp[(1) - (1)].myForExp_val)); }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 503 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_IfThenElse(getCurrentPosInfo(), (yyvsp[(1) - (1)].myIfThenElseExp_val)); }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 505 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_IfThen(getCurrentPosInfo(), (yyvsp[(1) - (1)].myIfThenExp_val)); }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 507 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_Comparison(getCurrentPosInfo(), (yyvsp[(1) - (1)].myComparisonExp_val)); }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 509 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_BooleanOperate(getCurrentPosInfo(), (yyvsp[(1) - (1)].myBooleanOperateExp_val)); }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 511 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_Assignment(getCurrentPosInfo(), (yyvsp[(1) - (1)].myAssignmentExp_val)); }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 513 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_Let(getCurrentPosInfo(), (yyvsp[(1) - (1)].myLetExp_val)); }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 515 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_While(getCurrentPosInfo(), (yyvsp[(1) - (1)].myWhileExp_val)); }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 517 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_Negation(getCurrentPosInfo(), (yyvsp[(1) - (1)].myNegationExp_val)); }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 519 "myTiger.y"
    { (yyval.myExp_val) = makeMyExp_Break(getCurrentPosInfo(), (yyvsp[(1) - (1)].myBreakExp_val)); }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 523 "myTiger.y"
    { g_AbstractSyntaxTree = (yyvsp[(1) - (1)].myExp_val); }
    break;


/* Line 1792 of yacc.c  */
#line 2430 "y.tab.c"
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

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
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
#line 526 "myTiger.y"


void yyerror()			{	errorReport("Syntax analysis error!");	}


myExp parseOneFile(char* parseFile)
{
	resetPos(g_posInfo);

    if ((yyin = fopen(parseFile, "r")) == NULL)
    {
		printf("can not open file: %s\n", parseFile);
        return 0;
    }
	    
    if (yyparse() != 0)//	0 stands for parse error
	{
		fclose(yyin);
		return ABSTRACT_SYNTAX_ERROR;
	}
    
    fclose(yyin);
    return g_AbstractSyntaxTree;
}
