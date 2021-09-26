#include <naiveMemManager.h>

#define MEM_LIMIT 0x51200000

static uint8_t* currentPos = (uint8_t*)0x900000;

void* memMap(size_t size){
    if((size_t)currentPos + size > MEM_LIMIT)
        return NULL;
    void* ptr = currentPos;
    currentPos += size;
    return ptr;
}