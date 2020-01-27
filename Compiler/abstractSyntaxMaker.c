#include "abstractSyntaxMaker.h"

#include <assert.h>

///////////////////////////////////////////////////////
//	`make` functions for all structures
///////////////////////////////////////////////////////

#define NULL	0

/*------------------------------------------------*/

/*
 *	NOTE:
 *	Because any symbol be of NULL or "" is not a valid symbol
 *	and makes no meanings when making structures, so check this
 *	circustoms first.
 *	(Used internally)
 */


//-----------------TyFields struct----------------//

myTyField makeMyTyField(mySymbol varName, mySymbol typeName)
{
    if (MySymbol_InvalidSymbol(varName) || MySymbol_InvalidSymbol(typeName))
	return (myTyField)NULL;

    myTyField	one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
    one->varName = varName;
	one->varEscape = false;
    one->typeName = typeName;
    return one;
}

	////////////////////////////////////////

myTyFieldList makeMyTyFieldList(myTyField field, myTyFieldList list)
{
	myTyFieldList	one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
	one->field = field;
	one->next = list;
	return one;
}

//--------------------Ty struct------------------------//

/*	member can be NULL, skip checking	*/
myTy makeMyTy_(myPos pos, enum myTyKind kind, void* member)
{
	myTy	one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
	one->pos = pos;
	one->kind = kind;
	
	switch (kind)
	{
	case NamedType:
		one->u.typeName = (mySymbol)member;			break;
	case RecordType:
		one->u.tyFieldList = (myTyFieldList)member;	break;
	case ArrayType:
		one->u.arrayTypeName =(mySymbol)member;			break;
	}
	return one;
}

//	function delegations
inline myTy makeMyTy_Named(myPos pos, mySymbol typeName)
{
    if (MySymbol_InvalidSymbol(typeName))	return (myTy)NULL;
    return makeMyTy_(pos, NamedType, (void*)typeName);
}

inline myTy makeMyTy_Record(myPos pos, myTyFieldList tyFieldList)
{
	return makeMyTy_(pos, RecordType, (void*)tyFieldList);
}

inline myTy makeMyTy_Array(myPos pos, mySymbol arrayTypeName)
{
    if (MySymbol_InvalidSymbol(arrayTypeName))	return (myTy)NULL;
    return makeMyTy_(pos, ArrayType, (void*)arrayTypeName);
}

	///////////////////////////////////

myTyDec makeMyTyDec(mySymbol name, myTy type)
{
    if (MySymbol_InvalidSymbol(name))	return (myTyDec)NULL;

    myTyDec	one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
    one->name = name;
    one->type = type;
    return one;
}

//----------------------Var struct--------------------//

myLongFormVar makeMyLongFormVar(mySymbol name, mySymbol type, myExp exp)
{
    if (MySymbol_InvalidSymbol(name) || MySymbol_InvalidSymbol(type))
	return (myLongFormVar)NULL;

    myLongFormVar	one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
    one->name = name;
    one->type = type;
    one->exp = exp;
	one->escape = false; //	default value
    return one;
}

	///////////////////////////////////
	
myShortFormVar makeMyShortFormVar(mySymbol name, myExp exp)
{
    if (MySymbol_InvalidSymbol(name))	return (myShortFormVar)NULL;

    myShortFormVar	one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
    one->name = name;
    one->exp = exp;
	one->escape = false;	//	default value
    return one;
}

	////////////////////////////////////

myVarDec makeMyVarDec_(enum myVarDecKind kind, void* member)
{
	myVarDec	one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
	one->kind = kind;
	switch (kind)
	{
	case LongFormVar:
		one->u.longFormVar = (myLongFormVar)member;		break;
	case ShortFormVar:
		one->u.shortFormVar = (myShortFormVar)member;	break;
	}
	
	return one;
}

//	function delegations

inline myVarDec makeMyVarDec_LongForm(myLongFormVar var)
{
	return makeMyVarDec_(LongFormVar, (void*)var);
}

inline myVarDec makeMyVarDec_ShortForm(myShortFormVar var)
{
	return makeMyVarDec_(ShortFormVar, (void*)var);
}

//--------------------FuncDec struct--------------------//

myProcedureDec makeMyProcedureDec(
    mySymbol name, myTyFieldList tyFieldList, myExp exp)
{
    if (MySymbol_InvalidSymbol(name))	return (myProcedureDec)NULL;

    myProcedureDec one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
    one->name = name;
    one->tyFieldList = tyFieldList;
    one->exp = exp;
    return one;
}

	//////////////////////////////
	
myFunctionDec makeMyFunctionDec(
    mySymbol name, myTyFieldList tyFieldList,
    mySymbol returnType, myExp exp)
{
    if (MySymbol_InvalidSymbol(name) || MySymbol_InvalidSymbol(returnType))
	return (myFunctionDec)NULL;

    myFunctionDec one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
    one->name = name;
    one->tyFieldList = tyFieldList;
    one->returnType = returnType;
    one->exp = exp;
    return one;
}

	///////////////////////////////
	
myFuncDec makeMyFuncDec_(enum myFuncDecKind kind, void* member)
{
	myFuncDec one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
	one->kind = kind;
	switch (kind)
	{
	case ProcedureDec:
		one->u.procedureDec = (myProcedureDec)member;	break;
	case FunctionDec:
		one->u.functionDec = (myFunctionDec)member;		break;
	}
	
	return one;
}

//	function delegations
inline myFuncDec makeMyFuncDec_Procedure(myProcedureDec dec)
{
	return makeMyFuncDec_(ProcedureDec, (void*)dec);
}

inline myFuncDec makeMyFuncDec_Function(myFunctionDec dec)
{
	return makeMyFuncDec_(FunctionDec, (void*)dec);
}

//---------------------Dec struct----------------------//

myDec makeMyDec_(myPos pos, enum myDecKind kind, void* member)
{
	myDec one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
	one->pos = pos;
	one->kind = kind;
	
	switch (kind)
	{
	case TypeDec:
		one->u.tyDec = (myTyDec)member;		break;
	case VarDec:
		one->u.varDec = (myVarDec)member;	break;
	case FuncDec:
		one->u.funcDec = (myFuncDec)member;	break;

	default:
	        assert("unsupported kind of myDecKind" && false);
	}
	
	return one;
}

//	function delegations
inline myDec makeMyDec_Type(myPos pos, myTypeDec dec)
{
	return makeMyDec_(pos, TypeDec, (void*)dec);
}

inline myDec makeMyDec_Var(myPos pos, myVarDec dec)
{
	return makeMyDec_(pos, VarDec, (void*)dec);
}

inline myDec makeMyDec_Func(myPos pos, myFuncDec dec)
{
	return makeMyDec_(pos, FuncDec, (void*)dec);
}

	/////////////////////////////////
	
myDecList makeMyDecList(myDec dec, myDecList list)
{
	myDecList one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
	one->dec = dec;
	one->next = list;
	return one;
}

//---------------------LValueExp struct------------------//

/*	NULL symbol is allowed here, but symbol made by "" string is not correct 	*/
myLValueAux makeMyLValueAux(mySymbol id, myExp exp, myLValueAux next)
{
    if (id != (mySymbol)NULL && (*MySymbol_GetName(id) == '\0'))
	return (myLValueAux)NULL;

    myLValueAux one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
    one->id = id;
    one->exp = exp;
    one->next = next;
	
    return one;
}

/*
mySymbol MyLValueAux_GetId(myLValueAux aux)
{
	if (aux == NULL) return NULL;
	return aux->id;
}

myExp MyLValueAux_GetExp(myLValueAux aux)
{
	if (aux == NULL) return NULL;
	return aux->exp;
}

myLValueAux MyLValueAux_GetNext(myLValueAux aux)
{
	if (aux == NULL) return NULL;
	return aux->next;
}
*/


	///////////////////////////////
	///////////////////////////////

/*	used internally	*/
static mySimpleVarAux makeMySimpleVarAux()
{
    return makeMemoryBlock(sizeof(mySimpleVarAux), MEMORY_TYPE_NONE);
}

	///////////////////////////////

/*	used internally	*/
static myRecordFieldAux makeMyRecordFieldAux(mySymbol id, myLValueAux next)
{
    if (MySymbol_InvalidSymbol(id))	return (myRecordFieldAux)NULL;

    myRecordFieldAux one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
    one->id = id;
    one->next = next;
    return one;
}

	//////////////////////////////
	
/*	used internally	*/
static myArraySubscriptAux makeMyArraySubsrciptAux(myExp exp, myLValueAux next)
{
	myArraySubscriptAux one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
	one->exp = exp;
	one->next = next;
	return one;
}

	/////////////////////////////
	
enum myLValueKind getLValueKind(myLValueAux aux)
{
	if (aux == (myLValueAux)NULL)
		return SimpleVar;
	if (aux->id == (mySymbol)NULL)
		return ArraySubscript;
	if (aux->exp == (myExp)NULL)
		return RecordField;
	//	never goes here
	assert(false);
}

	/////////////////////////////
	
myLValueExp makeMyLValue(myPos pos, mySymbol id, myLValueAux aux)
{
    //	no need to check aux, because a null aux means simple l-value
    if (MySymbol_InvalidSymbol(id))	return (myLValueExp)NULL;
	
    myLValueExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
    one->pos = pos;
    one->id = id;
    one->kind = getLValueKind(aux);
	
    switch (one->kind)
    {
    case SimpleVar:
	one->u.simpleVarAux = makeMySimpleVarAux();
	break;
    case RecordField:
	one->u.recordFieldAux = makeMyRecordFieldAux(aux->id, aux->next);
	break;
    case ArraySubscript:
	one->u.arraySubscriptAux = makeMyArraySubsrciptAux(aux->exp, aux->next);
	break;
    }
	
    return one;
}

/*
//	function delegations
inline myLValue makeMyLValue_SimpleVar(myPos pos, mySymbol id, mySimplerVar simpleVar)
{
	return makeMyLVaue_(pos, id, SimpleVar, (void*)simpleVar);
}

inline myLValue makeMyLValue_RecordField(myPos pos, mySymbol id, myRecordField recordField)
{
	return makeMyLVaue_(pos, id, RecordField, (void*)recordField);
}

inline myLValue makeMyLValue_ArraySubscript(
	myPos pos,  mySymbol id, myArraySubscript arraySubscript)
{
	return makeMyLVaue_(pos, id, ArraySubscript, (void*)arraySubscript);	
}
*/
//-----------------------FunctionCallExp struct--------------------//

myNoParamFunctionCallExp makeMyNoParamFunctionCallExp(mySymbol name)
{
    if (MySymbol_InvalidSymbol(name))	return NULL;

    myNoParamFunctionCallExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
    one->name = name;
    return one;
}

myParamFunctionCallExp makeMyParamFunctionCallExp(mySymbol name, myExpList expList)
{
    if (MySymbol_InvalidSymbol(name))	return NULL;

    myParamFunctionCallExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);

    one->name = name;
    one->expList = expList;

    return one;
}


static myFunctionCallExp makeMyFunctionCallExp_(
	enum myFunctionCallExpKind kind, void* member)
{
	myFunctionCallExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
	one->kind = kind;
	switch (kind)
	{
	case NoParamFunctionCallExp:
		one->u.noParamFunctionCallExp = (myNoParamFunctionCallExp)member;
		break;
	case ParamFunctionCallExp:
		one->u.paramFunctionCallExp = (myParamFunctionCallExp)member;
		break;
	}
	
	return one;
}

//	function delegations
myFunctionCallExp makeMyFunctionCallExp_NoParam(myNoParamFunctionCallExp exp)
{
	return makeMyFunctionCallExp_(NoParamFunctionCallExp, (void*)exp);
}

myFunctionCallExp makeMyFunctionCallExp_Param(myParamFunctionCallExp exp)
{
	return makeMyFunctionCallExp_(ParamFunctionCallExp, (void*)exp);
}

//----------------------NilExp struct-----------------------//


myNilExp makeMyNilExp()
{
    return makeMemoryBlock(sizeof(myNilExp), MEMORY_TYPE_NONE);
}

//-------------------IntegerLiteralExp struct------------------//

myIntegerLiteralExp makeMyIntegerLiteralExp(int value)
{
	myIntegerLiteralExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	one->value = value;
	return one;
}


//------------------StringLiteralExp struct-------------------//

myStringLiteralExp makeMyStringLiteralExp(myString str)
{
    if (str == (myString)NULL)	return (myStringLiteralExp)NULL;

    myStringLiteralExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
    one->str = str;
    return one;
}

//-------------------ArrayCreationExp struct--------------------//

myArrayCreationExp makeMyArrayCreationExp(
    mySymbol typeName, myExp length, myExp initial)
{
    if (MySymbol_InvalidSymbol(typeName))
	return (myArrayCreationExp)NULL;

    myArrayCreationExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);

    one->typeName = typeName;
    one->length = length;
    one->initial = initial;
	
    return one;
}

//------------------RecordField struct---------------------------//

myRecordField makeMyRecordField(mySymbol varName, myExp varValue)
{
    if (MySymbol_InvalidSymbol(varName))	return (myRecordField)NULL;

    myRecordField one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
    one->varName = varName;
    one->varValue = varValue;
	
    return one;
}

	///////////////////////////////
	
myRecordFieldList makeMyRecordFieldList(myRecordField field, myRecordFieldList list)
{
	myRecordFieldList one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
	one->field = field;
	one->next  = list;
	
	return one;
}

	////////////////////////////////
	
myNoFieldRecordCreationExp makeMyNoFieldRecordCreationExp(
    mySymbol typeName)
{
    if (MySymbol_InvalidSymbol(typeName))
	return (myNoFieldRecordCreationExp)NULL;

    myNoFieldRecordCreationExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);	
    one->typeName = typeName;
    return one;
}

	///////////////////////////////
	
myFieldRecordCreationExp makeMyFieldRecordCreationExp(
    mySymbol typeName, myRecordFieldList list)
{
    if (MySymbol_InvalidSymbol(typeName))
	return (myFieldRecordCreationExp)NULL;

    myFieldRecordCreationExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
    one->typeName = typeName;
    one->fieldList= list;
	
    return one;
}

	///////////////////////////////
	
myRecordCreationExp makeMyRecordCreationExp_NoField(
	myNoFieldRecordCreationExp exp)
{
	myRecordCreationExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);;
	
	one->kind = NoFieldRecordCreationExp;
	one->u.noFieldRecordCreationExp = exp;
	
	return one;
}
	
	///////////////////////////////
	
myRecordCreationExp makeMyRecordCreationExp_Field(
	myFieldRecordCreationExp exp)
{
	myRecordCreationExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);;
	
	one->kind = FieldRecordCreationExp;
	one->u.fieldRecordCreationExp = exp;
	
	return one;
}
	
//------------------ArithmeticExp struct----------------------//

myArithmeticExp makeMyArithmeticExp_(enum myArithmeticOp op, 
	myExp left, myExp right)
{
	myArithmeticExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
	one->op = op;
	one->left = left;
	one->right = right;
	
	return one;
}	

	////////////////////////////////////
	
myArithmeticExp makeMyArithmeticExp_Plus(myExp left, myExp right)
{
	return makeMyArithmeticExp_(opPlus, left, right);
}

	////////////////////////////////////

myArithmeticExp makeMyArithmeticExp_Minus(myExp left, myExp right)
{
	return makeMyArithmeticExp_(opMinus, left, right);
}

	////////////////////////////////////
	
myArithmeticExp makeMyArithmeticExp_Multiply(myExp left, myExp right)
{
	return makeMyArithmeticExp_(opMultiply, left, right);
}
	
	////////////////////////////////////

myArithmeticExp makeMyArithmeticExp_Divide(myExp left, myExp right)
{
	return makeMyArithmeticExp_(opDivide, left, right);
}
	
	
//-----------------ParenthesesExp struct---------------------//

myParenthesesExp makeMyParenthesesExp(myExp exp)
{
	myParenthesesExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	one->exp = exp;
	return one;
}

//------------------NoValueExp struct-----------------------//

inline myNoValueExp makeMyNoValueExp()
{
	myNoValueExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	return one;
}

//------------------SequencingExp struct--------------------//

mySequencingExp makeMySequencingExp(myExp exp1, myExp exp2, myExpList list)
{
	mySequencingExp	one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
	one->exp1 = exp1;
	one->exp2 = exp2;
	one->nextList = list;
	
	return one;
}

//------------------ForExp struct--------------------------//

myForExp makeMyForExp(mySymbol varName, myExp rangeLow,
		      myExp rangeHigh, myExp bodyExp)
{
    if (MySymbol_InvalidSymbol(varName))
	return (myForExp)NULL;

    myForExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
    one->varName = varName;
	one->varEscape = false;
    one->varRangeLow = rangeLow;
    one->varRangeHigh = rangeHigh;
    one->bodyExp = bodyExp;
	
    return one;
}

//----------------------IfThenElseExp struct------------------//

myIfThenElseExp makeMyIfThenElseExp(myExp exp1, myExp exp2, myExp exp3)
{
	myIfThenElseExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
	one->exp1 = exp1;
	one->exp2 = exp2;
	one->exp3 = exp3;
	
	return one;
}

//-----------------------IfThenExp struct----------------------//

myIfThenExp makeMyIfThenExp(myExp exp1, myExp exp2)
{
	myIfThenExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
	one->exp1 = exp1;
	one->exp2 = exp2;
	
	return one;
}

//-----------------------ComparisonExp struct-----------------------//

myComparisonExp makeMyComparisonExp_(enum myComparisonOp op,
	myExp left, myExp right)
{
	myComparisonExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
	one->op = op;
	one->left = left;
	one->right = right;
	
	return one;
}

	//////////////////////////////////////////
	
myComparisonExp makeMyComparisonExp_Equal(myExp left, myExp right)
{
	return makeMyComparisonExp_(opEqual, left, right);
}

	/////////////////////////////////////////
	
myComparisonExp makeMyComparisonExp_NotEqual(myExp left, myExp right)
{
	return makeMyComparisonExp_(opNotEqual, left, right);
}

	////////////////////////////////////////
	
myComparisonExp makeMyComparisonExp_LargerThan(myExp left, myExp right)
{
	return makeMyComparisonExp_(opLargerThan, left, right);
}

	////////////////////////////////////////
	
myComparisonExp makeMyComparisonExp_SmallerThan(myExp left, myExp right)
{
	return makeMyComparisonExp_(opSmallerThan, left, right);
}


	///////////////////////////////////////
	
myComparisonExp makeMyComparisonExp_LargerEqualThan(myExp left, myExp right)
{
	return makeMyComparisonExp_(opLargerEqualThan, left, right);
}	

	//////////////////////////////////////
	
myComparisonExp makeMyComparisonExp_SmallerEqualThan(myExp left, myExp right)
{
	return makeMyComparisonExp_(opSmallerEqualThan, left, right);
}


//-------------------BooleanOperateExp struct-----------------//

myBooleanOperateExp makeMyBooleanOperateExp_(
	enum myBooleanOperateOp op, myExp left, myExp right)
{
	myBooleanOperateExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
	one->op = op;
	one->left = left;
	one->right = right;
	
	return one;
}	

	//////////////////////////////////////
	
myBooleanOperateExp makeMyBooleanOperateExp_And(myExp left, myExp right)
{
	return makeMyBooleanOperateExp_( opBooleanAnd, left, right);
}

	//////////////////////////////////////
	
myBooleanOperateExp makeMyBooleanOperateExp_Or(myExp left, myExp right)
{
	return makeMyBooleanOperateExp_(opBooleanOr, left, right);
}

//-------------------AssignmentExp struct--------------------//

myAssignmentExp makeMyAssignmentExp(myLValueExp lValueExp, myExp exp)
{
	myAssignmentExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
	one->lValueExp = lValueExp;
	one->exp = exp;
	
	return one;
}

//--------------------LetExp struct-----------------------------//

myLetExp makeMyLetExp(myDecList decList, myExpList expList)
{
	myLetExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
	one->decList = decList;
	one->expList = expList;
	
	return one;
}

//---------------------WhileExp struct-----------------------//

myWhileExp makeMyWhileExp(myExp whileExp, myExp bodyExp)
{
	myWhileExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
	one->whileExp = whileExp;
	one->bodyExp = bodyExp;
	
	return one;
}

//-------------------------NegationExp struct-------------------//

myNegationExp makeMyNegationExp(myExp exp)
{
	myNegationExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	one->exp = exp;
	return one;
}

//-------------------------BreakExp struct---------------------//

myBreakExp makeMyBreakExp()
{
	//	do nothing
    return makeMemoryBlock(sizeof(myBreakExp), MEMORY_TYPE_NONE);
}

//------------------------Exp struct-----------------------//

myExp makeMyExp_(myPos pos, enum myExpKind kind, void* unionMember)
{
	myExp one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
	one->kind = kind;
	one->pos = pos;
	switch (kind)
	{
	case LValueExp:	        one->u.lValueExp = (myLValueExp)unionMember;				break;
	case FunctionCallExp:	one->u.functionCallExp = (myFunctionCallExp)unionMember;		break;
	case NilExp:		one->u.nilExp = (myNilExp)unionMember;					break;
	case IntegerLiteralExp:	one->u.integerLiteralExp = (myIntegerLiteralExp)unionMember;		break;
	case StringLiteralExp:	one->u.stringLiteralExp = (myStringLiteralExp)unionMember;		break;
	case ArrayCreationExp:	one->u.arrayCreationExp = (myArrayCreationExp)unionMember;		break;
	case RecordCreationExp:	one->u.recordCreationExp = (myRecordCreationExp)unionMember;		break;
	case ArithmeticExp:	one->u.arithmeticExp = (myArithmeticExp)unionMember;			break;
	case ParenthesesExp:	one->u.parenthesesExp = (myParenthesesExp)unionMember;			break;
	case NoValueExp:	one->u.noValueExp = (myNoValueExp)unionMember;				break;
	case SequencingExp:	one->u.sequencingExp = (mySequencingExp)unionMember;			break;
	case ForExp:		one->u.forExp = (myForExp)unionMember;					break;
	case IfThenElseExp:	one->u.ifThenElseExp = (myIfThenElseExp)unionMember;			break;
	case IfThenExp:		one->u.ifThenExp = (myIfThenExp)unionMember;				break;
	case ComparisonExp:	one->u.comparisonExp = (myComparisonExp)unionMember;			break;
	case BooleanOperateExp:	one->u.booleanOperateExp = (myBooleanOperateExp)unionMember;		break;
	case AssignmentExp:	one->u.assignmentExp = (myAssignmentExp)unionMember;			break;
	case LetExp:		one->u.letExp = (myLetExp)unionMember;					break;
	case WhileExp:		one->u.whileExp = (myWhileExp)unionMember;				break;
	case NegationExp:	one->u.negationExp = (myNegationExp)unionMember;			break;
	case BreakExp:		one->u.breakExp = (myBreakExp)unionMember;				break;
	}
	
	return one;
}

	/////////////////////////////////////
	
myExp makeMyExp_LValue(myPos pos, myLValueExp exp)
{
	return makeMyExp_(pos, LValueExp ,exp);
}

	/////////////////////////////////////
	
myExp makeMyExp_FunctionCall(myPos pos, myFunctionCallExp exp)
{
	return makeMyExp_(pos, FunctionCallExp ,exp);
}

	/////////////////////////////////////
	
myExp makeMyExp_Nil(myPos pos, myNilExp exp)
{
	return makeMyExp_(pos, NilExp ,exp);
}

	/////////////////////////////////////
	
myExp makeMyExp_IntegerLiteral(myPos pos, myIntegerLiteralExp exp)
{
	return makeMyExp_(pos, IntegerLiteralExp ,exp);
}

	/////////////////////////////////////
	
myExp makeMyExp_StringLiteral(myPos pos, myStringLiteralExp exp)
{
	return makeMyExp_(pos, StringLiteralExp ,exp);
}

	/////////////////////////////////////
	
myExp makeMyExp_ArrayCreation(myPos pos, myArrayCreationExp exp)
{
	return makeMyExp_(pos, ArrayCreationExp ,exp);
}

	/////////////////////////////////////
	
myExp makeMyExp_RecordCreation(myPos pos, myRecordCreationExp exp)
{
	return makeMyExp_(pos, RecordCreationExp ,exp);
}

	/////////////////////////////////////
	
myExp makeMyExp_Arithmetic(myPos pos, myArithmeticExp exp)
{
	return makeMyExp_(pos, ArithmeticExp, exp);
}

	/////////////////////////////////////
	
myExp makeMyExp_Parentheses(myPos pos, myParenthesesExp exp)
{
	return makeMyExp_(pos, ParenthesesExp, exp);
}

	/////////////////////////////////////
	
myExp makeMyExp_NoValue(myPos pos, myNoValueExp exp)
{
	return makeMyExp_(pos, NoValueExp, exp);
}

	/////////////////////////////////////
	
myExp makeMyExp_Sequencing(myPos pos, mySequencingExp exp)
{
	return makeMyExp_(pos, SequencingExp, exp);
}

	/////////////////////////////////////
	
myExp makeMyExp_For(myPos pos, myForExp exp)
{
	return makeMyExp_(pos, ForExp, exp);
}

	/////////////////////////////////////
	
myExp makeMyExp_IfThenElse(myPos pos, myIfThenElseExp exp)
{
	return makeMyExp_(pos, IfThenElseExp, exp);
}

	/////////////////////////////////////
	
myExp makeMyExp_IfThen(myPos pos, myIfThenExp exp)
{
	return makeMyExp_(pos, IfThenExp, exp);
}

	/////////////////////////////////////
	
myExp makeMyExp_Comparison(myPos pos, myComparisonExp exp)
{
	return makeMyExp_(pos, ComparisonExp, exp);
}

	/////////////////////////////////////
	
myExp makeMyExp_BooleanOperate(myPos pos, myBooleanOperateExp exp)
{
	return makeMyExp_(pos, BooleanOperateExp, exp);
}

	/////////////////////////////////////
	
myExp makeMyExp_Assignment(myPos pos, myAssignmentExp exp)
{
	return makeMyExp_(pos, AssignmentExp, exp);
}

	/////////////////////////////////////
	
myExp makeMyExp_Let(myPos pos, myLetExp exp)
{
	return makeMyExp_(pos, LetExp, exp);
}

	/////////////////////////////////////
	
myExp makeMyExp_While(myPos pos, myWhileExp exp)
{
	return makeMyExp_(pos, WhileExp, exp);
}

	/////////////////////////////////////
	
myExp makeMyExp_Negation(myPos pos, myNegationExp exp)
{
	return makeMyExp_(pos, NegationExp, exp);
}

	/////////////////////////////////////
	
myExp makeMyExp_Break(myPos pos, myBreakExp exp)
{
	return makeMyExp_(pos, BreakExp, exp);
}

myExpList makeMyExpList(myExp exp, myExpList list)
{
    myExpList one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);

    one->exp = exp;
    one->next = list;

    return one;
}
