#include "../myReport.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include <stdlib.h>


#include <assert.h>


//	struct value in myReport.c used for test
extern PosInfo g_posInfo;

//	assume line number and column number won't exceed
void getCurrentPosInfo_ShouldWorkRight(void)
{
	//	initial pos is line 1,column 0
	CU_ASSERT(g_posInfo.line == 1 && g_posInfo.column == 0);

	//	after value set
	g_posInfo.line = 12;
	g_posInfo.column = 12;
	CU_ASSERT(g_posInfo.line == 12 && g_posInfo.column == 12);
}

void recordTokenPos_ShouldWorkRight(void)
{
	//	setup fixture
	resetPos();
	int expected_line = 1, expected_column = 0;

	//	first function call, pos should not change
	recordTokenPos(false, 0);
	CU_ASSERT_EQUAL(getCurrentPosInfo().line, expected_line);
	CU_ASSERT_EQUAL(getCurrentPosInfo().column, expected_column);

	//	second function with valid parameters
	recordTokenPos(true, 1);
	expected_line = 2, expected_column = 1;
	CU_ASSERT_EQUAL(getCurrentPosInfo().line, expected_line);
	CU_ASSERT_EQUAL(getCurrentPosInfo().column, expected_column);

	//	thrid function call with negative column  parameters
	recordTokenPos(false, -1);
	expected_column = 0;
	CU_ASSERT_EQUAL(getCurrentPosInfo().column, expected_column);
}

void outputReport_ShouldWorkRight(void)
{
	//	test valid parameters
	CU_ASSERT(outputReport(1, 0, NULL) > 0);
	CU_ASSERT(outputReport(1, 44, NULL) > 0);
	CU_ASSERT(outputReport(1, 44, "optional text") > 0);

	//	test tokenNum boundaries
	CU_ASSERT((outputReport(0, 0, "valid") == -1) && (outputReport(-1, 0, "valid") == -1));
	CU_ASSERT((outputReport(47, 0, "valid") > 0) && (outputReport(48, 0, "valid") == -1));
	//	test invalid optional value
	CU_ASSERT((outputReport(1, (int)"invalid", "valid") > 0) &&
		  (outputReport(44, (int)"valid", "valid") > 0));
	//	test invalid optional text
	CU_ASSERT(outputReport(1, 0, NULL) > 0);	
}

//	no testing for errorReport().


//	write for test
int yytext()	{}

int main ()
{
  if (CU_initialize_registry() != CUE_SUCCESS)
  {
	printf("initial error!\n");
	exit(-1);
  }

  CU_pSuite suite = CU_add_suite("myReport suite", NULL, NULL);
  if (suite == NULL)
  {
	printf("adding suite failed.\n");
	exit(-1);
  }

  //	add test to the former suite, must 
  assert(CU_add_test(suite, "test func--getCurrentPosInfo()", getCurrentPosInfo_ShouldWorkRight) != NULL); 
  assert(CU_add_test(suite, "test func--recordTokenPos()", recordTokenPos_ShouldWorkRight) != NULL);
  assert(CU_add_test(suite, "test func--outputReport()", outputReport_ShouldWorkRight) != NULL);

  if (CU_basic_run_suite(suite) != CUE_SUCCESS)
  {
	printf("attempt to runn tests, failed!\n");
	exit(-1);
  }

  CU_cleanup_registry();
  return 0;
}
