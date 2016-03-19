#include "../makeMemory.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>

/*-------------------------test functions----------------------------*/

void registerMemoryType_ShouldReturnMinusOninvalidParam(void)
{
    //	fixture setup
    int invalid_type = -1;
    //	test
    int actual = registerMemoryType(invalid_type);
    //iverify
    CU_ASSERT_EQUAL(actual, -1);
}

void registerMemoryType_ShouldReturnPositiveOnSuccess(void)
{
    //	fixture setup
    int int_type = 1;
    //	test
    int actual = registerMemoryType(int_type);
    //	verify
    CU_ASSERT_EQUAL(actual, 1);
}

void registerMemoryType_ShouldReturnZeroOnSameType(void)
{
    //	fixture setup
    int new_type = 1234;
    int actual = registerMemoryType(new_type);
    CU_ASSERT_EQUAL(actual, 1);

    //	test
    actual = registerMemoryType(new_type);
    //	verify
    CU_ASSERT_EQUAL(actual, 0);
}

void registryMemoryType_ShouldRegisterOnExceedsSize(void)
{
    //	fixture setup
    int success = 1;
    for (unsigned i = 12; i < 23; ++i)
    {
	CU_ASSERT_EQUAL(registerMemoryType(i), success);
    }

    //	test and verify
    int initial_type = 12;
    CU_ASSERT_EQUAL(registerMemoryType(12), 0);
    //	should pass because former test also registered
    CU_ASSERT(getRegisteredTypeNum() > 10);
}

void registerMemoryType_ShouldInAscendentOrder(void)
{
    //	fxture setup and test
    int register_error = -1;
    CU_ASSERT_NOT_EQUAL_FATAL(registerMemoryType(1), register_error);
    CU_ASSERT_NOT_EQUAL_FATAL(registerMemoryType(2), register_error);
    CU_ASSERT_NOT_EQUAL_FATAL(registerMemoryType(3), register_error);

    //	verify
    for (unsigned i = 1; i < getRegisteredTypeNum(); ++i)
	CU_ASSERT(getRegisteredTypeArray()[i - 1] <
		  getRegisteredTypeArray()[i]);
}

void registerUniqueType_ShouldWorkRight(void)
{
    //	fixture setup and test function
    int unique_type = registerUniqueType();

    //	vieryf
    int expected_failure = -1;
    CU_ASSERT_NOT_EQUAL(unique_type, expected_failure);
}

void makeMemoryBlock_ShouldReturnNullOnNegativeBytes(void)
{
    //	fixture setup
    unsigned bytes = 2;
    int valid_type = 2;
    //	test
    void* ptr = makeMemoryBlock(bytes, valid_type);
    //	verify
    CU_ASSERT_NOT_EQUAL(ptr, NULL);
}

void makeMemoryBlock_ShouldSetHeadToFormer(void)
{
    //	fixture setup
    int memory_type = 1;
    MemoryChainPtr former = getMemoryChain();

    //	system under control
    CU_ASSERT_NOT_EQUAL(makeMemoryBlock(memory_type, 12), NULL);
    MemoryChainPtr current = getMemoryChain();

    //	verify
    CU_ASSERT_EQUAL(current->next, former);
}

void freeMemoryChain_ShoudSetHeadToNullAfterFree(void)
{
    //	fixture, test
    freeMemoryChain();
    //	verify
    CU_ASSERT_EQUAL(getMemoryChain(), NULL);
}

void freeMemoryChain_ShouldHaveNoSideEffectOnTwiceFree(void)
{
    //	fixture setup
    freeMemoryChain();

    MemoryChainPtr former_chain = getMemoryChain();
    unsigned former_type_num = getRegisteredTypeNum();
    const int* former_address = getRegisteredTypeArray();

    //	system under control
    freeMemoryChain();

    MemoryChainPtr current_chain = getMemoryChain();
    unsigned current_type_num = getRegisteredTypeNum();
    const int* current_address = getRegisteredTypeArray();

    //	verify
    CU_ASSERT_EQUAL(former_chain, current_chain);
    CU_ASSERT_EQUAL(former_type_num, current_type_num);
    CU_ASSERT_EQUAL(former_address, current_address);
}

void findMemoryBlock_ShouldReturnNullForNonePositiveParam(void)
{
    //	fixture setup
    int invalid_type_one = 0, invalid_type_two = -1;
    //	system under control
    void* actual_one = findMemoryBlock(invalid_type_one);
    void* actual_two = findMemoryBlock(invalid_type_two);
    //	verify
    void* expected = NULL;
    CU_ASSERT_EQUAL(actual_one, expected);
    CU_ASSERT_EQUAL(actual_two, expected);
}

void findMemoryBlock_ShouldReturnNullForTypeNotContained(void)
{
    //	fixture setup
    int type_not_contained = 12345;
    //	system under control
    void* actual = findMemoryBlock(type_not_contained);
    //	verify
    void* expected = NULL;
    CU_ASSERT_EQUAL(actual, expected);
}

void findMemoryBlock_ShouldFindNextMemoryBlockOfType(void)
{
    //	fixture setup
    int type = 321;

    void* expected_one = makeMemoryBlock(12, type);
    CU_ASSERT_NOT_EQUAL_FATAL(expected_one, NULL);

    void* expected_two = makeMemoryBlock(12, type);
    CU_ASSERT_NOT_EQUAL_FATAL(expected_two, NULL);

    void* expected_empty = NULL;

    //	system under control
    void* actual_two = findMemoryBlock(type);
    void* actual_one = findMemoryBlock(type);
    void* actual_empty = findMemoryBlock(type);

    //	verify
    CU_ASSERT_EQUAL(actual_one, expected_one);
    CU_ASSERT_EQUAL(actual_two, expected_two);
    CU_ASSERT_EQUAL(actual_empty, expected_empty);
}

/*-------------------------------------------------------------------*/

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
	{ "test func----registryMemoryType()", registerMemoryType_ShouldReturnMinusOninvalidParam },
	{ "test func----registerMemoryType()", registerMemoryType_ShouldReturnPositiveOnSuccess },
	{ "test func----registerMemoryType()", registerMemoryType_ShouldReturnZeroOnSameType },
	{ "test func----registerMemoryType()", registryMemoryType_ShouldRegisterOnExceedsSize },
	{ "test func----registerMemoryType()", registerMemoryType_ShouldInAscendentOrder },
	{ "test func----registerUniqueType()", registerUniqueType_ShouldWorkRight },
	{ "test func----makeMemoryBlock()", makeMemoryBlock_ShouldReturnNullOnNegativeBytes },
	{ "test func----makeMemoryBlock()", makeMemoryBlock_ShouldSetHeadToFormer },
	{ "test func----freeMemoryChain()", freeMemoryChain_ShoudSetHeadToNullAfterFree },
	{ "test func----freeMemoryChain()", freeMemoryChain_ShouldHaveNoSideEffectOnTwiceFree },
	{ "test func----findMemoryBlock()", findMemoryBlock_ShouldReturnNullForNonePositiveParam },
	{ "test func----findMemoryBlock()", findMemoryBlock_ShouldReturnNullForTypeNotContained },
	{ "test func----findMemoryBlock()", findMemoryBlock_ShouldFindNextMemoryBlockOfType }
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
