#include "../../myReport.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <string.h>

/*--------------------------------------------------------------*/

char* yytext;	//	fake variable used by errorReport() 

//	NOTE: some print functions are not to be tested by intention.

/*--------------------------------------------------------------*/

void test_resetPos_ByDefault_Resetted(void)
{
	resetPos();

	PosInfo pos = getCurrentPosInfo();
	CU_ASSERT_EQUAL(pos.column, 0);
	CU_ASSERT_EQUAL(pos.line, 1);
}

//////////////////////////////////////////////////////////////////////////////

void test_recordTokenPos_RecordOneToken_OnlyAddTheLengOfThatToken(void)
{
	PosInfo posBefore = getCurrentPosInfo();

	int tokenLength = 3;
	recordTokenPos(false, tokenLength);

	PosInfo posAfter = getCurrentPosInfo();
	int lengthAdded = posAfter.column - posBefore.column;
	int lineAdded = posAfter.line - posBefore.line;
	CU_ASSERT_EQUAL(lengthAdded, tokenLength);
	CU_ASSERT_EQUAL(lineAdded, 0);
}

void test_recordTokenPos_RecordOneNewLine_AddOneLineAndColumnReset(void)
{
	PosInfo posBefore = getCurrentPosInfo();

	recordTokenPos(true, 0);

	PosInfo posAfter = getCurrentPosInfo();
	int lineAdded = posAfter.line - posBefore.line;
	CU_ASSERT_EQUAL(posAfter.column, 0);
	CU_ASSERT_EQUAL(lineAdded, 1);
}

/*--------------------------------------------------------------*/

int main (int argc, char* argv[])
{
    if (CU_initialize_registry() != CUE_SUCCESS)
    {
		printf("Failed to initialize registry!\n");
		exit(-1);
    }

    CU_pSuite suite = CU_add_suite("suite for makeMemory", NULL, NULL);
    if (suite == NULL)
    {
		printf("Failed to add suite!\n");
		exit(-1);
    }

    //	add tests
    CU_TestInfo tests[] = {
		{ "", test_resetPos_ByDefault_Resetted },
		{ "", test_recordTokenPos_RecordOneToken_OnlyAddTheLengOfThatToken },
		{ "", test_recordTokenPos_RecordOneNewLine_AddOneLineAndColumnReset }	
    };
    
    for (unsigned i = 0; i < (sizeof(tests) / sizeof(tests[0])); ++i)
	if (CU_add_test(suite, tests[i].pName, tests[i].pTestFunc) == NULL)
	{
	    printf("attempt to add tests, failed!\n");
	    exit(-1);
	}

    if (CU_basic_run_suite(suite) != CUE_SUCCESS)
    {
		printf("Failed to run suite!\n");
		exit(-1);
    }

    CU_cleanup_registry();
    return 0;
}
