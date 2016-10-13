#ifndef MY_ENVIRONMENT_H
#define MY_ENVIRONMENT_H

#include "myTypes.h"

 /**
 *	@file	myEnvironment.h
 *	@author	lgxZJ@outlook.com
 *	@date	9/17/2016
 *
 *	@brief	Module about environment when doing semantic analysis.
 *
 *			Environment consists of two symbol tables about variable,
 *          functions, procedures and type in tiger source file.
 *
 *	@note	Func stands for Function and Procedure and these functions
 *          always succeed.
 *	@warning	The test cases may not be enough, if some bugs
 *				were found, please contact me!
 */


typedef struct myVarAndFuncEntry_ *myVarAndFuncEntry;
struct myVarAndFuncEntry_
{
    enum EntryType { VarEntry, FuncEntry } kind;
    union 
    {
        struct { myType type ;} varEntry;
        struct  { myTypeList formalParamTypes; myType returnType; } funcEntry;
    }u;
};

/**----------------------------------------------------------
                    make functions
-----------------------------------------------------------**/
//  RETURN : a valid var-entry 
myVarAndFuncEntry myEnvironment_makeVarEntry(myType type);
//  RETURN : a valid func-entry
myVarAndFuncEntry myEnvironment_makeFuncEntry(myTypeList formalParamTypes, myType returnType);

/**----------------------------------------------------------
                    check functions
----------------------------------------------------------**/
//  todo:   change name
bool myEnvironment_isVarEntry(myVarAndFuncEntry entry);
bool myEnvironment_isFuncEntry(myVarAndFuncEntry entry);

/**---------------------------------------------------------
                    add and get functions
----------------------------------------------------------**/
void MyEnvironment_addNewVarOrFunc(
    myTable varAndFuncEnv, mySymbol entryName, myVarAndFuncEntry newEntry);
void MyEnvironment_addNewType(
    myTable typeEnv, mySymbol typeName, myType newType);
myVarAndFuncEntry MyEnvironment_getVarOrFuncFromName(
    myTable varAndFuncEnv, mySymbol varOrFuncName);
myType MyEnvironment_getTypeFromName(
    myTable typeEnv, mySymbol typeName);

/**
 *  @return A symbol table containing predefined functions and 
 *          procedures in tiger language.
 *  @note   Call freeMemoryChain() to free whole memories.
 */
myTable myEnvironment_BaseVarAndFunc(void);


/**
 *  @return A symbol table containing intuitive types in tiger languege.
 *  @note   Call freeMemoryChain() to free whole memories.
 */
myTable myEnvironment_BaseType(void);


#endif