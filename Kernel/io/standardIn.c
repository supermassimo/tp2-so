#include <standardIn.h>
#include <dualDisplayManager.h>

static int input_buffer_0[BUFFER_SIZE];
static unsigned int currentToRead_0 = 0;
static unsigned int endOfContent_0 = 0;

static int input_buffer_1[BUFFER_SIZE];
static unsigned int currentToRead_1 = 0;
static unsigned int endOfContent_1 = 0;

int bufferIsFull(){
    if (getCurrentDisplay())
        return endOfContent_1 == BUFFER_SIZE;
    else
        return endOfContent_0 == BUFFER_SIZE;
}

int bufferIsEmpty(){
    if (getCurrentDisplay())
        return currentToRead_1 == endOfContent_1;
    else
        return currentToRead_0 == endOfContent_0;
    
}

void setInputBuffer(unsigned char* newBuffer, int end){
    int auxIdx = 0;
    while(auxIdx != end){
        if (getCurrentDisplay())
            input_buffer_1[endOfContent_1++] = newBuffer[auxIdx++];
        else
            input_buffer_0[endOfContent_0++] = newBuffer[auxIdx++];
    }
    if (getCurrentDisplay()){
        currentToRead_1 = 0;
    } else {
        currentToRead_0 = 0;
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
    if (getCurrentDisplay()){
        if(currentToRead_1 != endOfContent_1)
            return input_buffer_1[currentToRead_1++];
    } else {
        if(currentToRead_0 != endOfContent_0)
            return input_buffer_0[currentToRead_0++];
    }
    
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
    if (aux == -1){
        if (getCurrentDisplay()){
            currentToRead_1 = 0;
            endOfContent_1 = 0;
        } else {
            currentToRead_0 = 0;
            endOfContent_0 = 0;
        }
    }
    return i;
}