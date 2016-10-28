#include "../testHelper.h"
#include "../testFixture.h"

#include "../../myEscape.h"
#include "../../myParser.h"

#include <stdlib.h>

//////////////////////                      //////////////////////

//  fake function to pretend semantic module
myTable MySemantic_getVarAndFuncEnvironment(void)   { return 0; }
myTable MySemantic_getTypeEnvironment(void)         { return 0; }

///////////////////////        tests       ////////////////////////

//  NOTE:   for convience, these tests are completed by read in one file,
//  and then test. So, these tests are actually slow tests.


///////////////////////         main        /////////////////////

int main()
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return EXIT_FAILURE;

    CU_TestInfo tests[] = {
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return EXIT_FAILURE;

    if (!runTests(&suite))  return EXIT_FAILURE;
    freeMemoryChain();
    return EXIT_SUCCESS;
}