/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
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
/* Line 2058 of yacc.c  */
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


/* Line 2058 of yacc.c  */
#line 189 "y.tab.h"
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
