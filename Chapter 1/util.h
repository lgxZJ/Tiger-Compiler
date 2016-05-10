/**
 *	@file		util.h
 *	@brief		C header for utilities.
 *	@author		lgxZJ@outlook.com
 *	@date		06/05/2016
 *	@note		Trying.
 */
#ifndef UTIL_H
#define UTIL_H

//==============body============//
#include <assert.h>

/**
 *	@brief	define \c string to \c char*.
 */
typedef char *string;

/**
 *	@brief	make a string from a given pointer to zero-ended string.
 *	@param	A pointer to a string ended of zero.
 *	@return	A new-created string.
 */
string String (char *str);

/**
 *	@brief	define \c TRUE to 1
 */
#define TRUE	1
/**
 *	@brief	define \c FALSE to 0
 */
#define FALSE	0

/**
 *	@brief	Allocate memory with checking.
 *	@param	cbSize		Size of memory to be allocated, in bytes.
 *	@return	A pointer to the created memory.
 */
void* checked_malloc (int cbSize);
//==============end==========//

#endif
