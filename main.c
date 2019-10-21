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

    if (strcmp("", getOutBuffer()) == 0)
      continue;

    if (strcmp("exit", getOutBuffer()) == 0)
      break;

    int process_count = 0;

    char** processList = getSeparateProcessList(getOutBuffer(), &process_count);

    // cleanupOutBuffer();

    getArgumentList(processList[0], getParamBuffer());
    executeProgram(getParamBuffer()[0], getParamBuffer(), NULL);
    if (process_count > 1)
    {
      int i = 1;
      for (i; i < process_count; i ++)
      {
        char* copyOfBuffer = string_copy(getOutBuffer());

        getArgumentList(processList[i], getParamBuffer());
        executeProgram(getParamBuffer()[0], getParamBuffer(), copyOfBuffer);
        
        free(copyOfBuffer);
      }
    }

    // free up the list
    int i;
    for (i = 0; i < process_count; i ++)
    {
      free(processList[i]);
    }
    free(processList);

    printf("%s", getOutBuffer());

    cleanupOutBuffer();
    cleanupParamBuffer();
  }

  free(userName);
  free(hostName);
}