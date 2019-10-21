#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

void initializeParamBuffer();
char** getParamBuffer();
int getMaxParamBufferSize();
void cleanupParamBuffer();

void initializeOutBuffer();
char* getOutBuffer();
void setOutBuffer(char* buff);
int getMaxOutBufferSize();
void cleanupOutBuffer();

char* concat(char* stringOne, char* stringTwo);

char* string_copy(char* string);

int has_data(int fd);

void readLineConsole(char* buffer);

#endif
