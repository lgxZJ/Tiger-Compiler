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

#include "stdio.h"
#include "y.tab.h"
#include "myReport.h"
#include <string.h>

    extern FILE* yyin;
    extern char* yytext;
    extern PosInfo	g_posInfo;

    
#line 31 "y.tab.c"

#ifndef YYSTYPE
typedef int YYSTYPE;
#endif

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

#define TYPE 257
#define ARRAY 258
#define OF 259
#define ID 260
#define INT 261
#define STRING 262
#define VAR 263
#define FUNCTION 264
#define NIL 265
#define LET 266
#define IN 267
#define END 268
#define STRING_LITERAL 269
#define INTEGER_LITERAL 270
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
    1,    1,    2,    2,    2,    3,    3,    3,    3,    6,
    6,    7,    7,    7,    7,    8,    8,    8,    9,    9,
    9,    4,    4,    4,    5,    5,    5,   11,   12,   12,
   12,   13,   13,   14,   14,   15,   16,   17,   18,   18,
   19,   19,   19,   19,   20,   20,   21,   21,   21,   21,
   22,   23,   24,   25,   25,   26,   27,   28,   29,   29,
   29,   29,   29,   29,   30,   30,   31,   32,   33,   33,
   34,   34,   35,   36,   37,   10,   10,   10,   10,   10,
   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,
   10,   10,   10,   10,   10,   10,    0,
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
    0,   10,   11,   36,    0,   38,   37,    0,    0,    0,
   75,    0,    0,    0,    0,    0,    0,   77,   78,   79,
   80,   81,   82,   83,   84,   85,   86,   87,   88,   89,
   90,   91,   92,   93,   94,   95,   96,    0,    0,    0,
    0,   28,    0,    0,    0,    0,    0,    3,    4,    5,
    0,    0,    0,   52,    0,   74,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   44,   32,    0,    0,    0,    0,    0,    0,
    0,    0,    1,    0,    0,    0,   51,    0,    0,   43,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   49,   50,    0,    0,    0,    0,    0,   29,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   33,    0,    0,   30,    0,
    7,    0,   12,    9,    6,    8,    0,    0,    0,    0,
    0,    0,   69,   68,    0,    0,    0,    0,    0,    0,
    0,    0,   34,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   53,    0,    0,    0,   42,
   15,   14,   13,    0,    0,    0,    0,    0,    0,   71,
    0,   54,   41,    0,    0,   17,   16,    0,    0,    0,
    0,    0,    0,    0,    0,   45,    0,    0,    0,    0,
    0,   20,   19,
};
static const short yydgoto[] = {                         14,
   46,   47,   48,   49,   50,   15,  134,  141,  186,   16,
   17,  129,   18,  107,   19,   20,   21,   22,   23,  152,
   24,   25,   26,   27,  148,   28,   29,   30,   31,   32,
   33,   34,  117,  143,   35,   36,   37,
};
static const short yysindex[] = {                      -251,
 -136,    0,    0,    0, -214,    0,    0, -251, -251, -221,
    0, -173, -251,    0, -257,  801, -258,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -244, -104, -218,
 -251,    0, -176, -216, -209, -246, -214,    0,    0,    0,
  -82,  644, -223,    0,  715,    0, -243, -251, -251, -251,
 -251, -251, -251, -251, -251, -251, -251, -251, -251, -251,
 -251, -217,    0,    0,  677, -181,  761, -211, -202, -255,
 -189, -251,    0, -251, -251, -251,    0, -251, -197,    0,
  775,  811,  811,  -39,  -39,  -39,  -39,  -39,  -39, -229,
 -229,    0,    0,  801, -251, -251, -177, -251,    0, -247,
 -137, -131, -142, -251, -151,  731, -156,  -70,  801,  -45,
  747, -251, -144,  697,  677,    0,  789, -251,    0, -127,
    0, -151,    0,    0,    0,    0, -175, -161,  801, -148,
 -147, -251,    0,    0, -251, -251, -251, -146,  697, -251,
 -115, -134,    0, -181,  801,  -68, -130, -251, -251,  -56,
 -268,  731,  801,  657,  747,    0, -125,  801, -133,    0,
    0,    0,    0,  801,  801, -122, -122,  -26, -251,    0,
 -251,    0,    0, -251,  -97,    0,    0, -128, -124,  801,
  801,  697, -112, -251, -251,    0,   10,  801,  801, -122,
 -122,    0,    0,
};
static const short yyrindex[] = {                         0,
    1,    0,    0,    0,  -96,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  181,   46,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  -96,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -100,    1,    0,    0,    0,    0,
    0,  -83,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  177,  207,  237,  267,  304,  349,  392,  435,   91,
  134,    0,    0,  472,    0,    0,    0,    0,    0,    1,
    0,    0,    0,    0,  -88,  -69,    0,  645,  502,    0,
  -84,    0,    0,  -93, -100,    0,    0,    0,    0,    0,
    0,  -78,    0,    0,    0,    0,    0,    0, -190,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  -93,    0,
    0,    0,    0,    1,  532,    0,    0,    0,    0,    0,
    0,  -69,  560,    0,  -84,    0,    0,  593,    0,    0,
    0,    0,    0,  -81,  -66, -239, -239,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  -24,
  619,  -93,    0,    0,    0,    0,    0,   48,   93, -239,
 -239,    0,    0,
};
static const short yygindex[] = {                         0,
  191,    0,    0,    0,    0,  -43,  129,  110, -172,   -6,
    0,    7,    0,  119,    0,    0,    0,    0,    0, -145,
    0,    0,    0,    0,   92,    0,    0,    0,    0,    0,
    0,    0,    0,  101,    0,    0,    0,
};
#define YYTABLESIZE 1112
static const short yytable[] = {                         79,
   31,   51,   52,  167,  187,   55,   56,   42,    1,    2,
    3,  128,  178,    4,    5,   72,   89,    6,    7,    8,
   82,   57,    9,  179,   10,  113,   11,  202,  203,   58,
   71,   12,   75,  114,   77,   73,   90,   40,   53,  108,
   21,   76,   43,   80,   21,   76,  196,   13,   44,   45,
   81,   91,   92,   93,   94,   95,   96,   97,   98,   99,
  100,  101,  102,  103,  104,   86,   22,  133,  133,  138,
   69,   70,   22,   22,  105,  116,   22,  118,  119,  120,
  111,  121,  109,   78,    2,    3,    1,    2,    3,  112,
   47,    4,    5,  115,  122,    6,    7,    8,  124,  125,
    9,  127,   10,   40,   11,  108,  126,  139,  140,   12,
   54,  144,  172,  158,  150,  149,  177,  137,    2,    3,
  130,  155,  131,    2,    3,   13,  130,  159,  135,    2,
    3,  156,  160,   48,  189,  162,  161,  166,  163,  164,
  165,  132,   38,  168,  169,  170,   39,  132,   40,  173,
   41,  174,  175,  201,  183,    1,    2,    3,  184,  185,
    4,    5,  193,  194,    6,    7,    8,  195,  197,    9,
    2,   10,  190,   11,  191,   24,   65,  192,   12,   74,
   97,   24,   24,   35,   70,   24,   46,  198,  199,   84,
   23,  171,    2,    3,   13,   18,   23,   23,   72,   55,
   23,   18,  145,  176,    2,    3,   66,   59,   60,   61,
   62,   63,   64,   65,   66,   67,   68,   69,   70,   59,
   60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
   70,  146,   25,  188,    2,    3,   59,   83,   25,   25,
  136,  157,   25,  153,   59,   60,   61,   62,   63,   64,
   65,   66,   67,   68,   69,   70,  182,   31,   67,   68,
   69,   70,  180,   31,   31,    0,   60,   31,   31,  200,
    2,    3,   31,   31,    0,   31,    0,   31,    0,    0,
   31,    0,   31,    0,   31,    0,   31,    0,   31,   31,
   31,   31,   31,   31,   31,   31,   31,   31,   31,   31,
   31,   31,   76,   61,   27,    0,    0,    0,   76,   76,
   27,   27,   76,   76,   27,    0,    0,   76,   76,    0,
   76,    0,   76,    0,    0,   76,    0,   76,    0,   76,
    0,   76,    0,   76,    0,   76,   76,   76,   76,   76,
   76,   76,   76,   76,   76,   76,   76,   47,   63,   26,
    0,    0,    0,   47,   47,   26,   26,   47,   47,   26,
    0,    0,   47,   47,    0,   47,    0,   47,    0,    0,
   47,    0,   47,    0,   47,    0,   47,    0,   47,    0,
   47,   47,   47,   47,   47,   47,   47,   47,   47,   47,
   48,   62,    0,    0,    0,    0,   48,   48,    0,    0,
   48,   48,    0,    0,    0,   48,   48,    0,   48,    0,
   48,    0,    0,   48,    0,   48,    0,   48,    0,   48,
    0,   48,    0,   48,   48,   48,   48,   48,   48,   48,
   48,   48,   48,   65,   64,    0,    0,    0,    0,   65,
   65,    0,    0,   65,   65,    0,    0,    0,   65,   65,
    0,   65,    0,   65,    0,    0,   65,    0,   65,    0,
   65,    0,   65,   66,   65,    0,   65,   65,    0,   66,
   66,   67,    0,   66,   66,    0,    0,    0,   66,   66,
    0,   66,    0,   66,    0,    0,   66,    0,   66,    0,
   66,    0,   66,   59,   66,    0,   66,   66,    0,   59,
   59,   73,    0,   59,   59,    0,    0,    0,   59,   59,
    0,   59,    0,   59,    0,    0,   59,    0,   59,    0,
   59,    0,   59,   60,   59,    0,   59,   59,    0,   60,
   60,   40,    0,   60,   60,    0,    0,    0,   60,   60,
    0,   60,    0,   60,    0,    0,   60,    0,   60,    0,
   60,    0,   60,    0,   60,    0,   60,   60,    0,   57,
   61,    0,    0,    0,    0,    0,   61,   61,    0,    0,
   61,   61,    0,    0,    0,   61,   61,    0,   61,    0,
   61,    0,    0,   61,    0,   61,    0,   61,    0,   61,
    0,   61,   39,   61,   61,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   63,    0,    0,    0,    0,
    0,   63,   63,    0,    0,   63,   63,    0,   56,    0,
   63,   63,    0,   63,    0,   63,    0,    0,   63,    0,
   63,    0,   63,    0,   63,    0,   63,    0,   63,   63,
    0,    0,    0,    0,   58,    0,    0,    0,   62,    0,
    0,    0,    0,    0,   62,   62,    0,    0,   62,   62,
    0,    0,    0,   62,   62,    0,   62,    0,   62,    0,
    0,   62,    0,   62,    0,   62,    0,   62,    0,   62,
    0,   62,   62,    0,    0,    0,    0,    0,    0,    0,
    0,   64,    0,    0,    0,    0,    0,   64,   64,    0,
    0,   64,   64,    0,    0,    0,   64,   64,    0,   64,
    0,   64,    0,    0,   64,    0,   64,    0,   64,    0,
   64,    0,   64,    0,   64,   64,    0,    0,   67,    0,
    0,    0,    0,    0,   67,   67,    0,    0,   67,   67,
    0,    0,    0,   67,   67,    0,   67,    0,   67,    0,
    0,   67,    0,   67,    0,   67,    0,   67,   73,   67,
    0,    0,    0,    0,   73,   73,    0,    0,   73,   73,
    0,    0,    0,   73,   73,    0,   73,    0,   73,    0,
    0,   73,    0,   73,    0,   73,    0,   73,   40,   73,
    0,    0,    0,    0,   40,   40,    0,    0,   40,   40,
    0,    0,    0,   40,   40,    0,   40,    0,   40,    0,
    0,   40,    0,   40,    0,   40,   57,   40,    0,   40,
    0,    0,   57,   57,    0,    0,   57,   57,    0,    0,
    0,   57,   57,    0,   57,    0,   57,    0,    0,   57,
    0,   57,    0,   57,    0,   57,    0,   57,    0,   39,
    0,    0,    0,    0,    0,   39,   39,    0,    0,   39,
   39,    0,    0,    0,   39,   39,    0,   39,    0,   39,
    0,    0,   39,    0,   39,   56,   39,    0,   39,    0,
   39,   56,   56,    0,    0,   56,   56,    0,    0,    0,
   56,   56,    0,   56,    0,   56,    0,    0,   56,    0,
   56,   58,   56,    0,   56,    0,   56,   58,   58,    0,
    0,   58,   58,    0,    0,    0,   58,    0,   85,   58,
    0,   58,    0,    0,   58,    0,   58,    0,   58,    0,
   58,  181,   58,   59,   60,   61,   62,   63,   64,   65,
   66,   67,   68,   69,   70,    0,   59,   60,   61,   62,
   63,   64,   65,   66,   67,   68,   69,   70,  106,    0,
    0,    0,    0,    0,    0,    0,   59,   60,   61,   62,
   63,   64,   65,   66,   67,   68,   69,   70,  151,    0,
    0,    0,    0,    0,    0,    0,   59,   60,   61,   62,
   63,   64,   65,   66,   67,   68,   69,   70,   87,    0,
   88,    0,    0,    0,   59,   60,   61,   62,   63,   64,
   65,   66,   67,   68,   69,   70,  142,    0,    0,    0,
   59,   60,   61,   62,   63,   64,   65,   66,   67,   68,
   69,   70,  147,    0,    0,    0,   59,   60,   61,   62,
   63,   64,   65,   66,   67,   68,   69,   70,  110,    0,
   59,   60,   61,   62,   63,   64,   65,   66,   67,   68,
   69,   70,  123,    0,   59,   60,   61,   62,   63,   64,
   65,   66,   67,   68,   69,   70,  154,    0,   59,   60,
   61,   62,   63,   64,   65,   66,   67,   68,   69,   70,
   59,   60,   61,   62,   63,   64,   65,   66,   67,   68,
   69,   70,   61,   62,   63,   64,   65,   66,   67,   68,
   69,   70,
};
static const short yycheck[] = {                         43,
    0,    8,    9,  149,  177,   12,   13,    1,  260,  261,
  262,  259,  281,  265,  266,  260,  260,  269,  270,  271,
  267,  279,  274,  292,  276,  281,  278,  200,  201,  287,
  289,  283,   39,  289,   41,  280,  280,  285,  260,  287,
  280,  260,  257,  260,  284,    0,  192,  299,  263,  264,
  260,   58,   59,   60,   61,   62,   63,   64,   65,   66,
   67,   68,   69,   70,   71,  289,  257,  111,  112,  113,
  300,  301,  263,  264,  292,   82,  267,   84,   85,   86,
  292,   88,   76,  260,  261,  262,  260,  261,  262,  292,
    0,  265,  266,  283,  292,  269,  270,  271,  105,  106,
  274,  108,  276,  285,  278,  287,  284,  114,  260,  283,
  284,  268,  156,  289,  259,  122,  160,  260,  261,  262,
  258,  128,  260,  261,  262,  299,  258,  289,  260,  261,
  262,  259,  281,    0,  178,  142,  284,  284,  145,  146,
  147,  279,  279,  150,  260,  280,  283,  279,  285,  280,
  287,  158,  159,  197,  280,  260,  261,  262,  292,  282,
  265,  266,  260,  292,  269,  270,  271,  292,  281,  274,
  267,  276,  179,  278,  181,  257,    0,  184,  283,  284,
    0,  263,  264,  284,  268,  267,  280,  194,  195,  272,
  257,  260,  261,  262,  299,  284,  263,  264,  268,  284,
  267,  280,  273,  260,  261,  262,    0,  290,  291,  292,
  293,  294,  295,  296,  297,  298,  299,  300,  301,  290,
  291,  292,  293,  294,  295,  296,  297,  298,  299,  300,
  301,  277,  257,  260,  261,  262,    0,   47,  263,  264,
  112,  132,  267,  125,  290,  291,  292,  293,  294,  295,
  296,  297,  298,  299,  300,  301,  165,  257,  298,  299,
  300,  301,  162,  263,  264,   -1,    0,  267,  268,  260,
  261,  262,  272,  273,   -1,  275,   -1,  277,   -1,   -1,
  280,   -1,  282,   -1,  284,   -1,  286,   -1,  288,  289,
  290,  291,  292,  293,  294,  295,  296,  297,  298,  299,
  300,  301,  257,    0,  257,   -1,   -1,   -1,  263,  264,
  263,  264,  267,  268,  267,   -1,   -1,  272,  273,   -1,
  275,   -1,  277,   -1,   -1,  280,   -1,  282,   -1,  284,
   -1,  286,   -1,  288,   -1,  290,  291,  292,  293,  294,
  295,  296,  297,  298,  299,  300,  301,  257,    0,  257,
   -1,   -1,   -1,  263,  264,  263,  264,  267,  268,  267,
   -1,   -1,  272,  273,   -1,  275,   -1,  277,   -1,   -1,
  280,   -1,  282,   -1,  284,   -1,  286,   -1,  288,   -1,
  290,  291,  292,  293,  294,  295,  296,  297,  298,  299,
  257,    0,   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,
  267,  268,   -1,   -1,   -1,  272,  273,   -1,  275,   -1,
  277,   -1,   -1,  280,   -1,  282,   -1,  284,   -1,  286,
   -1,  288,   -1,  290,  291,  292,  293,  294,  295,  296,
  297,  298,  299,  257,    0,   -1,   -1,   -1,   -1,  263,
  264,   -1,   -1,  267,  268,   -1,   -1,   -1,  272,  273,
   -1,  275,   -1,  277,   -1,   -1,  280,   -1,  282,   -1,
  284,   -1,  286,  257,  288,   -1,  290,  291,   -1,  263,
  264,    0,   -1,  267,  268,   -1,   -1,   -1,  272,  273,
   -1,  275,   -1,  277,   -1,   -1,  280,   -1,  282,   -1,
  284,   -1,  286,  257,  288,   -1,  290,  291,   -1,  263,
  264,    0,   -1,  267,  268,   -1,   -1,   -1,  272,  273,
   -1,  275,   -1,  277,   -1,   -1,  280,   -1,  282,   -1,
  284,   -1,  286,  257,  288,   -1,  290,  291,   -1,  263,
  264,    0,   -1,  267,  268,   -1,   -1,   -1,  272,  273,
   -1,  275,   -1,  277,   -1,   -1,  280,   -1,  282,   -1,
  284,   -1,  286,   -1,  288,   -1,  290,  291,   -1,    0,
  257,   -1,   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,
  267,  268,   -1,   -1,   -1,  272,  273,   -1,  275,   -1,
  277,   -1,   -1,  280,   -1,  282,   -1,  284,   -1,  286,
   -1,  288,    0,  290,  291,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  257,   -1,   -1,   -1,   -1,
   -1,  263,  264,   -1,   -1,  267,  268,   -1,    0,   -1,
  272,  273,   -1,  275,   -1,  277,   -1,   -1,  280,   -1,
  282,   -1,  284,   -1,  286,   -1,  288,   -1,  290,  291,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,  257,   -1,
   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,  267,  268,
   -1,   -1,   -1,  272,  273,   -1,  275,   -1,  277,   -1,
   -1,  280,   -1,  282,   -1,  284,   -1,  286,   -1,  288,
   -1,  290,  291,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  257,   -1,   -1,   -1,   -1,   -1,  263,  264,   -1,
   -1,  267,  268,   -1,   -1,   -1,  272,  273,   -1,  275,
   -1,  277,   -1,   -1,  280,   -1,  282,   -1,  284,   -1,
  286,   -1,  288,   -1,  290,  291,   -1,   -1,  257,   -1,
   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,  267,  268,
   -1,   -1,   -1,  272,  273,   -1,  275,   -1,  277,   -1,
   -1,  280,   -1,  282,   -1,  284,   -1,  286,  257,  288,
   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,  267,  268,
   -1,   -1,   -1,  272,  273,   -1,  275,   -1,  277,   -1,
   -1,  280,   -1,  282,   -1,  284,   -1,  286,  257,  288,
   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,  267,  268,
   -1,   -1,   -1,  272,  273,   -1,  275,   -1,  277,   -1,
   -1,  280,   -1,  282,   -1,  284,  257,  286,   -1,  288,
   -1,   -1,  263,  264,   -1,   -1,  267,  268,   -1,   -1,
   -1,  272,  273,   -1,  275,   -1,  277,   -1,   -1,  280,
   -1,  282,   -1,  284,   -1,  286,   -1,  288,   -1,  257,
   -1,   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,  267,
  268,   -1,   -1,   -1,  272,  273,   -1,  275,   -1,  277,
   -1,   -1,  280,   -1,  282,  257,  284,   -1,  286,   -1,
  288,  263,  264,   -1,   -1,  267,  268,   -1,   -1,   -1,
  272,  273,   -1,  275,   -1,  277,   -1,   -1,  280,   -1,
  282,  257,  284,   -1,  286,   -1,  288,  263,  264,   -1,
   -1,  267,  268,   -1,   -1,   -1,  272,   -1,  275,  275,
   -1,  277,   -1,   -1,  280,   -1,  282,   -1,  284,   -1,
  286,  275,  288,  290,  291,  292,  293,  294,  295,  296,
  297,  298,  299,  300,  301,   -1,  290,  291,  292,  293,
  294,  295,  296,  297,  298,  299,  300,  301,  282,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  290,  291,  292,  293,
  294,  295,  296,  297,  298,  299,  300,  301,  282,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  290,  291,  292,  293,
  294,  295,  296,  297,  298,  299,  300,  301,  284,   -1,
  286,   -1,   -1,   -1,  290,  291,  292,  293,  294,  295,
  296,  297,  298,  299,  300,  301,  286,   -1,   -1,   -1,
  290,  291,  292,  293,  294,  295,  296,  297,  298,  299,
  300,  301,  286,   -1,   -1,   -1,  290,  291,  292,  293,
  294,  295,  296,  297,  298,  299,  300,  301,  288,   -1,
  290,  291,  292,  293,  294,  295,  296,  297,  298,  299,
  300,  301,  288,   -1,  290,  291,  292,  293,  294,  295,
  296,  297,  298,  299,  300,  301,  288,   -1,  290,  291,
  292,  293,  294,  295,  296,  297,  298,  299,  300,  301,
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
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"TYPE","ARRAY","OF","ID","INT",
"STRING","VAR","FUNCTION","NIL","LET","IN","END","STRING_LITERAL",
"INTEGER_LITERAL","IF","THEN","ELSE","WHILE","DO","FOR","TO","BREAK",
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
#line 207 "myTiger.y"

		  /*int main (int argc, char* argv[])
		    {
		    if (argc != 2)
		    {
		    printf("Usage a.out filename\n");
		    exit(1);
		    }
		    else
		    {
		    yyin = fopen(argv[1], "r");
		    }

		    int result = yyparse();
		    switch (result)
		    {
		    case 0:	printf("parse succeed!\n");				break;
		    case 1:	printf("invalid input, possible wrong syntax.\n");	break;
		    case 2:	printf("memory exhaustion!\n");				break;
		    }

		    return 0;
		    }*/

int main (int argc, char* argv[])
{
    printf("Useage a.out single-filename;\nOtherwise use files in testcase fold.\n\n");

    if (argc == 2)
    {
	int result = yyparse();
	switch (result)
	{
	case 0:	printf("parse succeed!\n");				break;
	case 1:	printf("invalid input, possible wrong syntax.\n");	break;
	case 2:	printf("memory exhaustion!\n");				break;
	}
    }
    else
    {
	char	filename[256] = {0};
	for (unsigned i = 1; i < 50; ++i)
	{
	    sprintf(filename, "testcases/test%i.tig", i);
	    yyin = fopen(filename, "r");
	
	    if (yyin == NULL)
	    {
		printf("No.%i file open error!\n", i);
		perror(NULL);
		exit(-1);
	    }

	    if (yyparse() != 0)
	    {
		printf("No.%i parse error!\n", i);
		exit(-1);
	    }

	    fclose(yyin);
	    resetPos();
	}
    }
}

void yyerror()
{
  printf("parse error:%s\tline:%i\tcolunm:%i\n",
     yytext, g_posInfo.line, g_posInfo.column);
}
 
#line 679 "y.tab.c"

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
