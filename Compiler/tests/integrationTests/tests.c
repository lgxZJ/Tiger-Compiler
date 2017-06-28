#include "../testHelper.h"

#include "../../myParser.h"
#include "../../mySemantic.c"

#include <assert.h>
#include <stdlib.h>

///////////////////////     test helpers    ///////////////////////

myExp parseAndAnalyzeOneFile(char* filename)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myExp exp = parseOneFile(filename);
    assert (exp != NULL);
    assert (exp != ABSTRACT_SYNTAX_ERROR);

    myTranslationAndType result = MySemantic_Exp(exp);
    assert (result != SEMANTIC_ERROR);

    return exp;
}

///////////////////////        tests       ////////////////////////

//  a parameterized test
void test_BuildTypeVarInFile_WhetherEscapes(char* filename, bool result)
{
    myExp exp = parseAndAnalyzeOneFile(filename);

    bool varEscapeFlag = exp->u.letExp->decList->
        dec->u.varDec->u.shortFormVar->escape;
    CU_ASSERT_EQUAL(varEscapeFlag, result);
}

////////////////

void test_IntVarFile_EscapeFinding_NotEscaped(void)
{
    char* filename = "./test-files/intVar.tig";
    bool escapeResult = true;
    test_BuildTypeVarInFile_WhetherEscapes(filename, escapeResult);
}

void test_StringVarFile_EscapeFinding_Escaped(void)
{
    char* filename = "./test-files/stringVar.tig";
    bool escapeResult = true;
    test_BuildTypeVarInFile_WhetherEscapes(filename, escapeResult);
}

void test_ArrayVarFile_EscapeFinding_Escaped(void)
{
    char* filename = "./test-files/arrayVar.tig";
    myExp exp = parseAndAnalyzeOneFile(filename);

    //  this var is of array type!!!
    bool varEscapeFlag = exp->u.letExp->decList->next->
        dec->u.varDec->u.longFormVar->escape;
    CU_ASSERT_EQUAL(varEscapeFlag, true);
}

void test_RecordVarFile_EscapeFinding_Escaped(void)
{
    char* filename = "./test-files/recordVar.tig";
    myExp exp = parseAndAnalyzeOneFile(filename);

    //  this var is of record type!!!
    bool varEscapeFlag = exp->u.letExp->decList->next->dec
        ->u.varDec->u.shortFormVar->escape; 
    CU_ASSERT_EQUAL(varEscapeFlag, true);
}

void test_NamedVarFile_EscapeFinding_TheSameWithActualType(void)
{
    char* filename = "./test-files/namedVar.tig";
    myExp exp = parseAndAnalyzeOneFile(filename);

    //  this var is of record type!!!
    bool namedIntVarEscapeFlag = exp->u.letExp->decList->next->next->next->
        dec->u.varDec->u.shortFormVar->escape;
    bool namedArrayVarEscapeFlag = exp->u.letExp->decList->next->next->next->next->
        dec->u.varDec->u.shortFormVar->escape;
    CU_ASSERT_EQUAL(namedIntVarEscapeFlag, true);
    CU_ASSERT_EQUAL(namedArrayVarEscapeFlag, true);
}

///////////////////////////////////////////////////////////

void test_HidedVarFile_EscapeFinding_OuterNotEscapedInnerEscaped(void)
{
    char* filename = "./test-files/hideVar.tig";
    myExp exp = parseAndAnalyzeOneFile(filename);

    bool outerVarEscapeFlag = exp->u.letExp->decList->dec
        ->u.varDec->u.shortFormVar->escape;
    bool innerVarEscapeFlag = exp->u.letExp->expList->exp->u.letExp
        ->decList->next->dec->u.varDec->u.longFormVar->escape;
    CU_ASSERT_EQUAL(outerVarEscapeFlag, true);
    CU_ASSERT_EQUAL(innerVarEscapeFlag, true);
}

void test_BlockUsedVarFile_EscapeFinding_NotEscaped(void)
{
    char* filename = "./test-files/blockUsedVar.tig";
    myExp exp = parseAndAnalyzeOneFile(filename);

    bool varEscapeFlag = exp->u.letExp->decList->dec
        ->u.varDec->u.shortFormVar->escape;
    CU_ASSERT_EQUAL(varEscapeFlag, true);
}

void test_NestedUseVarFile_EscapeFinding_Escaped(void)
{
    char* filename = "./test-files/nestedUsedVar.tig";
    myExp exp = parseAndAnalyzeOneFile(filename);

    bool varEscapeFlag = exp->u.letExp->decList->dec
        ->u.varDec->u.shortFormVar->escape;
    CU_ASSERT_EQUAL(varEscapeFlag, true);
}

////////////////////////////////////////////////////////////

void test_NotNestedForLoopVarFile_EscapeFinding_NotEscaped(void)
{
    char* filename = "./test-files/notNestedForLoopVar.tig";
    myExp exp = parseAndAnalyzeOneFile(filename);

    bool varEscapeFlag = exp->u.forExp->varEscape;
    CU_ASSERT_EQUAL(varEscapeFlag, true);
}

void test_NestedForLoopVarFile_EscapeFinding_Escaped(void)
{
    char* filename = "./test-files/nestedForLoopVar.tig";
    myExp exp = parseAndAnalyzeOneFile(filename);

    bool varEscapeFlag = exp->u.forExp->varEscape;
    CU_ASSERT_EQUAL(varEscapeFlag, true);
}

/////////////////////////////////////////////////////////

//  a parameterized test
void test_FuncFormals_WhetherEscape(char* filename, bool escapeResult)
{
    myExp exp = parseAndAnalyzeOneFile(filename);

    bool varEscapeFlag = exp->u.letExp->decList->dec
        ->u.funcDec->u.functionDec->tyFieldList->field->varEscape;
    CU_ASSERT_EQUAL(varEscapeFlag, escapeResult);
}

///////////

void test_NotNestedIntFuncFormalsFile_EscapeFinding_NotEscaped(void)
{
    char* filename = "./test-files/notNestedIntFuncFormals.tig";
    bool escapeResult = true;
    test_FuncFormals_WhetherEscape(filename, escapeResult);
}

void test_NestedIntFuncFormalsFile_EscapeFinding_Escaped(void)
{
    char* filename = "./test-files/nestedIntFuncFormals.tig";
    bool escapeResult = true;
    test_FuncFormals_WhetherEscape(filename, escapeResult);
}

void test_NotIntFuncFormalsFile_EscapeFinding_Escaped(void)
{
    char* filename = "./test-files/notIntFuncFormals.tig";
    myExp exp = parseAndAnalyzeOneFile(filename);

    bool varEscapeFlag = exp->u.letExp->decList->next->dec
        ->u.funcDec->u.functionDec->tyFieldList->field->varEscape;
    CU_ASSERT_EQUAL(varEscapeFlag, true);
}

///////////////////////         main        /////////////////////

int main()
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return EXIT_FAILURE;

    CU_TestInfo tests[] = {
        { "", test_IntVarFile_EscapeFinding_NotEscaped },
        { "", test_StringVarFile_EscapeFinding_Escaped },
        { "", test_ArrayVarFile_EscapeFinding_Escaped },
        { "", test_RecordVarFile_EscapeFinding_Escaped },
        { "", test_NamedVarFile_EscapeFinding_TheSameWithActualType },

        { "", test_HidedVarFile_EscapeFinding_OuterNotEscapedInnerEscaped },
        { "", test_BlockUsedVarFile_EscapeFinding_NotEscaped },
        { "", test_NestedUseVarFile_EscapeFinding_Escaped },

        { "", test_NotNestedForLoopVarFile_EscapeFinding_NotEscaped },
        { "", test_NestedForLoopVarFile_EscapeFinding_Escaped },

        { "", test_NotNestedIntFuncFormalsFile_EscapeFinding_NotEscaped },
        { "", test_NestedIntFuncFormalsFile_EscapeFinding_Escaped },
        { "", test_NotIntFuncFormalsFile_EscapeFinding_Escaped },
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return EXIT_FAILURE;

    if (!runTests(&suite))  return EXIT_FAILURE;
    freeMemoryChain();
    return EXIT_SUCCESS;
}
