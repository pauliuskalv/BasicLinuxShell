#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "exec.h"

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

  while(1)
  {
    printf("%s@%s$: ", userName, hostName);
    readLineConsole(getOutBuffer());

    if (strcmp("exit", getOutBuffer()) == 0)
      break;

    int process_count = 0;
    char** processList = getSeparateProcessList(getOutBuffer(), &process_count);

    // cleanupOutBuffer();

    if (process_count == 1)
    {
      // Single process
      getArgumentList(getOutBuffer(), getParamBuffer());
      executeProgram(getParamBuffer()[0], getParamBuffer(), NULL);
    }
    else
    {
      // Multiple processes
      getArgumentList(processList[0], getParamBuffer());
      executeProgram(getParamBuffer()[0], getParamBuffer(), NULL);

      int i = 1;
      for (i; i < process_count; i ++)
      {
        getArgumentList(processList[i], getParamBuffer());
        executeProgram(getParamBuffer()[0], getParamBuffer(), getOutBuffer());
      }

      // free up the list
      for (i = 0; i < process_count; i ++)
        free(processList[i]);
      free(processList);
    }

    printf("%s", getOutBuffer());

    cleanupOutBuffer();
    cleanupParamBuffer();
  }

  free(userName);
  free(hostName);
}