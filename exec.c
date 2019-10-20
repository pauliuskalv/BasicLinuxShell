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
    return getOutBuffer();
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
        paramBuffer[count] = string_copy(trim(token, NULL));
        count ++;
        token = strtok(NULL, " ");
    }
    // Convention demands that the last token be null
    paramBuffer[count] = 0;

    return paramBuffer;
}

char** getSeparateProcessList(char* buffer, int* size)
{
    char** list = (char**)malloc(sizeof(char*) * MAX_CHAINED_PROCESS_COUNT);

    char* token = strtok(buffer, "|");
    int count = 0;
    while (token)
    {
        list[count] = string_copy(trim(token, NULL));
        count ++;
        token = strtok(NULL, "|");
    }

    *size = count;

    return list;
}

int executeProgram(char* programName, char** paramBuffer, char* input)
{
    char* filePath = concat(defaultPathPrefix, programName);
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

    fcntl(fd1[0], F_SETFL, O_NONBLOCK);
    fcntl(fd1[1], F_SETFL, O_NONBLOCK);
    fcntl(fd2[0], F_SETFL, O_NONBLOCK);
    fcntl(fd2[1], F_SETFL, O_NONBLOCK);

    // Isolate program execution from the shell
    pid_t forkedProcess = fork();

    if (forkedProcess == 0)
    {
        // In child process
        if (input != NULL)
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

        execv(filePath, paramBuffer);

        exit(0);
    }
    else
    {
        // In parent process
        if (input != NULL)
        {
            write(fd1[1], getOutBuffer(), strlen(getOutBuffer()));
        }

        close(fd1[1]); // Close write end pipe

        // Wait for child process to finish executing
        wait(NULL);

        // Read child process output if there's any
        cleanupOutBuffer();
        read(fd2[0], getOutBuffer(), getMaxOutBufferSize());

        close(fd1[0]);
        close(fd2[0]);
        close(fd2[1]);
    }

    cleanupParamBuffer();

    return 0;
}