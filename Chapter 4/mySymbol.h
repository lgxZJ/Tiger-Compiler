#ifndef MYSYMBOL_H
#define MYSYMBOL_H

/**
 *	@defgroup	Symbol	Symbols
 *	@{
 **/

/**
 *	@file	mySymbol.h
 *	@author	lgxZJ@outlook.com
 *	@date	5/24/2016
 *
 *	@brief	Module about symbols.
 */

//
//	Symbol module:
//
//	mySymbol is just a address of mySymbol_ structure
//		containing symbol names of language source file
//	By using this pointer it makes up a symbol list
//

#include "myTable.h"


typedef char*	myString;

/**	@cond	*/

typedef struct mySymbol_
{	
	myString		name;
	struct mySymbol_*	next;
}* mySymbol;

/**	@endcond	*/



/**
 *	@brief  Check symbols' validity.
 *	@param[in]	The symbol to be checked.
 *	@return	If the symbol is valid, return true; Otherwise, return false.
 *	@remark	Invalid symbols are NULLs and empty symbols.
 */
bool invalidSymbol(mySymbol symbol);

/**
 *	@brief	Make s symbol from a string.
 *
 *	@param[in]	str	The string to make symbol from.
 *
 *	@return	If the string is NULL or empty, return \b NULL; If the string
 *			is valid, return a symbol made from it.
 *
 *	@remark	When a new symbol is made, it is also added to the internal
 *			symbol table to avoid remaking. If the symbol made is already
 *			existed, this function simply returns it.
 */
mySymbol MySymbol_MakeSymbol(myString str);

/**
 *	@brief	Get the string name of the given symbol.
 *	@param[in]	symbol	The symbol to extract name from.
 *	@return	The extracted string name.
 */
myString MySymbol_GetName(const mySymbol symbol);

/**
 *	@brief	Make a new empty table to hold symbols.
 *	@param	void	This function hsa no parameters.
 *	@return	If the function succeeds, the return value is a new table.
 *			If the function fails, the return value is \b NULL.
 */
myTable MySymbol_MakeNewTable();

/**
 *	@brief	Enter a \b symbol-value pair, namely a binding, into a table.
 *
 *	@param[in]	table	The table where to enter.
 *	@param[in]	symbol	The symbol to enter.
 *	@param[in]	value	The value to enter.
 *
 *	@return	A bool value that indicates whether the function succeeds.
 */
bool MySymbol_Enter(myTable table, const mySymbol symbol, void* const value);

/**
 *	@brief	Look a symbol's most recent value in the given table.
 *
 *	@param[in]	table	The table where to look for.
 *	@param[in]	symbol	The symbol to be looked.
 *	@return	If the function successfully looked, return the symbol's value;
 *			If the function fails, return \b NULL.
 *
 *	@todo	Change the return type.		
 */
void* MySymbol_Look(const myTable table, const mySymbol symbol);

/**
 *	@brief	Start a new scope in the given table.
 *	@param[in]	table	The table where to start a new scope.
 *	@return	A bool which indicates whether the function succeeds.
 */
bool MySymbol_BeginScope(myTable table);

/**
 *	@brief	End the most recent scope in the given table.
 *	@param[in]	table	The table where to end a scope.
 *	@return	This function has no return value.
 *
 *	@remark	When a scope ends, all bindings inside are also removed.
 */
void MySymbol_EndScope(myTable table);

/**	@}	*/

#endif
