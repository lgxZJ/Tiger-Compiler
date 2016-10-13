#ifndef MY_ABSTRACT_SYNTAX_H
#define MY_ABSTRACT_SYNTAX_H

/**	@defgroup	AbstractSyntax	Abstract Syntax
 *	@{
 */

/**
 *	@file	myAbstractSyntax.h
 *	@date	5/23/2016
 *	@author	lgxZJ@outlook.com
 *
 *	@brief	Module about abstract syntax.
 *
 *			This file provides a way to construct the abstract syntax
 *			tree in the process of compilation.
 *			Each "make" function allocates an appropriate structure memory
 *			and assigns values to corresponding fields.
 *			Only "Variable", "Declaration", "Expression" have `pos` field
 *			and pos are decided by each production's ending terminal
 *			position.
 *
 *	@attention	All functions begin with \b "makeMy" which means they
 *				simply \b make structures with given parameters. These
 *				are so simple and straight that i decided to give no more
 *				descriptions.
 *
 *	@note	When finished using, remember to call freeMemoryChain() to
 *			free all allocated memory.
 *			For more about the \b origins of above structures and \b
 *			Abstract \b Syntax, see \b Tiger \b Book.
 */



/*
 *	NOTE:
 *		To make tests work, search
 *	`enum myLValueKind getLValueKind(myLValueAux aux);`,
 *	uncomment that line (This function is used internally)
 *	and the 'static' keyword at the source file. After
 *	testing, comment it back.
 */


#include "mySymbol.h"
#include "myReport.h"

/*-----------------------------------------------------------------
						Structure declarations
 -----------------------------------------------------------------*/

/**	@cond	*/

//-----------------------myString typedef-------------------------//

typedef char*	myString; 

 //--------------------myPos typedef------------------------//

typedef PosInfo myPos;
 

 //--------------------TyField struct------------------------// 

 struct		myTyField_
 {
	 mySymbol	varName;
	 mySymbol	typeName;
 };
 typedef	struct myTyField_*		myTyField;
 

	//////////////////////////////////////////
 
 struct		myTyFieldList_
 {
	myTyField		field;
	struct myTyFieldList_*	next;
 };
 typedef	struct myTyFieldList_*	myTyFieldList;
 
 
 //--------------------Ty struct------------------------//
 
 struct myTy_
 {
	 enum myTyKind { NamedType = 0, RecordType = 1, ArrayType = 2 } kind;
	 myPos	pos;
	 union
	 {
		mySymbol		typeName;
		myTyFieldList		tyFieldList;
		mySymbol		arrayTypeName;
	 }u;
 };
 typedef struct myTy_*		myTy;

 
	///////////////////////////////////
 
 struct myTyDec_
 {
	 mySymbol	name;
	 myTy		type;
 };
 typedef struct myTyDec_*	myTyDec;
 
 
 //----------------------Var struct--------------------//
 
struct myExp_;
typedef struct myExp_* myExp;

 struct		myLongFormVar_
 {
	 mySymbol	name;
	 mySymbol	type;
	 myExp		exp;
 };
 typedef	struct myLongFormVar_*		myLongFormVar;
 
 
	//////////////////////////////////
 
 struct		myShortFormVar_
 {
	 mySymbol	name;
	 myExp		exp;
 };
 typedef	struct myShortFormVar_*	myShortFormVar;
 
 
		/////////////////////////////////
 
 struct		myVarDec_
 {
	 enum myVarDecKind { LongFormVar, ShortFormVar } kind;
	 union
	 {
		 myLongFormVar		longFormVar;
		 myShortFormVar		shortFormVar;
	 }u;
 };
 typedef	struct myVarDec_*	myVarDec;
 
 
 //--------------------FuncDec struct-------	-------------//
 
 struct		myProcedureDec_
 {
	 mySymbol		name;
	 myTyFieldList	tyFieldList;
	 myExp			exp;
 };
 typedef	struct myProcedureDec_*	myProcedureDec;
 

 	/////////////////////////////
 
 struct		myFunctionDec_
 {
	 mySymbol		name;
	 myTyFieldList		tyFieldList;
	 mySymbol		returnType;
	 myExp			exp;
 };
 typedef struct	myFunctionDec_*		myFunctionDec;
 
	
	//////////////////////////////
 
 struct		myFuncDec_
 {
	 enum myFuncDecKind { ProcedureDec, FunctionDec } kind;
	 union
	 {
		myProcedureDec		procedureDec;
		myFunctionDec		functionDec;
	 }u;
 };
 typedef struct	myFuncDec_*		myFuncDec;
 
 
 //---------------------Dec struct----------------------//
 
typedef	myTyDec		myTypeDec;
 
struct myDec_
{
	enum myDecKind { TypeDec, VarDec, FuncDec }	kind;
	myPos	pos;
	union 
	{
		myTypeDec	tyDec;
		myVarDec	varDec;
		myFuncDec	funcDec;
	}u;
};
typedef struct myDec_*	myDec;

	
	////////////////////////////

typedef	struct	myDecList_
{
	myDec		dec;
	struct myDecList_*	next;
}*	myDecList;



//---------------------LValueExp struct------------------//

//	used for yacc parse because of eliminating left-recursion
typedef	struct	myLValueAux_
{
	mySymbol	id;
	myExp		exp;
	struct myLValueAux_*	next;
}* myLValueAux;

	//////////////////////////

//	NOTE:	To make 'l_value_aux' in myTiger.y consistent,
//	all *Aux structures must contain the same member 'myLValueAux'
//	to make that syntax works right when construct the abstract
//	syntax.

struct	mySimpleVar_
{
	//	nothing
};
typedef struct	mySimpleVar_*	mySimpleVarAux;

	/////////////////////////

struct	myRecordFieldAux_
{
    mySymbol	id;
    myLValueAux	next;
};
typedef struct	myRecordFieldAux_*	myRecordFieldAux;

	//////////////////////////
	
struct	myArraySubscript_
{
	myExp		exp;
	myLValueAux	next;
};
typedef struct	myArraySubscript_*	myArraySubscriptAux;

	//////////////////////////
	
struct	myLValueExp_
{ 
	enum myLValueKind { SimpleVar, RecordField, ArraySubscript } kind;
	myPos	pos;
	mySymbol	id;
	union
	{
		mySimpleVarAux			simpleVarAux;
		myRecordFieldAux		recordFieldAux;
		myArraySubscriptAux		arraySubscriptAux;
	}u;
};
typedef struct	myLValueExp_*	myLValueExp;

//	used internally
//enum myLValueKind getLValueKind(myLValueAux aux);


//-----------------------FunctionCallExp struct--------------------//

typedef struct	myNoParamFunctionCallExp_
{
	mySymbol	name;
}*	myNoParamFunctionCallExp;

	
	////////////////////

struct myExpList_;
typedef struct myExpList_* myExpList;

typedef	struct	myParamFunctionCallExp_
{
	mySymbol	name;
	myExpList	expList;
}*	myParamFunctionCallExp;

	
	////////////////////

struct	myFunctionCallExp_
{
	enum myFunctionCallExpKind {
		NoParamFunctionCallExp, ParamFunctionCallExp
	} kind;
	union
	{
		myNoParamFunctionCallExp	noParamFunctionCallExp;
		myParamFunctionCallExp		paramFunctionCallExp;
	}u;
};
typedef struct	myFunctionCallExp_*	myFunctionCallExp;


//----------------------NilExp struct-----------------------//

typedef	struct	myNilExp_
{
	//	contain nothing
}*	myNilExp;


//-------------------IntegerLiteralExp struct------------------//

typedef	struct	myIntegerLiteralExp_
{
	int		value;
}*	myIntegerLiteralExp;


//--------------------StringLiteralExp struct------------------//

typedef	struct	myStringLiteralExp_
{
	myString	str;	//	see util.h
}*	myStringLiteralExp;


//--------------------ArrayCreationExp struct------------------//

typedef	struct	myArrayCreationExp_
{
	mySymbol	typeName;
	myExp		length;
	myExp		initial;
}*	myArrayCreationExp;


//-------------------RecordCreation struct--------------------//

typedef	struct	myRecordField_
{
	mySymbol	varName;
	myExp		varValue;
}*	myRecordField;


	///////////////////////
	
typedef	struct	myRecordFieldList_
{
	myRecordField			field;
	struct myRecordFieldList_*	next;
}*	myRecordFieldList;


	///////////////////////
	
typedef	struct	myNoFieldRecordCreationExp_
{
	mySymbol	typeName;
}*	myNoFieldRecordCreationExp;

	
	///////////////////////

typedef	struct	myFieldRecordCreationExp_
{
	mySymbol       		typeName;
	myRecordFieldList	fieldList;
}*	myFieldRecordCreationExp;

	
	///////////////////////

typedef	struct	myRecordCreationExp_
{
	enum { NoFieldRecordCreationExp, FieldRecordCreationExp } kind;
	union
	{
		myNoFieldRecordCreationExp	noFieldRecordCreationExp;
		myFieldRecordCreationExp	fieldRecordCreationExp;
	}u;
}* myRecordCreationExp;


//----------------------ArithmeticExp struct--------------------//

typedef	struct	myArithmeticExp_
{
	enum myArithmeticOp { opPlus, opMinus, opMultiply, opDivide } op;
	myExp		left;
	myExp		right;
}*	myArithmeticExp;

	
//-------------------ParenthesesExp struct-------------------//

typedef	struct	myParenthesesExp_
{
	myExp		exp;
}*	myParenthesesExp;


//---------------------NoValueExp struct---------------------//

typedef	struct	myNoValueExp_
{	
	//	contain nothing
}*	myNoValueExp;


//---------------------SequencingExp struct------------------//

//	two or more expressions
typedef	struct	mySequencingExp_
{
	myExp		exp1;
	myExp		exp2;
	myExpList	nextList;	//	can be nullptr
}*	mySequencingExp;


//---------------------ForExp struct------------------------//

typedef	struct	myForExp_
{
	mySymbol	varName;
	myExp		varRangeLow;
	myExp		varRangeHigh;
	myExp		bodyExp;
}*	myForExp;


//----------------------IfThenElseExp struct--------------------//

typedef	struct	myIfThenElseExp_
{
	myExp		exp1;
	myExp		exp2;
	myExp		exp3;
}*	myIfThenElseExp;


//----------------------IfThenExp struct--------------------------//

typedef	struct	myIfThenExp_
{
	myExp	exp1;
	myExp	exp2;
}*	myIfThenExp;


//----------------------ComparisonExp struct----------------------//

typedef	struct	myComparisonExp_
{
	enum myComparisonOp {
		opEqual, opNotEqual, opLargerThan, opSmallerThan,
		opLargerEqualThan, opSmallerEqualThan
	} op;
	myExp	left;
	myExp	right;
}*	myComparisonExp;


//-------------------BooleanOperateExp struct--------------------//

typedef	struct	myBooleanOperateExp_
{
	enum myBooleanOperateOp { opBooleanAnd, opBooleanOr } op;
	myExp		left;
	myExp		right;
}*	myBooleanOperateExp;


//--------------------AssignmentExp struct----------------------//

typedef	struct	myAssignmentExp_
{
	myLValueExp	lValueExp;
	myExp		exp;
}*	myAssignmentExp;


//--------------------LetExp struct--------------------------//

typedef	struct	myLetExp_
{
	myDecList		decList;//	may be null
	myExpList		expList;//	may be null
}*	myLetExp;


//--------------------WhileExp struct------------------------//

typedef	struct	myWhileExp_
{
	myExp	whileExp;
	myExp	bodyExp;
}*	myWhileExp;


//--------------------NegationExp struct---------------------//

typedef	struct	myNegationExp_
{
	myExp		exp;
}*	myNegationExp;


//---------------------BreakExp struct---------------------//

typedef	struct	myBreakExp_
{
	//	need nothing
}*	myBreakExp;


//----------------------Exp struct--------------------------//

struct	myExp_
{
	enum myExpKind {
		LValueExp,			FunctionCallExp,	NilExp, 
		IntegerLiteralExp,	StringLiteralExp,	ArrayCreationExp,
		RecordCreationExp,	ArithmeticExp,		ParenthesesExp,
		NoValueExp,			SequencingExp,		ForExp,
		IfThenElseExp ,		IfThenExp,			ComparisonExp,
		BooleanOperateExp,	AssignmentExp,		LetExp,
		WhileExp,			NegationExp,		BreakExp
	}	kind;
	myPos	pos;
	union
	{
		myLValueExp			lValueExp;
		myFunctionCallExp	functionCallExp;
		myNilExp			nilExp;
		myIntegerLiteralExp	integerLiteralExp;
		myStringLiteralExp	stringLiteralExp;
		myArrayCreationExp	arrayCreationExp;
		myRecordCreationExp	recordCreationExp;
		myArithmeticExp		arithmeticExp;
		myParenthesesExp	parenthesesExp;
		myNoValueExp		noValueExp;
		mySequencingExp		sequencingExp;
		myForExp			forExp;
		myIfThenElseExp		ifThenElseExp;
		myIfThenExp			ifThenExp;
		myComparisonExp		comparisonExp;
		myBooleanOperateExp	booleanOperateExp;
		myAssignmentExp		assignmentExp;
		myLetExp			letExp;
		myWhileExp			whileExp;
		myNegationExp		negationExp;
		myBreakExp			breakExp;
	}u;
};
typedef	struct myExp_*	myExp;


typedef	struct	myExpList_
{
	myExp		exp;
	myExpList	next;
}*	myExpList;

/**	@endcond	*/


/*--------------------------------------------------------------------
						Function prototypes
 --------------------------------------------------------------------*/

/**	
 *	@name Ty About
 *	@{
 */

 myTyField		makeMyTyField		(mySymbol varName, mySymbol typeName);
 myTyFieldList 	makeMyTyFieldList	(myTyField field, myTyFieldList list);

 myTy 			makeMyTy_Named		(myPos pos, mySymbol typeName);
 myTy 			makeMyTy_Record		(myPos pos, myTyFieldList tyFieldList);
 myTy			makeMyTy_Array		(myPos pos, mySymbol arrayName);

 myTyDec		makeMyTyDec			(mySymbol name, myTy type);

/**	@}	*/


/**
 *	@name	Var About
 *	@{
 */

 myLongFormVar 	makeMyLongFormVar(
	 mySymbol name,
	 mySymbol type,
	 myExp exp);
 myShortFormVar makeMyShortFormVar(
	 mySymbol name,
	 myExp exp);
 myVarDec 		makeMyVarDec_LongForm	(myLongFormVar var);
 myVarDec 		makeMyVarDec_ShortForm	(myShortFormVar var);

/**	@}	*/


/**
 *	@name	SubRoutine About
 *	@{
 */

 myProcedureDec makeMyProcedureDec(
	mySymbol		name,
	myTyFieldList	tyFieldList,
	myExp			exp
	);
 myFunctionDec 	makeMyFunctionDec(
	mySymbol		name,
	myTyFieldList	tyFieldList,
	mySymbol		returnType,
	myExp			exp
	);
 myFuncDec 		makeMyFuncDec_Procedure	(myProcedureDec dec);
 myFuncDec 		makeMyFuncDec_Function	(myFunctionDec dec);

/**	@}	*/


/**
 *	@name	Dec About
 *	@{
 */

myDec makeMyDec_Type	(myPos pos, myTypeDec dec);
myDec makeMyDec_Var		(myPos pos, myVarDec dec);
myDec makeMyDec_Func	(myPos pos, myFuncDec dec);

myDecList makeMyDecList	(myDec dec, myDecList list);

/**	@}	*/


/**
 *	@name	LeftValue About
 *	@{
 */

myLValueAux	makeMyLValueAux(
	mySymbol id,
	myExp exp,
	myLValueAux next);
myLValueExp makeMyLValue(
	myPos pos,
	mySymbol id,
	myLValueAux aux);

/**	@}	*/


/**
 *	@name	SubRoutine Call About
 *	@{
 */

myNoParamFunctionCallExp 	makeMyNoParamFunctionCallExp(
	mySymbol name);
myParamFunctionCallExp 		makeMyParamFunctionCallExp(
	mySymbol 	name,
	myExpList 	exps);
myFunctionCallExp 			makeMyFunctionCallExp_NoParam(
	myNoParamFunctionCallExp exp);
myFunctionCallExp 			makeMyFunctionCallExp_Param(
	myParamFunctionCallExp 	 exp);

/**	@}	*/





/**
 *	@name	Literal About
 *	@{
 */

myIntegerLiteralExp makeMyIntegerLiteralExp	(int value);
myStringLiteralExp 	makeMyStringLiteralExp	(myString str);

/**	@} 	*/


/**
 *	@name	Array About
 *	@{
 */

myArrayCreationExp makeMyArrayCreationExp(
	mySymbol	typeName,
	myExp		length,
	myExp		initial
	);

/**	@}	*/


/**
 *	@name	Record About
 *	@{
 */

myRecordField 		makeMyRecordField(
	mySymbol 	varName,
	myExp 		varValue);
myRecordFieldList 	makeMyRecordFieldList(
	myRecordField		field,
	myRecordFieldList	list
	);

myNoFieldRecordCreationExp 	makeMyNoFieldRecordCreationExp(
	mySymbol typeName);
myFieldRecordCreationExp 	makeMyFieldRecordCreationExp(
	mySymbol			typeName,
	myRecordFieldList 	list);

myRecordCreationExp makeMyRecordCreationExp_NoField(
	myNoFieldRecordCreationExp 	exp);
myRecordCreationExp makeMyRecordCreationExp_Field(
	myFieldRecordCreationExp 	exp);

/**	@}	*/


/**
 *	@name	Arithmetic About
 *	@{
 */

myArithmeticExp makeMyArithmeticExp_Plus(
	myExp left, myExp right);
myArithmeticExp makeMyArithmeticExp_Minus(
	myExp left, myExp right);
myArithmeticExp makeMyArithmeticExp_Multiply(
	myExp left, myExp right);
myArithmeticExp makeMyArithmeticExp_Divide(
	myExp left, myExp right);

/**	@}	*/


/**
 *	@name	Sequencing About
 *	@{
 */

mySequencingExp makeMySequencingExp(
	myExp		exp1,
	myExp		exp2,
	myExpList	expList
	);

/**	@}	*/


/**
 *	@name	Control Flow About
 *	@{
 */

myWhileExp 	makeMyWhileExp(
	myExp whileExp,
	myExp bodyExp);

myForExp 	makeMyForExp(
	mySymbol varName,
	myExp rangeLow,
	myExp rangeHigh,
	myExp bodyExp
	);

myIfThenExp 	makeMyIfThenExp(
	myExp exp1, myExp exp2);	
myIfThenElseExp makeMyIfThenElseExp(
	myExp exp1, myExp exp2, myExp exp3);

myBreakExp 	makeMyBreakExp();

/**	@}	*/


/**
 *	@name	Comparison About
 *	@{
 */

myComparisonExp makeMyComparisonExp_Equal(
	myExp left, myExp right);
myComparisonExp makeMyComparisonExp_NotEqual(
	myExp left, myExp right);
myComparisonExp makeMyComparisonExp_LargerThan(
	myExp left, myExp right);
myComparisonExp makeMyComparisonExp_SmallerThan(
	myExp left, myExp right);
myComparisonExp makeMyComparisonExp_LargerEqualThan(
	myExp left, myExp right);
myComparisonExp makeMyComparisonExp_SmallerEqualThan(
	myExp left, myExp right);	

/**	@}	*/


/**
 *	@name	BooleanOperate About
 *	@{
 */

myBooleanOperateExp makeMyBooleanOperateExp_And(
	myExp left, myExp right);
myBooleanOperateExp makeMyBooleanOperateExp_Or(
	myExp left, myExp right);

/**	@}	*/


/**
 *	@name	Assignment About
 *	@{
 */

myAssignmentExp makeMyAssignmentExp(
	myLValueExp lValueExp,
	myExp 		exp);

/**	@}	*/


/**
 *	@name	Let About
 *	@{
 */

myLetExp	makeMyLetExp(
	myDecList decList,
	myExpList expList);

/**	@}	*/


/**
 *	@name	Others About
 *	@{
 */

myNilExp 		makeMyNilExp		();
myNoValueExp 	makeMyNoValueExp	();
myNegationExp 	makeMyNegationExp	(myExp exp);
myParenthesesExp makeMyParenthesesExp(myExp exp);

/**	@}	*/


/**
 *	@name	Exp About
 *	@{
 */

myExp makeMyExp_LValue			(myPos pos, myLValueExp exp);
myExp makeMyExp_FunctionCall	(myPos pos, myFunctionCallExp exp);
myExp makeMyExp_Nil				(myPos pos, myNilExp exp);
myExp makeMyExp_IntegerLiteral	(myPos pos, myIntegerLiteralExp exp);
myExp makeMyExp_StringLiteral	(myPos pos, myStringLiteralExp exp);
myExp makeMyExp_ArrayCreation	(myPos pos, myArrayCreationExp exp);
myExp makeMyExp_RecordCreation	(myPos pos, myRecordCreationExp exp);
myExp makeMyExp_Arithmetic		(myPos pos, myArithmeticExp exp);
myExp makeMyExp_Parentheses		(myPos pos, myParenthesesExp exp);
myExp makeMyExp_NoValue			(myPos pos, myNoValueExp exp);
myExp makeMyExp_Sequencing		(myPos pos, mySequencingExp exp);
myExp makeMyExp_For				(myPos pos, myForExp exp);
myExp makeMyExp_IfThenElse		(myPos pos, myIfThenElseExp exp);
myExp makeMyExp_IfThen			(myPos pos, myIfThenExp exp);
myExp makeMyExp_Comparison		(myPos pos, myComparisonExp exp);
myExp makeMyExp_BooleanOperate	(myPos pos, myBooleanOperateExp exp);
myExp makeMyExp_Assignment		(myPos pos, myAssignmentExp exp);
myExp makeMyExp_Let				(myPos pos, myLetExp exp);
myExp makeMyExp_While			(myPos pos, myWhileExp exp);
myExp makeMyExp_Negation		(myPos pos, myNegationExp exp);
myExp makeMyExp_Break			(myPos pos, myBreakExp exp);

myExpList makeMyExpList			(myExp exp, myExpList list);

/**	@}	*/

/**	@}	*/

#endif
