#ifndef TEST_HELPER_H
#define TEST_HELPER_H


//
//  A simple test helper functions for tests.
//

#include <CUnit/Basic.h>

#include <stdbool.h>

bool initialTests(CU_pSuite* ppSuite);
bool addTests(CU_pSuite* ppSuite, CU_TestInfo* tests, int num);
bool runTests(CU_pSuite* ppSuite);

#endif