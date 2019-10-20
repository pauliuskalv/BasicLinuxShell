#include "util.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define PARAM_BUFFER_SIZE 256
#define OUT_BUFFER_SIZE 8192

char** paramBuffer = NULL;
char* outBuffer = NULL;

void initializeParamBuffer()
{
    paramBuffer = (char**)calloc(PARAM_BUFFER_SIZE, sizeof(char*));
}

void initializeOutBuffer()
{
    outBuffer = (char*)malloc(sizeof(char) * OUT_BUFFER_SIZE);
}

int getMaxParamBufferSize()
{
    return PARAM_BUFFER_SIZE;
}

int getMaxOutBufferSize()
{
    return OUT_BUFFER_SIZE;
}

char** getParamBuffer()
{
    return paramBuffer;
}

char* getOutBuffer()
{
    return outBuffer;
}

void cleanupParamBuffer()
{
    int i = 0;
    for (i = 0; i < PARAM_BUFFER_SIZE; i ++)
    {
        if (paramBuffer[i] != NULL)
        {
            free(paramBuffer[i]);
            paramBuffer[i] = NULL;
        }
    }
}

void cleanupOutBuffer()
{
    int i = 0;
    for (i; i < OUT_BUFFER_SIZE; i ++)
        outBuffer[i] = NULL;
}

void setParameterAt(int index, char* parameter)
{
    paramBuffer[index + 1] = parameter;
}

char* getParameterAt(int index)
{
    return paramBuffer[index + 1];
}

void readLineConsole(char* buffer)
{
    gets(buffer, OUT_BUFFER_SIZE);
    //fgets(buffer, OUT_BUFFER_SIZE, stdin);
}

char* concat(char* stringOne, char* stringTwo)
{
    size_t sizeOfOne = strlen(stringOne);
    size_t sizeOfTwo = strlen(stringTwo);
    char* concatenatedString = NULL;

    concatenatedString = malloc(sizeof(char) * (sizeOfOne + sizeOfTwo));

    int i;
    for (i = 0; i < sizeOfOne; i ++)
        concatenatedString[i] = stringOne[i];

    for (i = 0; i < sizeOfTwo; i ++)
        concatenatedString[sizeOfOne + i] = stringTwo[i];

    return concatenatedString;
}

char *ltrim(char *str, const char *seps)
{
    size_t totrim;
    if (seps == NULL) {
        seps = "\t\n\v\f\r ";
    }
    totrim = strspn(str, seps);
    if (totrim > 0) {
        size_t len = strlen(str);
        if (totrim == len) {
            str[0] = '\0';
        }
        else {
            memmove(str, str + totrim, len + 1 - totrim);
        }
    }
    return str;
}

char *rtrim(char *str, const char *seps)
{
    int i;
    if (seps == NULL) {
        seps = "\t\n\v\f\r ";
    }
    i = strlen(str) - 1;
    while (i >= 0 && strchr(seps, str[i]) != NULL) {
        str[i] = '\0';
        i--;
    }
    return str;
}

char *trim(char *str, const char *seps)
{
    return ltrim(rtrim(str, seps), seps);
}

char* string_copy(char* string)
{
    size_t size = strlen(string);
    char* toReturn = (char*)malloc(sizeof(char)*size);

    int i = 0;
    for (i; i < size; i ++)
        toReturn[i] = string[i];

    return toReturn;
}