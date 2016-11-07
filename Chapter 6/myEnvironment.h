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

///
/// \brief      make a valid(not NULL) variable entry.
/// \param[in]  access  an access representation of this variable.
/// \param[in]  type    the type of this variable.
/// \return     a variable entry.
///
myVarAndFuncEntry myEnvironment_makeVarEntry(
    Trans_myAccess access, myType type);

///
/// \brief      make a valid(not NULL) function entry.
///
/// \param[in]  level  the level of this function.
/// \param[in]  label  the label of this function.
/// \param[in]  formalParamTypes    the types of function formals.
/// \param[in]  returnType          the return type of this function.
/// \return     a function entry.
///
myVarAndFuncEntry myEnvironment_makeFuncEntry(
    Trans_myLevel level, myLabel label,
    myTypeList formalParamTypes, myType returnType);

/**----------------------------------------------------------
                    check functions
----------------------------------------------------------**/

///
/// \brief      check if the given entry is a variable entry.
/// \param[in]  a valid entry to be checked.
/// \return     a bool.
/// 
bool myEnvironment_isVarEntry(myVarAndFuncEntry entry);

///
/// \brief      check if the given entry is a function entry.
/// \param[in]  a valid entry to be checked.
/// \return     a bool.
///
bool myEnvironment_isFuncEntry(myVarAndFuncEntry entry);

/**---------------------------------------------------------
                    add and get functions
----------------------------------------------------------**/

///
/// \brief      add a new variable of function in the given environment.
///
/// \param[in]  varAndFuncEnv   an environment where to add.
/// \param[in]  entryName       an entry name, a variable or function name.
/// \param[in]  newEntry        an entry.
///     
void MyEnvironment_addNewVarOrFunc(
    myTable varAndFuncEnv, mySymbol entryName, myVarAndFuncEntry newEntry);

///
/// \brief      add a new type in the given environment.
///
/// \param[in]  typeEnv     an environment where to add.
/// \param[in]  typeName    the name of the type.
/// \param[in]  newType     the type value.
///
void MyEnvironment_addNewType(
    myTable typeEnv, mySymbol typeName, myType newType);

///
/// \brief      get an entry with the given name.
///
/// \param[in]  varAndFuncEnv   an environment where to look.
/// \param[in]  varOrFuncName   a name of variable or function.
/// \return     if there is an entry with the given name, it is returned;
///             otherwise, NULL is returned.
///    
myVarAndFuncEntry MyEnvironment_getVarOrFuncFromName(
    myTable varAndFuncEnv, mySymbol varOrFuncName);

///
/// \brief      get an type value with the given type name.
///
/// \param[in]  typeEnv     an environment where to look.
/// \param[in]  typeName    the type name.
/// \return     if there is an type with the given name, it is returned;
///             otherwise, NULL is returned.
///
myType            MyEnvironment_getTypeFromName(
    myTable typeEnv, mySymbol typeName);

///
/// \brief      get function formal types from a function entry.
/// \param[in]  funcEntry   a function entry.
/// \return     the formal types of the function inferred by the given entry.
///
/// \remark     it's a checked runtime error to pass a invalid function
///             entry into this function.
/// 
myTypeList  MyEnvironment_getFuncFormalTypes(myVarAndFuncEntry funcEntry);

///
/// \brief      get function return type from a function entry.
/// \param[in]  funcEntry   a function entry.
/// \return     the return type of the function inferred by the given entry.
///
/// \remark     it's a checked runtime error to pass a invalid function
///             entry into this function.
/// 
myType      MyEnvironment_getFuncReturnType(myVarAndFuncEntry funcEntry);

///
/// \brief      get function level from a function entry.
/// \param[in]  funcEntry   a function entry.
/// \return     the level of the function inferred by the given entry.
///
/// \remark     it's a checked runtime error to pass a invalid function
///             entry into this function.
/// 
Trans_myLevel MyEnvironment_getFuncLevel(myVarAndFuncEntry funcEntry);

///
/// \brief      get function label from a function entry.
/// \param[in]  funcEntry   a function entry.
/// \return     the label of the function inferred by the given entry.
///
/// \remark     it's a checked runtime error to pass a invalid function
///             entry into this function.
/// 
myLabel MyEnvironment_getFuncLabel(myVarAndFuncEntry funcEntry);

///
/// \brief      get variable type from a variable entry.
/// \param[in]  funcEntry   a variable entry.
/// \return     the type of the variable inferred by the given entry.
///
/// \remark     it's a checked runtime error to pass a invalid variable
///             entry into this function.
/// 
myType MyEnvironment_getVarType(myVarAndFuncEntry varEntry);

///
/// \brief      update type value in the given type environment.
///
/// \param[in]  typeEnv     the given type environment.
/// \param[in]  typeName    the name of type whose value is to be updated.
/// \param[in]  newType     the new type value.
/// \return     if the type is found and updated successfully, it returns true;
//              otherwise, it returns false.
/// 
bool MyEnvironment_updateTypeValue(
    myTable typeEnv, mySymbol typeName, myType newType);

/**
 *  @brief      update the level of function(Needed by recursive processing).
 *
 *  @param[in]  varAndFuncEnv   the environment from which to look for functions.
 *  @param[in]  funcName        the name of function.
 *  @param[in]  level           the value of level to update.
 *  @return     this function has no return values.     
 */
void MyEnvironment_updateFuncLevel(
    myTable varAndFuncEnv, mySymbol funcName, Trans_myLevel level);

/**
 *  @brief      update the label of function(Needed by recursive processing).
 *
 *  @param[in]  varAndFuncEnv   the environment from which to look for functions.
 *  @param[in]  funcName        the name of function.
 *  @param[in]  label           the value of label to update.
 *  @return     this function has no return values.     
 */
void MyEnvironment_updateFuncLabel(
    myTable varAndFuncEnv, mySymbol funcName, myLabel label);

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
