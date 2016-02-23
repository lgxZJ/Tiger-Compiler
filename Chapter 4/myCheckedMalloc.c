#include "myCheckedMalloc.h"
#include <assert.h>

void* myCheckedMalloc(size_t sizeInBytes)
{
	void* ptr = malloc(sizeInBytes);
	assert(ptr);	//	will be removed on release

	return ptr;
}
