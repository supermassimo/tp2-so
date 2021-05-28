#ifndef _INPUT_H_
#define _INPUT_H_

#include <stddef.h>

#define BUFFER_SIZE 256

int bufferIsEmpty();
int bufferIsFull();
int getBufferContent(unsigned char* target, size_t size_limit);
void setInputBuffer(unsigned char* newBuffer, int start, int end);
void deleteLastFromBuffer();

#endif