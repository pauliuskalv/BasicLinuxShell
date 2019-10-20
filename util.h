#ifndef UTIL_H
#define UTIL_H

void initializeParamBuffer();
char** getParamBuffer();
int getMaxParamBufferSize();
void cleanupParamBuffer();

void initializeOutBuffer();
char* getOutBuffer();
int getMaxOutBufferSize();
void cleanupOutBuffer();

char* concat(char* stringOne, char* stringTwo);
char *trim(char *str, const char *seps);

char* string_copy(char* string);

void readLineConsole(char* buffer);

#endif
