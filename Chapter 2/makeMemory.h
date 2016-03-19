#ifndef MAKE_MEMORY_H
#define MAKE_MEMORY_H

/*
 *	Module:			makeMemory.c
 *	Functionality:		Allocate memory, stores each single
 *			block position in chain and free all memory.
 *
 *	NOTE:	This should be the base module of memory allocation,
 *      another other module shall use features provided by this
 *	module.
 */


typedef struct MemoryChain_
{
    void*		        data;
    int				type;//	-1 stand for none-type
    struct MemoryChain_*	next;
}*	MemoryChainPtr;

/*
 *	Memory Type:	Content type in memory block. 0 for none-type,
 *			positive for user-type. (No predefined types!)
 */
#define MEMORY_TYPE_NONE	0

/*
 *	Register Memory Type:	Make sure the Type of Memory you are
 *	Allocating is unique.
 */


/*	get array address containing memory types	*/
const int* getRegisteredTypeArray(void);
/*	get registryed memory type	*/
unsigned getRegisteredTypeNum(void);
/*	get the head node of memory chain	*/

/*	register `Memory Type` for later allocating (user should not use this)	*/
int registerMemoryType(int type);
/*	register a unique type which has't been used	*/
int registerUniqueType(void);

/*	allocate memory	with certain Memory Type	*/
void* makeMemoryBlock(unsigned bytes, int type);
/*	list all blocks of certain Memory Type	*/
void* findMemoryBlock(unsigned type);

/*	free all allocated memory	*/
void freeMemoryChain(void);
/*	get head of `Memory Chain` (user should not use this)	*/
MemoryChainPtr getMemoryChain(void);


#endif
