#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20130304

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)

#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "myTiger.y"

  #include "myAbstractSyntax.h"
  #include "myPrintAbstractSyntax.h"
  #include "y.tab.h"
  #include <stdio.h>
  #include <stdlib.h>

  extern FILE* yyin;
  extern char* yytext;
  extern PosInfo g_posInfo;

  myExp		g_AbstractSyntaxTree;
  
  extern int yylex();/*	prototype for lex scanning*/

#line 19 "myTiger.y"
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union	{
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
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 79 "y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define INTEGER_LITERAL 257
#define ID 258
#define STRING_LITERAL 259
#define INT 260
#define STRING 261
#define TYPE 262
#define ARRAY 263
#define OF 264
#define VAR 265
#define FUNCTION 266
#define NIL 267
#define LET 268
#define IN 269
#define END 270
#define IF 271
#define THEN 272
#define ELSE 273
#define WHILE 274
#define DO 275
#define FOR 276
#define TO 277
#define BREAK 278
#define LEFT_CURLY_BRACE 279
#define RIGHT_CURLY_BRACE 280
#define COLON 281
#define COMMA 282
#define LEFT_PARENTHESIS 283
#define RIGHT_PARENTHESIS 284
#define DOT 285
#define SEMICOLON 286
#define LEFT_BRACKET 287
#define RIGHT_BRACKET 288
#define ASSIGNMENT_SIGN 289
#define LOGICAL_AND 290
#define LOGICAL_OR 291
#define EQUAL_SIGN 292
#define NOT_EQUAL_SIGN 293
#define LESS_SIGN 294
#define GREATER_SIGN 295
#define LESS_EQUAL_SIGN 296
#define GREATER_EQUAL_SIGN 297
#define ADD_SIGN 298
#define MINUS_SIGN 299
#define MULTIPLY_SIGN 300
#define DIVIDE_SIGN 301
#define YYERRCODE 256
static const short yylhs[] = {                           -1,
    2,    2,    3,    3,    3,    4,    4,    4,    4,    7,
    7,    8,    8,    8,    8,    9,    9,    9,   10,   10,
   10,    5,    5,    5,    6,    6,    6,   11,   12,   12,
   12,   13,   13,   14,   14,   18,   19,   20,   21,   21,
   22,   22,   22,   22,   23,   23,   24,   24,   24,   24,
   25,   26,   27,   15,   15,   28,   29,   30,   31,   31,
   31,   31,   31,   31,   32,   32,   33,   34,   16,   16,
   17,   17,   35,   36,   37,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    0,
};
static const short yylen[] = {                            2,
    2,    0,    1,    1,    1,    4,    4,    4,    4,    1,
    1,    1,    3,    3,    3,    4,    4,    0,    5,    5,
    0,    4,    6,    6,    7,    9,    9,    2,    3,    4,
    0,    3,    5,    3,    0,    1,    1,    1,    6,    6,
    7,    7,    3,    3,    5,    0,    3,    3,    3,    3,
    3,    2,    6,    3,    0,    8,    6,    4,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    5,    2,    0,
    3,    0,    4,    2,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,
};
static const short yydefred[] = {                         0,
   37,    0,   38,   10,   11,   36,    0,    0,    0,    0,
   75,    0,    0,    0,    0,    0,    0,   77,   78,   79,
   80,   81,   82,   83,   84,   85,   86,   87,   88,   89,
   90,   91,   92,   93,   94,   95,   96,    0,    0,    0,
    0,   28,    0,    0,    0,    0,    0,    3,    4,    5,
    0,    0,    0,   52,    0,   74,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   44,   32,    0,    0,    0,    0,    0,    0,
    0,    0,    1,    0,    0,    0,   51,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   49,   50,
    0,   43,    0,    0,    0,    0,    0,    0,   29,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   33,    0,    0,   30,    7,
    0,    0,   12,    9,    6,    8,    0,    0,    0,    0,
    0,    0,   69,   68,    0,    0,    0,    0,    0,    0,
    0,    0,   34,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   53,    0,    0,    0,   42,
   15,   14,   13,    0,    0,    0,    0,    0,    0,   71,
    0,   54,   41,    0,    0,   17,   16,    0,    0,    0,
    0,    0,    0,    0,    0,   45,    0,    0,    0,    0,
    0,   20,   19,
};
static const short yydgoto[] = {                         14,
   15,   46,   47,   48,   49,   50,   16,  134,  141,  186,
   17,  129,   18,  107,  148,  117,  143,   19,   20,   21,
   22,   23,  152,   24,   25,   26,   27,   28,   29,   30,
   31,   32,   33,   34,   35,   36,   37,
};
static const short yysindex[] = {                       -75,
    0, -239,    0,    0,    0,    0, -134,  -75,  -75, -208,
    0, -173,  -75,    0,  623, -269, -223,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -242, -112, -191,
  -75,    0, -246, -185, -162, -172, -134,    0,    0,    0,
  -81, -270, -171,    0,  693,    0,  -75,  -75,  -75,  -75,
  -75,  -75,  -75,  -75,  -75,  -75,  -75,  -75, -241,  -75,
  -75, -163,    0,    0,  655, -106,  723, -150, -149, -218,
 -133,  -75,    0,  -75,  -75,  -75,    0,  -75,  761,  761,
  341,  341,  341, -209, -209, -209, -258, -258,    0,    0,
 -141,    0,  737,  623,  -75,  -75, -121,  -75,    0, -253,
 -186, -154, -146,  -75, -137,  -64, -101,  -52,  623,  635,
  709,  -75, -126,  675,  655,    0,  751,  -75,    0,    0,
  -94, -137,    0,    0,    0,    0, -109,  -95,  623,  -79,
  -80,  -75,    0,    0,  -75,  -75,  -75,  -78,  675,  -75,
  -58,  -73,    0, -106,  623, -100,  -55,  -75,  -75,  -93,
 -273,  -64,  623,  610,  709,    0,  -29,  623,  -87,    0,
    0,    0,    0,  623,  623,  -30,  -30,  -84,  -75,    0,
  -75,    0,    0,  -75,    2,    0,    0,  -23,  -20,  623,
  623,  675,   -4,  -75,  -75,    0,  -63,  623,  623,  -30,
  -30,    0,    0,
};
static const short yyrindex[] = {                         0,
    0,    1,    0,    0,    0,    0,    6,    0,    0,    0,
    0,    0,    0,    0,  279,    0,   41,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    6,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   -2,    1,    0,    0,    0,    0,
    0,   10,    0,    0,    0,    0,    0,    0,  256,  286,
  317,  344,  382,  157,  190,  223,   81,  119,    0,    0,
    0,    0,    0,  420,    0,    0,    0,    0,    0,    1,
    0,    0,    0,    0,    4,   14,    0,  611,  453,    0,
   20,    0,    0,   35,   -2,    0,    0,    0,    0,    0,
    0,   39,    0,    0,    0,    0,    0,    0, -142,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   35,    0,
    0,    0,    0,    1,  486,    0,    0,    0,    0,    0,
    0,   14,  519,    0,   20,    0,    0,  549,    0,    0,
    0,    0,    0, -132,  -12, -235, -235,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   -7,
  578,   35,    0,    0,    0,    0,    0,   -1,   43, -235,
 -235,    0,    0,
};
static const short yygindex[] = {                         0,
   -6,  273,    0,    0,    0,    0,  -43,  210,  192, -164,
    0,    7,    0,  201,  163,    0,  168,    0,    0,    0,
    0,    0, -145,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,
};
#define YYTABLESIZE 1062
static const short yytable[] = {                         79,
   31,   51,   52,  167,   85,   55,   56,  178,   42,   69,
  128,   78,  187,    4,    5,   72,  101,   70,  179,   57,
   58,   59,   60,   61,   62,   63,   64,   65,   66,   67,
   68,   40,   75,  108,   77,  202,  203,   73,  102,   38,
   76,   67,   68,   39,   21,   40,  196,   41,   21,   53,
   89,   90,   91,   92,   93,   94,   95,   96,   97,   98,
   99,  100,  113,  103,  104,   71,   76,  133,  133,  138,
  114,  130,   80,    4,    5,  116,  131,  118,  119,  120,
   47,  121,  109,    1,    2,    3,    4,    5,   65,   66,
   67,   68,  132,    6,    7,   81,   82,    8,  124,  125,
    9,  127,   10,  135,   11,    4,    5,  139,  131,   12,
   54,  137,  172,    4,    5,  149,  177,   86,   48,   22,
  140,  155,   22,   22,  132,   13,   22,   43,  105,   24,
   44,   45,   24,   24,  189,  162,   24,  150,  163,  164,
  165,  111,  112,  168,    1,    2,    3,    4,    5,  115,
  122,  174,  175,  201,    6,    7,   63,  171,    8,    4,
    5,    9,  126,   10,  176,   11,    4,    5,  144,  156,
   12,   74,  190,  188,  191,    4,    5,  192,   40,  158,
  108,    1,    2,    3,    4,    5,   13,  198,  199,   62,
   84,    6,    7,  159,  200,    8,    4,    5,    9,  169,
   10,  160,   11,  161,  184,  166,  170,   12,   57,   58,
   59,   60,   61,   62,   63,   64,   65,   66,   67,   68,
  145,  142,   64,   13,  173,   57,   58,   59,   60,   61,
   62,   63,   64,   65,   66,   67,   68,   57,   58,   59,
   60,   61,   62,   63,   64,   65,   66,   67,   68,   23,
  183,  185,   23,   23,   25,   65,   23,   25,   25,  193,
   27,   25,   31,   27,   27,   31,   31,   27,  194,   31,
   31,  195,   31,   31,    2,   31,  197,   31,   97,   70,
   31,   35,   31,   72,   31,   66,   31,   18,   31,   31,
   31,   31,   31,   31,   31,   31,   31,   31,   31,   31,
   31,   31,   76,   55,   26,   76,   76,   26,   26,   76,
   76,   26,   76,   76,   46,   76,   59,   76,   18,   83,
   76,  136,   76,  157,   76,  153,   76,  182,   76,  180,
   76,   76,   76,   76,   76,   76,   76,   76,   76,   76,
   76,   76,   47,   60,    0,   47,   47,    0,    0,   47,
   47,    0,   47,   47,    0,   47,    0,   47,    0,    0,
   47,    0,   47,    0,   47,    0,   47,    0,   47,    0,
   47,   47,   47,   47,   47,   47,   47,   47,   47,   47,
   48,   61,    0,   48,   48,    0,    0,   48,   48,    0,
   48,   48,    0,   48,    0,   48,    0,    0,   48,    0,
   48,    0,   48,    0,   48,    0,   48,    0,   48,   48,
   48,   48,   48,   48,   48,   48,   48,   48,   63,   67,
    0,   63,   63,    0,    0,   63,   63,    0,   63,   63,
    0,   63,    0,   63,    0,    0,   63,    0,   63,    0,
   63,    0,   63,    0,   63,    0,   63,   63,   63,   63,
   63,   62,   73,    0,   62,   62,    0,    0,   62,   62,
    0,   62,   62,    0,   62,    0,   62,    0,    0,   62,
    0,   62,    0,   62,    0,   62,    0,   62,    0,   62,
   62,   62,   62,   62,   64,   40,    0,   64,   64,    0,
    0,   64,   64,    0,   64,   64,    0,   64,    0,   64,
    0,    0,   64,    0,   64,    0,   64,    0,   64,    0,
   64,    0,   64,   64,   64,   64,   64,   65,   57,    0,
   65,   65,    0,    0,   65,   65,    0,   65,   65,    0,
   65,    0,   65,    0,    0,   65,    0,   65,    0,   65,
    0,   65,    0,   65,    0,   65,   65,   66,   39,    0,
   66,   66,    0,    0,   66,   66,    0,   66,   66,    0,
   66,    0,   66,    0,    0,   66,    0,   66,    0,   66,
    0,   66,    0,   66,    0,   66,   66,   56,   59,    0,
    0,   59,   59,    0,    0,   59,   59,    0,   59,   59,
    0,   59,    0,   59,    0,    0,   59,    0,   59,    0,
   59,    0,   59,    0,   59,   60,   59,   59,   60,   60,
   58,    0,   60,   60,    0,   60,   60,    0,   60,    0,
   60,    0,    0,   60,    0,   60,    0,   60,    0,   60,
    0,   60,    0,   60,   60,   62,   63,   64,   65,   66,
   67,   68,    0,   61,    0,    0,   61,   61,    0,    0,
   61,   61,    0,   61,   61,    0,   61,    0,   61,    0,
    0,   61,    0,   61,    0,   61,    0,   61,    0,   61,
    0,   61,   61,    0,    0,    0,    0,    0,    0,    0,
    0,   67,    0,    0,   67,   67,    0,    0,   67,   67,
    0,   67,   67,    0,   67,    0,   67,    0,    0,   67,
    0,   67,    0,   67,    0,   67,    0,   67,    0,    0,
    0,    0,    0,    0,   73,    0,    0,   73,   73,    0,
    0,   73,   73,    0,   73,   73,    0,   73,    0,   73,
    0,    0,   73,    0,   73,    0,   73,    0,   73,    0,
   73,    0,    0,    0,    0,    0,    0,   40,    0,    0,
   40,   40,    0,    0,   40,   40,    0,   40,   40,    0,
   40,    0,   40,    0,    0,   40,    0,   40,    0,   40,
    0,   40,    0,   40,    0,    0,    0,    0,    0,    0,
   57,    0,    0,   57,   57,    0,    0,   57,   57,    0,
   57,   57,    0,   57,    0,   57,    0,    0,   57,    0,
   57,    0,   57,    0,   57,    0,   57,    0,    0,    0,
   39,    0,    0,   39,   39,    0,    0,   39,   39,    0,
   39,   39,    0,   39,    0,   39,    0,    0,   39,    0,
   39,    0,   39,    0,   39,    0,   39,    0,    0,   56,
    0,    0,   56,   56,    0,    0,   56,   56,    0,   56,
   56,    0,   56,    0,   56,    0,    0,   56,    0,   56,
    0,   56,    0,   56,    0,   56,    0,    0,    0,    0,
    0,    0,   58,    0,    0,   58,   58,    0,    0,   58,
   58,    0,   58,    0,  181,   58,    0,   58,    0,    0,
   58,    0,   58,    0,   58,    0,   58,    0,   58,   57,
   58,   59,   60,   61,   62,   63,   64,   65,   66,   67,
   68,  146,   57,   58,   59,   60,   61,   62,   63,   64,
   65,   66,   67,   68,   57,   58,   59,   60,   61,   62,
   63,   64,   65,   66,   67,   68,  106,    0,    0,    0,
    0,    0,    0,    0,   57,   58,   59,   60,   61,   62,
   63,   64,   65,   66,   67,   68,  151,    0,    0,    0,
    0,    0,    0,    0,   57,   58,   59,   60,   61,   62,
   63,   64,   65,   66,   67,   68,   87,    0,   88,    0,
    0,    0,   57,   58,   59,   60,   61,   62,   63,   64,
   65,   66,   67,   68,  147,    0,    0,    0,   57,   58,
   59,   60,   61,   62,   63,   64,   65,   66,   67,   68,
  110,    0,   57,   58,   59,   60,   61,   62,   63,   64,
   65,   66,   67,   68,  123,    0,   57,   58,   59,   60,
   61,   62,   63,   64,   65,   66,   67,   68,  154,    0,
   57,   58,   59,   60,   61,   62,   63,   64,   65,   66,
   67,   68,   59,   60,   61,   62,   63,   64,   65,   66,
   67,   68,
};
static const short yycheck[] = {                         43,
    0,    8,    9,  149,  275,   12,   13,  281,    2,  279,
  264,  258,  177,  260,  261,  258,  258,  287,  292,  290,
  291,  292,  293,  294,  295,  296,  297,  298,  299,  300,
  301,  285,   39,  287,   41,  200,  201,  280,  280,  279,
    0,  300,  301,  283,  280,  285,  192,  287,  284,  258,
   57,   58,   59,   60,   61,   62,   63,   64,   65,   66,
   67,   68,  281,   70,   71,  289,  258,  111,  112,  113,
  289,  258,  258,  260,  261,   82,  263,   84,   85,   86,
    0,   88,   76,  257,  258,  259,  260,  261,  298,  299,
  300,  301,  279,  267,  268,  258,  269,  271,  105,  106,
  274,  108,  276,  258,  278,  260,  261,  114,  263,  283,
  284,  258,  156,  260,  261,  122,  160,  289,    0,  262,
  258,  128,  265,  266,  279,  299,  269,  262,  292,  262,
  265,  266,  265,  266,  178,  142,  269,  264,  145,  146,
  147,  292,  292,  150,  257,  258,  259,  260,  261,  283,
  292,  158,  159,  197,  267,  268,    0,  258,  271,  260,
  261,  274,  284,  276,  258,  278,  260,  261,  270,  264,
  283,  284,  179,  258,  181,  260,  261,  184,  285,  289,
  287,  257,  258,  259,  260,  261,  299,  194,  195,    0,
  272,  267,  268,  289,  258,  271,  260,  261,  274,  258,
  276,  281,  278,  284,  292,  284,  280,  283,  290,  291,
  292,  293,  294,  295,  296,  297,  298,  299,  300,  301,
  273,  286,    0,  299,  280,  290,  291,  292,  293,  294,
  295,  296,  297,  298,  299,  300,  301,  290,  291,  292,
  293,  294,  295,  296,  297,  298,  299,  300,  301,  262,
  280,  282,  265,  266,  262,    0,  269,  265,  266,  258,
  262,  269,  262,  265,  266,  265,  266,  269,  292,  269,
  270,  292,  272,  273,  269,  275,  281,  277,    0,  270,
  280,  284,  282,  270,  284,    0,  286,  284,  288,  289,
  290,  291,  292,  293,  294,  295,  296,  297,  298,  299,
  300,  301,  262,  284,  262,  265,  266,  265,  266,  269,
  270,  269,  272,  273,  280,  275,    0,  277,  280,   47,
  280,  112,  282,  132,  284,  125,  286,  165,  288,  162,
  290,  291,  292,  293,  294,  295,  296,  297,  298,  299,
  300,  301,  262,    0,   -1,  265,  266,   -1,   -1,  269,
  270,   -1,  272,  273,   -1,  275,   -1,  277,   -1,   -1,
  280,   -1,  282,   -1,  284,   -1,  286,   -1,  288,   -1,
  290,  291,  292,  293,  294,  295,  296,  297,  298,  299,
  262,    0,   -1,  265,  266,   -1,   -1,  269,  270,   -1,
  272,  273,   -1,  275,   -1,  277,   -1,   -1,  280,   -1,
  282,   -1,  284,   -1,  286,   -1,  288,   -1,  290,  291,
  292,  293,  294,  295,  296,  297,  298,  299,  262,    0,
   -1,  265,  266,   -1,   -1,  269,  270,   -1,  272,  273,
   -1,  275,   -1,  277,   -1,   -1,  280,   -1,  282,   -1,
  284,   -1,  286,   -1,  288,   -1,  290,  291,  292,  293,
  294,  262,    0,   -1,  265,  266,   -1,   -1,  269,  270,
   -1,  272,  273,   -1,  275,   -1,  277,   -1,   -1,  280,
   -1,  282,   -1,  284,   -1,  286,   -1,  288,   -1,  290,
  291,  292,  293,  294,  262,    0,   -1,  265,  266,   -1,
   -1,  269,  270,   -1,  272,  273,   -1,  275,   -1,  277,
   -1,   -1,  280,   -1,  282,   -1,  284,   -1,  286,   -1,
  288,   -1,  290,  291,  292,  293,  294,  262,    0,   -1,
  265,  266,   -1,   -1,  269,  270,   -1,  272,  273,   -1,
  275,   -1,  277,   -1,   -1,  280,   -1,  282,   -1,  284,
   -1,  286,   -1,  288,   -1,  290,  291,  262,    0,   -1,
  265,  266,   -1,   -1,  269,  270,   -1,  272,  273,   -1,
  275,   -1,  277,   -1,   -1,  280,   -1,  282,   -1,  284,
   -1,  286,   -1,  288,   -1,  290,  291,    0,  262,   -1,
   -1,  265,  266,   -1,   -1,  269,  270,   -1,  272,  273,
   -1,  275,   -1,  277,   -1,   -1,  280,   -1,  282,   -1,
  284,   -1,  286,   -1,  288,  262,  290,  291,  265,  266,
    0,   -1,  269,  270,   -1,  272,  273,   -1,  275,   -1,
  277,   -1,   -1,  280,   -1,  282,   -1,  284,   -1,  286,
   -1,  288,   -1,  290,  291,  295,  296,  297,  298,  299,
  300,  301,   -1,  262,   -1,   -1,  265,  266,   -1,   -1,
  269,  270,   -1,  272,  273,   -1,  275,   -1,  277,   -1,
   -1,  280,   -1,  282,   -1,  284,   -1,  286,   -1,  288,
   -1,  290,  291,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   -1,   -1,  265,  266,   -1,   -1,  269,  270,
   -1,  272,  273,   -1,  275,   -1,  277,   -1,   -1,  280,
   -1,  282,   -1,  284,   -1,  286,   -1,  288,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,   -1,  265,  266,   -1,
   -1,  269,  270,   -1,  272,  273,   -1,  275,   -1,  277,
   -1,   -1,  280,   -1,  282,   -1,  284,   -1,  286,   -1,
  288,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,
  265,  266,   -1,   -1,  269,  270,   -1,  272,  273,   -1,
  275,   -1,  277,   -1,   -1,  280,   -1,  282,   -1,  284,
   -1,  286,   -1,  288,   -1,   -1,   -1,   -1,   -1,   -1,
  262,   -1,   -1,  265,  266,   -1,   -1,  269,  270,   -1,
  272,  273,   -1,  275,   -1,  277,   -1,   -1,  280,   -1,
  282,   -1,  284,   -1,  286,   -1,  288,   -1,   -1,   -1,
  262,   -1,   -1,  265,  266,   -1,   -1,  269,  270,   -1,
  272,  273,   -1,  275,   -1,  277,   -1,   -1,  280,   -1,
  282,   -1,  284,   -1,  286,   -1,  288,   -1,   -1,  262,
   -1,   -1,  265,  266,   -1,   -1,  269,  270,   -1,  272,
  273,   -1,  275,   -1,  277,   -1,   -1,  280,   -1,  282,
   -1,  284,   -1,  286,   -1,  288,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,   -1,  265,  266,   -1,   -1,  269,
  270,   -1,  272,   -1,  275,  275,   -1,  277,   -1,   -1,
  280,   -1,  282,   -1,  284,   -1,  286,   -1,  288,  290,
  291,  292,  293,  294,  295,  296,  297,  298,  299,  300,
  301,  277,  290,  291,  292,  293,  294,  295,  296,  297,
  298,  299,  300,  301,  290,  291,  292,  293,  294,  295,
  296,  297,  298,  299,  300,  301,  282,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  290,  291,  292,  293,  294,  295,
  296,  297,  298,  299,  300,  301,  282,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  290,  291,  292,  293,  294,  295,
  296,  297,  298,  299,  300,  301,  284,   -1,  286,   -1,
   -1,   -1,  290,  291,  292,  293,  294,  295,  296,  297,
  298,  299,  300,  301,  286,   -1,   -1,   -1,  290,  291,
  292,  293,  294,  295,  296,  297,  298,  299,  300,  301,
  288,   -1,  290,  291,  292,  293,  294,  295,  296,  297,
  298,  299,  300,  301,  288,   -1,  290,  291,  292,  293,
  294,  295,  296,  297,  298,  299,  300,  301,  288,   -1,
  290,  291,  292,  293,  294,  295,  296,  297,  298,  299,
  300,  301,  292,  293,  294,  295,  296,  297,  298,  299,
  300,  301,
};
#define YYFINAL 14
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 301
#if YYDEBUG
static const char *yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"INTEGER_LITERAL","ID",
"STRING_LITERAL","INT","STRING","TYPE","ARRAY","OF","VAR","FUNCTION","NIL",
"LET","IN","END","IF","THEN","ELSE","WHILE","DO","FOR","TO","BREAK",
"LEFT_CURLY_BRACE","RIGHT_CURLY_BRACE","COLON","COMMA","LEFT_PARENTHESIS",
"RIGHT_PARENTHESIS","DOT","SEMICOLON","LEFT_BRACKET","RIGHT_BRACKET",
"ASSIGNMENT_SIGN","LOGICAL_AND","LOGICAL_OR","EQUAL_SIGN","NOT_EQUAL_SIGN",
"LESS_SIGN","GREATER_SIGN","LESS_EQUAL_SIGN","GREATER_EQUAL_SIGN","ADD_SIGN",
"MINUS_SIGN","MULTIPLY_SIGN","DIVIDE_SIGN",
};
static const char *yyrule[] = {
"$accept : program",
"decs : dec decs",
"decs :",
"dec : tydec",
"dec : vardec",
"dec : fundec",
"tydec : TYPE build_in_type EQUAL_SIGN ID",
"tydec : TYPE ID EQUAL_SIGN ID",
"tydec : TYPE build_in_type EQUAL_SIGN ty",
"tydec : TYPE ID EQUAL_SIGN ty",
"build_in_type : INT",
"build_in_type : STRING",
"ty : build_in_type",
"ty : LEFT_CURLY_BRACE tyfields RIGHT_CURLY_BRACE",
"ty : ARRAY OF build_in_type",
"ty : ARRAY OF ID",
"tyfields : ID COLON build_in_type tyfields_aux",
"tyfields : ID COLON ID tyfields_aux",
"tyfields :",
"tyfields_aux : COMMA ID COLON build_in_type tyfields_aux",
"tyfields_aux : COMMA ID COLON ID tyfields_aux",
"tyfields_aux :",
"vardec : VAR ID ASSIGNMENT_SIGN exp",
"vardec : VAR ID COLON build_in_type ASSIGNMENT_SIGN exp",
"vardec : VAR ID COLON ID ASSIGNMENT_SIGN exp",
"fundec : FUNCTION ID LEFT_PARENTHESIS tyfields RIGHT_PARENTHESIS EQUAL_SIGN exp",
"fundec : FUNCTION ID LEFT_PARENTHESIS tyfields RIGHT_PARENTHESIS COLON build_in_type EQUAL_SIGN exp",
"fundec : FUNCTION ID LEFT_PARENTHESIS tyfields RIGHT_PARENTHESIS COLON ID EQUAL_SIGN exp",
"l_value : ID l_value_aux",
"l_value_aux : DOT ID l_value_aux",
"l_value_aux : LEFT_BRACKET exp RIGHT_BRACKET l_value_aux",
"l_value_aux :",
"function_call : ID LEFT_PARENTHESIS RIGHT_PARENTHESIS",
"function_call : ID LEFT_PARENTHESIS exp function_call_aux RIGHT_PARENTHESIS",
"function_call_aux : COMMA exp function_call_aux",
"function_call_aux :",
"nil : NIL",
"integer_literal : INTEGER_LITERAL",
"string_literal : STRING_LITERAL",
"array_creation : build_in_type LEFT_BRACKET exp RIGHT_BRACKET OF exp",
"array_creation : ID LEFT_BRACKET exp RIGHT_BRACKET OF exp",
"record_creation : build_in_type LEFT_CURLY_BRACE ID EQUAL_SIGN exp record_creation_aux RIGHT_CURLY_BRACE",
"record_creation : ID LEFT_CURLY_BRACE ID EQUAL_SIGN exp record_creation_aux RIGHT_CURLY_BRACE",
"record_creation : build_in_type LEFT_CURLY_BRACE RIGHT_CURLY_BRACE",
"record_creation : ID LEFT_CURLY_BRACE RIGHT_CURLY_BRACE",
"record_creation_aux : COMMA ID EQUAL_SIGN exp record_creation_aux",
"record_creation_aux :",
"arithmetic : exp ADD_SIGN exp",
"arithmetic : exp MINUS_SIGN exp",
"arithmetic : exp MULTIPLY_SIGN exp",
"arithmetic : exp DIVIDE_SIGN exp",
"parentheses : LEFT_PARENTHESIS exp RIGHT_PARENTHESIS",
"no_value : LEFT_PARENTHESIS RIGHT_PARENTHESIS",
"sequencing : LEFT_PARENTHESIS exp SEMICOLON exp sequencing_aux RIGHT_PARENTHESIS",
"sequencing_aux : SEMICOLON exp sequencing_aux",
"sequencing_aux :",
"for : FOR ID ASSIGNMENT_SIGN exp TO exp DO exp",
"if_then_else : IF exp THEN exp ELSE exp",
"if_then : IF exp THEN exp",
"comparison : exp EQUAL_SIGN exp",
"comparison : exp NOT_EQUAL_SIGN exp",
"comparison : exp LESS_SIGN exp",
"comparison : exp LESS_EQUAL_SIGN exp",
"comparison : exp GREATER_SIGN exp",
"comparison : exp GREATER_EQUAL_SIGN exp",
"boolean_operate : exp LOGICAL_AND exp",
"boolean_operate : exp LOGICAL_OR exp",
"assignment : l_value ASSIGNMENT_SIGN exp",
"let : LET decs IN expseq END",
"expseq : exp let_aux",
"expseq :",
"let_aux : SEMICOLON exp let_aux",
"let_aux :",
"while : WHILE exp DO exp",
"negation : MINUS_SIGN exp",
"break : BREAK",
"exp : l_value",
"exp : function_call",
"exp : nil",
"exp : integer_literal",
"exp : string_literal",
"exp : array_creation",
"exp : record_creation",
"exp : arithmetic",
"exp : parentheses",
"exp : no_value",
"exp : sequencing",
"exp : for",
"exp : if_then_else",
"exp : if_then",
"exp : comparison",
"exp : boolean_operate",
"exp : assignment",
"exp : let",
"exp : while",
"exp : negation",
"exp : break",
"program : exp",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  500
#endif
#endif

#define YYINITSTACKSIZE 500

typedef struct {
    unsigned stacksize;
    short    *s_base;
    short    *s_mark;
    short    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;
#line 528 "myTiger.y"


void yyerror()
{
  printf("parse error:%s\tline:%i\tcolunm:%i\n",
     yytext, g_posInfo.line, g_posInfo.column);
}
 
//	parse one file
//	return 1 for success, 0 for failure
int parseOneAndOutput(char* parseFile, char* outputFile)
{
    if ((yyin = fopen(parseFile, "r")) == NULL)
    {
	printf("can not open file: %s\n", parseFile);
        return 0;
    }
	    
    //do parse
    if (yyparse() == 0)
	printf("parse succeed!\n");
    else
	return 0;
    

    FILE* output = NULL;
    if ((output = fopen(outputFile, "w")) == NULL)
    {
	printf("create file error!\n");
	fclose(yyin);
	return 0;
    }

    MyPrint_Exp(output, g_AbstractSyntaxTree, 0);
    printf("output file wrote!\n\n");

    fclose(output);
    fclose(yyin);

    return 1;
}

int main (int argc, char* argv[])
{
    FILE* output = NULL;

    if (argc != 2)
    {
	printf("Usage a.out filename\nDefault run files in testcases \
folder except merge.tig and queens.tig\n\n");
	printf("---------------------------------\n");

	char parseFile[256] = {0}, outputFile[256] = {0};
	for (unsigned i = 1; i < 50; ++i)
	{
	    sprintf(parseFile, "testcases/test%i.tig", i);
	    sprintf(outputFile, "outputTxts/AbstractSyntax_test%i.txt",
		    i);
	    
	    printf("No.%i\n", i);
	    if (!parseOneAndOutput(parseFile, outputFile))
		break;
	}
    }
    else
    {
	parseOneAndOutput(argv[1], "SingleParseResult.txt");
    }

    //	free all chained memory
    freeMemoryChain();
    return 0;
}
#line 715 "y.tab.c"

#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (short *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack)) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
#line 149 "myTiger.y"
	{ yyval.myDecList_val = makeMyDecList(yystack.l_mark[-1].myDec_val, yystack.l_mark[0].myDecList_val); }
break;
case 2:
#line 150 "myTiger.y"
	{ yyval.myDecList_val = (myDecList)NULL; }
break;
case 3:
#line 153 "myTiger.y"
	{ yyval.myDec_val = makeMyDec_Type(getCurrentPosInfo(), yystack.l_mark[0].myTypeDec_val); }
break;
case 4:
#line 154 "myTiger.y"
	{ yyval.myDec_val = makeMyDec_Var(getCurrentPosInfo(), yystack.l_mark[0].myVarDec_val); }
break;
case 5:
#line 155 "myTiger.y"
	{ yyval.myDec_val = makeMyDec_Func(getCurrentPosInfo(), yystack.l_mark[0].myFuncDec_val); }
break;
case 6:
#line 159 "myTiger.y"
	{ yyval.myTypeDec_val = makeMyTyDec(
			MySymbol_MakeSymbol(yystack.l_mark[-2].myString_val),
			makeMyTy_Named(
				getCurrentPosInfo(),
				MySymbol_MakeSymbol(yystack.l_mark[0].myString_val)));
		}
break;
case 7:
#line 166 "myTiger.y"
	{ yyval.myTypeDec_val = makeMyTyDec(
			MySymbol_MakeSymbol(yystack.l_mark[-2].myString_val),
			    makeMyTy_Named(
				getCurrentPosInfo(),
				MySymbol_MakeSymbol(yystack.l_mark[0].myString_val)));
		}
break;
case 8:
#line 173 "myTiger.y"
	{ yyval.myTypeDec_val = makeMyTyDec(
			MySymbol_MakeSymbol(yystack.l_mark[-2].myString_val),
			yystack.l_mark[0].myTy_val);
		}
break;
case 9:
#line 178 "myTiger.y"
	{ yyval.myTypeDec_val = makeMyTyDec(
			MySymbol_MakeSymbol(yystack.l_mark[-2].myString_val),
			yystack.l_mark[0].myTy_val);
		}
break;
case 10:
#line 184 "myTiger.y"
	{ yyval.myString_val = "int"; }
break;
case 11:
#line 185 "myTiger.y"
	{ yyval.myString_val = "string"; }
break;
case 12:
#line 193 "myTiger.y"
	{ yyval.myTy_val = makeMyTy_Named(
			getCurrentPosInfo(),
		    	MySymbol_MakeSymbol(yystack.l_mark[0].myString_val));
		}
break;
case 13:
#line 198 "myTiger.y"
	{ yyval.myTy_val = makeMyTy_Record(
			getCurrentPosInfo(),
			yystack.l_mark[-1].myTyFieldList_val);
		}
break;
case 14:
#line 203 "myTiger.y"
	{ yyval.myTy_val = makeMyTy_Array(
			getCurrentPosInfo(),
			MySymbol_MakeSymbol(yystack.l_mark[0].myString_val));
		}
break;
case 15:
#line 208 "myTiger.y"
	{ yyval.myTy_val = makeMyTy_Array(
			getCurrentPosInfo(),
			MySymbol_MakeSymbol(yystack.l_mark[0].myString_val));
		}
break;
case 16:
#line 214 "myTiger.y"
	{ yyval.myTyFieldList_val = makeMyTyFieldList(
				makeMyTyField(
					MySymbol_MakeSymbol(yystack.l_mark[-3].myString_val),
					MySymbol_MakeSymbol(yystack.l_mark[-1].myString_val)),
				yystack.l_mark[0].myTyFieldList_val);
			}
break;
case 17:
#line 221 "myTiger.y"
	{ yyval.myTyFieldList_val = makeMyTyFieldList(
				makeMyTyField(
					MySymbol_MakeSymbol(yystack.l_mark[-3].myString_val),
					MySymbol_MakeSymbol(yystack.l_mark[-1].myString_val)),
				yystack.l_mark[0].myTyFieldList_val);
			}
break;
case 18:
#line 227 "myTiger.y"
	{ yyval.myTyFieldList_val = (myTyFieldList)NULL; }
break;
case 19:
#line 230 "myTiger.y"
	{ yyval.myTyFieldList_val = makeMyTyFieldList(
				makeMyTyField(
					MySymbol_MakeSymbol(yystack.l_mark[-3].myString_val),
					MySymbol_MakeSymbol(yystack.l_mark[-1].myString_val)),
				yystack.l_mark[0].myTyFieldList_val);
			}
break;
case 20:
#line 237 "myTiger.y"
	{ yyval.myTyFieldList_val = makeMyTyFieldList(
				makeMyTyField(
					MySymbol_MakeSymbol(yystack.l_mark[-3].myString_val),
					MySymbol_MakeSymbol(yystack.l_mark[-1].myString_val)),
				yystack.l_mark[0].myTyFieldList_val);
			}
break;
case 21:
#line 243 "myTiger.y"
	{ yyval.myTyFieldList_val = (myTyFieldList)NULL; }
break;
case 22:
#line 247 "myTiger.y"
	{ yyval.myVarDec_val = makeMyVarDec_ShortForm(
			makeMyShortFormVar(
				MySymbol_MakeSymbol(yystack.l_mark[-2].myString_val),
				yystack.l_mark[0].myExp_val));
		}
break;
case 23:
#line 253 "myTiger.y"
	{ yyval.myVarDec_val = makeMyVarDec_LongForm(
			makeMyLongFormVar(
				MySymbol_MakeSymbol(yystack.l_mark[-4].myString_val),
				MySymbol_MakeSymbol(yystack.l_mark[-2].myString_val),
				yystack.l_mark[0].myExp_val));
		}
break;
case 24:
#line 260 "myTiger.y"
	{ yyval.myVarDec_val = makeMyVarDec_LongForm(
			makeMyLongFormVar(
				MySymbol_MakeSymbol(yystack.l_mark[-4].myString_val),
				MySymbol_MakeSymbol(yystack.l_mark[-2].myString_val),
				yystack.l_mark[0].myExp_val));
		}
break;
case 25:
#line 269 "myTiger.y"
	{ yyval.myFuncDec_val = makeMyFuncDec_Procedure(
			makeMyProcedureDec(
				MySymbol_MakeSymbol(yystack.l_mark[-5].myString_val),
				yystack.l_mark[-3].myTyFieldList_val,
				yystack.l_mark[0].myExp_val));
		}
break;
case 26:
#line 276 "myTiger.y"
	{ yyval.myFuncDec_val = makeMyFuncDec_Function(
			makeMyFunctionDec(
				MySymbol_MakeSymbol(yystack.l_mark[-7].myString_val),
				yystack.l_mark[-5].myTyFieldList_val,
				MySymbol_MakeSymbol(yystack.l_mark[-2].myString_val),
				yystack.l_mark[0].myExp_val));
		}
break;
case 27:
#line 284 "myTiger.y"
	{ yyval.myFuncDec_val = makeMyFuncDec_Function(
			makeMyFunctionDec(
				MySymbol_MakeSymbol(yystack.l_mark[-7].myString_val),
				yystack.l_mark[-5].myTyFieldList_val,
				MySymbol_MakeSymbol(yystack.l_mark[-2].myString_val),
				yystack.l_mark[0].myExp_val));
		}
break;
case 28:
#line 299 "myTiger.y"
	{ yyval.myLValue_val = makeMyLValue(
				getCurrentPosInfo(),
				MySymbol_MakeSymbol(yystack.l_mark[-1].myString_val),
				yystack.l_mark[0].myLValueAux_val);
			}
break;
case 29:
#line 306 "myTiger.y"
	{ yyval.myLValueAux_val = makeMyLValueAux(
				MySymbol_MakeSymbol(yystack.l_mark[-1].myString_val),
				(myExp)NULL,
				yystack.l_mark[0].myLValueAux_val);
			}
break;
case 30:
#line 312 "myTiger.y"
	{ yyval.myLValueAux_val = makeMyLValueAux(
				(mySymbol)NULL,
				yystack.l_mark[-2].myExp_val,
				yystack.l_mark[0].myLValueAux_val);
			}
break;
case 31:
#line 317 "myTiger.y"
	{ yyval.myLValueAux_val = (myLValueAux)NULL; }
break;
case 32:
#line 321 "myTiger.y"
	{ yyval.myFunctionCallExp_val = makeMyFunctionCallExp_NoParam(
				makeMyNoParamFunctionCallExp(
					MySymbol_MakeSymbol(yystack.l_mark[-2].myString_val)));
			}
break;
case 33:
#line 326 "myTiger.y"
	{ yyval.myFunctionCallExp_val = makeMyFunctionCallExp_Param(
				makeMyParamFunctionCallExp(
					MySymbol_MakeSymbol(yystack.l_mark[-4].myString_val),
					makeMyExpList(
						yystack.l_mark[-2].myExp_val, yystack.l_mark[-1].myExpList_val)));
			}
break;
case 34:
#line 334 "myTiger.y"
	{ yyval.myExpList_val = makeMyExpList(yystack.l_mark[-1].myExp_val, yystack.l_mark[0].myExpList_val); }
break;
case 35:
#line 335 "myTiger.y"
	{ yyval.myExpList_val = (myExpList)NULL; }
break;
case 36:
#line 338 "myTiger.y"
	{ yyval.myNilExp_val = makeMyNilExp(); }
break;
case 37:
#line 341 "myTiger.y"
	{ yyval.myIntegerLiteralExp_val = makeMyIntegerLiteralExp(yystack.l_mark[0].int_val); }
break;
case 38:
#line 343 "myTiger.y"
	{ yyval.myStringLiteralExp_val = makeMyStringLiteralExp(yystack.l_mark[0].myString_val); }
break;
case 39:
#line 347 "myTiger.y"
	{ yyval.myArrayCreationExp_val = makeMyArrayCreationExp(
				MySymbol_MakeSymbol(yystack.l_mark[-5].myString_val),
				yystack.l_mark[-3].myExp_val,
				yystack.l_mark[0].myExp_val);
			}
break;
case 40:
#line 353 "myTiger.y"
	{ yyval.myArrayCreationExp_val = makeMyArrayCreationExp(
				MySymbol_MakeSymbol(yystack.l_mark[-5].myString_val),
				yystack.l_mark[-3].myExp_val,
				yystack.l_mark[0].myExp_val);
			}
break;
case 41:
#line 361 "myTiger.y"
	{ yyval.myRecordCreationExp_val = makeMyRecordCreationExp_Field(
				makeMyFieldRecordCreationExp(
					MySymbol_MakeSymbol(yystack.l_mark[-6].myString_val),
					makeMyRecordFieldList(
						makeMyRecordField(
							MySymbol_MakeSymbol(yystack.l_mark[-4].myString_val),
							yystack.l_mark[-2].myExp_val),
							yystack.l_mark[-1].myRecordFieldList_val)));
			}
break;
case 42:
#line 371 "myTiger.y"
	{ yyval.myRecordCreationExp_val = makeMyRecordCreationExp_Field(
				makeMyFieldRecordCreationExp(
					MySymbol_MakeSymbol(yystack.l_mark[-6].myString_val),
					makeMyRecordFieldList(
						makeMyRecordField(
							MySymbol_MakeSymbol(yystack.l_mark[-4].myString_val),
							yystack.l_mark[-2].myExp_val),
							yystack.l_mark[-1].myRecordFieldList_val)));
			}
break;
case 43:
#line 381 "myTiger.y"
	{ yyval.myRecordCreationExp_val = makeMyRecordCreationExp_NoField(
				makeMyNoFieldRecordCreationExp(
					MySymbol_MakeSymbol(yystack.l_mark[-2].myString_val)));
			}
break;
case 44:
#line 386 "myTiger.y"
	{ yyval.myRecordCreationExp_val = makeMyRecordCreationExp_NoField(
				makeMyNoFieldRecordCreationExp(
					MySymbol_MakeSymbol(yystack.l_mark[-2].myString_val)));
			}
break;
case 45:
#line 392 "myTiger.y"
	{ yyval.myRecordFieldList_val = makeMyRecordFieldList(
					makeMyRecordField(
						MySymbol_MakeSymbol(yystack.l_mark[-3].myString_val),
						yystack.l_mark[-1].myExp_val),
					yystack.l_mark[0].myRecordFieldList_val); }
break;
case 46:
#line 397 "myTiger.y"
	{ yyval.myRecordFieldList_val = (myRecordFieldList)NULL; }
break;
case 47:
#line 400 "myTiger.y"
	{ yyval.myArithmeticExp_val = makeMyArithmeticExp_Plus(yystack.l_mark[-2].myExp_val, yystack.l_mark[0].myExp_val); }
break;
case 48:
#line 401 "myTiger.y"
	{ yyval.myArithmeticExp_val = makeMyArithmeticExp_Minus(yystack.l_mark[-2].myExp_val, yystack.l_mark[0].myExp_val); }
break;
case 49:
#line 402 "myTiger.y"
	{ yyval.myArithmeticExp_val = makeMyArithmeticExp_Multiply(yystack.l_mark[-2].myExp_val, yystack.l_mark[0].myExp_val); }
break;
case 50:
#line 403 "myTiger.y"
	{ yyval.myArithmeticExp_val = makeMyArithmeticExp_Divide(yystack.l_mark[-2].myExp_val, yystack.l_mark[0].myExp_val); }
break;
case 51:
#line 407 "myTiger.y"
	{ yyval.myParenthesesExp_val = makeMyParenthesesExp(yystack.l_mark[-1].myExp_val); }
break;
case 52:
#line 411 "myTiger.y"
	{ yyval.myNoValueExp_val = makeMyNoValueExp(); }
break;
case 53:
#line 415 "myTiger.y"
	{ yyval.mySequencingExp_val = makeMySequencingExp(yystack.l_mark[-4].myExp_val, yystack.l_mark[-2].myExp_val, yystack.l_mark[-1].myExpList_val); }
break;
case 54:
#line 417 "myTiger.y"
	{ yyval.myExpList_val = makeMyExpList(yystack.l_mark[-1].myExp_val, yystack.l_mark[0].myExpList_val); }
break;
case 55:
#line 418 "myTiger.y"
	{ yyval.myExpList_val = (myExpList)NULL; }
break;
case 56:
#line 422 "myTiger.y"
	{ yyval.myForExp_val = makeMyForExp(
				MySymbol_MakeSymbol(yystack.l_mark[-6].myString_val),
				yystack.l_mark[-4].myExp_val,
				yystack.l_mark[-2].myExp_val,
				yystack.l_mark[0].myExp_val);
			}
break;
case 57:
#line 430 "myTiger.y"
	{ yyval.myIfThenElseExp_val = makeMyIfThenElseExp(yystack.l_mark[-4].myExp_val, yystack.l_mark[-2].myExp_val, yystack.l_mark[0].myExp_val); }
break;
case 58:
#line 433 "myTiger.y"
	{ yyval.myIfThenExp_val = makeMyIfThenExp(yystack.l_mark[-2].myExp_val, yystack.l_mark[0].myExp_val); }
break;
case 59:
#line 437 "myTiger.y"
	{ yyval.myComparisonExp_val = makeMyComparisonExp_Equal(yystack.l_mark[-2].myExp_val, yystack.l_mark[0].myExp_val); }
break;
case 60:
#line 439 "myTiger.y"
	{ yyval.myComparisonExp_val = makeMyComparisonExp_NotEqual(yystack.l_mark[-2].myExp_val, yystack.l_mark[0].myExp_val); }
break;
case 61:
#line 441 "myTiger.y"
	{ yyval.myComparisonExp_val = makeMyComparisonExp_SmallerThan(yystack.l_mark[-2].myExp_val, yystack.l_mark[0].myExp_val); }
break;
case 62:
#line 443 "myTiger.y"
	{ yyval.myComparisonExp_val = makeMyComparisonExp_SmallerEqualThan(yystack.l_mark[-2].myExp_val, yystack.l_mark[0].myExp_val); }
break;
case 63:
#line 445 "myTiger.y"
	{ yyval.myComparisonExp_val = makeMyComparisonExp_LargerThan(yystack.l_mark[-2].myExp_val, yystack.l_mark[0].myExp_val); }
break;
case 64:
#line 447 "myTiger.y"
	{ yyval.myComparisonExp_val = makeMyComparisonExp_LargerEqualThan(yystack.l_mark[-2].myExp_val, yystack.l_mark[0].myExp_val); }
break;
case 65:
#line 451 "myTiger.y"
	{ yyval.myBooleanOperateExp_val = makeMyBooleanOperateExp_And(yystack.l_mark[-2].myExp_val, yystack.l_mark[0].myExp_val); }
break;
case 66:
#line 453 "myTiger.y"
	{ yyval.myBooleanOperateExp_val = makeMyBooleanOperateExp_Or(yystack.l_mark[-2].myExp_val, yystack.l_mark[0].myExp_val); }
break;
case 67:
#line 457 "myTiger.y"
	{ yyval.myAssignmentExp_val = makeMyAssignmentExp(yystack.l_mark[-2].myLValue_val, yystack.l_mark[0].myExp_val); }
break;
case 68:
#line 460 "myTiger.y"
	{ yyval.myLetExp_val = makeMyLetExp(yystack.l_mark[-3].myDecList_val, yystack.l_mark[-1].myExpList_val); }
break;
case 69:
#line 462 "myTiger.y"
	{ yyval.myExpList_val = makeMyExpList(yystack.l_mark[-1].myExp_val, yystack.l_mark[0].myExpList_val); }
break;
case 70:
#line 463 "myTiger.y"
	{ yyval.myExpList_val = (myExpList)NULL; }
break;
case 71:
#line 465 "myTiger.y"
	{ yyval.myExpList_val = makeMyExpList(yystack.l_mark[-1].myExp_val, yystack.l_mark[0].myExpList_val); }
break;
case 72:
#line 466 "myTiger.y"
	{ yyval.myExpList_val = (myExpList)NULL; }
break;
case 73:
#line 469 "myTiger.y"
	{ yyval.myWhileExp_val = makeMyWhileExp(yystack.l_mark[-2].myExp_val, yystack.l_mark[0].myExp_val); }
break;
case 74:
#line 473 "myTiger.y"
	{ yyval.myNegationExp_val = makeMyNegationExp(yystack.l_mark[0].myExp_val); }
break;
case 75:
#line 476 "myTiger.y"
	{ yyval.myBreakExp_val = makeMyBreakExp(); }
break;
case 76:
#line 480 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_LValue(getCurrentPosInfo(), yystack.l_mark[0].myLValue_val); }
break;
case 77:
#line 482 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_FunctionCall(getCurrentPosInfo(), yystack.l_mark[0].myFunctionCallExp_val); }
break;
case 78:
#line 484 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_Nil(getCurrentPosInfo(), yystack.l_mark[0].myNilExp_val); }
break;
case 79:
#line 486 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_IntegerLiteral(getCurrentPosInfo(), yystack.l_mark[0].myIntegerLiteralExp_val); }
break;
case 80:
#line 488 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_StringLiteral(getCurrentPosInfo(), yystack.l_mark[0].myStringLiteralExp_val); }
break;
case 81:
#line 490 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_ArrayCreation(getCurrentPosInfo(), yystack.l_mark[0].myArrayCreationExp_val); }
break;
case 82:
#line 492 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_RecordCreation(getCurrentPosInfo(), yystack.l_mark[0].myRecordCreationExp_val); }
break;
case 83:
#line 494 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_Arithmetic(getCurrentPosInfo(), yystack.l_mark[0].myArithmeticExp_val); }
break;
case 84:
#line 496 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_Parentheses(getCurrentPosInfo(), yystack.l_mark[0].myParenthesesExp_val); }
break;
case 85:
#line 498 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_NoValue(getCurrentPosInfo(), yystack.l_mark[0].myNoValueExp_val); }
break;
case 86:
#line 500 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_Sequencing(getCurrentPosInfo(), yystack.l_mark[0].mySequencingExp_val); }
break;
case 87:
#line 502 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_For(getCurrentPosInfo(), yystack.l_mark[0].myForExp_val); }
break;
case 88:
#line 504 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_IfThenElse(getCurrentPosInfo(), yystack.l_mark[0].myIfThenElseExp_val); }
break;
case 89:
#line 506 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_IfThen(getCurrentPosInfo(), yystack.l_mark[0].myIfThenExp_val); }
break;
case 90:
#line 508 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_Comparison(getCurrentPosInfo(), yystack.l_mark[0].myComparisonExp_val); }
break;
case 91:
#line 510 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_BooleanOperate(getCurrentPosInfo(), yystack.l_mark[0].myBooleanOperateExp_val); }
break;
case 92:
#line 512 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_Assignment(getCurrentPosInfo(), yystack.l_mark[0].myAssignmentExp_val); }
break;
case 93:
#line 514 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_Let(getCurrentPosInfo(), yystack.l_mark[0].myLetExp_val); }
break;
case 94:
#line 516 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_While(getCurrentPosInfo(), yystack.l_mark[0].myWhileExp_val); }
break;
case 95:
#line 518 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_Negation(getCurrentPosInfo(), yystack.l_mark[0].myNegationExp_val); }
break;
case 96:
#line 520 "myTiger.y"
	{ yyval.myExp_val = makeMyExp_Break(getCurrentPosInfo(), yystack.l_mark[0].myBreakExp_val); }
break;
case 97:
#line 524 "myTiger.y"
	{ g_AbstractSyntaxTree = yystack.l_mark[0].myExp_val; }
break;
#line 1447 "y.tab.c"
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (short) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
