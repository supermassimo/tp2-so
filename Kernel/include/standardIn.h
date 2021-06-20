#ifndef _INPUT_H_
#define _INPUT_H_

#include <stddef.h>

#define BUFFER_SIZE 120

int bufferIsEmpty();
int bufferIsFull();
int getBufferContent(char* target, size_t size_limit);
void setInputBuffer(char* newBuffer, int end);
void deleteLastFromBuffer();

#endif