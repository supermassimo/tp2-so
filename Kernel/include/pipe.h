#ifndef PIPE_H
#define PIPE_H

#include <semaphore.h>
#include <console.h>

#define PIPE_BUFFER_SIZE 512

typedef struct Pipe {
    int id;
    char buffer[PIPE_BUFFER_SIZE];
    int nwrite;
    int nread;
    char* semName;
    struct Pipe *next;
} Pipe;

int createPipe();
void closePipe(int index);
int writePipe(int index, const char* buf, int count);
int readPipe(int index, char* buf, int count);
void printPipes();

#endif