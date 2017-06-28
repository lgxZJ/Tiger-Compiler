#ifndef ABSTRACT_SYNTAX_H
#define ABSTRACT_SYNTAX_H

#include "myAbstractSyntax.h"

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


#endif