#ifndef MYPRINTABSTRACTSYNTAX_H
#define MYPRINTABSTRACTSYNTAX_H

/**
 *	@defgroup	PrintAbstractSyntax	Abstract Syntax Printing
 *	@{
 */

/**
 *	@file	myPrintABstractSyntax.h
 *	@author	lgxZJ@outlook.com
 *	@date	5/24/2016
 *
 *	@brief	Module about expression printing.
 */


#include "myAbstractSyntax.h"
#include <stdio.h>

/**
 *	@brief	Print an expression.
 *
 *	@param[in]	out		The descriptor of file to print to.
 *	@param[in]	exp		The expression to print.
 *	@param[in]	indentSpaceNum	The number of indent spaces.
 *
 *	@return	This function has no return value.
 *	@sa		myAbstractSyntax.h
 */
void MyPrint_Exp(FILE* out, myExp exp, int indentSpaceNum);

/**	@}	*/



/*------------------function prototypes for tests-------------*/

/*
void MyPrint_TyFieldList(FILE* out, myTyFieldList tyFieldList, int indentSpaceNum);
void MyPrint_Ty(FILE* out, myTy ty, int indentSpaceNum);
void MyPrint_TypeDec(FILE* out, myTypeDec tyDec, int indentSpaceNum);
void MyPrint_LongFormVar(FILE* out, myLongFormVar var, int indentSpaceNum);
void MyPrint_ShortFormVar(FILE* out, myShortFormVar var, int indentSpaceNum);
void MyPrint_VarDec(FILE* out, myVarDec varDec, int indentSpaceNum);
void MyPrint_ProcedureDec(FILE* out, myProcedureDec dec, int indentSpaceNum);
void MyPrint_FunctionDec(FILE* out, myFunctionDec dec, int indentSpaceNum);
void MyPrint_FuncDec(FILE* out, myFuncDec funcDec, int indentSpaceNum);
void MyPrint_Dec(FILE* out, myDec dec, int indentSpaceNum);
void MyPrint_DecList(FILE* out, myDecList decList, int indentSpaceNum);
void MyPrint_LValueExp(FILE* out, myLValueExp lValueExp, int indentSpaceNum);
void MyPrint_NoParamFunctionCallExp(FILE* out, myNoParamFunctionCallExp exp, int indentSpaceNum);
void MyPrint_ParamFunctionCallExp(FILE* out, myParamFunctionCallExp exp, int indentSpaceNum);
void MyPrint_FunctionCallExp(FILE* out, myFunctionCallExp exp, int indentSpaceNum);
void MyPrint_NilExp(FILE* out, myNilExp notUsed, int indentSpaceNum);
void MyPrint_IntegerLiteralExp(FILE* out, myIntegerLiteralExp exp, int indentSpaceNum);
void MyPrint_StringLiteralExp(FILE* out, myStringLiteralExp exp, int indentSpaceNum);
void MyPrint_ArrayCreationExp(FILE* out, myArrayCreationExp exp, int indentSpaceNum);
void MyPrint_NoFieldRecordCreationExp(FILE* out, myNoFieldRecordCreationExp exp, int indentSpaceNum);
void MyPrint_FieldRecordCreationExp(FILE* out, myFieldRecordCreationExp exp, int indentSpaceNum);
void MyPrint_RecordCreationExp(FILE* out, myRecordCreationExp exp, int indentSpaceNum);
void MyPrint_ArithmeticExp(FILE* out, myArithmeticExp exp, int indentSpaceNum);
void MyPrint_ParenthesesExp(FILE* out, myParenthesesExp exp, int indentSpaceNum);
void MyPrint_NoValueExp(FILE* out, myNoValueExp notUsed, int indentSpaceNum);
void MyPrint_SequencingExp(FILE* out, mySequencingExp exp, int indentSpaceNum);
void MyPrint_ForExp(FILE* out, myForExp exp, int indentSpaceNum);
void MyPrint_IfThenElseExp(FILE* out, myIfThenElseExp exp, int indentSpaceNum);
void MyPrint_IfThenExp(FILE* out, myIfThenExp exp, int indentSpaceNum);
void MyPrint_ComparisonExp(FILE* out, myComparisonExp exp, int indentSpaceNum);
void MyPrint_BooleanOperateExp(FILE* out, myBooleanOperateExp exp, int indentSpaceNum);
void MyPrint_AssignmentExp(FILE* out, myAssignmentExp exp, int indentSpaceNum);
void MyPrint_LetExp(FILE* out, myLetExp exp, int indentSpaceNum);
void MyPrint_WhileExp(FILE* out, myWhileExp exp, int indentSpaceNum);
void MyPrint_NegationExp(FILE* out, myNegationExp notUsed, int indentSpaceNum);
void MyPrint_BreakExp(FILE* out, myBreakExp notUsed, int indentSpaceNum);
void MyPrint_ExpList(FILE* out, myExpList expList, int indentSpaceNum);
*/

#endif
