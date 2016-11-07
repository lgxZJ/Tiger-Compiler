#include "myEnvironment.h"
#include "typeMaker.h"

#include <assert.h> //  for assert()
#include <stddef.h> //  for NULL definition

//////////////////////////////////////////////////////////////

struct myVarAndFuncEntry_
{
    enum EntryType { VarEntry, FuncEntry, EmptyEntry } kind;
    union 
    {
        struct { Trans_myAccess access; myType type ;} varEntry;
        struct  { Trans_myLevel level;      myLabel label;
            myTypeList formalParamTypes;    myType returnType; } funcEntry;
    }u;
};

///////////////////////////////////////////////////////////////

myVarAndFuncEntry myEnvironment_makeVarEntry(
    Trans_myAccess access,  myType type)
{
    myVarAndFuncEntry entry = 
        makeMemoryBlock(sizeof(*entry), MEMORY_TYPE_NONE);
    assert(entry);

    entry->kind = VarEntry;
    entry->u.varEntry.access = access;
    entry->u.varEntry.type = type;

    return entry;
}

myVarAndFuncEntry myEnvironment_makeFuncEntry(
    Trans_myLevel level, myLabel label,
    myTypeList formalParamTypes, myType returnType)
{
    myVarAndFuncEntry entry =
        makeMemoryBlock(sizeof(*entry), MEMORY_TYPE_NONE);
    assert(entry);

    entry->kind = FuncEntry;
    entry->u.funcEntry.level = level;
    entry->u.funcEntry.label = label;
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

///////////////////////////////////////////////////////////

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

myType MyEnvironment_getVarType(myVarAndFuncEntry varEntry)
{
    assert (varEntry);
    assert (myEnvironment_isVarEntry(varEntry));
    return varEntry->u.varEntry.type;
}

/////////////////////////////////////////////////////////

//  NOTE:   type must has been already added
myType MyEnvironment_getTypeFromName(
    myTable typeEnv, mySymbol typeName)
{
    return (myType)MySymbol_Look(typeEnv, typeName);
}

//  NOTE:  The parameter must ba a valid function entry.
myTypeList MyEnvironment_getFuncFormalTypes(myVarAndFuncEntry funcEntry)
{
    assert (funcEntry);
    assert (myEnvironment_isFuncEntry(funcEntry));
    return funcEntry->u.funcEntry.formalParamTypes;
}

//  NOTE:  The parameter must ba a valid function entry.
myType MyEnvironment_getFuncReturnType(myVarAndFuncEntry funcEntry)
{
    assert (funcEntry);
    assert (myEnvironment_isFuncEntry(funcEntry));
    return funcEntry->u.funcEntry.returnType;
}

Trans_myLevel MyEnvironment_getFuncLevel(myVarAndFuncEntry funcEntry)
{
    assert (funcEntry);
    assert (myEnvironment_isFuncEntry(funcEntry));
    return funcEntry->u.funcEntry.level;
}

myLabel MyEnvironment_getFuncLabel(myVarAndFuncEntry funcEntry)
{
    assert (funcEntry);
    assert (myEnvironment_isFuncEntry(funcEntry));
    return funcEntry->u.funcEntry.label;
}

////////////////////////////////////////////////////////////

bool MyEnvironment_updateTypeValue(
    myTable typeEnv, mySymbol typeName, myType newType)
{
    assert (typeEnv && newType);
    assert (!MySymbol_InvalidSymbol(typeName));
    return MySymbol_Set(typeEnv, typeName, newType);
}

void MyEnvironment_updateFuncLevel(
    myTable varAndFuncEnv, mySymbol funcName, Trans_myLevel level)
{
    assert (varAndFuncEnv && level);
    assert (!MySymbol_InvalidSymbol(funcName));

    myVarAndFuncEntry entry = MySymbol_Look(varAndFuncEnv, funcName);
    assert (entry && myEnvironment_isFuncEntry(entry));

    entry->u.funcEntry.level = level;
}

void MyEnvironment_updateFuncLabel(
    myTable varAndFuncEnv, mySymbol funcName, myLabel label)
{
    assert (varAndFuncEnv && label);
    assert (!MySymbol_InvalidSymbol(funcName));

    myVarAndFuncEntry entry = MySymbol_Look(varAndFuncEnv, funcName);
    assert (entry && myEnvironment_isFuncEntry(entry));

    entry->u.funcEntry.label = label;
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
        Trans_outermostLevel(),
        Temp_newNamedLabel("print"),
        makeType_TypeList(makeType_String(), NULL),
        makeType_NoReturn());
    MySymbol_Enter(table, MySymbol_MakeSymbol("print"), entry);

    //  add `flush` Func
    entry = myEnvironment_makeFuncEntry(
        Trans_outermostLevel(),
        Temp_newNamedLabel("flush"),
        makeType_TypeList(NULL, NULL),
        makeType_NoReturn());
    MySymbol_Enter(table, MySymbol_MakeSymbol("flush"), entry);


    //  add `getchar` Func
    entry = myEnvironment_makeFuncEntry(
        Trans_outermostLevel(),
        Temp_newNamedLabel("getchar"),
        makeType_TypeList(NULL, NULL),
        makeType_String());
    MySymbol_Enter(table, MySymbol_MakeSymbol("getchar"), entry);

    //  add `ord` Func
    entry = myEnvironment_makeFuncEntry(
        Trans_outermostLevel(),
        Temp_newNamedLabel("ord"),
        makeType_TypeList(makeType_String(), NULL),
        makeType_Int());
    MySymbol_Enter(table, MySymbol_MakeSymbol("ord"), entry);

    //  add `chr` Func
    entry = myEnvironment_makeFuncEntry(
        Trans_outermostLevel(),
        Temp_newNamedLabel("chr"),
        makeType_TypeList(makeType_Int(), NULL),
        makeType_String());
    MySymbol_Enter(table, MySymbol_MakeSymbol("chr"), entry);

    //  add `size` Func
    entry = myEnvironment_makeFuncEntry(
        Trans_outermostLevel(),
        Temp_newNamedLabel("size"),
        makeType_TypeList(makeType_String(), NULL),
        makeType_Int());
    MySymbol_Enter(table, MySymbol_MakeSymbol("size"), entry);

    //  add `substring` Func
    entry = myEnvironment_makeFuncEntry(
        Trans_outermostLevel(),
        Temp_newNamedLabel("substring"),
        makeType_TypeList(
            makeType_String(), 
            makeType_TypeList(
                makeType_Int(), 
                makeType_TypeList(makeType_Int(), NULL))),
        makeType_String());
    MySymbol_Enter(table, MySymbol_MakeSymbol("substring"), entry);

    //  add `concat` Func
    entry = myEnvironment_makeFuncEntry(
        Trans_outermostLevel(),
        Temp_newNamedLabel("concat"),
        makeType_TypeList(
            makeType_String(),
            makeType_TypeList(
                makeType_String(),
                NULL)),
        makeType_String());
    MySymbol_Enter(table, MySymbol_MakeSymbol("concat"), entry);

    //  add `not` Func
    entry = myEnvironment_makeFuncEntry(
        Trans_outermostLevel(),
        Temp_newNamedLabel("not"),
        makeType_TypeList(makeType_Int(), NULL),
        makeType_Int());
    MySymbol_Enter(table, MySymbol_MakeSymbol("not"), entry);

    //  add `exit` Func
    entry = myEnvironment_makeFuncEntry(
        Trans_outermostLevel(),
        Temp_newNamedLabel("exit"),
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
