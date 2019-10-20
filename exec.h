#ifndef EXEC_H
#define EXEC_H

char** getArgumentList(char* buffer, char** paramBuffer);
char** getSeparateProcessList(char* buffer, int* size);

char* getUserName();
char* getHostName();

int executeProgram(char* programName, char** paramBuffer, char* input);

#endif
