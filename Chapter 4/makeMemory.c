#include "makeMemory.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*-------------------------Global Variables-----------------------*/


//static const unsigned MEMORY_TYPE_NONE = 0;//	used internally

static MemoryChainPtr	g_head = NULL;
static MemoryType*	g_memoryTypes = NULL;

unsigned	g_memoryTypeSize = 0;
unsigned	g_currentTypeNum = 0;


/*------------------------------------------------------------------*/


inline const MemoryType* getRegisteredTypeArray(void)
{
    return g_memoryTypes;
}

inline unsigned getRegisteredTypeNum(void)
{
    return g_currentTypeNum;
}

inline MemoryChainPtr getMemoryChain(void)
{
    return g_head;
}

/*--------------------------------------------------------------*/

int int_compare(const void* lhs, const void* rhs)
{
    return (*(MemoryType*)lhs) - (*(MemoryType*)rhs);
}

/*	return 1 for success, -1 for error, 0 for exists	*/
int registerMemoryType(MemoryType type)
{
    //	invalid type
    if (type < 0)	return MT_ERROR;

    for (unsigned i = 0; i < g_currentTypeNum; ++i)
    {
	if (g_memoryTypes[i] == type)	return MT_EXISTS;
	//	others are larger then `type`, skip compare
	if (g_memoryTypes[i] > type)	break;
    }

    //	if type array is full filled
    if (g_currentTypeNum == g_memoryTypeSize)
    {
	//	add capacity
	g_memoryTypeSize += 10;
	void* tmp = makeMemoryBlock(sizeof(MemoryType) * g_memoryTypeSize,
				    MEMORY_TYPE_NONE);
	assert(tmp);

	//	copy 
	memcpy(tmp, g_memoryTypes,
	       sizeof(MemoryType) * (g_memoryTypeSize - 10));

	//		No needs to free memory because of using
	//	`makeMemoryBlock`, memorys can be freed at one
	//	calling at the end.
	g_memoryTypes = tmp;
    }
    
    //	add new type
    g_memoryTypes[g_currentTypeNum] = type;
    ++g_currentTypeNum;

    //	sort in ascendent order
    qsort(g_memoryTypes, g_currentTypeNum, sizeof(MemoryType),
	  int_compare);

    return MT_SUCCESS;
}

/*-------------------------------------------------------------------*/

/*
 *	RETURN:	-1 for error, positive for successfully registered type
 */
MemoryType registerUniqueType(void)
{
    for (unsigned i = 1; i < (unsigned)-1; ++i)
    {
	switch (registerMemoryType(i))
	{
	case MT_ERROR:		return MT_ERROR;//	if error
	case MT_SUCCESS:	return i;	//	if succeed
	case MT_EXISTS:		continue;	//	if exists
	}
	
	//	never goes here
	assert(0);
    }

    //	can not find appropriate types which hasn't been used
    return MT_ERROR;
}

/*-------------------------------------------------------------------*/

/*	return null pointer for no enough memory, otherwise for success.
 *
 *	NOTE:	It's caller's responsibility to make sure type's
 *	uniqueness.
 */
void* makeMemoryBlock(unsigned bytes, MemoryType type)
{
    //	allocate chain memory holding data
    MemoryChainPtr chainPtr =
	(MemoryChainPtr)malloc(sizeof(*chainPtr));
    if (chainPtr == NULL)	return NULL;

    //	allocate data block
    chainPtr->data = malloc(bytes);
    if (chainPtr->data == NULL)
    {
	free(chainPtr);
	return NULL;
    }

    chainPtr->type = type;
    chainPtr->next = g_head;

    g_head = chainPtr;
    return chainPtr->data;
}

/*------------------------------------------------------------------*/

/*	NOTE:	(This function is only used by user) Users should
 *	not find memory block of MEMORY_TYPE_NONE which is used
 *	internally. The caller make sures that the type searching
 *	for is unique.
 *
 *	RETURN:	NULL for not found, not NULL for found
 */
void* findMemoryBlock(MemoryType type)
{
    /*  This variavle stands for the current searching node	*/
    static MemoryChainPtr current_ = NULL;
    /*  This variable is used to keep the type of former finding */
    static MemoryType type_ = MEMORY_TYPE_NONE;


    /*	if another traversal, set head node first;
	else continue next finding	*/
    if (type_ != type)
    {
	type_ = type;
	current_ = g_head;
    }

    //	find next memory block
    while (current_)
    {
	MemoryChainPtr tmp = current_;
	//	set next search node
	current_ = current_->next;

	if (tmp->type == type_)
	    return tmp->data;
    }

    return current_ = NULL;
}

/*-----------------------------------------------------------------*/

void freeMemoryChain(void)
{
    MemoryChainPtr next = NULL;
    while (g_head)
    {
	next = g_head->next;

	free(g_head->data);
	free(g_head);

	g_head = next;
    }
   
    g_head = NULL;
}


/*---------------------------END OF FILE---------------------------*/
