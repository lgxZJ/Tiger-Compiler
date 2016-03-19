#incude "stringTranslate.h"
#include "makeMemory.h"
#include "string.h"

/*	module Memory Type	*/
static int g_translate_memory_type = NULL;

char* translateEscape(char* str)
{
    if (str == NULL)	return NULL;

    //	if first use, register a unique Memory Type
    if (g_translate_memory_type == NULL)
	g_translate_memory_type = registerUniqueType();

    int string_length = strlen(str);
    for (unsigned i = 0; i < string_length; ++i)
    {
	//todo: translate escape sequence
	//	two character escape
	if (string_length >= 2)
	{
	    if (strncmp(str[i], "\\^", 2) == 0)
	}
	//	three character escape
	if (string_length >= 3)
	{

	}
    }
}
