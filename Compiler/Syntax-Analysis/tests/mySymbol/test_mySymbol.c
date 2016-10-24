#include "../../../TestHelper/testHelper.h"

#include "../../mySymbol.h"

//////////////////////////////////////////////////////////////////////

void test_MySymbolInvalidSymbol_PassNull_ReturnTrue(void)
{
    mySymbol nullSymbol = NULL;

    bool result = MySymbol_InvalidSymbol(nullSymbol);

    CU_ASSERT_EQUAL(result, true);
}

void test_MySymbolInvalidSymbol_PassValid_ReturnFalse(void)
{
    mySymbol validSymbol = MySymbol_MakeSymbol("as");

    bool result = MySymbol_InvalidSymbol(validSymbol);

    CU_ASSERT_EQUAL(result, false);
}

/////////////////////////////////////////

void test_MySymbolIsSymbolEqual_NotEqualSymbol_ReturnFalse(void)
{
    mySymbol symbolOne = MySymbol_MakeSymbol("symbolOne");
    mySymbol symbolTwo = MySymbol_MakeSymbol("symbolTwo");

    bool result = MySymbol_IsSymbolEqual(symbolOne, symbolTwo);

    CU_ASSERT_EQUAL(result, false);
}

void test_MySymbolIsSymbolEqual_SameSymbol_ReturnTrue(void)
{
    mySymbol symbol = MySymbol_MakeSymbol("symbol");

    bool result = MySymbol_IsSymbolEqual(symbol, symbol);

    CU_ASSERT_EQUAL(result, true);
}

/////////////////////////////////////////

void test_MySymbolMakeSymbol_PassNullOrEmptyString_ReturnNull(void)
{
    myString nullString = NULL;
    myString emptyString = "";

    mySymbol nullResult = MySymbol_MakeSymbol(nullString);
    mySymbol emptyResult = MySymbol_MakeSymbol(emptyString);

    CU_ASSERT_EQUAL(nullResult, NULL);
    CU_ASSERT_EQUAL(emptyResult, NULL);
}

void test_MySymbolMakeSymbol_PassValidString_MakeOne(void)
{
    myString validString = "valid";

    mySymbol returnSymbol = MySymbol_MakeSymbol(validString);

    CU_ASSERT_NOT_EQUAL(returnSymbol, NULL);
}

void test_MySymbolMakeSymbol_PassSameString_ReturnExisted(void)
{
    myString string = "string";
    mySymbol existedSymbol = MySymbol_MakeSymbol(string);

    mySymbol resultSymbol = MySymbol_MakeSymbol(string);

    CU_ASSERT_EQUAL(resultSymbol, existedSymbol);
}

////////////////////////////////////////////////

void test_MySymbolGetName_ByDefault_GetStringNameOfSymbol(void)
{
    myString stringMadeFrom = "string";
    mySymbol symbol = MySymbol_MakeSymbol(stringMadeFrom);

    myString stringGot = MySymbol_GetName(symbol);

    CU_ASSERT_EQUAL(stringMadeFrom, stringGot);
}

/////////////////////////////////////////////////

void test_MySymbolEnter_LegalParams_ReturnTrueAndAddOne(void)
{
    myTable table = MySymbol_MakeNewTable();
    mySymbol symbol = MySymbol_MakeSymbol("as");
    void* valueEntered = (void*)121;

    MySymbol_Enter(table, symbol, valueEntered);

    void* valueGot = MySymbol_Look(table, symbol);
    CU_ASSERT_EQUAL(valueGot, valueEntered);
}

///////////////////////////////////////////////////////////

void test_MySymbolLook_PassSymbolNotEntered_ReturnNull(void)
{
    myTable table = MySymbol_MakeNewTable();
    mySymbol notEnteredSymbol = MySymbol_MakeSymbol("not entered");

    void* result = MySymbol_Look(table, notEnteredSymbol);

    CU_ASSERT_EQUAL(result, NULL);
}

void test_MySymbolLook_EnterSome_CanGetAny(void)
{
    myTable table = MySymbol_MakeNewTable();
    mySymbol symbolOne = MySymbol_MakeSymbol("one");
    mySymbol symbolTwo = MySymbol_MakeSymbol("two");
    void* valueOne = (void*)12;
    void* valueTwo = (void*)34;
    MySymbol_Enter(table, symbolOne, valueOne);
    MySymbol_Enter(table, symbolTwo, valueTwo);

    void* valueOneGot = MySymbol_Look(table, symbolOne);
    void* valueTwoGot = MySymbol_Look(table, symbolTwo);

    CU_ASSERT_EQUAL(valueOne, valueOneGot);
    CU_ASSERT_EQUAL(valueTwo, valueTwoGot);
}

///////////////////////////////////////////////////////////

void test_MySymbolSet_ValidSet_TheOneSetted(void)
{
    myTable table = MySymbol_MakeNewTable();
    mySymbol symbol = MySymbol_MakeSymbol("symbol");
    void* valueBefore = (void*)12;
    MySymbol_Enter(table, symbol, valueBefore);

    void* valueSetted = (void*)12;
    MySymbol_Set(table, symbol, valueSetted);

    void* valueGot = MySymbol_Look(table, symbol);
    CU_ASSERT_EQUAL(valueGot, valueSetted);    
}

///////////////////////////////////////////////////////////

void test_MySymbolBeginScope_AddSomeAfterScope_CanFindAny(void)
{
    myTable table = MySymbol_MakeNewTable();
    MySymbol_BeginScope(table);
    mySymbol symbolOne = MySymbol_MakeSymbol("one");
    mySymbol symbolTwo = MySymbol_MakeSymbol("two");
    void* valueOne = (void*)12;
    void* valueTwo = (void*)23;
    MySymbol_Enter(table, symbolOne, valueOne);
    MySymbol_Enter(table, symbolTwo, valueTwo);

    void* valueOneGot = MySymbol_Look(table, symbolOne);
    void* valueTwoGot = MySymbol_Look(table, symbolTwo);

    CU_ASSERT_EQUAL(valueOneGot, valueOne);
    CU_ASSERT_EQUAL(valueTwoGot, valueTwo); 
}

void test_MySymbolBeginScope_AfterCalled_BindingsInsideFormerScopeCanBeFound(void)
{
    myTable table = MySymbol_MakeNewTable();
    mySymbol symbolBeforeScope = MySymbol_MakeSymbol("symbol");
    void* value = (void*)112;
    MySymbol_Enter(table, symbolBeforeScope, value);

    MySymbol_BeginScope(table);

    void* valueGot = MySymbol_Look(table, symbolBeforeScope);

    CU_ASSERT_EQUAL(valueGot, value); 
}

void test_MySymbolBeginScope_EnterOneWithSameSymbolAfterCalled_OnlyFindLaterOne(void)
{
    myTable table = MySymbol_MakeNewTable();
    mySymbol symbol = MySymbol_MakeSymbol("symbol");
    void* valueBefore = (void*)12;
    void* valueAfter = (void*)23;
    MySymbol_Enter(table, symbol, valueBefore);

    MySymbol_BeginScope(table);
    MySymbol_Enter(table, symbol, valueAfter);

    void* valueGot = MySymbol_Look(table, symbol);
    CU_ASSERT_EQUAL(valueGot, valueAfter);
}

void test_MySymbolBeginScope_BeginTwoScope_OnlyFindTheLastwithSameSymbol(void)
{
    myTable table = MySymbol_MakeNewTable();
    mySymbol symbol = MySymbol_MakeSymbol("symbol");
    void* valueBefore = (void*)12;
    void* valueAfter = (void*)23;
    MySymbol_Enter(table, symbol, valueBefore);
    MySymbol_BeginScope(table);
    MySymbol_Enter(table, symbol, valueBefore);

    MySymbol_BeginScope(table);
    MySymbol_Enter(table, symbol, valueAfter);

    void* valueGot = MySymbol_Look(table, symbol);
    CU_ASSERT_EQUAL(valueGot, valueAfter);
}

////////////////////////////////////////////////////////////

void test_MySymbolEndScope_AfterCalled_BindingsInThatScopeCanNotBeFound(void)
{
    myTable table = MySymbol_MakeNewTable();
    mySymbol symbolOne = MySymbol_MakeSymbol("symbolOne");
    mySymbol symbolTwo = MySymbol_MakeSymbol("symbolTwo");
    MySymbol_BeginScope(table);
    MySymbol_Enter(table, symbolOne, (void*)12);
    MySymbol_Enter(table, symbolTwo, (void*)23);

    MySymbol_EndScope(table);

    mySymbol valueOneGot = MySymbol_Look(table, symbolOne);
    mySymbol valueTwoGot = MySymbol_Look(table, symbolTwo);
    CU_ASSERT_EQUAL(valueOneGot, NULL);
    CU_ASSERT_EQUAL(valueTwoGot, NULL);
}

void test_MySymbolEndScope_AfterCalled_SameBindingInFormerScopeCanBeFound(void)
{
    myTable table = MySymbol_MakeNewTable();
    mySymbol sameSymbol = MySymbol_MakeSymbol("symbol");
    void* valueBefore = (void*)12;
    void* valueAfter = (void*)34;
    MySymbol_Enter(table, sameSymbol, valueBefore);
    MySymbol_BeginScope(table);
    MySymbol_Enter(table, sameSymbol, valueAfter);

    MySymbol_EndScope(table);
    void* valueGot = MySymbol_Look(table, sameSymbol);

    CU_ASSERT_EQUAL(valueGot, valueBefore);
}

void test_MySymbolEndScope_CalledWithNoCorrespondingBeingScope_ActAsDefaultScope(void)
{
    myTable table = MySymbol_MakeNewTable();
    mySymbol symbolOne = MySymbol_MakeSymbol("symbolOne");
    mySymbol symbolTwo = MySymbol_MakeSymbol("symbolTwo");
    MySymbol_Enter(table, symbolOne, (void*)12);
    MySymbol_Enter(table, symbolTwo, (void*)23);

    MySymbol_EndScope(table);

    mySymbol valueOneGot = MySymbol_Look(table, symbolOne);
    mySymbol valueTwoGot = MySymbol_Look(table, symbolTwo);
    CU_ASSERT_EQUAL(valueOneGot, NULL);
    CU_ASSERT_EQUAL(valueTwoGot, NULL);
}

//////////////////////////////////////////////////////////////////////

int main ()
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return -1;

    CU_TestInfo tests[] = {
        { "", test_MySymbolInvalidSymbol_PassNull_ReturnTrue },
        { "", test_MySymbolInvalidSymbol_PassValid_ReturnFalse },

        { "", test_MySymbolIsSymbolEqual_NotEqualSymbol_ReturnFalse },
        { "",  test_MySymbolIsSymbolEqual_SameSymbol_ReturnTrue},

        { "", test_MySymbolMakeSymbol_PassNullOrEmptyString_ReturnNull },
        { "", test_MySymbolMakeSymbol_PassValidString_MakeOne },
        { "", test_MySymbolMakeSymbol_PassSameString_ReturnExisted },

        { "", test_MySymbolGetName_ByDefault_GetStringNameOfSymbol },

        { "", test_MySymbolEnter_LegalParams_ReturnTrueAndAddOne },

        { "", test_MySymbolLook_PassSymbolNotEntered_ReturnNull },
        { "", test_MySymbolLook_EnterSome_CanGetAny },

        { "", test_MySymbolSet_ValidSet_TheOneSetted },

        { "", test_MySymbolBeginScope_AfterCalled_BindingsInsideFormerScopeCanBeFound },
        { "", test_MySymbolBeginScope_AddSomeAfterScope_CanFindAny },
        { "", test_MySymbolBeginScope_EnterOneWithSameSymbolAfterCalled_OnlyFindLaterOne },
        { "", test_MySymbolBeginScope_BeginTwoScope_OnlyFindTheLastwithSameSymbol },

        { "", test_MySymbolEndScope_AfterCalled_BindingsInThatScopeCanNotBeFound },
        { "", test_MySymbolEndScope_AfterCalled_SameBindingInFormerScopeCanBeFound },
        { "", test_MySymbolEndScope_CalledWithNoCorrespondingBeingScope_ActAsDefaultScope }
        
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return -1;

    if (!runTests(&suite))  return -1;

    freeMemoryChain();
    return 0;
}
