#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "exec.h"

#include <sys/wait.h>

void mainLoop();

int main()
{
  initializeParamBuffer();
  initializeOutBuffer();
  
  mainLoop();

  return 0;
}

void mainLoop()
{
  char* userName = string_copy(getUserName());
  char* hostName = string_copy(getHostName());
  int retval = 0;

  while(1)
  {
    printf("%s@%s$: ", userName, hostName);
    readLineConsole(getOutBuffer().bytes);

    if (strcmp("", getOutBuffer().bytes) == 0)
      continue;

    if (strcmp("exit", getOutBuffer().bytes) == 0)
      break;

    int process_count = 0;

    char** processList = getSeparateProcessList(getOutBuffer().bytes, &process_count);

    // cleanupOutBuffer();

    getArgumentList(processList[0], getParamBuffer());
    retval = executeProgram(getParamBuffer()[0], getParamBuffer(), 0, 0);

    if (process_count > 1 && retval == 0)
    {
      int i = 1;
      for (i; i < process_count && retval == 0; i ++)
      {
        getArgumentList(processList[i], getParamBuffer());
        retval = executeProgram(getParamBuffer()[0], getParamBuffer(), getOutBuffer().pipe, i == process_count - 1 ? 1 : 0);
      }
    }

    // free up the list
    int i;
    for (i = 0; i < process_count; i ++)
    {
      free(processList[i]);
    }
    free(processList);

    readPipe(getOutBuffer().pipe);
    clearPipe();

    if (retval == 0)
      printf("%s", getOutBuffer());

    cleanupParamBuffer();
  }

  free(userName);
  free(hostName);
}
