#include "myEnvironment.h"
#include "typeMaker.h"

#include <assert.h> //  for assert()
#include <stddef.h> //  for NULL definition



myVarAndFuncEntry myEnvironment_makeVarEntry(myType type)
{
    myVarAndFuncEntry entry = 
        makeMemoryBlock(sizeof(*entry), MEMORY_TYPE_NONE);
    assert(entry);

    entry->kind = VarEntry;
    entry->u.varEntry.type = type;

    return entry;
}

myVarAndFuncEntry myEnvironment_makeFuncEntry(
    myTypeList formalParamTypes, myType returnType)
{
    myVarAndFuncEntry entry =
        makeMemoryBlock(sizeof(*entry), MEMORY_TYPE_NONE);
    assert(entry);

    entry->kind = FuncEntry;
    entry->u.funcEntry.formalParamTypes = formalParamTypes;
    entry->u.funcEntry.returnType = returnType;

    return entry;
}

bool myEnvironment_isVarEntry(myVarAndFuncEntry entry)
{
    return entry->kind == VarEntry; 
}

bool myEnvironment_isFuncEntry(myVarAndFuncEntry entry)   
{
    return entry->kind == FuncEntry; 
}


void MyEnvironment_addNewVarOrFunc(
    myTable varAndFuncEnv, mySymbol entryName, myVarAndFuncEntry newEntry)
{
    MySymbol_Enter(varAndFuncEnv, entryName, newEntry);
}

void MyEnvironment_addNewType(
    myTable typeEnv, mySymbol typeName, myType newType)
{
    MySymbol_Enter(typeEnv, typeName, newType);
}

//  NOTE:   entry must has been already added
myVarAndFuncEntry MyEnvironment_getVarOrFuncFromName(
    myTable varAndFuncEnv, mySymbol varOrFuncName)
{
    return (myVarAndFuncEntry)MySymbol_Look(varAndFuncEnv, varOrFuncName);
}

//  NOTE:   type must has been already added
myType MyEnvironment_getTypeFromName(
    myTable typeEnv, mySymbol typeName)
{
    return (myType)MySymbol_Look(typeEnv, typeName);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/* base environment where contains predefiend functions in tiger language */
myTable myEnvironment_BaseVarAndFunc(void)
{
    myTable table = MySymbol_MakeNewTable();
    assert (table);

    //  add `print` Func
    myVarAndFuncEntry entry = myEnvironment_makeFuncEntry(
        makeType_TypeList(makeType_String(), NULL),
        makeType_NoReturn());
    MySymbol_Enter(table, MySymbol_MakeSymbol("print"), entry);

    //  add `flush` Func
    entry = myEnvironment_makeFuncEntry(
        makeType_TypeList(NULL, NULL),
        makeType_NoReturn());
    MySymbol_Enter(table, MySymbol_MakeSymbol("flush"), entry);


    //  add `getchar` Func
    entry = myEnvironment_makeFuncEntry(
        makeType_TypeList(NULL, NULL),
        makeType_String());
    MySymbol_Enter(table, MySymbol_MakeSymbol("getchar"), entry);

    //  add `ord` Func
    entry = myEnvironment_makeFuncEntry(
        makeType_TypeList(makeType_String(), NULL),
        makeType_Int());
    MySymbol_Enter(table, MySymbol_MakeSymbol("ord"), entry);

    //  add `chr` Func
    entry = myEnvironment_makeFuncEntry(
        makeType_TypeList(makeType_Int(), NULL),
        makeType_String());
    MySymbol_Enter(table, MySymbol_MakeSymbol("chr"), entry);

    //  add `size` Func
    entry = myEnvironment_makeFuncEntry(
        makeType_TypeList(makeType_String(), NULL),
        makeType_Int());
    MySymbol_Enter(table, MySymbol_MakeSymbol("size"), entry);

    //  add `substring` Func
    entry = myEnvironment_makeFuncEntry(
        makeType_TypeList(
            makeType_String(), 
            makeType_TypeList(
                makeType_Int(), 
                makeType_TypeList(makeType_Int(), NULL))),
        makeType_String());
    MySymbol_Enter(table, MySymbol_MakeSymbol("substring"), entry);

    //  add `concat` Func
    entry = myEnvironment_makeFuncEntry(
        makeType_TypeList(
            makeType_String(),
            makeType_TypeList(
                makeType_String(),
                NULL)),
        makeType_String());
    MySymbol_Enter(table, MySymbol_MakeSymbol("concat"), entry);

    //  add `not` Func
    entry = myEnvironment_makeFuncEntry(
        makeType_TypeList(makeType_Int(), NULL),
        makeType_Int());
    MySymbol_Enter(table, MySymbol_MakeSymbol("not"), entry);

    //  add `exit` Func
    entry = myEnvironment_makeFuncEntry(
        makeType_TypeList(makeType_Int(), NULL),
        makeType_NoReturn());
    MySymbol_Enter(table, MySymbol_MakeSymbol("exit"), entry);

    return table;
}


myTable myEnvironment_BaseType(void)
{
    myTable table = MySymbol_MakeNewTable();
    assert(table != NULL);

    myType type = makeType_Int();

    MySymbol_Enter(table, MySymbol_MakeSymbol("int"), type);

    type = makeType_String();
    MySymbol_Enter(table, MySymbol_MakeSymbol("string"), type);

    return table;
}
