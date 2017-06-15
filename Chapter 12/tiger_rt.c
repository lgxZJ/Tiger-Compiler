#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void prints(const char* str)
{
    fputs(str, stdout);
}

void printc(char ch)
{
    putchar(ch);
}

void printn(int num)
{
    char buf[20];
    sprintf(buf, "%d", num);
    fputs(buf, stdout);
}

void flush(void)
{
    fflush(stdout);
}

char* getcharr(void)
{
    char* str = (char*)calloc(2, sizeof(char));
    assert (str != NULL);

    int ch = getchar();
    if (ch != EOF)
        str[0] = ch;

    return str;
}

int ord(const char* str)
{
    if (str == NULL || *str == '\0')
        return -1;
    else
        return str[0];
}

char* chr(int i)
{
    //  must from '!' to '~'
    assert (i >= 33 && i <= 126);

    char* str = (char*)calloc(2, sizeof(char));
    assert (str != NULL);
    
    str[0] = i;
    return str;
}

int size(const char* str)
{
    assert (str != NULL);
    return strlen(str);
}

char* substring(const char* str, int index, int length)
{
    int strLength = strlen(str);
    assert (index >= 0 && index < strLength);
    assert (length >= 0 && length <= strLength);
    assert (index + length <= strLength);

    char* resultStr = (char*)malloc(sizeof(char) * (length + 1));
    assert (resultStr != NULL);

    strncpy(resultStr, &str[index], length);
    return resultStr;
}

char* concat(const char* str1, const char* str2)
{
    assert (str1 != NULL && str2 != NULL);
    
    char* resultStr = (char*)malloc(sizeof(char) * (strlen(str1) + strlen(str2) +1));
    assert (resultStr != NULL);

    strcpy(resultStr, str1);
    strcat(resultStr, str2);
    return resultStr;
}

int not(int i)
{
    return !i;
}

void exitt(int status)
{
    exit(status);
}

void assertt(int expResult, const char* filename, int line)
{
    if (expResult == 0)
    {
        char msg[50];
        sprintf(msg, "assertt fiailed, file[%s] line[%d] !", filename, line);
        fputs(msg, stderr);
        abort();
    }
}