#include <memManager.h>

static uint8_t* currentPos = (uint8_t*)0x900000;

void* memAlloc(size_t size){
    void* ptr = currentPos;
    currentPos += size;
    return ptr;
}

void memFree(void* startPos){
    // TODO
}