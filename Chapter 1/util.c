#include "util.h"
#include <string.h>
#include <stdlib.h>
#include <string.h>


string String (char *source)
{
	//add null character
	string str = (string)checked_malloc (strlen (source) + 1);
	strcpy ((char*)str, source);
	return str;
}

void* checked_malloc (int length)
{
	void* ptr = malloc (length);
	assert (ptr);//assert under debug mode
	return ptr;
}
