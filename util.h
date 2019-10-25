#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

struct out_buffer {
    char* bytes;
    int bytesInBuffer;
};

void initializeParamBuffer();
char** getParamBuffer();
int getMaxParamBufferSize();
void cleanupParamBuffer();

void initializeOutBuffer();
struct out_buffer getOutBuffer();
void setBytesInBuffer(int count);

int getMaxOutBufferSize();
void cleanupOutBuffer();

char* concat(char* stringOne, char* stringTwo);

char* string_copy(char* string);

int has_data(int fd);
int can_write(int fd);

void readLineConsole(char* buffer);

#endif
