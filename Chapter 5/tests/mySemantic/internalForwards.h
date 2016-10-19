#ifndef INTERNAL_FORWARDS_H
#define INTERNAL_FORWARDS_H

#include "../../myTypes.h"
#include "../../mySemantic.h"


//////////////////////////////////////////////////////////////////////////////
//
//  forwards of semantic module

myType typeContainsLValueAux(
    myTable varAndFuncEnv, myTable typeEnv, myType type, myLValueAux aux);

myTranslationAndType MySemantic_LValueExp_SimpleVar(
    myTable varAndFuncEnv, myLValueExp lValueExp);
myTranslationAndType MySemantic_LValueExp_RecordField(
    myTable varAndFuncEnv, myTable typeEnv, myLValueExp lValueExp);
myTranslationAndType MySemantic_LValueExp_ArraySubscript(
    myTable varAndFuncEnv, myTable typeEnv, myLValueExp lValueExp);

myTranslationAndType MySemantic_LValueExp(
    myTable varAndFuncEnv, myTable typeEnv, myLValueExp lValueExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_NilExp(
    myTable varAndFuncEnv, myTable typeEnv, myNilExp nilExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_StringLiteralExp(
    myTable varAndFuncEnv, myTable typeEnv, myStringLiteralExp stringLiteralExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_IntegerLiteralExp(
    myTable varAndFuncEnv, myTable typeEnv, myIntegerLiteralExp integerLiteralExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_FunctionCallExp_Param(
    myTable varAndFuncEnv, myTable typeEnv,
    myParamFunctionCallExp paramFunctionCallExp);

myTranslationAndType MySemantic_FunctionCallExp_NoParam(
    myTable varAndFuncEnv, myTable typeEnv,
    myNoParamFunctionCallExp noParamFunctionCallExp);

myTranslationAndType MySemantic_FunctionCallExp(
    myTable varAndFuncEnv, myTable typeEnv, myFunctionCallExp functionCallExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_ArrayCreationExp(
    myTable varAndFuncEnv, myTable typeEnv, myArrayCreationExp arrayCreationExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_RecordCreationExp_NoField(
    myTable varAndFuncEnv, myTable typeEnv,
    myNoFieldRecordCreationExp noFieldRecordCreationExp);

myTranslationAndType MySemantic_RecordCreationExp_Field(
    myTable varAndFuncEnv, myTable typeEnv,
    myFieldRecordCreationExp fieldRecordCreationExp);

myTranslationAndType MySemantic_RecordCreationExp(
    myTable varAndFuncEnv, myTable typeEnv, myRecordCreationExp recordCreationExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_ArithmeticExp(
    myTable varAndFuncEnv, myTable typeEnv, myArithmeticExp arithmeticExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_ParenthesesExp(
    myTable varAndFuncEnv, myTable typeEnv, myParenthesesExp parenthesesExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_SequencingExp(
    myTable varAndFuncEnv, myTable typeEnv, mySequencingExp sequencingExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_NoValueExp(
    myTable varAndFuncEnv, myTable typeEnv, myNoValueExp noValueExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_ForExp(
    myTable varAndFuncEnv, myTable typeEnv, myForExp forExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_IfThenElseExp(
    myTable varAndFuncEnv, myTable typeEnv, myIfThenElseExp ifThenElseExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_IfThenExp(
    myTable varAndFuncEnv, myTable typeEnv, myIfThenExp ifThenExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_ComparisonExp(
    myTable varAndFuncEnv, myTable typeEnv, myComparisonExp comparisonExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_BooleanOperateExp(
    myTable varAndFuncEnv, myTable typeEnv, myBooleanOperateExp booleanOperateExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_AssignmentExp(
    myTable varAndFuncEnv, myTable typeEnv, myAssignmentExp assignmentExp);

//////////////////////////////////////////////////////////////////////////////

bool MySemantic_Dec_Type_Named(
    myTable typeEnv, mySymbol newTypeName, mySymbol existedTypeName);
bool MySemantic_Dec_Type_Record(
    myTable typeEnv, mySymbol newTypeName, myTyFieldList fields);
bool MySemantic_Dec_Type_Array(
    myTable typeEnv, mySymbol newTypeName, mySymbol arrayTypeName);

bool MySemantic_Dec_Var_ShortForm(
    myTable varAndFuncEnv, myTable typeEnv, myShortFormVar shortFormVar);
bool MySemantic_Dec_Var_LongForm(
    myTable varAndFuncEnv, myTable typeEnv, myLongFormVar longFormVar);

bool MySemantic_Dec_Func_Procedure(
    myTable varAndFuncEnv, myTable typeEnv, myProcedureDec procedureDec);
bool MySemantic_Dec_Func_Function(
    myTable varAndFuncEnv, myTable typeEnv, myFunctionDec functionDec);

bool MySemantic_Decs(
    myTable varAndFuncEnv, myTable typeEnv, myDecList decs);
    
//////////////

myTranslationAndType MySemantic_LetExp(
    myTable varAndFuncEnv, myTable typeEnv, myLetExp letExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_WhileExp(
    myTable varAndFuncEnv, myTable typeEnv, myWhileExp whileExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_NegationExp(
    myTable varAndFuncEnv, myTable typeEnv, myNegationExp negationExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_BreakExp(
    myTable varAndFuncEnv, myTable typeEnv, myBreakExp breakExp);

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//  forwards of recursiveDecs module
bool MySemantic_Dec_Type_OnePass(myTable typeEnv, myTypeDec typeDec);
bool MySemantic_Dec_Type_Named_TwoPass(
    myTable typeEnv, mySymbol newTypeName, mySymbol existedTypeName);
bool MySemantic_Dec_Type_Record_TwoPass(
    myTable typeEnv, mySymbol newTypeName, myTyFieldList fields);
bool MySemantic_Dec_Type_Array_TwoPass(
    myTable typeEnv, mySymbol newTypeName, mySymbol elementTypeName);

bool MySemantic_Dec_Func_Procedure_OnePass(
    myTable varAndFuncEnv, myTable typeEnv, myProcedureDec procedureDec);
bool MySemantic_Dec_Func_Function_OnePass(
    myTable varAndFuncEnv, myTable typeEnv, myFunctionDec functionDec);
bool MySemantic_Dec_Func_Procedure_TwoPass(
    myTable varAndFuncEnv, myTable typeEnv, myProcedureDec procedureDec);
bool MySemantic_Dec_Func_Function_TwoPass(
    myTable varAndFuncEnv, myTable typeEnv, myFunctionDec functionDec);


#endif