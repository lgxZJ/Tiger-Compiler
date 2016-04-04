#ifndef MY_ABSTRACT_SYNTAX_H
#define MY_ABSTRACT_SYNTAX_H

/*
 *	myAbstractSyntax.h:
 *		Abstract Syntax module
 */

/////////////////////////////////////////////////////////////////////
//	mySymbol type is defined in another file
/////////////////////////////////////////////////////////////////////

/*
 *	native struct type			:	begin with "my" and end with '_'
 *	pointer to native struct	:	begin with "my" and end with no '_'
 *	enum number					:	begin with an upper case
 *	variable					|	begin with a lower case
 */

 ////////////////////////////////////////////////////////////////////
 //		each "make" function allocates an appropriate structure and
 //	assigns values to corresponding fields
 //		only "Variable", "Declaration", "Expression" have `pos` field
 //	and pos are decided by each production's ending terminal position
 /////////////////////////////////////////////////////////////////////


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

 
 //----------------myString typedef-------------------//
 
 typedef char*	myString;
 
 //-----------------Pos typedef---------------------//
 
typedef PosInfo myPos;
 
 //-----------------TyFields struct----------------//
 
 struct		myTyField_
 {
	 mySymbol	varName;
	 mySymbol	typeName;
 };
 typedef	struct myTyField_*		myTyField;
 
 myTyField makeMyTyField(mySymbol varName, mySymbol typeName);
 
	//////////////////////////////////////////
 
 struct		myTyFieldList_
 {
	myTyField		field;
	struct myTyFieldList_*	next;
 };
 typedef	struct myTyFieldList_*	myTyFieldList;
 
 myTyFieldList makeMyTyFieldList(myTyField field, myTyFieldList list);
 
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
 
 myTy makeMyTy_(myPos pos, enum myTyKind kind, void* member);
 
 //	following functions call the former function internally
 myTy makeMyTy_Named(myPos pos, mySymbol typeName);
 myTy makeMyTy_Record(myPos pos, myTyFieldList tyFieldList);
 myTy makeMyTy_Array(myPos pos, mySymbol arrayName);
 
	///////////////////////////////////
 
 struct myTyDec_
 {
	 mySymbol	name;
	 myTy		type;
 };
 typedef struct myTyDec_*	myTyDec;
 
 myTyDec makeMyTyDec(mySymbol name, myTy type);
 
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
 
 myLongFormVar makeMyLongFormVar(mySymbol name, mySymbol type, myExp exp);
 
	//////////////////////////////////
 
 struct		myShortFormVar_
 {
	 mySymbol	name;
	 myExp		exp;
 };
 typedef	struct myShortFormVar_*	myShortFormVar;
 
 myShortFormVar makeMyShortFormVar(mySymbol name, myExp exp);
 
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
 
 myVarDec makeMyVarDec_(enum myVarDecKind kind, void* member);
 
 //	function delegations
 
 myVarDec makeMyVarDec_LongForm(myLongFormVar var);
 myVarDec makeMyVarDec_ShortForm(myShortFormVar var);
 
 //--------------------FuncDec struct-------	-------------//
 
 struct		myProcedureDec_
 {
	 mySymbol		name;
	 myTyFieldList	tyFieldList;
	 myExp			exp;
 };
 typedef	struct myProcedureDec_*	myProcedureDec;
 
 myProcedureDec makeMyProcedureDec(
	mySymbol		name,
	myTyFieldList	tyFieldList,
	myExp			exp
	);
 
	/////////////////////////////
 
 struct		myFunctionDec_
 {
	 mySymbol		name;
	 myTyFieldList		tyFieldList;
	 mySymbol		returnType;
	 myExp			exp;
 };
 typedef struct	myFunctionDec_*		myFunctionDec;
 
 myFunctionDec makeMyFunctionDec(
	mySymbol		name,
	myTyFieldList	tyFieldList,
	mySymbol		returnType,
	myExp			exp
	);
	
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
 
 myFuncDec makeMyFuncDec_(enum myFuncDecKind kind, void* member);
 
 //	function delegations
 myFuncDec makeMyFuncDec_Procedure(myProcedureDec dec);
 myFuncDec makeMyFuncDec_Function(myFunctionDec dec);
 
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

myDec makeMyDec_(myPos pos, enum myDecKind kind, void* member);

//	function delegations
myDec makeMyDec_Type(myPos pos, myTypeDec dec);
myDec makeMyDec_Var(myPos pos, myVarDec dec);
myDec makeMyDec_Func(myPos pos, myFuncDec dec);
	
	////////////////////////////

typedef	struct	myDecList_
{
	myDec		dec;
	struct myDecList_*	next;
}*	myDecList;

myDecList makeMyDecList(myDec dec, myDecList list);

//---------------------LValueExp struct------------------//

//	used for yacc parse because of eliminating left-recursion
typedef	struct	myLValueAux_
{
	mySymbol	id;
	myExp		exp;
	struct myLValueAux_*	next;
}* myLValueAux;

myLValueAux	makeMyLValueAux(mySymbol id, myExp exp, myLValueAux next);

/*enum myLValueKind getLValueKind(myLValueAux aux);*/

/*
mySymbol	MyLValueAux_GetId(myLValueAux aux);
myExp		MyLValueAux_GetExp(myLValueAux aux);
myLValueAux	MyLValueAux_GetNext(myLValueAux aux);
*/

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

//mySimpleVarAux makeMySimpleVarAux();

	/////////////////////////

struct	myRecordFieldAux_
{
    mySymbol	id;
    myLValueAux	next;
};
typedef struct	myRecordFieldAux_*	myRecordFieldAux;

//myRecordFieldAux makeMyRecordFieldAux(mySymbol id, myLValueAux next);

	//////////////////////////
	
struct	myArraySubscript_
{
	myExp		exp;
	myLValueAux	next;
};
typedef struct	myArraySubscript_*	myArraySubscriptAux;

//myArraySubscriptAux makeMyArraySubscriptAux(myExp exp, myLValueAux next);

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

myLValueExp makeMyLValue(myPos pos, mySymbol id, myLValueAux aux);

/*
//	function delegations
myLValue makeMyLValue_SimpleVar(myPos pos, mySymbol id, mySimpleVarAux simpleVarAux);
myLValue makeMyLValue_RecordField(myPos pos, mySymbol id, myRecordFieldAux recordFieldAux);
myLValue makeMyLValue_ArraySubscript(myPos pos, mySymbol id, myArraySubscriptAux arraySubscriptAux);
*/
//-----------------------FunctionCallExp struct--------------------//

typedef struct	myNoParamFunctionCallExp_
{
	mySymbol	name;
}*	myNoParamFunctionCallExp;

myNoParamFunctionCallExp makeMyNoParamFunctionCallExp(
	mySymbol name);
	
	////////////////////

struct myExpList_;
typedef struct myExpList_* myExpList;

typedef	struct	myParamFunctionCallExp_
{
	mySymbol	name;
	myExpList	expList;
}*	myParamFunctionCallExp;

myParamFunctionCallExp makeMyParamFunctionCallExp(
	mySymbol name, myExpList exps);
	
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

//static myFunctionCallExp makeMyFunctionCallExp_(enum myFunctionCallExpKind kind, void* member);

//	function delegations
myFunctionCallExp makeMyFunctionCallExp_NoParam(
	myNoParamFunctionCallExp exp);
myFunctionCallExp makeMyFunctionCallExp_Param(
	myParamFunctionCallExp exp);

//----------------------NilExp struct-----------------------//

typedef	struct	myNilExp_
{
	//	contain nothing
}*	myNilExp;

myNilExp makeMyNilExp();

//-------------------IntegerLiteralExp struct------------------//

typedef	struct	myIntegerLiteralExp_
{
	int		value;
}*	myIntegerLiteralExp;

myIntegerLiteralExp makeMyIntegerLiteralExp(int value);

//--------------------StringLiteralExp struct------------------//

typedef	struct	myStringLiteralExp_
{
	myString	str;	//	see util.h
}*	myStringLiteralExp;

myStringLiteralExp makeMyStringLiteralExp(myString str);

//--------------------ArrayCreationExp struct------------------//

typedef	struct	myArrayCreationExp_
{
	mySymbol	typeName;
	myExp		length;
	myExp		initial;
}*	myArrayCreationExp;

myArrayCreationExp makeMyArrayCreationExp(
	mySymbol	typeName,
	myExp		length,
	myExp		initial
	);

//-------------------RecordCreation struct--------------------//

typedef	struct	myRecordField_
{
	mySymbol	varName;
	myExp		varValue;
}*	myRecordField;

myRecordField makeMyRecordField(
	mySymbol varName, myExp varValue);

	///////////////////////
	
typedef	struct	myRecordFieldList_
{
	myRecordField			field;
	struct myRecordFieldList_*	next;
}*	myRecordFieldList;

myRecordFieldList makeMyRecordFieldList(
	myRecordField		field,
	myRecordFieldList	list
	);

	///////////////////////
	
typedef	struct	myNoFieldRecordCreationExp_
{
	mySymbol	typeName;
}*	myNoFieldRecordCreationExp;

myNoFieldRecordCreationExp makeMyNoFieldRecordCreationExp(
	mySymbol typeName);
	
	///////////////////////

typedef	struct	myFieldRecordCreationExp_
{
	mySymbol       		typeName;
	myRecordFieldList	fieldList;
}*	myFieldRecordCreationExp;

myFieldRecordCreationExp makeMyFieldRecordCreationExp(
	mySymbol typeName, myRecordFieldList list);
	
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

myRecordCreationExp makeMyRecordCreationExp_NoField(
	myNoFieldRecordCreationExp exp);
myRecordCreationExp makeMyRecordCreationExp_Field(
	myFieldRecordCreationExp exp);

//----------------------ArithmeticExp struct--------------------//

typedef	struct	myArithmeticExp_
{
	enum myArithmeticOp { opPlus, opMinus, opMultiply, opDivide } op;
	myExp		left;
	myExp		right;
}*	myArithmeticExp;

myArithmeticExp makeMyArithmeticExp_(
	enum myArithmeticOp	op,
	myExp			left,
	myExp			right
	);

//	the following functions call the former function internally
myArithmeticExp makeMyArithmeticExp_Plus(
	myExp left, myExp right);
myArithmeticExp makeMyArithmeticExp_Minus(
	myExp left, myExp right);
myArithmeticExp makeMyArithmeticExp_Multiply(
	myExp left, myExp right);
myArithmeticExp makeMyArithmeticExp_Divide(
	myExp left, myExp right);
	
//-------------------ParenthesesExp struct-------------------//

typedef	struct	myParenthesesExp_
{
	myExp		exp;
}*	myParenthesesExp;

myParenthesesExp makeMyParenthesesExp(myExp exp);

//---------------------NoValueExp struct---------------------//

typedef	struct	myNoValueExp_
{	
	//	contain nothing
}*	myNoValueExp;

myNoValueExp makeMyNoValueExp();

//---------------------SequencingExp struct------------------//

//	two or more expressions
typedef	struct	mySequencingExp_
{
	myExp		exp1;
	myExp		exp2;
	myExpList	nextList;	//	can be nullptr
}*	mySequencingExp;

mySequencingExp makeMySequencingExp(
	myExp		exp1,
	myExp		exp2,
	myExpList	expList
	);

//---------------------ForExp struct------------------------//

typedef	struct	myForExp_
{
	mySymbol	varName;
	myExp		varRangeLow;
	myExp		varRangeHigh;
	myExp		bodyExp;
}*	myForExp;

myForExp makeMyForExp(
	mySymbol varName,
	myExp rangeLow, myExp rangeHigh,
	myExp bodyExp
	);

//----------------------IfThenElseExp struct--------------------//

typedef	struct	myIfThenElseExp_
{
	myExp		exp1;
	myExp		exp2;
	myExp		exp3;
}*	myIfThenElseExp;

myIfThenElseExp makeMyIfThenElseExp(
	myExp exp1, myExp exp2, myExp exp3);

//----------------------IfThenExp struct--------------------------//

typedef	struct	myIfThenExp_
{
	myExp	exp1;
	myExp	exp2;
}*	myIfThenExp;

myIfThenExp makeMyIfThenExp(myExp exp1, myExp exp2);

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

myComparisonExp makeMyComparisonExp_(
	enum myComparisonOp	op,
	myExp			left,	
	myExp			right
	);
	
//	the following functions call the former function internally
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

//-------------------BooleanOperateExp struct--------------------//

typedef	struct	myBooleanOperateExp_
{
	enum myBooleanOperateOp { opBooleanAnd, opBooleanOr } op;
	myExp		left;
	myExp		right;
}*	myBooleanOperateExp;

myBooleanOperateExp makeMyBooleanOPerateExp_(
	enum myBooleanOperateOp	op,
	myExp				left,
	myExp				right
	);
	
//	the following functions call the former function internally
myBooleanOperateExp makeMyBooleanOperateExp_And(
	myExp left, myExp right);
myBooleanOperateExp makeMyBooleanOperateExp_Or(
	myExp left, myExp right);

//--------------------AssignmentExp struct----------------------//

typedef	struct	myAssignmentExp_
{
	myLValueExp	lValueExp;
	myExp		exp;
}*	myAssignmentExp;

myAssignmentExp makeMyAssignmentExp(
	myLValueExp lValueExp, myExp exp);

//--------------------LetExp struct--------------------------//

typedef	struct	myLetExp_
{
	myDecList		decList;//	may be null
	myExpList		expList;//	may be null
}*	myLetExp;

myLetExp makeMyLetExp(myDecList decList, myExpList expList);

//--------------------WhileExp struct------------------------//

typedef	struct	myWhileExp_
{
	myExp	whileExp;
	myExp	bodyExp;
}*	myWhileExp;

myWhileExp makeMyWhileExp(myExp whileExp, myExp bodyExp);

//--------------------NegationExp struct---------------------//

typedef	struct	myNegationExp_
{
	myExp		exp;
}*	myNegationExp;

myNegationExp makeMyNegationExp(myExp exp);

//---------------------BreakExp struct---------------------//

typedef	struct	myBreakExp_
{
	//	need nothing
}*	myBreakExp;

myBreakExp makeMyBreakExp();

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

myExp makeMyExp_(myPos pos, enum myExpKind kind, void* unionMember);

//	following functions call the former function internally
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
myExp makeMyExp_Comparison		(myPos, myComparisonExp exp);
myExp makeMyExp_BooleanOperate	(myPos pos, myBooleanOperateExp exp);
myExp makeMyExp_Assignment		(myPos pos, myAssignmentExp exp);
myExp makeMyExp_Let				(myPos pos, myLetExp exp);
myExp makeMyExp_While			(myPos pos, myWhileExp exp);
myExp makeMyExp_Negation		(myPos pos, myNegationExp exp);
myExp makeMyExp_Break			(myPos pos, myBreakExp exp);


typedef	struct	myExpList_
{
	myExp		exp;
	myExpList	next;
}*	myExpList;

myExpList makeMyExpList(myExp exp, myExpList list);

#endif
