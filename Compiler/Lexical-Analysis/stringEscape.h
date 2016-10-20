#ifndef STRING_ESCAPE_H
#define STRING_ESCAPE_H

/**
 *	@file	stringEscape.h
 *	@author	lgxzJ@outlook.com
 *	@date	5/17/2016
 *
 *	@brief	This file provides the functionality of translating escape
 *			sequences.
 *
 *			The escape characters can be translated are defined also in
 *			the book--Morden Compiler Implementation in C.
 */

/**
 *	@brief	Translate escape characters.
 *
 *	@param	str		A pointer to a string to be translated.
 *	@return	A translated string.
 *
 *	@remark	The return string is an new allocated string which internally
 *			use \b makeMemoryBlock(). So, if one attempts to free it, use
 *			\b freeMemoryChain().
 *	@warning	This function only do translations, the caller make sures
 *				the escape characters contained is valid.
 *
 *	@sa		makeMemoryBlock, freeMemoryChain.
 */
char* translateEscape(char* str);

#endif
