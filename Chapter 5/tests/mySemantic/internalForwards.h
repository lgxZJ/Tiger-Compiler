#ifndef INTERNAL_FORWARDS_H
#define INTERNAL_FORWARDS_H

#include "../../myTypes.h"
#include "../../mySemantic.h"


//////////////////////////////////////////////////////////////////////////////
//
//  forwards of semantic module

myType typeContainsLValueAux(
    myType type, myLValueAux aux);

myTranslationAndType MySemantic_LValueExp_SimpleVar(
    myLValueExp lValueExp);
myTranslationAndType MySemantic_LValueExp_RecordField(
    myLValueExp lValueExp);
myTranslationAndType MySemantic_LValueExp_ArraySubscript(
    myLValueExp lValueExp);

myTranslationAndType MySemantic_LValueExp(
    myLValueExp lValueExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_NilExp(
    myNilExp nilExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_StringLiteralExp(
    myStringLiteralExp stringLiteralExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_IntegerLiteralExp(
    myIntegerLiteralExp integerLiteralExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_FunctionCallExp_Param(
    
    myParamFunctionCallExp paramFunctionCallExp);

myTranslationAndType MySemantic_FunctionCallExp_NoParam(
    
    myNoParamFunctionCallExp noParamFunctionCallExp);

myTranslationAndType MySemantic_FunctionCallExp(
    myFunctionCallExp functionCallExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_ArrayCreationExp(
    myArrayCreationExp arrayCreationExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_RecordCreationExp_NoField(
    
    myNoFieldRecordCreationExp noFieldRecordCreationExp);

myTranslationAndType MySemantic_RecordCreationExp_Field(
    
    myFieldRecordCreationExp fieldRecordCreationExp);

myTranslationAndType MySemantic_RecordCreationExp(
    myRecordCreationExp recordCreationExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_ArithmeticExp(
    myArithmeticExp arithmeticExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_ParenthesesExp(
    myParenthesesExp parenthesesExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_SequencingExp(
    mySequencingExp sequencingExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_NoValueExp(
    myNoValueExp noValueExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_ForExp(
    myForExp forExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_IfThenElseExp(
    myIfThenElseExp ifThenElseExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_IfThenExp(
    myIfThenExp ifThenExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_ComparisonExp(
    myComparisonExp comparisonExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_BooleanOperateExp(
    myBooleanOperateExp booleanOperateExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_AssignmentExp(
    myAssignmentExp assignmentExp);

//////////////////////////////////////////////////////////////////////////////

bool MySemantic_Dec_Type_Named(
    mySymbol newTypeName, mySymbol existedTypeName);
bool MySemantic_Dec_Type_Record(
    mySymbol newTypeName, myTyFieldList fields);
bool MySemantic_Dec_Type_Array(
    mySymbol newTypeName, mySymbol arrayTypeName);

bool MySemantic_Dec_Var_ShortForm(
    myShortFormVar shortFormVar);
bool MySemantic_Dec_Var_LongForm(
    myLongFormVar longFormVar);

bool MySemantic_Dec_Func_Procedure(
    myProcedureDec procedureDec);
bool MySemantic_Dec_Func_Function(
    myFunctionDec functionDec);

bool MySemantic_Decs(
    myDecList decs);
    
//////////////

myTranslationAndType MySemantic_LetExp(
    myLetExp letExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_WhileExp(
    myWhileExp whileExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_NegationExp(
    myNegationExp negationExp);

//////////////////////////////////////////////////////////////////////////////

myTranslationAndType MySemantic_BreakExp(
    myBreakExp breakExp);

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//  forwards of recursiveDecs module
bool MySemantic_Dec_Type_OnePass(myTypeDec typeDec);
bool MySemantic_Dec_Type_Named_TwoPass(
    mySymbol newTypeName, mySymbol existedTypeName);
bool MySemantic_Dec_Type_Record_TwoPass(
    mySymbol newTypeName, myTyFieldList fields);
bool MySemantic_Dec_Type_Array_TwoPass(
    mySymbol newTypeName, mySymbol elementTypeName);

bool MySemantic_Dec_Func_Procedure_OnePass(
    myProcedureDec procedureDec);
bool MySemantic_Dec_Func_Function_OnePass(
    myFunctionDec functionDec);
bool MySemantic_Dec_Func_Procedure_TwoPass(
    myProcedureDec procedureDec);
bool MySemantic_Dec_Func_Function_TwoPass(
    myFunctionDec functionDec);


#endif