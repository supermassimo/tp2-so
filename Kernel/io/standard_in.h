#ifndef _INPUT_H_
#define _INPUT_H_

#define BUFFER_SIZE 256

static int bufferIsEmpty();
static int bufferIsFull();
int peekLastKey();
int getBufferContent(unsigned char* target, size_t size_limit);
static void setBuffer(static char* newBuffer, int size);
void deleteLastFromBuffer ();

#endif