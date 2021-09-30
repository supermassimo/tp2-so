#include <stdint.h>
#include <stddef.h>
#include <lib.h>

/* memAlloc options*/
#define NO_ACTION 0
#define SET_ZERO  1

typedef struct MemoryInfo{
    size_t totalMemory;
    size_t occupiedMemory;
} MemoryInfo;

void* memAlloc(size_t size, int options);
int memFree(void *blockp);
void getMemInfo(MemoryInfo* meminfo);