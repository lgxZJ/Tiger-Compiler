#include "../mySymbol.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/*-------------------------------------------------------------*/
/*	NOTE:
 *		To use following tests, uncomment following blocks,
 *	uncomment following prototpyes and remove 'static' keyword
 *	in mySymbol.c file.
 *		After test, re-write 'static' back.
 *
 *	Prototypes:
 *	mySymbol makeSymbol_(myString name, mySymbol next);
 *	int hashFunc_MySymbol(char* c);
 */


void makeSymbol__ShouldReturnNullOnInvalidParams(void)
{
  //	fixture setup and system under control
  mySymbol actual = (mySymbol)makeSymbol_((mySymbol)NULL, NULL);
  mySymbol expected = NULL;
  //	verify
  CU_ASSERT_EQUAL(actual, expected);

  //	system under control
  actual = (mySymbol)makeSymbol_("", NULL);
  //	verify
  CU_ASSERT_EQUAL(actual, expected);

  //	teardown (nothing)
}

void makeSymbol__ShouldReturnRightOnValidParams(void)
{
  //	fixture setup and system under control
  mySymbol actual = (mySymbol)makeSymbol_("symbol", NULL);
  
  //	verify
  CU_ASSERT_NOT_EQUAL(actual, NULL);
  CU_ASSERT_EQUAL(strcmp(actual->name, "symbol"), 0);
  CU_ASSERT_EQUAL(actual->next, NULL);

  //	teardown (nothing)
}

void hashFunc_MySymbol_ShouldReturnNullOnInvalidParams(void)
{
  //	fixture setup and test function (NULL param)
  int actual = hashFunc_MySymbol((char*)NULL);
  int expected = -1;

  //	verify
  CU_ASSERT_EQUAL(actual, -1)

  //	test function (null string)
  actual = hashFunc_MySymbol("");

  //	verify
  CU_ASSERT_EQUAL(actual, expected);

  //	teardown 
  //	nothing to do
}

void hashFunc_MySymbol_ShouldNotReturnNullOnVaildParams(void)
{
  //	fixture setup, test, and verify
  CU_ASSERT_NOT_EQUAL(hashFunc_MySymbol("valid"), -1);
}

void MySymbol_MakeSymbol_ShouldReturnNullOnInvalidParam(void)
{
  //	fixture setup and function test (NULL param)
  mySymbol expected = NULL;
  mySymbol actual = MySymbol_MakeSymbol((myString)NULL);
  //	verify
  CU_ASSERT_EQUAL(actual, expected);

  //	function test (null string)
  actual = MySymbol_MakeSymbol("");
  //	verify
  CU_ASSERT_EQUAL(actual, expected);
}

void MySymbol_MakeSymbol_ShouldMakeSymbolOnValidParam(void)
{
  //	fixture setup
  mySymbol actual;

  //	system under control (fisrt make)
  actual = MySymbol_MakeSymbol("str1");

  //	verify
  CU_ASSERT_EQUAL(strcmp(actual->name, "str1"), 0);
  CU_ASSERT_EQUAL(actual->next, NULL);
 
  //	system under control (second make with different str)
  actual = MySymbol_MakeSymbol("str2");
  //	verify
  CU_ASSERT_EQUAL(strcmp(actual->name, "str2"), 0);
  CU_ASSERT_EQUAL(strcmp(actual->next->name, "str1"), 0);

  //	teardown (NOTE:
  //	pointers can not be freed, or will cause an error of infinite loop in following tests
}

void MySymbol_MakeSymbol_ShouldNotMakeOnSameStringParam(void)
{
  //	fixture setup
  mySymbol actual, expected;

  //	guard condition
  expected = MySymbol_MakeSymbol("same");
  CU_ASSERT_NOT_EQUAL(expected, NULL);
  //	system under control (same input string)
  actual = MySymbol_MakeSymbol("same");
  
  //	verify
  CU_ASSERT_EQUAL(actual, expected);
}

void MySymbol_GetName_ShouldWorkRight(void)
{
  //	simple test
  mySymbol symbol = MySymbol_MakeSymbol("hot");
  CU_ASSERT_EQUAL(strcmp(MySymbol_GetName(symbol), "hot"), 0);
}

void MySymbol_MakeNewTable_ShoudReturnSymbolUnlessNoMemoryLeft(void)
{
  //    simple test
  CU_ASSERT_NOT_EQUAL(MySymbol_MakeNewTable(), NULL);
}

void MySymbol_Enter_ShouldReturnFalseOnInvalidParams(void)
{
  //	fixture setup
  bool actual, expected = false;
  myTable table = MySymbol_MakeNewTable();

  //	system under control (NULL table)
  actual = MySymbol_Enter((myTable)NULL, MySymbol_MakeSymbol("symbol"), "value");
  //	verify
  CU_ASSERT_EQUAL(actual, expected);

  //	system under control (NULL symbol)
  actual = MySymbol_Enter(table, MySymbol_MakeSymbol(NULL), "value");
  //	verify
  CU_ASSERT_EQUAL(actual, expected);

  /*
  //	system under control (intended "" symbol)
  actual = MySymbol_Enter(table, (mySymbol)"", "value");
  //	verify
  CU_ASSERT_EQUAL(actual, expected);
  */

  //	teardown (nothing)
}

void MySymbol_Enter_ShoudReturnTrueOnValidParams()
{
  //	fixture setup
  myTable table = MySymbol_MakeNewTable();
  mySymbol symbol = MySymbol_MakeSymbol("key");

  //	gurad condition
  CU_ASSERT_NOT_EQUAL(table, NULL);
  CU_ASSERT_NOT_EQUAL(symbol, NULL);

  //	system under control
  bool actual = MySymbol_Enter(table, symbol, "value");
  bool expected = true;

  //	verify
  CU_ASSERT_EQUAL(actual, expected);
}

void MySymbol_Look_ShouldReturnNullOnInvalidParams()
{
  //	fixture setup
  myTable table = MySymbol_MakeNewTable();
  CU_ASSERT_NOT_EQUAL(table, (myTable)NULL);

  mySymbol symbol = MySymbol_MakeSymbol("symbol");
  CU_ASSERT_NOT_EQUAL(symbol, (mySymbol)NULL);

  void* expected = NULL, *actual;

  //	system under control (NULL table)
  actual = MySymbol_Look((myTable)NULL, symbol);
  //	verify
  CU_ASSERT_EQUAL(actual, expected);

  //	system under control (NULL symbol)
  actual = MySymbol_Look(table, (mySymbol)NULL);
  //	verify
  CU_ASSERT_EQUAL(actual, expected);

  //	system under control (with both invalid params)
  actual = MySymbol_Look((myTable)NULL, (mySymbol)NULL);
  //	verify
  CU_ASSERT_EQUAL(actual, expected);
}

void MySymbol_Look_ShouldReturnValueOnValidInput()
{
  //	fixture setup
  myTable table = MySymbol_MakeNewTable();
  CU_ASSERT_NOT_EQUAL(table, (myTable)NULL);

  mySymbol symbol = MySymbol_MakeSymbol("symbol");
  CU_ASSERT_NOT_EQUAL(symbol, (mySymbol)NULL);

  void* actual, *expected;
  int value = 1;

  //	system under control (empty table)
  actual = MySymbol_Look(table, symbol);
  //	verify
  CU_ASSERT_EQUAL(actual, (void*)NULL);

  //	system under control (look after first enter)
  CU_ASSERT_EQUAL(MySymbol_Enter(table, symbol, &value), true);
  actual = MySymbol_Look(table, symbol);
  //	verify
  CU_ASSERT_EQUAL(actual, &value);
}

void MySymbol_Look_CanLookBothBinderAfterSecondEnter()
{
  //	fixture setup
  myTable table = MySymbol_MakeNewTable();
  CU_ASSERT_NOT_EQUAL(table, (myTable)NULL);

  int value1 = 1, value2 = 2;

  mySymbol symbol1 = MySymbol_MakeSymbol("symbol1");
  CU_ASSERT_NOT_EQUAL(symbol1, (mySymbol)NULL);
  CU_ASSERT_EQUAL(MySymbol_Enter(table, symbol1, &value1), true);

  mySymbol symbol2 = MySymbol_MakeSymbol("symbol2");
  CU_ASSERT_NOT_EQUAL(symbol2, (mySymbol)NULL);
  CU_ASSERT_EQUAL(MySymbol_Enter(table, symbol2, &value2), true);

  //	system under control
  void* actual = MySymbol_Look(table, symbol1);
  CU_ASSERT_EQUAL(actual, &value1);

  actual = MySymbol_Look(table, symbol2);
  CU_ASSERT_EQUAL(actual, &value2);
}

void MySymbol_BeginScope_ShouldReturnFalseOnInvalidParam()
{
  //	no fixture setup

  //	system under control
  bool result = MySymbol_BeginScope((myTable)NULL);

  //	verify
  CU_ASSERT_EQUAL(result, false);
}

void MySymbol_BeginScope_ShouldReturnTrueOnValidParam()
{
  //	fixture setup
  myTable table = MySymbol_MakeNewTable();
  CU_ASSERT_NOT_EQUAL(table, NULL);

  //	system under control
  bool actual = MySymbol_BeginScope(table);

  //	verify
  CU_ASSERT_EQUAL(actual, true);
  CU_ASSERT_EQUAL(strcmp(MySymbol_GetName(table->top), "scope"), 0);
}

void MyTable_EndScope_ShouldDoNothingOnInvalidParam()
{
  //	fixture setup
  myTable table = MySymbol_MakeNewTable();
  CU_ASSERT_NOT_EQUAL(table, (myTable)NULL);
  CU_ASSERT_EQUAL(MySymbol_BeginScope(table), true);
		  
  //	system under control
  MySymbol_EndScope((myTable)NULL);

  //	verify
  CU_ASSERT_EQUAL(strcmp(MySymbol_GetName(table->top), "scope"), 0);
}

void MyTable_EndScope_ShouldRemoveBindersWithinCurrentScopeOnValidParam()
{
  //	fixture setup
  myTable table = MySymbol_MakeNewTable();
  CU_ASSERT_NOT_EQUAL(table, (myTable)NULL);

  mySymbol symbol = MySymbol_MakeSymbol("symbol");
  CU_ASSERT_NOT_EQUAL(symbol, (mySymbol)NULL);

  //	enter symbol with inside last scope
  CU_ASSERT_EQUAL(MySymbol_Enter(table, symbol, "value"), true);

  //	begin new scope
  CU_ASSERT_EQUAL(MySymbol_BeginScope(table), true);

  //	insert another symbol
  mySymbol symbol_new = MySymbol_MakeSymbol("symbol_new");
  CU_ASSERT_NOT_EQUAL(symbol_new, NULL);

  //	system under control
  MySymbol_EndScope(table);

  //	verify
  CU_ASSERT_EQUAL(strcmp(table->top, "symbol"), 0);
}

void invalidSymbol_ShouldWorkRight(void)
{
    //	fixture setup
    mySymbol symbol_null = MySymbol_MakeSymbol("");
    mySymbol symbol_valid = MySymbol_MakeSymbol("valid");

    //	system under control
    CU_ASSERT_EQUAL(invalidSymbol((mySymbol)NULL), true);
    CU_ASSERT_EQUAL(invalidSymbol(symbol_null), true);
    CU_ASSERT_EQUAL(invalidSymbol(symbol_valid), false);
}



/*-------------------------------------------------------------*/

int main (int argc, char *argv[])
{
  if (CU_initialize_registry() != CUE_SUCCESS)
  {
    printf("initialization error!\n");
    exit(-1);
  }

  CU_pSuite suite = CU_add_suite("only suite", NULL, NULL);
  if (suite == NULL)
  {
    printf("attempt to add suite, error!\n");
    exit(-1);
  }

  //	add tests
  assert(CU_add_test(suite, "test func----makeSymbol_()", makeSymbol__ShouldReturnNullOnInvalidParams));
  assert(CU_add_test(suite, "test func----makeSymbol_()", makeSymbol__ShouldReturnRightOnValidParams));
  assert(CU_add_test(suite, "test func----hashFunc_MySymbol()", hashFunc_MySymbol_ShouldReturnNullOnInvalidParams));
  assert(CU_add_test(suite, "test func--hashFunc_MySymbol()", hashFunc_MySymbol_ShouldNotReturnNullOnVaildParams));
  assert(CU_add_test(suite, "test func----MySymbol_MakeSymbol()", MySymbol_MakeSymbol_ShouldReturnNullOnInvalidParam));
  assert(CU_add_test(suite, "test func----MySymbol_MakeSymbol()", MySymbol_MakeSymbol_ShouldMakeSymbolOnValidParam));
  assert(CU_add_test(suite, "test func----MySymbol_MakeSymbol()", MySymbol_MakeSymbol_ShouldNotMakeOnSameStringParam));
  assert(CU_add_test(suite, "test func----MySymbol_GetName()", MySymbol_GetName_ShouldWorkRight));
  assert(CU_add_test(suite, "test func----MySymbol_MakeNewTable()", MySymbol_MakeNewTable_ShoudReturnSymbolUnlessNoMemoryLeft));
  assert(CU_add_test(suite, "test func----MySymbol_Enter()", MySymbol_Enter_ShouldReturnFalseOnInvalidParams));
  assert(CU_add_test(suite, "test func----MySymbol_Enter()", MySymbol_Enter_ShoudReturnTrueOnValidParams));
  assert(CU_add_test(suite, "test func----MySymbol_Look()", MySymbol_Look_ShouldReturnNullOnInvalidParams));
  assert(CU_add_test(suite, "test func----MySymbol_Lool()", MySymbol_Look_ShouldReturnValueOnValidInput));
  assert(CU_add_test(suite, "test func----MySymbol_Look()", MySymbol_Look_CanLookBothBinderAfterSecondEnter));
  assert(CU_add_test(suite, "test func----MySymbol_BeginScope()", MySymbol_BeginScope_ShouldReturnFalseOnInvalidParam));
  assert(CU_add_test(suite, "test func----MySymbol_BeginScope()", MySymbol_BeginScope_ShouldReturnTrueOnValidParam));
  assert(CU_add_test(suite, "test func---MySymbol_EndScope()", MyTable_EndScope_ShouldDoNothingOnInvalidParam));
  assert(CU_add_test(suite, "test func---invalidSymbol()", invalidSymbol_ShouldWorkRight));

  if (CU_basic_run_suite(suite) != CUE_SUCCESS)
  {
    printf("attempt to run suite, error!\n");
    exit(-1);
  }

  CU_cleanup_registry();
  freeMemoryChain();
  return 0;
}
