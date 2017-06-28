#include "../testHelper.h"

#include "../../myParser.h"
#include "../../mySemantic.h"
#include "../../myEnvironment.h"

#include <stdlib.h>
#include <assert.h>

///////////////////////     test helpers    ///////////////////////

static bool parseFile(char* filename)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myExp exp = parseOneFile(filename);
    assert (exp != NULL);
    assert (exp != ABSTRACT_SYNTAX_ERROR);

    myTranslationAndType result = MySemantic_Exp(exp);
    return result != SEMANTIC_ERROR;
}

///////////////////////        tests       ////////////////////////

void test_MySemanticSequencing_MiddleExpsError_ReturnError(void)
{
    CU_ASSERT_EQUAL (
        parseFile("./test-files/sequencing_middleExpError.tig"),
        false);
}

void test_MySemanticSequencing_MiddleExps_ReturnNoError(void)
{
    CU_ASSERT_EQUAL (
        parseFile("./test-files/sequencing_middleExpNoError.tig"),
        true);
}

//////////////////////////////

void test_MySemanticComparison_RecordInequality_ReturnError(void)
{
    CU_ASSERT_EQUAL (
        parseFile("./test-files/comparison_recordInequality.tig"),
        false);
}

void test_MySemanticComparison_ArrayInequality_ReturnError(void)
{
    CU_ASSERT_EQUAL (
        parseFile("./test-files/comparison_arrayInequality.tig"),
        false);
}

void test_MySemanticComparison_ArrayEquality_ReturnNoError(void)
{
    CU_ASSERT_EQUAL (
        parseFile("./test-files/comparison_arrayEquality.tig"),
        true);
}

void test_MySemanticComparison_RecordEquality_ReturnNoError(void)
{
    CU_ASSERT_EQUAL (
        parseFile("./test-files/comparison_recordEquality.tig"),
        true);
}

///////////////////////         main        /////////////////////

int main()
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return EXIT_FAILURE;

    CU_TestInfo tests[] = {
        { "", test_MySemanticSequencing_MiddleExpsError_ReturnError },
        { "", test_MySemanticSequencing_MiddleExps_ReturnNoError },

        { "", test_MySemanticComparison_RecordInequality_ReturnError },
        { "", test_MySemanticComparison_ArrayInequality_ReturnError },
        { "", test_MySemanticComparison_ArrayEquality_ReturnNoError },
        { "", test_MySemanticComparison_RecordEquality_ReturnNoError },
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return EXIT_FAILURE;

    if (!runTests(&suite))  return EXIT_FAILURE;
    freeMemoryChain();
    return EXIT_SUCCESS;
}
