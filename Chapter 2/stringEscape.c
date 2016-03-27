#include "stringEscape.h"
#include "makeMemory.h"
#include <string.h>	//	for functions of string manipulations
#include <stdbool.h>	//	for type `bool`
#include <stdlib.h>	//	for `strtol()` using
#include <stdio.h>	//	for `sprintf()` using
#include <assert.h>

/*	module Memory Type	*/
static int g_translate_memory_type = MEMORY_TYPE_NONE;

/*
 *	NOTE:	Following `translateXXX` functions do not check
 *	parameters passed in, the caller makes it;
 */

#define NORMAL_ESCAPE_LENGTH	2
#define CONTROL_ESCAPE_LENGTH	3
#define OCTAL_ESCAPE_LENGTH	4
#define NOT_ESCAPED		0

/*	return escape string length, NOT_ESCAPED for not escaped   */
static unsigned translateNormalEscape(char* str, int* i_ptr, int string_length,
		  char* result, int result_pos)
{
    //	have enough stirng length
    if ((*i_ptr + 2) <= string_length )
    {
	char* ch[] = { "\\n", "\\t", "\\\"", "\\\\" };
	char c[] = { '\n', '\t', '"', '\\' };

	for (unsigned j = 0; j < (sizeof(ch) / sizeof(ch[0])); ++j)
	{
	    if (strncmp(&str[*i_ptr], ch[j], 2) == 0)
	    {
		sprintf(result + result_pos, "%c", c[j]);
	        return NORMAL_ESCAPE_LENGTH;
	    }
	}
    }

    return NOT_ESCAPED;
}

static unsigned translateControlEscape(char* str, int* i_ptr, int string_length,
		    char* result, int result_pos)
{
    char* ch[] = { "\\^@", "\\^[", "\\^\\",
		   "\\^]", "\\^^", "\\^_" };
    char c[] = { '\x0', '\x1B', '\x1C', '\x1D',
		 '\x1E', '\x1F' };
	    
    if (strncmp(&str[*i_ptr], "\\^", 2) == 0 &&
	(str[*i_ptr + 2] >= 'A' && str[*i_ptr + 2] <= 'Z'))
    {
	sprintf(result + result_pos, "%c", '\x01' + str[*i_ptr + 2] - 'A');
	return CONTROL_ESCAPE_LENGTH;
    }
    else
    {
	for (unsigned j = 0; j < (sizeof(ch) / sizeof(ch[0])); ++j)
	    if (strncmp(&str[*i_ptr], ch[j], 3) == 0)
	    {
		sprintf(result + result_pos, "%c", c[j]);
		return CONTROL_ESCAPE_LENGTH;
	    }
    }
	    
    return NOT_ESCAPED;
}

/*	Caller make sures the length of string is enough	*/
bool containThreeOctalNumber(char* str)
{
    return (str[0] >= '0' && str[0] <= '7') &&
	(str[1] >= '0' && str[1] <= '7') &&
	(str[2] >= '0' && str[2] <= '7');
}

static unsigned translateOctalEscape(
    char* str, int* i_ptr, int string_length,
    char* result, int result_pos)
{
    if ((*i_ptr + 4) <= string_length &&
	str[*i_ptr] == '\\' &&
	containThreeOctalNumber(&str[(*i_ptr) + 1]))
    {
/*
  void* tmp = makeMemoryBlock(sizeof(char) * 4, g_translate_memory_type);
  strcpy(tmp, str[i]);

  int value = strtol(tmp, NULL, 8);
*/
	int value = -1;

	if (strncmp(&str[(*i_ptr) + 1], "000", 3) == 0)
	    value = 0;
        else
	{
	    value = strtol(&str[(*i_ptr) + 1], NULL, 8);
	    //	no valid conversion (include valid 0)
	    if (value == 0)
		return NOT_ESCAPED;
	}
	//	if can not convert
	sprintf(result + result_pos,"%c", '\x0' + value);
	return OCTAL_ESCAPE_LENGTH;
    }

    return NOT_ESCAPED;
}

/*	
 *	NOTE:	The Caller make sures that the multi-line fomrat is
 *	correct which means is a multi-line consists of two '\'
 *	characters. The only supported characters between `\---\`
 *	are '\t' and ' ', and '\n' character must be contained.
 *
 *	RETURN:	return ignored character numbers. (0 for not escaped)
 */
static unsigned translateMultiline(
    char* str, int* i_ptr, int string_length,
    char* result, int result_pos)
{
    if (str[*i_ptr] == '\\')
    {
	bool haveNewLine = false;
	for (unsigned i = (*i_ptr) + 1; i < string_length; ++i)
	{
	    switch (str[i])
	    {
	    case '\n':	haveNewLine = true;
	    case '\t':	
	    case ' ':	continue;

	    case '\\':
		if (haveNewLine)
		    return i - *i_ptr + 1;
		else
		    return NOT_ESCAPED;

	    default:	return NOT_ESCAPED;
	    }
	}
	//	no more characters after '\\'
	return NOT_ESCAPED;
    }
    return NOT_ESCAPED;

}

/*
 *	NOTE:	Caller make sures that escape characters are valid,
 *	this function only take charge to translate it.
 *
 *	Translate all escape characters into there meanings, invalid
 *	escape characters are ignored.
 */
char* translateEscape(char* str)
{
    if (str == NULL)	return NULL;

    //	if first use, register a unique Memory Type
    if (g_translate_memory_type == 0)
    {
	g_translate_memory_type = registerUniqueType();
	if (g_translate_memory_type == MT_ERROR)
	    return NULL;
    }

    int string_length = strlen(str);
    //	each escape character makes string shorter after escape,
    //	so, `string_length` is enough
    char* result = makeMemoryBlock(sizeof(char) * (string_length + 1),
				   g_translate_memory_type);

    //	`pos` of result string
    for (unsigned i = 0, pos = 0; i < string_length; ++pos)
    {
	unsigned escaped_num = 0;
        if (escaped_num = translateNormalEscape(str, &i, string_length, result, pos))
	    i += escaped_num;
	else if (escaped_num = translateControlEscape(str, &i, string_length, result, pos))
	    i += escaped_num;
	else if (escaped_num = translateOctalEscape(str, &i, string_length, result, pos))
	    i += escaped_num;
	else if (escaped_num = translateMultiline(str, &i, string_length, result, pos))
	{
	    i += escaped_num;
	    --pos;
	}
	else
	{
	    sprintf(result + pos, "%c", str[i]);
	    ++i;
	}
    }

    result[string_length] = '\0';
    return result;
}
