#include "../myCheckedMalloc.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

void myCheckedAllocate_ShouldNotFail(void)
{
  //	should fails only for little memory left
  void* ptr = myCheckedMalloc(10);
  CU_ASSERT_NOT_EQUAL(ptr, NULL);
}

int main ()
{
  //	initial test registry
  if (CU_initialize_registry() == CUE_NOMEMORY)
  {
	printf("no enough memory!\n");
	exit(-1);
  }

  //	add suits to the former registry
  CU_pSuite suitePtr = CU_add_suite("myCheckedAllocate.c test suite", NULL, NULL);
  if (suitePtr == NULL)
  {
	printf("suite add failed!\n");
	exit(-1);
  }

  //	add tests to the former suite
  CU_pTest testPtr = CU_add_test(suitePtr, "test func----myCheckedAllocate(size_t sizeInBytes)", myCheckedAllocate_ShouldNotFail);
  if (testPtr == NULL)
  {
	printf("test add failed!\n");
	exit(-1);
  }

  //	run needed tests
  if (CU_basic_run_suite(suitePtr) != CUE_SUCCESS)
  {
	printf("suite failed to run!\n");
	exit(-1);
  }

  //	cleanup registry
  CU_cleanup_registry();
  return 0;
}
