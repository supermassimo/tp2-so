#include <sharedMemory.h>

static sh_mem shared_mem[10] = {0};

int open(int id) {
    if(id > 10 || id < 0) {
        return -1;
    }
    if(shared_mem[id].used == 0) {
        shared_mem[id].used = 1;
        shared_mem[id].mem_start = memAlloc(SIZE, NO_ACTION);
    } 
    return 0;
}

void *get(int id) {
    if(id > 10 || id < 0) {
        return NULL;
    }
    if(shared_mem[id].used == 0) {
        open(id);
    } 
    return shared_mem[id].mem_start;
}