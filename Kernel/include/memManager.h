#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <stdint.h>
#include <stddef.h>
#include <lib.h>

#define PAGE_SIZE   0x1000
#define MEM_LIMIT   536870912             // 512MB
#define MEM_BASE    9437184

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
void* memMap(size_t size);
void incrementOccupiedMemory(size_t increment);
void decrementOccupiedMemory(size_t decrement);

#endif