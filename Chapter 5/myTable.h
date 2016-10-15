#ifndef MYTABLE_H
#define MYTABLE_H

/**
 *	@defgroup	HashTable	Binding Table
 *	@{
 */

/**
 *	@file	myTable.h
 *	@author	lgxZJ@outlook.com
 *	@date	5/24/2016
 *
 *	@brief	Module about hash table of bindings.
 *
 *			This module provide table-about functions to be used with
 *			symbols. A binding is a pair of form "key-value".
 *
 *	@note	No algorithm should use these functions directly, because
 *			programming with void* is too error-prone.	Instead, each
 *			module should make "wrapper" functions that take well-typeded
 *			arguments and call the "*_" functions in this module.
 */


/*
 *	Generic hash table module:
 *		No algorithm should use these functions directly, because
 *		programming with void* is too error-prone.	Instead, each
 *		module should make "wrapper" functions that take well-typeded
 *		arguments and call the _ functions.
 */
 
 #include "makeMemory.h"
 #include <stdbool.h>
 

/**	@cond	*/

 #define		TAB_SIZE		127
 
typedef struct myBinder_*	myBinder;
typedef struct myTable_*	myTable;

/**	@endcond	*/


/**
 *	@brief	Make s new empty table which maps "key -> value".
 *	@param	void	This function has no parameters.
 *	@return	A valid(not NULL) new Table.
 */
myTable	MyTable_MakeEmptyTable_(void);

/**
 *	@brief	Enter a key-value binder into a given table.
 *	@param[in]	table	The table where to enter.
 *	@param[in]	key		The key to be entered.
 *	@param[in]	value	A not NULL value to be entered.
 *	@return	A bool variable which indicates the function's success.
 */
bool MyTable_Enter_(myTable table, void* const key, void* const value);

/**
 *	@brief	Look a key most recently entered inside a table.
 *	@param[in]	table	The table where to look.
 *	@param[in]	key		The key whose value is to be looked.
 *	@return	If successfully found, return the value of \a key;
 *			If fails, return \b NULL. 
 */
void* MyTable_Look_(const myTable table, void* const key);

/**
 *	@brief	Set a key' value which is already entered.
 *	@param[in]	table	The table where to look.
 *	@param[in]	key		The key whose value is to be looked.
 *  @param[in]  value   The value to set.
 *	@return	If successfully setted, it returns true;
 *			If `table` or `key` are NULL or key not found, it returns false.
 */
bool MyTable_Set_(myTable table, void* const key, void* const value);

/**
 *	@brief	Pop the top binding's value inside the given table.
 *	@param[in]	table	The table whose top binding is to be popped.
 *	@return	If the function fails, the return value is \b NULL;
 *			If the function succeeds, the return value is top binding's
 *			value.
 *	@remark	It's caller's responsibility to make sure that there must
 *			be a binding inside the given table.
 */
void* MyTable_Pop_(myTable table);

/*
//	the caller makes params valid
typedef void (*ShowFunc)(void* key, void* value);
*/
/**
 *	@brief	dump the given table using the given dump-function.
 *	@param[in]	table	The table to be dumped.
 *	@param[in]	show	The dump-function to be used.
 *	@return This function has no return values.
 *//*
void MyTable_Dump_(const myTable table, ShowFunc show);*/

/**	@}	*/

#endif
