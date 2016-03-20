#incude "stringTranslate.h"
#include "makeMemory.h"
#include "string.h"

/*	module Memory Type	*/
static int g_translate_memory_type = NULL;

void translateTwo(char* str, int* i_ptr, int string_length,
		  char* result, int result_pos)
{
    if ((*i_ptr + 2) <= string_length )
    {
	char* ch[] = { "\\n", "\\t", "\\\"", "\\\\" };
	char c[] = { '\n', '\t', '"', '\\' };

	for (unsigned j = 0; j < (sizeof(ch) / sizeof(ch[0])); ++j)
	{
	    if (strncmp(str[*i_ptr], ch[j], 2) == 0)
	    {
		sprintf(result + result_pos, "%c", c[j]);
		break;
	    }
	}

	*i_ptr += 2;
    }
}

void translateThree(char* str, int* i_ptr, int string_length,
		    char* result, int result_pos)
{
    char* ch[] = { "\\^@", "\\^[", "\\^\\",
		   "\\^]", "\\^^", "\\^_" };
    char c[] = { '\x0', '\x1B', '\x1C', '\x1D',
		 '\x1E', '\x1F' };
	    
    if (strncmp(str[*i_ptr], "\\^", 2) == 0 &&
	(str[*i_ptr + 2] >= 'A' && str[*i_ptr + 2] <= 'Z'))

	sprintf(result + result_pos, "%c", '\x01' + str[*i_ptr + 2] - 'A');
    else
    {
	for (unsigned j = 0; j < (sizeof(ch) / sizeof(ch[0])); ++j)
	    if (strncmp(str[*i_ptr], ch[j], 3) == 0)
	    {
		sprintf(result + result_pos, "%c", c[j]);
		break;
	    }
    }
	    
    *i_ptr += 3;
}

void translateFour();

char* translateEscape(char* str)
{
    if (str == NULL)	return NULL;

    //	if first use, register a unique Memory Type
    if (g_translate_memory_type == NULL)
	g_translate_memory_type = registerUniqueType();

    int string_length = strlen(str);
    //	each escape character makes string shorter after escape,
    //	so, `string_length` is enough
    char* result = makeMemoryBlock(sizeof(char) * (string_length + 1),
				   g_translate_memory_type);

    //	`pos` of result string
    for (unsigned i = 0, pos = 0; i < string_length; ++pos)
    {
	//todo: translate escape sequence
	//	two character escape
	if ((i + 2) <= string_length )
	{
	    char* ch[] = { "\\n", "\\t", "\\\"", "\\\\" };
	    char c[] = { '\n', '\t', '"', '\\' };

	    for (unsigned i = 0; i < (sizeof(ch) / sizeof(ch[0])); ++i)
	    {
		if (strncmp(str[i], ch[i], 2) == 0)
		{
		    sprintf(result + pos, "%c", c[i]);
		    break;
		}
	    }

	    i += 2;
	}
	//	three character escape
	if ((i + 3) <= string_length)
	{
	    char* ch[] = { "\\^@", "\\^[", "\\^\\",
			   "\\^]", "\\^^", "\\^_" };
	    char c[] = { '\x0', '\x1B', '\x1C', '\x1D',
			 '\x1E', '\x1F' };
	    
	    if (strncmp(str[i], "\\^", 2) == 0 &&
		(str[i + 2] >= 'A' && str[i + 2] <= 'Z'))
		sprintf(result + pos, "%c", '\x01' + str[i + 2] - 'A');
	    else
	    {
		for (unsigned i = 0; i < (sizeof(ch) / sizeof(ch[0])); ++i)
		{
		    if (strncmp(str[i], ch[i], 3) == 0)
		    {
			sprintf(result + pos, "%c", c[i]);
			break;
		    }
		}
	    }
	    
	    i += 3;
	}
	//	four character escape
	if ((i + 4) <= string_length)
	{
	    
	}
    }
}
