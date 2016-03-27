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
 *	NOTE:		The only difference between `MemoryType` and
 *	`UserMemoryType` is that `MemoryType` contains MEMORY_TYPE_NONE
 *	but `UserMemoryType` not.
 *
 *	Memory Type:	Content type in memory block. 0 for none-type,
 *			positive for user-type. (No predefined types!)
 *
 *	User Memory Type:   An unsigned number representing user-type.
 */

typedef int		MemoryType;
typedef unsigned        UserMemoryType;

#define MEMORY_TYPE_NONE	0

/*	Macros for module using	*/
#define MT_ERROR	-1
#define MT_SUCCESS	1
#define MT_EXISTS	0


/*
 *	Register Memory Type:	Make sure the Type of Memory you are
 *	Allocating is unique.
 */


/*	get array address containing memory types	*/
const MemoryType* getRegisteredTypeArray(void);
/*	get registryed memory type	*/
unsigned getRegisteredTypeNum(void);
/*	get the head node of memory chain	*/

/*	register `Memory Type` for later allocating (user should not use this)	*/
int registerMemoryType(MemoryType type);
/*	register a unique type which has't been used	*/
UserMemoryType registerUniqueType(void);

/*	allocate memory	with certain Memory Type	*/
void* makeMemoryBlock(unsigned bytes, MemoryType type);
/*	list all blocks of certain Memory Type	*/
void* findMemoryBlock(UserMemoryType type);

/*	free all allocated memory	*/
void freeMemoryChain(void);
/*	get head of `Memory Chain` (user should not use this)	*/
MemoryChainPtr getMemoryChain(void);


#endif
