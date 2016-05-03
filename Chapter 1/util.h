/**
 *	@file		util.h
 *	@warning	Only used for roughly seeing.
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
 *	@brief	make a string from a given pointer to char.
 */
string String (char *);

/**
 *	@brief	define \c TRUE to 1
 */
#define TRUE	1
/**
 *	@brief	define \c FALSE to 0
 */
#define FALSE	0

/**
 *	@brief	allocate memory with checking.
 */
void* checked_malloc (int);
//==============end==========//

#endif
