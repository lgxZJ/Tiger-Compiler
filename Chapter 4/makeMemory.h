#ifndef MAKE_MEMORY_H
#define MAKE_MEMORY_H

/**
 *	@defgroup MemoryAllocate Memory Allocate.
 */
/**	@{	*/

/**
 *	@file	makeMemory.h
 *	@author	lgxZJ@outlook.com
 *	@date	5/14/2016
 *
 *	@brief	Module about memory allocating.
 *
 *			This module provide a way that users can allocate
 *			memory in heap, search certain memory blocks and
 *			free all memory at a time. Indeed, once one memory
 *			block is allocated, it is also recorded internally.
 *			Thus, search like functions can be done with this
 *			information.
 *
 *	@note	This should be the base module of memory allocation,
 *      	another other module shall use features provided by
 *			this module.
 *
 *	@warning	The test cases may not be enough, if some bugs
 *				were found, please contact me!
 */

/**	@cond	*/

/**
 *	@brief	The node type of memory chain.
 */
typedef struct MemoryChain_
{
    void*		        	data;/**<	pointer to memory block	*/
    int						type;/**<	0 stand for none-type	*/
    struct MemoryChain_*	next;/**<	point to next node	*/
}*	MemoryChainPtr;

/**	@endcond	*/

/*
 *	//@remark	The only difference between \b `MemoryType` and
 *	//		\b `UserMemoryType` is that \b `MemoryType` contains
 *	//		\b MEMORY_TYPE_NONE \b but `UserMemoryType` not.
 *	//		The only reason why defining this two similar types
 *	//		is to distinguish user type from the type containing
 *	//		internally used memory type.
 *	//
 *	//@remark	Internally used memory type is not recommended to be used
 *	//		by users since is will cause mixture.
 *	//		
 *	//
 *	//Memory Type:	A type indicates content types in memory block.
 *	//				@ref \b MEMORY_TYPE_NONE for none-type,
 *	//				others for user-type. (Not predefined types!)
 *	//
 *	//User Memory Type:   A type @ref UserMemoryType indicates memory
 *	//					types used by users.
 */

/**	@cond	*/

/**
 *	@brief	Memory type.
 *
 *			This type indicates which type of content is
 *			residing in the memory block.
 */
typedef int				MemoryType;

/**	@endcond	*/

/*
 *	@brief	\b Memory Type used by users.
 *
 *			\b UserMemoryType is similiar with \b MemoryType except
 *			not containing type of \b MEMORY_TYPE_NONE, which is
 *			used by system internally and not recommended to be used
 *			by users.
 *//*
typedef unsigned        UserMemoryType;*/

/**
 *	@brief	An predefined memory type which indicates memory with
 *			\b "None" type.
 *
 *			The purpose of defining this macro is to make it easy
 *			to allocate memory blocks when users are actually not
 *			interested in the content contained and are not intended
 *			to regain this memory block from the memory chain.
 *
 *	@note	Users are not encouraged to use this memory type except
 *			for knowing what is being done.
 */
#define MEMORY_TYPE_NONE	0

/**
 *	@brief	Status macro which indicates a function \b error.
 */
#define MT_ERROR	-1
/**
 *	@brief	Status macro which indicates a function \b success.
 */
#define MT_SUCCESS	1
/**
 *	@brief	Status macro which indicates the memory type registered
 *			is alerady \b existed.
 */
#define MT_EXISTS	0


/*
 *	Register Memory Type:	Make sure the Type of Memory you are
 *	Allocating is unique.
 */


/**
 *	@brief	Get registered memory type array.
 *	@param	void 	This function has no parameters.
 *
 *	@return	If any memory type has been registered, the return value
 *			is the \b address of the memory type array. Users can use
 *			getRegisteredTypeNum() to acquire the registered memory
 *			type number. If no memory types was registered, the return
 *			value is \b NULL;
 *
 *	@remark	If no memory types was registered, then no memory block
 *			is allocated before.
 *	@sa		getRegisteredTypeNum().
 */
const MemoryType* getRegisteredTypeArray(void);
/**
 *	@brief	Get registered memory type number.
 *	@param	void 	This function has no parameters.
 *	@return	The return value is an unsigned type number.
 *	@sa		getRegisteredTypeArray().
 */
unsigned getRegisteredTypeNum(void);


/**
 *	@brief		Try register a memory type for later memory allocating.
 *
 *	@param[in]	type		A memory type to be registered. It can be
 *				\b MEMORY_TYPE_NONE or any integer value.
 *
 *	@return		If the function succeeds and the memory type is registered
 *				successfully, the return value is \b MT_SUCCESS. If the
 *				function succeeds but memory type is already registered,
 *				the return value is \b MT_EXISTS.
 *				If the function fails, the return is \b MT_ERROR.
 *
 *	@remark		Users can try to register with any interger value, once
 *				succeeds, it can be used for later memory allocating.
 *
 *	@note		It is heavily not recommended to use because of it is
 *				verbose, try to use registerUniqueType() instead.
 *
 *	@sa			MEMORY_TYPE_NONE, registerUniqueType,
 *				MT_SUCCESS, MT_ERROR, MT_EXISTS,
 *				makeMemoryBlock.
 */
int registerMemoryType(MemoryType type);

/**
 *	@brief	Register a new \b unique memory type for later memory
 *			allocating.
 *	@param	void 	This function has no parameters.
 *	@return	If the function succeeds, the return value is the registered
 *			memory type. If the function fails, the return value is \b
 *			MT_ERROR.
 *	@sa		MT_ERROR, makeMemoryBlock.
 */
MemoryType registerUniqueType(void);

/**
 *	@brief	Allocate a memory block of a certain memory type which can
 *			be used later for searching.
 *
 *	@param[in]	bytes	The bytes of memory to be allocated.
 *	@param[in]	type	The memory type of allocated memory which
 *						must be registered first. To register memory
 *						types, use registerUniqueType().
 *
 *	@return	If the function succeeds, the return value is a pointer to
 *			the created memory block tag with type \a type. If the
 *			function fails, the return value is \b NULL which indicates
 *			there is no enough memory.
 *
 *	@remark	Any memory allocated by this function is internally traced.
 *			When finish using all these memory blocks, call
 *			freeMemoryChain() to release them. One should always use
 *			one memory type to allocate memory for the same memory
 *			content.
 *
 *	@note	The caller can use \b MEMORY_TYPE_NONE as the memory type,
 *			but be careful!. For details, see @ref MEMORY_TYPE_NONE.
 *	@warning	User must not free these blocks through c function \b
 *				free(), the only way to free is to use freeMemoryChain().
 *
 *	@sa		freeMemoryChain, findMemoryBlock,
 *			MemoryType, MEMORY_TYPE_NONE.
 */
void* makeMemoryBlock(unsigned bytes, MemoryType type);

/**
 *	@brief		Find a certain memory block.
 *
 *	@param[in]	type	The type of memory block to be found.
 *	@return		\b NULL for not found, \b non-NULL for found.
 *
 *	@remark		Users can pass the same memory type to this function
 *				to traverse all the memory blocks with \a type type,
 *				and the traverse ends when it returns \b NULL.
 *				Once users use a new memory type, it begins a new
 *				searching.
 *
 *	@note		Users can search \b MEMORY_TYPE_NONE memory blocks,
 *				but be sure to not change the contents of those, for
 *				it is internally used and may cause problems.
 *
 *	@sa			makeMemoryBlock, freeMemoryChain.
 */
void* findMemoryBlock(MemoryType type);

/**
 *	@brief	Free all allocated memory blocks.
 *	@param	void 	This function has no parameters.
 *
 *	@remark	Once freed, all pointers pointed to those blocks should never
 *			be dereferenced.
 *	@sa		makeMemoryBlock, findMemoryBlock.
 */
void freeMemoryChain(void);

/**
 *	@brief	Get the head of the memory chain.
 *	@param	void 	This function has no parameters.
 *	@return	A pointer to the memory chain.
 *
 *	@warning	This function is intended to be used internally, users
 *				should not use it!
 *	@warning	This function may be deleted next revision.
 *
 *	@sa			MemoryChain_.
 */
MemoryChainPtr getMemoryChain(void);

/**	@}	*/

#endif
