#ifndef TEST_HELPER_H
#define TEST_HELPER_H


//
//  A simple test helper functions for tests.
//

#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <CUnit/Console.h>

#include <stdbool.h>

bool initialTests(CU_pSuite* ppSuite);
bool addTests(CU_pSuite* ppSuite, CU_TestInfo* tests, int num);

//	Basic run mode
bool runTests(CU_pSuite* ppSuite);
//  Automated run mode
void runTests_Automated(void);
//	Console run mode
void runTests_Console(void);

#endif
