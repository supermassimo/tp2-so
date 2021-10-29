#include <memManager.h>

extern void* _memAlloc(size_t size);
extern int _memFree(void *blockp);

static uint8_t* nextToMap= (uint8_t*)MEM_BASE;
static long long occupiedMemory = 0;

void incrementOccupiedMemory(size_t increment){
    occupiedMemory += increment;
}

void decrementOccupiedMemory(size_t decrement){
    occupiedMemory -= decrement;
}

void* memMap(size_t size){
    if((size_t)nextToMap + size > MEM_LIMIT)
        return NULL;
    void* blockp = nextToMap;
    nextToMap += size;
    incrementOccupiedMemory(size);
    return blockp;
}

static void applyOptions(void* mem, size_t size, int options){
    switch(options){
        case 1:
            memset(mem, 0, size);
            break;
        default:
            break;
    }
}

void* memAlloc(size_t size, int options){
    void *allocMem = _memAlloc(size);
    applyOptions(allocMem, size, options);
    return allocMem;
}

int memFree(void *blockp){
    return _memFree(blockp);
}

void getMemInfo(MemoryInfo *meminfo){
    meminfo->totalMemory = MEM_LIMIT - MEM_BASE;
    meminfo->occupiedMemory = occupiedMemory;
}