#include "testHelper.h"

#include <CUnit/CUnit.h>

bool initialTests(CU_pSuite* ppSuite)
{
    if (CU_initialize_registry() != CUE_SUCCESS)
    {
	    printf("Failed to initialize registry!\n");
	    return false;
    }

    *ppSuite = CU_add_suite("One test suite.", NULL, NULL);
    if (ppSuite == NULL)
    {
	    printf("Failed to add suite!\n");
	    return false;
    }

    return true;
}

bool addTests(CU_pSuite* ppSuite, CU_TestInfo* tests, int num)
{
    for (unsigned i = 0; i < num; ++i)
	    if (CU_add_test(*ppSuite, tests[i].pName, tests[i].pTestFunc) == NULL)
	    {
	        printf("attempt to add tests, failed!\n");
	        return false;
	    }

    return true;
}

/////////////////////////////////////////////////////////////////
//			run functions

bool runTests(CU_pSuite* ppSuite)
{
    if (CU_basic_run_suite(*ppSuite) != CUE_SUCCESS)
    {
	    printf("Failed to run suite!\n");
	    return false;
    }

    CU_cleanup_registry();
    return true;
}

void runTests_Console(void)
{
    CU_console_run_tests();
}


void runTests_Automated(void)
{
    CU_automated_run_tests();
}