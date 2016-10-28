#ifndef MY_ENVIRONMENT_H
#define MY_ENVIRONMENT_H

#include "myTypes.h"
#include "myTranslate.h"

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

/**----------------------------------------------------------
                    make functions
-----------------------------------------------------------**/ 
//  RETURN : a valid var-entry 
myVarAndFuncEntry myEnvironment_makeVarEntry(
    Trans_myAccess access, myType type);
//  RETURN : a valid func-entry
myVarAndFuncEntry myEnvironment_makeFuncEntry(
    Trans_myLevel level, myLabel label,
    myTypeList formalParamTypes, myType returnType);

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
myType            MyEnvironment_getTypeFromName(
    myTable typeEnv, mySymbol typeName);

// todo:    refactor mySemantic module
myTypeList  MyEnvironment_getFuncFormalTypes(myVarAndFuncEntry funcEntry);
myType      MyEnvironment_getFuncReturnType(myVarAndFuncEntry funcEntry);

myType MyEnvironment_getVarType(myVarAndFuncEntry varEntry);

//  RETURN:
//      If type with name `typeName` is not found, it returns false;
//      otherwise, it returns true.
bool MyEnvironment_setTypeFromName(
    myTable varAndFuncEnv, mySymbol typeName, myType type);

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
