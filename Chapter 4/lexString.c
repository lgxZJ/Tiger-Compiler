#include "lexString.h"
#include "stdlib.h"
#include "string.h"

/*===================private data and types=================*/


typedef struct StringChain_
{
    char*			str;
    struct StringChain_*	next;
}*	StringChainPtr;


/*	to test, uncomment the `static` keyword*	*/
static StringChainPtr g_head = NULL;

/*	return string ptr if exists, otherwise return NULL	*/
static char* findStringInChain(char* str, int length)
{
    StringChainPtr ptr = g_head;
    while (ptr)
    {
	if (strncmp(str, ptr->str, length + 1) == 0)
	    return ptr->str;
	ptr = ptr->next;
    }
    return NULL;
}

/*==========================================================*/


/*
 *	return non-null if successfully, null otherwise.
 */
char* lexString(char* src, int length)
{
    if (src == NULL || (*(char*)src) == '\0' ||
		length <= 0 || (strlen(src) < length))
		return NULL;

    //	check if already exists
    char* result = findStringInChain(src, length);
    if (result)		return result;

    //	allocate necessary memory
    StringChainPtr chainPtr = malloc(sizeof(*chainPtr));
    if (chainPtr == NULL)	return NULL;

    char* str = malloc(sizeof(char*) * (length + 1));
    if (str == NULL)
    {
		free(chainPtr);
		return NULL;
    }
    
    //	copy string
    strncpy(str, src, length);
    str[length] = '\0';

    chainPtr->str = str;
    chainPtr->next = g_head;
    g_head = chainPtr;

    return str;
}

void freeStrings(void)
{
    StringChainPtr ptr = g_head;
    while (ptr)
    {
		//	each valid StringChain_ contains valid string ptr
		free(ptr->str);
		ptr->str = NULL;

		ptr = ptr->next;
    }
}
