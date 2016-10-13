#include "../testHelper.h"

#include "../../makeMemory.h"
#include "../../myError.h"

///////////////////////////////////////////////////////////////////
//                          fast tests                          //

void testPushErrorCodeWithMsg_ByDefault_PushedOne(void)
{
    unsigned expectedErrorCode = ERROR_SUBSCRIPT_NOT_INT;
    unsigned countBeforePush = MyError_getErrorCount(); 

    MyError_pushErrorCodeWithMsg(expectedErrorCode, "msg");

    unsigned countAfterPush = MyError_getErrorCount();
    CU_ASSERT_EQUAL(countAfterPush - countBeforePush, 1);
}

void testGetErrorCode_BeDefault_GetWhatAdded(void)
{
    unsigned codeToPush = ERROR__ARITHMETIC__EXPRESSION_TYPE_NOT_INT;
    MyError_pushErrorCode(codeToPush);

    unsigned codePos = MyError_getErrorCount() - 1;
    unsigned codeGet = MyError_getErrorCode(codePos);

    CU_ASSERT_EQUAL(codeGet, codeToPush);
}


void testGetErrorMsg_ByDefault_GetWhatAdded(void)
{
    char*   msgToPush = "haha";
    unsigned codeToPush = ERROR_NO; 
    MyError_pushErrorCodeWithMsg(codeToPush, msgToPush);

    unsigned msgPos = MyError_getErrorCount() - 1; 
    char* msgGet = MyError_getErrorMsg(msgPos);

    CU_ASSERT_EQUAL(msgGet, msgToPush);
}

void testClearErrors_ByDefault_NoErrorsAfterClear(void)
{
    MyError_pushErrorCode(ERROR_NO);

    MyError_clearErrors();

    unsigned countAfterClear = MyError_getErrorCount();
    unsigned zeroError = 0;
    CU_ASSERT_EQUAL(countAfterClear, zeroError);
}

void testIsErrorSet_AfterPush_ErrorSetted(void)
{
    MyError_pushErrorCode(ERROR_NO);

    bool errorFlagAfterPush = MyError_isErrorSet();
    bool errorSetted = true;

    CU_ASSERT_EQUAL(errorFlagAfterPush, errorSetted);
}

void testPrintErrors_AfterPrint_ErrorsCleared(void)
{
    MyError_pushErrorCode(ERROR_NO);
    MyError_pushErrorCodeWithMsg(ERROR_NO, "hahahaha");

    MyError_printErrors();

    unsigned errorCountAfterPrint = MyError_getErrorCount();
    CU_ASSERT_EQUAL(errorCountAfterPrint, 0);
}

///////////////////////////////////////////////////////////////////////////
//                          slow tests                                  //

void testPushErrorCodeWithMsg_ErrorStackFullFilled_PushedOne(void)
{
    int pushNum = MyError_getErrorCapacity() + 1;
    unsigned countBeforePush = MyError_getErrorCount();
    unsigned pushedErrorCode = ERROR__ARITHMETIC__EXPRESSION_TYPE_NOT_INT;
    char*    pushedErrorMsg = "test";

    for (int i = 0; i < pushNum; ++i)
        MyError_pushErrorCodeWithMsg(pushedErrorCode, pushedErrorMsg);

    unsigned countAfterPush = MyError_getErrorCount();
    CU_ASSERT_EQUAL(countAfterPush - countBeforePush, pushNum);
    for (int i = 0; i < pushNum; ++i)
    {
        CU_ASSERT_EQUAL(MyError_getErrorCode(i), pushedErrorCode);
        CU_ASSERT_EQUAL(MyError_getErrorMsg(i), pushedErrorMsg);
    }
}

///////////////////////////////////////////////////////////////////////////

int main (int argc, char* argv[])
{
    CU_pSuite suite;
    if (!initialTests(&suite))
        return -1;

    CU_TestInfo tests[] = {
        { "", testPushErrorCodeWithMsg_ByDefault_PushedOne },
        { "", testGetErrorCode_BeDefault_GetWhatAdded },
        { "", testGetErrorMsg_ByDefault_GetWhatAdded },
        { "", testClearErrors_ByDefault_NoErrorsAfterClear },
        { "", testIsErrorSet_AfterPush_ErrorSetted },
        { "", testPrintErrors_AfterPrint_ErrorsCleared },
        { "", testPushErrorCodeWithMsg_ErrorStackFullFilled_PushedOne }
    };
    
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return -1;

    if (!runTests(&suite))
        return -1;

    freeMemoryChain();
    return 0;
}