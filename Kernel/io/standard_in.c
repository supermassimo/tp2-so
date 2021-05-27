#include <standard_in.h>

static int input_buffer[BUFFER_SIZE];
static unsigned int nextToStore = 0;
static unsigned int currentToRead = 0;

int getBufferSize(){
    return BUFFER_SIZE;
}

static int bufferIsEmpty(){
    return nextToStore == 0;
}

static int bufferIsFull(){
    return nextToStore == BUFFER_SIZE;
}

static void setBuffer(static char* newBuffer, int size){
    for (int i=0; i<size && i<BUFFER_SIZE; i++){
        input_buffer[i] = newBuffer[i];
    }
    nextToStore = 0;
        currentToRead = 0;
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
    if(currentToRead != nextToStore){
        if(currentToRead == BUFFER_SIZE)
            currentToRead = 0;
        return input_buffer[currentToRead++];
    }
    return -1;
}

// Fills array sent with buffer content on string format (ends with '\0') 
// and returns # of keys read from buffer
int getBufferContent(unsigned char* target, int size_limit){
    int i=0, aux;
    while((aux = getKey()) != -1 && i<size_limit){
        target[i++] = aux;
    }
    target[i] = 0;
    return i;
}