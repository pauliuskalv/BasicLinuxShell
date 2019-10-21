#include "util.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/poll.h>

#define PARAM_BUFFER_SIZE 256
#define OUT_BUFFER_SIZE 32768

char** paramBuffer = NULL;
char* outBuffer = NULL;

void initializeParamBuffer()
{
    paramBuffer = (char**)malloc(sizeof(char*) * PARAM_BUFFER_SIZE);
}

void initializeOutBuffer()
{
    outBuffer = (char*)malloc(sizeof(char) * OUT_BUFFER_SIZE);
}

void setOutBuffer(char* buff)
{
    outBuffer = buff;
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
    // Make outBuffer zero length
    int i = 0;
    for (i; i < OUT_BUFFER_SIZE; i ++)
        outBuffer[i] = '\0';
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
    // scanf("%s", outBuffer);
    // gets(buffer, OUT_BUFFER_SIZE);
    // fgets(buffer, OUT_BUFFER_SIZE, STDIN_FILENO);
    if (fgets(buffer, OUT_BUFFER_SIZE, stdin)) {
        buffer[strcspn(buffer,"\n")] = '\0';
    }
    // getline(buffer, sizeof buffer, 1);
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

char* string_copy(char* string)
{
    size_t size = strlen(string);
    char* toReturn = (char*)malloc(sizeof(char)*size + 1);

    int i = 0;
    for (i; i < size; i ++)
        toReturn[i] = string[i];

    toReturn[i] = '\0';

    return toReturn;
}

int has_data(int fd)
{
    struct pollfd * poll_params = malloc(sizeof(struct pollfd));
    int retval;

    poll_params[0].fd = fd;
    poll_params[0].events = POLLIN;
    poll_params[0].revents = POLLIN;
    
    retval = poll(poll_params, 1, 0);

    free(poll_params);

    return retval;
}