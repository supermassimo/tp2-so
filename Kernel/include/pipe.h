#include <semaphore.h>
#include <console.h>

#define BUFFER_SIZE 512

typedef struct Pipe {
    int id;
    char buffer[BUFFER_SIZE];
    int nwrite;
    int nread;
    char *semName;
    struct Pipe *next;
} Pipe;

int createPipe();
void closePipe(int index);
int writePipe(int index, char* const buf, int count);
int readPipe(int index, char* buf, int count);
void printPipes();