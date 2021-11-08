#include <memManager.h>

#define SIZE 1024

typedef struct sh_mem{
    int used;
    void *mem_start;
} sh_mem;

void *get(int id);