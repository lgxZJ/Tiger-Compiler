#include "testFixture.h"

#include "../myEnvironment.h"
#include "../typeMaker.h"
#include "../abstractSyntaxMaker.h"
#include "../mySemantic.h"

#include <stdlib.h> //  for macro NULL

///////////////////////////////////////////////////////////////////
//  help functions

mySymbol makeOneSymbol(void)
{
    return MySymbol_MakeSymbol("randomSymbol");
}

mySymbol makeSymbol_Int(void)
{
    return MySymbol_MakeSymbol("int");
}

mySymbol makeSymbol_String(void)
{
    return MySymbol_MakeSymbol("string");
}

myPos makeOnePos()
{
    myPos pos;
    pos.column = pos.line;
    return pos;
}

myExp makeOneLetExp_IntVar(mySymbol varSymbol)
{
    return makeMyExp_Let(makeOnePos(), 
        makeMyLetExp(
            makeMyDecList(
                makeMyDec_Var(makeOnePos(), makeMyVarDec_ShortForm(
                    makeMyShortFormVar(varSymbol, makeOneExp_Integer()))),
                NULL),
            makeMyExpList(NULL, NULL)));
}

myExp makeOneExp_Integer(void)
{
    return makeMyExp_IntegerLiteral(
            makeOnePos(),
            makeMyIntegerLiteralExp(1));
}

myExp makeOneExp_String(void)
{
    return makeMyExp_StringLiteral(
            makeOnePos(),
            makeMyStringLiteralExp("1"));
}

myExp makeOneExp_Nil(void)
{
    return makeMyExp_Nil(makeOnePos(), makeMyNilExp());
}

myExp makeOneExp_Break(void)
{
    return makeMyExp_Break(makeOnePos(), makeMyBreakExp());
}

myExp makeOneExp_NoValue(void)
{
    return makeMyExp_NoValue(
        makeOnePos(), makeMyNoValueExp());
}

myExp makeOneExp_Record(mySymbol recordTypeName)
{
    return makeMyExp_RecordCreation(makeOnePos(),
        makeMyRecordCreationExp_NoField(
            makeMyNoFieldRecordCreationExp(recordTypeName)));
}

myExp makeOneIllegalExp_Integer(void)
{
    return makeMyExp_IfThenElse(makeOnePos(),
        makeMyIfThenElseExp(makeOneExp_String(),
            makeOneExp_Integer(), makeOneExp_Integer()));
}

myLValueExp makeOneLegalLValueExp_SimpleVar_Int(void)
{
    mySymbol variableName = makeOneSymbol();
    MySymbol_Enter(
        MySemantic_getVarAndFuncEnvironment(),
        variableName,
        myEnvironment_makeVarEntry(
            (Trans_myAccess)NULL,
            makeType_Int()));

    return makeMyLValue(makeOnePos(), variableName, NULL);
}

myLValueExp makeOneLegalLValueExp_Record(myType recordType)
{
    mySymbol variableName = makeOneSymbol();
    MySymbol_Enter(
        MySemantic_getVarAndFuncEnvironment(),
        variableName,
        myEnvironment_makeVarEntry(
            (Trans_myAccess)NULL,recordType));

    return makeMyLValue(makeOnePos(), variableName, NULL);
}

myNoFieldRecordCreationExp makeOneNoFieldCreationExp(void)
{
    return makeMyNoFieldRecordCreationExp(MySymbol_MakeSymbol("record creation"));
}

myLValueExp makeOneLValueExp_TwoIntArraySubscript(
    mySymbol arrayName, myExp subscript1, myExp subscript2)
{
    return makeMyLValue(makeOnePos(), arrayName,
        makeMyLValueAux(NULL,
            subscript1,
            makeMyLValueAux(NULL,
            subscript2,
            NULL)));
}

myType makeOneArray_StringArrayArray(void)
{
    return makeType_Array(makeType_Array(makeType_String()));
}

myType makeAndAddOneType_NoFieldRecord(mySymbol recordTypeName)
{
    myType recordType = makeType_Record(makeType_TypeFieldList(NULL, NULL));
    MyEnvironment_addNewType(
        MySemantic_getTypeEnvironment(),
        recordTypeName,
        recordType);
    return recordType;
}

myProcedureDec makeOneProcedureDec_Body(myExp body)
{
    return makeMyProcedureDec(
        MySymbol_MakeSymbol("procedureName"),
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), makeSymbol_Int()), NULL),
        body);
}

myFunctionDec makeOneFunctionDec_Body(myExp body, mySymbol returnTypeName)
{
    return makeMyFunctionDec(
        MySymbol_MakeSymbol("procedureName"),
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), makeSymbol_Int()), NULL),
        returnTypeName,
        body);
}

myFunctionDec makeOneFunctionDec_Int(
    mySymbol functionName, myExp body, mySymbol returnTypeName)
{
    return makeMyFunctionDec(
        functionName,
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), makeSymbol_Int()), NULL),
        returnTypeName,
        body);
}

myProcedureDec makeOneProcedureDec_NoReturn(void)
{
    return makeOneProcedureDec_Body(makeOneExp_NoValue());
}

myProcedureDec makeOneProcedureDec_Integer(void)
{
    return makeOneProcedureDec_Body(makeOneExp_Integer());
}

myExp makeOneExp_NoParamprocedureCall(mySymbol funcName)
{
    return makeMyExp_FunctionCall(makeOnePos(), makeMyFunctionCallExp_NoParam(
        makeMyNoParamFunctionCallExp(funcName)));
}

myDecList makeLegalVars(void)
{
    return makeMyDecList(makeMyDec_Var(makeOnePos(),
        makeMyVarDec_ShortForm(makeMyShortFormVar(makeOneSymbol(), makeOneExp_Integer()))),
        makeMyDecList(makeMyDec_Var(makeOnePos(),
            makeMyVarDec_ShortForm(makeMyShortFormVar(makeOneSymbol(), makeOneExp_Integer()))),
            NULL));
}

myDecList makeIllegalVars(void)
{
    return makeMyDecList(makeMyDec_Var(makeOnePos(),
        makeMyVarDec_ShortForm(makeMyShortFormVar(makeOneSymbol(), makeOneExp_Integer()))),
        makeMyDecList(makeMyDec_Var(makeOnePos(),
            makeMyVarDec_ShortForm(makeMyShortFormVar(makeOneSymbol(), makeOneExp_NoValue()))),
            NULL));
}

myDecList makeLegalRecursiveNamedDecs(mySymbol A, mySymbol B, mySymbol C)
{
    return makeMyDecList(makeMyDec_Type(makeOnePos(),
        makeMyTyDec(A, makeMyTy_Named(makeOnePos(), B))),
            makeMyDecList(makeMyDec_Type(makeOnePos(),
            makeMyTyDec(B, makeMyTy_Named(makeOnePos(), C))),
                makeMyDecList(makeMyDec_Type(makeOnePos(),
                makeMyTyDec(C, makeMyTy_Named(makeOnePos(), makeSymbol_Int()))),
            NULL)));
}

myDecList makeIllegalRecursiveNamedDecs_Manual(
    mySymbol typeNameA, mySymbol typeNameB, mySymbol typeNameC)
{
    return makeMyDecList(makeMyDec_Type(makeOnePos(),
        makeMyTyDec(typeNameA, makeMyTy_Named(makeOnePos(), typeNameB))),
            makeMyDecList(makeMyDec_Type(makeOnePos(),
            makeMyTyDec(typeNameB, makeMyTy_Named(makeOnePos(), typeNameC))),
                makeMyDecList(makeMyDec_Type(makeOnePos(),
                makeMyTyDec(typeNameC, makeMyTy_Named(makeOnePos(), typeNameA))),
            NULL)));
}

myDecList makeIllegalRecursiveNamedDecs_Recursive(void)
{
    mySymbol sametypeName = makeOneSymbol();
    return makeMyDecList(makeMyDec_Type(makeOnePos(),
        makeMyTyDec(sametypeName, makeMyTy_Named(makeOnePos(), makeSymbol_Int()))),
            makeMyDecList(makeMyDec_Type(makeOnePos(),
            makeMyTyDec(sametypeName, makeMyTy_Named(makeOnePos(), MySymbol_MakeSymbol("string")))), NULL));
}

myDecList makeLegalRecursiveFuncDecs(
    mySymbol funcNameA, mySymbol funcNameB, mySymbol funcNameC)
{
    return makeMyDecList(makeMyDec_Func(makeOnePos(), 
        makeMyFuncDec_Procedure(makeMyProcedureDec(funcNameA, NULL, 
            makeOneExp_NoParamprocedureCall(funcNameB)))), 
        makeMyDecList(makeMyDec_Func(makeOnePos(), 
            makeMyFuncDec_Procedure(makeMyProcedureDec(funcNameB, NULL, 
                makeOneExp_NoParamprocedureCall(funcNameC)))), 
        makeMyDecList(makeMyDec_Func(makeOnePos(), 
            makeMyFuncDec_Procedure(makeMyProcedureDec(funcNameC, NULL, 
                makeOneExp_NoParamprocedureCall(funcNameA)))), NULL)));
}

myDecList makeIllegalRecursiveFuncDecs(void)
{
    mySymbol sameFuncName = makeOneSymbol();
    return makeMyDecList(makeMyDec_Func(makeOnePos(), 
        makeMyFuncDec_Procedure(makeMyProcedureDec(sameFuncName, NULL, 
            makeOneExp_NoValue()))), 
        makeMyDecList(makeMyDec_Func(makeOnePos(), 
            makeMyFuncDec_Procedure(makeMyProcedureDec(sameFuncName, NULL, 
                makeOneExp_NoValue()))), NULL));
}