#ifndef LEXSTRING_H
#define LEXSTRING_H

/*--------------------------------------------------------------------
 *	File:		lexString.h
 *	Functionality:	make string which contains the first `length` characters of a long string (do not make same strings)
 *------------------------------------------------------------------*/



/*	make string from ptr and length
 *
 *	if already accloated, return same string,
 *	else accloate a new one. 
 */
char* lexString(char* ptr, int length);

/*	free memory	*/
void freeStrings(void);


/*------------------------------------------------------------------*
 *	uncomment followings for tests				    *
 *	and uncomment typedefs at source file			    *
 * -----------------------------------------------------------------*/
/*
typedef struct StringChain_
{
    char*			str;
    struct StringChain_*	next;
}*	StringChainPtr;

extern StringChainPtr g_head;
*/

#endif
