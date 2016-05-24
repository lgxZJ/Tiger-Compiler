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
extern YYSTYPE yylval;
