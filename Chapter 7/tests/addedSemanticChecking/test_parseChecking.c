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

void test_MySemanticSequencing_MiddleExpsShouldBeChecked(void)
{
    assert (
        parseFile("./test-files/sequencing_middleExpError.tig")
        == false);
}

///////////////////////         main        /////////////////////

int main()
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return EXIT_FAILURE;

    CU_TestInfo tests[] = {
        { "", test_MySemanticSequencing_MiddleExpsShouldBeChecked },
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return EXIT_FAILURE;

    if (!runTests(&suite))  return EXIT_FAILURE;
    freeMemoryChain();
    return EXIT_SUCCESS;
}
