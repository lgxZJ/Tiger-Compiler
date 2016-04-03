#include "myPrintAbstractSyntax.h"


/*
 *	Print Module:	myPrintAbstractSyntax.c
 *		print abstract syntax data structures.
 */

//	todo:	add static back to some functions

//	todo:forward declarations

//	todo: change line

//	todo: static keyward

//	todo: refactor printSymbol(mySymbol)

/*	forward declarations	*/
void MyPrint_ExpList(FILE* out, myExpList expList, int indentSpaceNum);

/*====================================================================================*/

/*	make indent into output file (used internally)	*/
static void indent(FILE* out, int numOfSpace)
{
	for  (unsigned i = 0; i < numOfSpace; ++i)
		fputc(' ', out);
}

/*====================================================================================*/

void MyPrint_TyFieldList(FILE* out, myTyFieldList tyFieldList, int indentSpaceNum)
{
    while (tyFieldList)
    {
	//indent(out, indentSpaceNum);
	fprintf(out, "%s : %s, ",
		MySymbol_GetName(tyFieldList->field->varName),
		MySymbol_GetName(tyFieldList->field->typeName)
	);

	tyFieldList = tyFieldList->next;
    };
}

void MyPrint_Ty(FILE* out, myTy ty, int indentSpaceNum)
{
	if (ty == NULL)		return;
	
	switch (ty->kind)
	{
	case NamedType:
		fprintf(out, "%s,\n", MySymbol_GetName(ty->u.typeName));
		break;
	case RecordType:
		MyPrint_TyFieldList(out, ty->u.tyFieldList, indentSpaceNum);
		fprintf(out, "\n");
		break;
	case ArrayType:
		fprintf(out, "%s,\n", MySymbol_GetName(ty->u.arrayTypeName));
		break;
	}
}

void MyPrint_TypeDec(FILE* out, myTypeDec tyDec, int indentSpaceNum)
{
	if (tyDec == NULL)		return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myTypeDec(");
	
	switch (tyDec->type->kind)
	{
	case NamedType:
	    fprintf(out, "myNamedType(\n");	break;
	case RecordType:
	    fprintf(out, "myRecordType(\n");	break;
	case ArrayType:
	    fprintf(out, "myArrayType(\n");	break;
	}

	indent(out, indentSpaceNum + 2);
	fprintf(out, "%s,\n", MySymbol_GetName(tyDec->name));

	indent(out, indentSpaceNum + 2);
	MyPrint_Ty(out, tyDec->type, indentSpaceNum + 2);
	
	indent(out, indentSpaceNum);
	fprintf(out, "))");
}

	/*---------------------------------------*/
	
void MyPrint_LongFormVar(FILE* out, myLongFormVar var, int indentSpaceNum)
{
	if (var == NULL)		return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myLongFormVar(\n");
	
	indent(out, indentSpaceNum + 2);
	fprintf(out, "%s,\n", MySymbol_GetName(var->name));
	
	indent(out, indentSpaceNum + 2);
	fprintf(out, "%s,\n", MySymbol_GetName(var->type));
	
	MyPrint_Exp(out, var->exp, indentSpaceNum + 2);

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

void MyPrint_ShortFormVar(FILE* out, myShortFormVar var, int indentSpaceNum)
{
	if (var == NULL)		return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myShortFormVar(\n");
	
	indent(out, indentSpaceNum + 2);
	fprintf(out, "%s,\n", MySymbol_GetName(var->name));
	
	MyPrint_Exp(out, var->exp, indentSpaceNum + 2);

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}
	
void MyPrint_VarDec(FILE* out, myVarDec varDec, int indentSpaceNum)
{
	if (varDec == NULL)		return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myVarDec(\n");
	
	switch (varDec->kind)
	{
	case LongFormVar:
		MyPrint_LongFormVar(out, varDec->u.longFormVar, indentSpaceNum + 2);
		break;
	case ShortFormVar:
		MyPrint_ShortFormVar(out, varDec->u.shortFormVar, indentSpaceNum + 2);
		break;
	}

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

	/*---------------------------------------*/

void MyPrint_ProcedureDec(FILE* out, myProcedureDec dec, int indentSpaceNum)
{
	if (dec == NULL)		return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myProcedureDec(\n");
	
	//	print procedure name
	indent(out, indentSpaceNum + 2);
	fprintf(out, "%s,\n", MySymbol_GetName(dec->name));
	
	//	print procedure parameters
	indent(out, indentSpaceNum + 2);
	MyPrint_TyFieldList(out, dec->tyFieldList, indentSpaceNum + 2);
	
	//	print procedure body
	fprintf(out, "\n");
	MyPrint_Exp(out, dec->exp, indentSpaceNum + 2);

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}
	
void MyPrint_FunctionDec(FILE* out, myFunctionDec dec, int indentSpaceNum)
{
	if (dec == NULL)	return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myFunctionDec(\n");
	
	//	print function name
	indent(out, indentSpaceNum + 2);
	fprintf(out, "%s,\n", MySymbol_GetName(dec->name));
	
	indent(out, indentSpaceNum + 2);
	MyPrint_TyFieldList(out, dec->tyFieldList, indentSpaceNum + 2);
	fprintf(out, "\n");

	//	print function parameters
	indent(out, indentSpaceNum + 2);
	fprintf(out, "%s,\n", MySymbol_GetName(dec->returnType));
	
	//	print function body
	MyPrint_Exp(out, dec->exp, indentSpaceNum + 2);

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}
	
void MyPrint_FuncDec(FILE* out, myFuncDec funcDec, int indentSpaceNum)
{
	if (funcDec == NULL)	return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myFuncDec(\n");
	
	switch (funcDec->kind)
	{
	case ProcedureDec:
		MyPrint_ProcedureDec(out, funcDec->u.procedureDec, indentSpaceNum + 2);
		break;
	case FunctionDec:
		MyPrint_FunctionDec(out, funcDec->u.functionDec, indentSpaceNum + 2);
		break;
	}

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}
	
	/*-------------------------------------*/
	
void MyPrint_Dec(FILE* out, myDec dec, int indentSpaceNum)
{
	if (dec == NULL)		return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myDec(\n");
	
	switch (dec->kind)
	{
	case TypeDec:	MyPrint_TypeDec(out, dec->u.tyDec, indentSpaceNum + 2);	break;
	case VarDec:	MyPrint_VarDec(out, dec->u.varDec, indentSpaceNum + 2);	break;
	case FuncDec:	MyPrint_FuncDec(out, dec->u.funcDec,indentSpaceNum + 2);break;
	}

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

void MyPrint_DecList(FILE* out, myDecList decList, int indentSpaceNum)
{
	if (decList == NULL)	return;
	
	while (decList)
	{
		MyPrint_Dec(out, decList->dec, indentSpaceNum);
		fprintf(out, ",\n");
		decList = decList->next;
	}
}

/*=====================================================================================*/

static void MyPrint_SimpleVar(FILE* out, mySymbol id,
		       mySimpleVarAux simpleVarAux, int indentSpaceNum)
{
    if (invalidSymbol(id) || simpleVarAux == (mySimpleVarAux)NULL)
	return;
	
    indent(out, indentSpaceNum);
    fprintf(out, "mySimpleVar(%s)", MySymbol_GetName(id));
}

static void MyPrint_RecordField(FILE* out, mySymbol id,
				myRecordFieldAux recordFieldAux, int indentSpaceNum)
{
    if (invalidSymbol(id) || recordFieldAux == (myRecordFieldAux)NULL)
	return;
	
    indent(out, indentSpaceNum);
    fprintf(out, "myRecordField(\n");

    /*	print 1th member	*/
    indent(out, indentSpaceNum + 2);
    fprintf(out, "%s", MySymbol_GetName(id));
	
    /*	print remaining members	*/
    fprintf(out, ",\n");
    indent(out, indentSpaceNum + 2);
    fprintf(out, "%s", MySymbol_GetName(recordFieldAux->id));

    myLValueAux aux = recordFieldAux->next;
    while (aux)
    {
	fprintf(out, ",\n");

	//	if record field
	if (aux->exp == NULL)
	{
	    indent(out, indentSpaceNum + 2);
	    fprintf(out, "%s", MySymbol_GetName(aux->id));
	}
	else//	if array subscript
	{
	    MyPrint_Exp(out, aux->exp, indentSpaceNum + 2);
	}
		
	aux = aux->next;
    }

    fprintf(out, "\n");
    indent(out, indentSpaceNum);
    fprintf(out, ")");
}

static void MyPrint_ArraySubscript(FILE* out, mySymbol id,
				   myArraySubscriptAux arraySubscriptAux, int indentSpaceNum)
{
    if (invalidSymbol(id) || arraySubscriptAux == (myArraySubscriptAux)NULL)
	return;
	
    indent(out, indentSpaceNum);
    fprintf(out, "myArraySubscript(\n");

    /*	print 1th member	*/
    indent(out, indentSpaceNum + 2);
    fprintf(out, "%s", MySymbol_GetName(id));
	
    /*	print remaining members	*/
    fprintf(out, ",\n");
    MyPrint_Exp(out, arraySubscriptAux->exp, indentSpaceNum + 2);

    myLValueAux aux = arraySubscriptAux->next;
    while (aux)
    {
	fprintf(out, ",\n");

	//	if array subscript
	if (aux->id == NULL)
	{
	    MyPrint_Exp(out, aux->exp, indentSpaceNum + 2);
	}
	else//	if record field
	{
	    indent(out, indentSpaceNum + 2);
	    fprintf(out, "%s", MySymbol_GetName(aux->id));
	}

        aux = aux->next;
    }

    fprintf(out, "\n");
    indent(out, indentSpaceNum);
    fprintf(out, ")");
}

void MyPrint_LValueExp(FILE* out, myLValueExp lValueExp, int indentSpaceNum)
{
	if (lValueExp == NULL)	return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myLValueExp(\n");
	
	switch (lValueExp->kind)
	{
	case SimpleVar:
		MyPrint_SimpleVar(out, lValueExp->id, lValueExp->u.simpleVarAux, indentSpaceNum + 2);
		break;
	case RecordField:
		MyPrint_RecordField(out, lValueExp->id, lValueExp->u.recordFieldAux, indentSpaceNum + 2);
		break;
	case ArraySubscript:
		MyPrint_ArraySubscript(out, lValueExp->id, lValueExp->u.arraySubscriptAux, indentSpaceNum + 2);
		break;
	}

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

/*=====================================================================================*/

void MyPrint_NoParamFunctionCallExp(FILE* out, myNoParamFunctionCallExp exp, int indentSpaceNum)
{
	if (exp == NULL)	return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myNoParamFunctionCallExp(%s)", MySymbol_GetName(exp->name));
}

void MyPrint_ParamFunctionCallExp(FILE* out, myParamFunctionCallExp exp, int indentSpaceNum)
{
	if (exp == NULL)				return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myParamFunctionCallExp(\n");
	
	/*	print 1th member	*/
	indent(out, indentSpaceNum + 2);
	fprintf(out, "%s,\n", MySymbol_GetName(exp->name));
	/*	print 2th member	*/
	MyPrint_ExpList(out, exp->expList, indentSpaceNum + 2);

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
	
}

void MyPrint_FunctionCallExp(FILE* out, myFunctionCallExp exp, int indentSpaceNum)
{
	if (exp == NULL)			return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myFunctionCallExp(\n");
	
	switch (exp->kind)
	{
	case NoParamFunctionCallExp:
		MyPrint_NoParamFunctionCallExp(out, exp->u.noParamFunctionCallExp, indentSpaceNum + 2);
		break;
	case ParamFunctionCallExp:
		MyPrint_ParamFunctionCallExp(out, exp->u.paramFunctionCallExp, indentSpaceNum + 2);
		break;
	}

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

/*=======================================================================================*/

/*	no need of exp, but we have to declare it here for language reason	*/
void MyPrint_NilExp(FILE* out, myNilExp notUsed, int indentSpaceNum)
{
	indent(out, indentSpaceNum);
	fprintf(out, "myNilExp()");
}

/*======================================================================================*/

void MyPrint_IntegerLiteralExp(FILE* out, myIntegerLiteralExp exp, int indentSpaceNum)
{
	if (exp == NULL)		return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myIntegerLiteralExp(%i)", exp->value);
}

/*======================================================================================*/

void MyPrint_StringLiteralExp(FILE* out, myStringLiteralExp exp, int indentSpaceNum)
{
	if (exp == NULL)		return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myStringLiteralExp(%s)", exp->str);
}

/*=====================================================================================*/

void MyPrint_ArrayCreationExp(FILE* out, myArrayCreationExp exp, int indentSpaceNum)
{
	if (exp == NULL)		return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myArrayCreationExp(\n");
	
	/*	print 1th member	*/
	indent(out, indentSpaceNum + 2);
	fprintf(out, "%s,\n", MySymbol_GetName(exp->typeName));
	/*	print 2th member	*/
	MyPrint_Exp(out, exp->length, indentSpaceNum + 2);
	fprintf(out, ",\n");
	/*	print 3th member	*/
	MyPrint_Exp(out, exp->initial, indentSpaceNum + 2);

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

/*=====================================================================================*/

void MyPrint_NoFieldRecordCreationExp(FILE* out, myNoFieldRecordCreationExp exp, int indentSpaceNum)
{
	if (exp == NULL)		return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myNoFieldRecorCreationExp(%s)", MySymbol_GetName(exp->typeName));
}

/*	print record field, so not increment indent number	*/
void myPrint_RecordField(FILE* out, myRecordField field, int indentSpaceNum)
{
	if (field == NULL)		return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "%s,\n", MySymbol_GetName(field->varName));
	
	MyPrint_Exp(out, field->varValue, indentSpaceNum);
}

void MyPrint_FieldRecordCreationExp(FILE* out, myFieldRecordCreationExp exp, int indentSpaceNum)
{
	if (exp == NULL)		return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myFieldRecordCreationExp(\n");
	
	indent(out, indentSpaceNum + 2);
	fprintf(out, "%s,\n", MySymbol_GetName(exp->typeName));
	
	myRecordFieldList fieldList = exp->fieldList;
	while (fieldList)
	{
	    //	todo:
	    indent(out, indentSpaceNum + 4);
	    fprintf(out, "%s,\n", MySymbol_GetName(fieldList->field->varName));
	    MyPrint_Exp(out, fieldList->field->varValue, indentSpaceNum + 4);
	    fprintf(out, "\n");

	    fieldList = fieldList->next;
	}

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

void MyPrint_RecordCreationExp(FILE* out, myRecordCreationExp exp, int indentSpaceNum)
{
	if (exp == NULL)		return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myRecordCreationExp(\n");
	
	switch (exp->kind)
	{
	case NoFieldRecordCreationExp:
		MyPrint_NoFieldRecordCreationExp(out, exp->u.noFieldRecordCreationExp, indentSpaceNum + 2);
		break;
	case FieldRecordCreationExp:
		MyPrint_FieldRecordCreationExp(out, exp->u.fieldRecordCreationExp, indentSpaceNum + 2);
		break;
	}

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

/*======================================================================================*/

void MyPrint_ArithmeticExp(FILE* out, myArithmeticExp exp, int indentSpaceNum)
{
	if (exp == NULL)		return;
	
	char*	opStr[] = {"opPlus", "opMinus", "opMultiply", "opDivide"};
	
	indent(out, indentSpaceNum);
	fprintf(out, "myArithmeticExp(\n");
	
	MyPrint_Exp(out, exp->left, indentSpaceNum + 2);
	fprintf(out, ",\n");
	
	indent(out, indentSpaceNum + 2);
	fprintf(out, "%s,\n", opStr[exp->op]);
	
	MyPrint_Exp(out, exp->right, indentSpaceNum + 2);

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

/*======================================================================================*/

void MyPrint_ParenthesesExp(FILE* out, myParenthesesExp exp, int indentSpaceNum)
{
	if (exp == NULL)		return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myParenthesesExp(\n");
	
	MyPrint_Exp(out, exp->exp, indentSpaceNum + 2);

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

/*=====================================================================================*/

/*	no need of exp, but c do not support anonymous parameters	*/
void MyPrint_NoValueExp(FILE* out, myNoValueExp notUsed, int indentSpaceNum)
{
	indent(out, indentSpaceNum);
	fprintf(out, "myNoValueExp()");
}

/*=====================================================================================*/

void MyPrint_SequencingExp(FILE* out, mySequencingExp exp, int indentSpaceNum)
{
	if (exp == NULL)		return;
	
	indent(out, indentSpaceNum + 2);
	fprintf(out, "mySequencingExp(\n");
	
	MyPrint_Exp(out, exp->exp1, indentSpaceNum + 2);
	fprintf(out, ",\n");

	MyPrint_Exp(out, exp->exp2, indentSpaceNum + 2);
	fprintf(out, ",\n");

	MyPrint_ExpList(out, exp->nextList, indentSpaceNum + 2);

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

/*======================================================================================*/

void MyPrint_ForExp(FILE* out, myForExp exp, int indentSpaceNum)
{
	if (exp == NULL)		return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myForExp(\n");
	
	indent(out, indentSpaceNum + 2);
	fprintf(out, "%s,\n", MySymbol_GetName(exp->varName));
	
	MyPrint_Exp(out, exp->varRangeLow, indentSpaceNum + 2);
	fprintf(out, ",\n");

	MyPrint_Exp(out, exp->varRangeHigh, indentSpaceNum + 2);
	fprintf(out, ",\n");

	MyPrint_Exp(out, exp->bodyExp, indentSpaceNum + 2);

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

/*======================================================================================*/

void MyPrint_IfThenElseExp(FILE* out, myIfThenElseExp exp, int indentSpaceNum)
{
	if (exp == NULL)	return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myIfThenElseExp(\n");
	
	MyPrint_Exp(out, exp->exp1, indentSpaceNum + 2);
	fprintf(out, ",\n");

	MyPrint_Exp(out, exp->exp2, indentSpaceNum + 2);
	fprintf(out, ",\n");

	MyPrint_Exp(out, exp->exp3, indentSpaceNum + 2);

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

/*======================================================================================*/

void MyPrint_IfThenExp(FILE* out, myIfThenExp exp, int indentSpaceNum)
{
	if (exp == NULL)		return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myIfThenExp(\n");
	
	MyPrint_Exp(out, exp->exp1, indentSpaceNum + 2);
	fprintf(out, ",\n");

	MyPrint_Exp(out, exp->exp2, indentSpaceNum + 2);

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

/*======================================================================================*/

void MyPrint_ComparisonExp(FILE* out, myComparisonExp exp, int indentSpaceNum)
{
	if (exp == NULL)	return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myComparisonExp(\n");
	
	char*	opStr[] = {
		"opEqual", "opNotEqual", "opLargerThan", "opSmallerThan",
		"opLargerEqualThan", "opSmallerEqualThan"
	};
	
	MyPrint_Exp(out, exp->left, indentSpaceNum + 2);
	fprintf(out, ",\n");

	indent(out, indentSpaceNum + 2);
	fprintf(out, "%s,\n", opStr[exp->op]);
	
	MyPrint_Exp(out, exp->right, indentSpaceNum + 2);

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

/*======================================================================================*/

void MyPrint_BooleanOperateExp(FILE* out, myBooleanOperateExp exp, int indentSpaceNum)
{
	if (exp == NULL)	return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myBooleanOperateExp(\n");
	
	char*	opStr[] = { "opBooleanAnd", "opBooleanOr" };
	MyPrint_Exp(out, exp->left, indentSpaceNum + 2);
	fprintf(out, ",\n");
	
	indent(out, indentSpaceNum + 2);
	fprintf(out, "%s,\n", opStr[exp->op]);
	
	MyPrint_Exp(out, exp->right, indentSpaceNum + 2);

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

/*======================================================================================*/

void MyPrint_AssignmentExp(FILE* out, myAssignmentExp exp, int indentSpaceNum)
{
	if (exp == NULL)		return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myAssignmentExp(\n");
	
	MyPrint_LValueExp(out, exp->lValueExp, indentSpaceNum + 2);
	fprintf(out, ",\n");

	MyPrint_Exp(out, exp->exp, indentSpaceNum + 2);

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

/*======================================================================================*/

void MyPrint_LetExp(FILE* out, myLetExp exp, int indentSpaceNum)
{
	if (exp == NULL)		return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myLetExp(\n");
	
	MyPrint_DecList(out, exp->decList, indentSpaceNum + 2);
	fprintf(out, ",\n");
	MyPrint_ExpList(out, exp->expList, indentSpaceNum + 2);

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

/*======================================================================================*/

void MyPrint_WhileExp(FILE* out, myWhileExp exp, int indentSpaceNum)
{
	if (exp == NULL)	return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myWhileExp(\n");
	
	MyPrint_Exp(out, exp->whileExp, indentSpaceNum + 2);
	fprintf(out, ",\n");

	MyPrint_Exp(out, exp->bodyExp, indentSpaceNum + 2);

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

/*======================================================================================*/

/*	no need of exp, but c does not support anonymous parameters	*/
void MyPrint_NegationExp(FILE* out, myNegationExp negationExp, int indentSpaceNum)
{
	//if (exp == NULL)		return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myNegationExp(\n");

	MyPrint_Exp(out, negationExp->exp, indentSpaceNum + 2);

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

/*======================================================================================*/

/*	no need of exp, but for language reason we need to declare it	*/
void MyPrint_BreakExp(FILE* out, myBreakExp notUsed, int indentSpaceNum)
{
	//if (exp == NULL)		return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myBreakExp()");
}

/*======================================================================================*/

/*	This function checks the validity of `out`	*/
void MyPrint_Exp(FILE* out, myExp exp, int indentSpaceNum)
{
	if (out == NULL || exp == NULL)	return;
	
	indent(out, indentSpaceNum);
	fprintf(out, "myExp(");
	
	switch (exp->kind)
	{
	case LValueExp:
	    MyPrint_LValueExp(out, exp->u.lValueExp, indentSpaceNum + 2);
		break;
	case FunctionCallExp:	
	    MyPrint_FunctionCallExp(out, exp->u.functionCallExp, indentSpaceNum + 2);
		break;
	case NilExp:			
	    MyPrint_NilExp(out, exp->u.nilExp, indentSpaceNum + 2);
		break;
	case IntegerLiteralExp:	
		MyPrint_IntegerLiteralExp(out, exp->u.integerLiteralExp, indentSpaceNum + 2);
		break;
	case StringLiteralExp:	
	    MyPrint_StringLiteralExp(out, exp->u.stringLiteralExp, indentSpaceNum + 2);
		break;
	case ArrayCreationExp:	
	    MyPrint_ArrayCreationExp(out, exp->u.arrayCreationExp, indentSpaceNum + 2);
		break;
	case RecordCreationExp:	
	    MyPrint_RecordCreationExp(out, exp->u.recordCreationExp, indentSpaceNum + 2);
		break;
	case ArithmeticExp:		
	    MyPrint_ArithmeticExp(out, exp->u.arithmeticExp, indentSpaceNum + 2);
		break;
	case ParenthesesExp:	
	    MyPrint_ParenthesesExp(out, exp->u.parenthesesExp, indentSpaceNum + 2);
		break;
	case NoValueExp:		
	    MyPrint_NoValueExp(out, exp->u.noValueExp, indentSpaceNum + 2);
		break;
	case SequencingExp:		
	    MyPrint_SequencingExp(out, exp->u.sequencingExp, indentSpaceNum + 2);
		break;
	case ForExp:			
	    MyPrint_ForExp(out, exp->u.forExp, indentSpaceNum + 2);
		break;
	case IfThenElseExp:		
	    MyPrint_IfThenElseExp(out, exp->u.ifThenElseExp, indentSpaceNum + 2);
		break;
	case IfThenExp:			
	    MyPrint_IfThenExp(out, exp->u.ifThenExp, indentSpaceNum + 2);
		break;
	case ComparisonExp:		
	    MyPrint_ComparisonExp(out, exp->u.comparisonExp, indentSpaceNum + 2);
		break;
	case BooleanOperateExp:	
	    MyPrint_BooleanOperateExp(out, exp->u.booleanOperateExp, indentSpaceNum + 2);
		break;
	case AssignmentExp:		
	    MyPrint_AssignmentExp(out, exp->u.assignmentExp, indentSpaceNum + 2);
		break;
	case LetExp:			
	    MyPrint_LetExp(out, exp->u.letExp, indentSpaceNum + 2);
		break;
	case WhileExp:			
	    MyPrint_WhileExp(out, exp->u.whileExp, indentSpaceNum + 2);
		break;
	case NegationExp:		
	    MyPrint_NegationExp(out, exp->u.negationExp, indentSpaceNum + 2);
		break;
	case BreakExp:			
	    MyPrint_BreakExp(out, exp->u.breakExp, indentSpaceNum + 2);
		break;
	}

	fprintf(out, "\n");
	indent(out, indentSpaceNum);
	fprintf(out, ")");
}

void MyPrint_ExpList(FILE* out, myExpList expList, int indentSpaceNum)
{
	if (expList == NULL)	return;
	
	//indent(out, indentSpaceNum);
	MyPrint_Exp(out, expList->exp, indentSpaceNum);
	fprintf(out, ",\n");
	
	MyPrint_ExpList(out, expList->next, indentSpaceNum);
}

/*==============================END OF FILE===================================*/
