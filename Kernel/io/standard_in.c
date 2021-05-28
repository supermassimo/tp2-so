#include <standard_in.h>

static int input_buffer[BUFFER_SIZE];
static unsigned int currentToRead = 0;
static unsigned int endOfContent = 0;

int bufferIsFull(){
    return endOfContent == BUFFER_SIZE;
}

int bufferIsEmpty(){
    return currentToRead == endOfContent;
}

void setInputBuffer(unsigned char* newBuffer, int start, int end){
    int auxIdx = start;
    while(auxIdx != end){
        if(auxIdx == BUFFER_SIZE)
            auxIdx = 0;
        input_buffer[endOfContent++] = newBuffer[auxIdx++];
    }
}

// Returns last stored key without removing it from buffer
// int peekLastKey(){
//     int last = nextToStore - 1;
//     if(last == -1)
//         last = BUFFER_SIZE - 1;
//     return input_buffer[last];
// }

// If there are keys on buffer, returns the first available. Else returns -1
int getKey(){
    if(currentToRead != endOfContent)
        return input_buffer[currentToRead++];
    return -1;
}

// Fills array sent with buffer content on string format (ends with '\0')
// and returns # of keys read from buffer
int getBufferContent(unsigned char* target, size_t size_limit){
    int i=0, aux;
    while((aux = getKey()) != -1 && i<size_limit){
        target[i++] = aux;
    }
    target[i] = 0;
    currentToRead = 0;
    endOfContent = 0;
    return i;
}