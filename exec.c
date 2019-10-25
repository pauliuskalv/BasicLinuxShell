#include "exec.h"

#include "util.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_CHAINED_PROCESS_COUNT 64

char* defaultPathPrefix = "/bin/";
char* defaultWhoAmI = "whoami";
char* defaultHostname = "hostname";

char* getProgramOutput()
{
    return getOutBuffer().bytes;
}

/**
 *  Executes the whoami command to get the user's name
 * */
char* getUserName()
{
    cleanupParamBuffer();
    cleanupOutBuffer();

    if (executeProgram(defaultWhoAmI, getParamBuffer(), 0) == 0)
    {
        strrchr(getProgramOutput(), '\n')[0] = (char)0;
        return getProgramOutput();
    }
    else
    {
        return NULL;
    }
}

/**
 * Executes the hostname command to get the machine's name
 * */
char* getHostName()
{
    cleanupParamBuffer();
    cleanupOutBuffer();

    if (executeProgram(defaultHostname, getParamBuffer(), 0) == 0)
    {
        strrchr(getProgramOutput(), '\n')[0] = (char)0;
        return getProgramOutput();
    }
    else
    {
        return NULL;
    }
}

char** getArgumentList(char* buffer, char** paramBuffer)
{
    char* token = strtok(buffer, " ");
    int count = 0;
    while (token)
    {
        paramBuffer[count] = string_copy(token);
        count ++;
        token = strtok(NULL, " ");
    }
    // Convention demands that the last token be null
    paramBuffer[count] = 0;

    return paramBuffer;
}

char** getSeparateProcessList(char* buffer, int* size)
{
    char** list = (char**)calloc(MAX_CHAINED_PROCESS_COUNT, sizeof(char*));

    char* token = strtok(buffer, "|");
    int count = 0;
    while (token)
    {
        list[count] = string_copy(token);
        count ++;
        token = strtok(NULL, "|");
    }

    *size = count;

    return list;
}

int executeProgram(char* programName, char** paramBuffer, int pipeInput)
{
    char* filePath = concat(defaultPathPrefix, programName);

    // Check if file is in path and is executable
    if (access(filePath, F_OK | X_OK) != 0)
    {
        printf("Unknown program %s\n", programName);
        free(filePath);
        return -1;
    }

    paramBuffer[0] = filePath;

    // Declare pipes
    int fd1[2]; // parent -> child
    int fd2[2]; // child -> parent

    if (pipe(fd1) != 0)
    {
        printf("Error creating first pipe\n");
        return -1;
    }

    if (pipe(fd2) != 0)
    {
        printf("Error creating second pipe\n");
        return -1;
    }

    // Isolate program execution from the shell
    pid_t forkedProcess = fork();

    if (forkedProcess == 0)
    {
        // In child process
        if (pipeInput)
        {
            // Redirect stdin to read pipe
            dup2(fd1[0], STDIN_FILENO);
        }

        // Redirect stdout to write pipe
        dup2(fd2[1], STDOUT_FILENO);

        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);

        execvp(filePath, paramBuffer);

        exit(1);
    }
    else
    {
        // In parent process
        if (pipeInput && can_write(fd1[1]))
        {
            write(fd1[1], getOutBuffer().bytes, getOutBuffer().bytesInBuffer);
        }

        close(fd1[1]); // Close write end pipe

        // Read child process output if there's any
        int dataRead = 0;
        while (has_data(fd2[0]))
        {
            dataRead += read(fd2[0], getOutBuffer().bytes + dataRead, getMaxOutBufferSize() - dataRead);
        }

        if (dataRead == 0)
        {
            getOutBuffer().bytes[0] = 0;
        }
        else if (getMaxOutBufferSize() > dataRead)
        {
            getOutBuffer().bytes[dataRead] = 0;
        }

        setBytesInBuffer(dataRead);

        close(fd1[0]);
        close(fd2[0]);
        close(fd2[1]);
    }

    cleanupParamBuffer();

    return 0;
}