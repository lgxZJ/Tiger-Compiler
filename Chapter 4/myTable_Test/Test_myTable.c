#include "../myTable.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <assert.h>
#include <string.h>

//	NOTE:
//		To run this test, remove 'static' key word in myTable.c,
//	uncomment following test functions and uncomment coresponding test
//	adding block.
//		After testing, you must rewrite 'static' back.
//
//		To test, uncomment function prototypes below also.
//
//	int hashFunc_MyTable(void* const key);
//	myBinder makeMyBinder(void* key, void* value, myBinder next, void* prevTop);
/*
void hashFunc_ShouldWorkRight(void)
{
	//	test invalid string
	CU_ASSERT_EQUAL(hashFunc_MyTable((void*)NULL), -1);
	//	test null string
	CU_ASSERT_EQUAL(hashFunc_MyTable(""), -1);
	//	test valid string
	CU_ASSERT(hashFunc_MyTable("valid") != -1);
}


void makeMyBinder_ShouldWorkRight(void)
{
	//	fixture setup
	myBinder actual[3];

	//	test valid key
	actual[0] = makeMyBinder("valid", "valid", (const myBinder)NULL, (void* const)NULL);
	CU_ASSERT_NOT_EQUAL(actual[0], NULL);
	//	test NULL key
	actual[1] = makeMyBinder((void* const)NULL, "valid", (const myBinder)NULL, (void* const)NULL);
	CU_ASSERT_EQUAL(actual[1], NULL);
	//	test "" key
	actual[2] = makeMyBinder((void* const)"", "valid", (const myBinder)NULL, (void* const)NULL);
	CU_ASSERT_EQUAL(actual[2], NULL);
	//	because function tested just set member to params inside
	//	so skipped to test its members

	//	teradown
	for (unsigned i = 0; i < 3; ++i)
		free(actual[i]);
}
*/



void MyTable_MakeEmptyTable_ShouldWorkRight(void)
{
	//	fixture setup and system under control
	myTable table = MyTable_MakeEmptyTable_();
	//	test, should not return NULL unless no enough memory
	CU_ASSERT_NOT_EQUAL(table, NULL);
	//	members should be initialized with NULL
	CU_ASSERT_EQUAL(table->top, NULL)
	for (int i = 0; i < TAB_SIZE; ++i)
		CU_ASSERT_EQUAL(table->binder[i], NULL);
}


void MyTable_Enter_ShouldChangeOnValidParam(void)
{
	//	fixture setup
	int value = 0;
	myTable	table = MyTable_MakeEmptyTable_();

	//	test initial value
	CU_ASSERT_EQUAL(table->top, NULL);

	//	run testing function and verify
	CU_ASSERT_EQUAL(MyTable_Enter_(table, "id", &value), true);
	//	verify
	CU_ASSERT(strcmp(table->top, "id") == 0);
	
	//	test
	CU_ASSERT_EQUAL(MyTable_Enter_(table, "id2", &value), true);
	//	verify (should change top key)
	CU_ASSERT(strcmp(table->top, "id2") == 0);

	//	teardown
	free(table);
}

void MyTable_Enter_ShouldNotChangeOnInvalidParam(void)
{
	//	fixture setup
	int	value = 1;
	myTable	table = MyTable_MakeEmptyTable_();
	//	guard conditions
	CU_ASSERT_EQUAL(table->top, NULL);

	//	system under control (invalid null string)
	CU_ASSERT_EQUAL(MyTable_Enter_(table, "", &value), false);
	//	verify
	CU_ASSERT_EQUAL(table->top, NULL);

	//	system under control (invalid null pointer)
	CU_ASSERT_EQUAL(MyTable_Enter_(table, (void*)NULL, &value),false);
	//	verify
	CU_ASSERT_EQUAL(table->top, NULL);

	//	system under control (invalid null value)
	CU_ASSERT_EQUAL(MyTable_Enter_(table, "valid", NULL), false);
	//	verify
	CU_ASSERT_EQUAL(table->top, NULL);

	//	teardown
	free(table);
}

void MyTable_Look_ShouldReturnNullOnInvalidParams(void)
{
  //	fixture setup
  myTable table = MyTable_MakeEmptyTable_();
  void* actual, *expected = NULL;

  //	system under control (invalid table param)
  actual = MyTable_Look_(NULL, "valid");
  //	verify
  CU_ASSERT_EQUAL(actual, expected);

  //	system under control (invalid key param)
  actual = MyTable_Look_(table, NULL);
  //	verify
  CU_ASSERT_EQUAL(actual, NULL);

  //	system under control (invalid key param)
  actual = MyTable_Look_(table, "");
  //    verify
  CU_ASSERT_EQUAL(actual, expected);

  //	teardown
  free(table);
}

void MyTable_ShouldReturnRightOnValidParams(void)
{
  //	fixture setup
  myTable table = MyTable_MakeEmptyTable_();
  void* actual, *expected;

  //    system under control (initial is empty)
  actual = MyTable_Look_(table, "valid");
  //	verify
  expected = NULL;
  CU_ASSERT_EQUAL(actual, expected);

  //	system under control (first enter)
  int value = 0;
  CU_ASSERT(MyTable_Enter_(table, "id", &value));
  actual = MyTable_Look_(table, "id");
  //	verify
  expected = &value;
  CU_ASSERT_EQUAL(actual, expected);

  //	system under control (second enter)
  int value2 = 1;
  CU_ASSERT(MyTable_Enter_(table, "id2", &value2));
  actual = MyTable_Look_(table, "id2");
  //	verify
  expected = &value2;
  CU_ASSERT_EQUAL(actual, expected);

  actual = MyTable_Look_(table, "id");
  expected = &value;
  CU_ASSERT_EQUAL(actual, expected);

  //	teardown
  free(table);
}

void MyTable_Pop_ShouldReturnNULLOnEmptyTable(void)
{
  //    fixture setup
  myTable table = MyTable_MakeEmptyTable_();
  void* actual, *expected = NULL;

  //	system under control (invalid table)
  actual = MyTable_Pop_((myTable)NULL);
  //	verify
  CU_ASSERT_EQUAL(actual, expected);

  //	system under control (empty table)
  actual = MyTable_Pop_(table);
  //	verify
  CU_ASSERT_EQUAL(actual, expected);

  //	teardown
  free(table);
}

void MyTable_Pop_ShouldReturnCorrectlyOnNonEmptyTable(void)
{
  //	fixture setup
  myTable table = MyTable_MakeEmptyTable_();
  char* key1 = "id1", *key2 = "id2";
  void* actual;
  //	gurad condition
  CU_ASSERT_EQUAL(MyTable_Enter_(table, key1, "value1"), true);
  CU_ASSERT_EQUAL(MyTable_Enter_(table, key2, "value2"), true);

  //	system under control (first pop)
  actual = MyTable_Pop_(table);
  //	verify
  CU_ASSERT_EQUAL(actual, key2);
  CU_ASSERT_EQUAL(table->top, key1);

  //	system under control (second pop)
  actual = MyTable_Pop_(table);
  //	verify
  CU_ASSERT_EQUAL(actual, key1);
  CU_ASSERT_EQUAL(table->top, NULL);

  //	teardown
  free(table);
}

//	NOTE:
//	test below tests dump function in table module,
//	open dumpTest.txt to see if outputs are appropriate

void dumpShower(void* key, void* value)
{
  FILE* file = fopen("dumpTest.txt", "a");
  if (file == NULL)	return;

  fprintf(file, "%s:\t%s\n", (char*)key, (char*)value);

  fclose(file);
}

void MyTable_Dump_SeeOutPut(void)
{
  //	fixture setup
  myTable table = MyTable_MakeEmptyTable_();
  //	gurad condition
  CU_ASSERT_EQUAL(MyTable_Enter_(table, "key1", "value1"), true);
  CU_ASSERT_EQUAL(MyTable_Enter_(table, "key2", "value2"), true);
  CU_ASSERT_EQUAL(MyTable_Enter_(table, "key3", "value3"), true);

  //	invoke MyTable_Dump_()
  MyTable_Dump_(table, dumpShower);

  //	teardown
  free(table);
}


int main ()
{
	if (CU_initialize_registry() != CUE_SUCCESS)
	{
		printf("initial registry error!\n");
		exit(-1);
	}

	CU_pSuite suite = CU_add_suite("test suite ==== myTable", NULL, NULL);
	if (suite == NULL)
	{
		printf("suite add error!\n");
		CU_cleanup_registry();
		exit(-1);
	}

	//assert(CU_add_test(suite, "test func----hashFunc_MyTable()", hashFunc_ShouldWorkRight));
	//assert(CU_add_test(suite, "test func----makeMyBinder()", makeMyBinder_ShouldWorkRight));
	assert(CU_add_test(suite, "test func----MyTable_MakeEmptyTable()",
MyTable_MakeEmptyTable_ShouldWorkRight));
	assert(CU_add_test(suite, "test func----MyTable_Enter_()", MyTable_Enter_ShouldNotChangeOnInvalidParam));
	assert(CU_add_test(suite, "test func----MyTable_Enter_()", MyTable_Enter_ShouldChangeOnValidParam));
	assert(CU_add_test(suite, "test func----MyTable_Look_()", MyTable_Look_ShouldReturnNullOnInvalidParams));
	assert(CU_add_test(suite, "test func----MyTble_Look_()", MyTable_ShouldReturnRightOnValidParams));
	assert(CU_add_test(suite, "test func----MyTable_Pop_()", MyTable_Pop_ShouldReturnNULLOnEmptyTable));
	assert(CU_add_test(suite, "test func----MyTable_Pop_()", MyTable_Pop_ShouldReturnCorrectlyOnNonEmptyTable));
	assert(CU_add_test(suite, "test func----MyTable_Dump_()", MyTable_Dump_SeeOutPut));

	if (CU_basic_run_suite(suite) != CUE_SUCCESS)
	{
		printf("attempt to run, failed!\n");
		CU_cleanup_registry();
		exit(-1);
	}

	CU_cleanup_registry();
}
